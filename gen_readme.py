#!/usr/bin/env python
# -*- coding: utf-8 -*-
import os
import re
from datetime import datetime
from json import load

# Scan c++ source code to generate README
# The following marks in source code comments will be detected and used

"""
<<this line will be replaced by hyper link to the source code at this line>>

//README START

Mark down contents

/*README END*/

//<<
void c_code_that_will_be_included_in_code_blocks_in_README()
{

}
//>>


/*README START

more mark down contents
//README END
"""


CC = frozenset(['.cc', '.cpp', '.h', '.hh', 'hpp'])
TOP = os.path.dirname(os.path.realpath(__file__))

MD_START = re.compile(r'README *START')
MD_END = re.compile(r'README *END')
LINK = re.compile(r'<<(.*)>>')
CODE_START = re.compile(r'^ *//<< *$')
CODE_END = re.compile(r'^ *//>> *$')

CPP_SINGLE_COMMENT = re.compile(r'^ *//.*$')
CPP_COMMENT_OPEN = re.compile(r'^ */\*.*$')
CPP_COMMENT_CLOSE = re.compile(r'.*\*/.*$')

# set of (level, title)
HEADERS_WRITTEN = set()

class Comment:
    def __init__(self) -> None:
        self.inside_comment_block = False

    def is_comment_line(self, line) -> bool:
        is_open = re.match(CPP_COMMENT_OPEN, line) is not None
        is_close = re.match(CPP_COMMENT_CLOSE, line) is not None
        if is_open or is_close:
            # print(f"C2 {is_open} {is_close}")
            self.inside_comment_block = is_open
            return True
        elif self.inside_comment_block or re.match(CPP_SINGLE_COMMENT, line):
            # print(f"C1 {line}")
            return True
        return False
    
    def remove_comment_char(self, line) -> str:
        # todo: use python 3.10 pattern match
        if re.match(CPP_SINGLE_COMMENT, line):
            return line.replace('//', '')
        elif re.match(CPP_COMMENT_OPEN, line):
            return line.replace('/*', '')
        elif re.match(CPP_COMMENT_CLOSE, line):
            return line.replace('*/', '')
        else:
            return line


def get_files():
    for dirpath, _ , files in os.walk(os.path.join(TOP, 'src')):
        if TOP == dirpath or len(files) == 0:
            continue
        for f in files:
            fn, fe = os.path.splitext(f)
            if '.' in fn or '#' in fn:
                continue
            if fe in CC:
                yield os.path.join(dirpath, f)


def scrap_markdown_contents(source):
    """
    Return a list of (original line No. , line text)
    """
    print("Scan {} ".format(source))
    contents = []
    recording = False

    def add_md_open_close_line(l, ln, pattern):
        contents.append((ln, re.sub(pattern, '', l)))

    with open(source, 'r') as f:
        lineno = 1
        for line in f:
            if re.search(MD_START, line):
                recording = True
                add_md_open_close_line(line, lineno, MD_START)
            elif re.search(MD_END, line):
                add_md_open_close_line(line, lineno, MD_END)
                recording = False
            elif recording:
                contents.append((lineno, line))
            lineno += 1

    return contents if len(contents) > 0 else None


def parse_folder_meta_file(meta_file):
    payload = None
    with open(meta_file, 'rt') as f:
        payload = load(f)
    return payload


def get_folder_meta(path):
    meta_file = os.path.join(path, ".dir_meta.json")
    return parse_folder_meta_file(meta_file) if os.path.exists(meta_file) else None


def make_headers(paths):
    # one lever for each sub-folder
    level = 1
    headers = []
    paths = paths.split(os.sep)
    path = paths[0]

    # decide title: ignore src folder
    for p in paths[1:]:
        path = os.path.join(path, p)
        meta = get_folder_meta(path)
        title = meta['title'] if meta and 'title' in meta else p
        header = (level, title)
        if header not in HEADERS_WRITTEN:
            HEADERS_WRITTEN.add(header)
            headers.append('#' * level + ' ' + title + '\n')
        level += 1

    return level - 1, headers


def decorate_content(src, content):
    CODE_BLOCK_START = "```c++\n"
    CODE_BLOCK_END = "```\n"
    dir = os.path.dirname(src)
    level, decorated = make_headers(dir)
    inside_code_block = False

    c = Comment()
    for lineno, line in content:
        if line[0] == '#':
            line = '#' * level + line

        # is there LINK mark?
        search = re.search(LINK, line)
        if search:
            line = re.sub(LINK, "[{}]({}#L{})".format(search.group(1), src, lineno),
                          line)
        elif re.match(CODE_START, line):
            inside_code_block = True
            if decorated[-1] == CODE_BLOCK_END:
                # two conesctive code blocks
                del decorated[-1]
                continue
            line = CODE_BLOCK_START
        elif re.match(CODE_END, line):
            line = CODE_BLOCK_END
            inside_code_block = False
            decorated.append(line)
            continue

        if not inside_code_block:
            if c.is_comment_line(line):
                line = c.remove_comment_char(line)
                # print(f'REMOVED COMMET CHAR {line}')
            else:
                # print(f'IGNORE {line}')
                continue

        decorated.append(line)
    return decorated


def main():
    readme = os.path.join(TOP, 'README.md')

    with open(readme, 'w+t') as f:
        f.write('*Generated at {} by [gen_readme.py](gen_readme.py)*\n'.format(datetime.now().strftime('%c')))
        with open(os.path.join(TOP, '_readme'), 'rt') as template:
            f.write(template.read())
        for source_file in get_files():
            src = source_file.replace(TOP + os.sep, '')
            content = scrap_markdown_contents(src)
            if content:
                print(f"\tFound MD content in {src}")
                # print(content)
                f.writelines(decorate_content(src, content))


if __name__ == "__main__":
    main()
