#ifndef COND_LOG_H
#define COND_LOG_H
namespace Utils {

template<typename OSTREAM>
class conditional_log{
private:
    OSTREAM& os_;
    bool log_;
public:
    conditional_log(bool cond, OSTREAM& os = std::cout):os_(os), log_(cond){}
    template<typename T>
    conditional_log& operator << (T value) {
        if(log_){
            os_<<value;
        }
        return *this;
    }
};

}
#endif