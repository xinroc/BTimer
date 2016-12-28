

#ifndef TIMER_H
#define TIMER_H
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
//定时器执行函数类型
typedef boost::function<void  (void)> TimerHandler;

/**
 * @brief The Timer class 定时器类
 */
class BTimer
{
public:

    /**
     * @brief Timer 构造函数
     * @param nInterval 时间间隔
     * @param hander 执行函数
     */
    BTimer(unsigned int nInterval,TimerHandler hander,bool bContinuous = false);
    ~BTimer();

    /**
     * @brief StartTimer 启动定时器 只运行一次
     */
    void StartTimer();

    /**
     * @brief StopTimer   ///定时器暂停，无论对于连续还是只运行一次的定时器，都停止运行
     */
    void StopTimer();


    /**
     * @brief ReStartTimer 重新启动定时器，相当于StopTimer+StartTimer
     */
    void ReStartTimer();


private:
    class IMPL;
    boost::shared_ptr< IMPL>  m_pimpl;
};

#endif // TIMER_H
