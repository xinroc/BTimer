#define BOOST_CHRONO_HEADER_ONLY
#include <boost/asio.hpp>
#include <boost/chrono.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <iostream>
#include "timer.h"


typedef boost::asio::basic_waitable_timer<boost::chrono::high_resolution_clock> HighResolutionTimer;
typedef boost::chrono::milliseconds Milliseconds;
typedef boost::system::error_code BSErrorCode;
typedef boost::shared_ptr<boost::asio::io_service::work> BWorkPtr;
typedef boost::asio::strand BStrand;
typedef boost::shared_ptr<boost::asio::strand> BStrandPtr;
class BTimer::IMPL
{
public:
    IMPL(unsigned int nInterval,TimerHandler hander,bool bContinuous);
    ~IMPL();
    void TimerFunction(const BSErrorCode &error);


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
    unsigned int m_nInterval;//定时器间隔，单位，毫秒 
    boost::asio::io_service m_io;
    TimerHandler m_handler;//回调函数
    BWorkPtr m_pWork;
    BStrandPtr m_pStrand;
    HighResolutionTimer  m_Timer;
    boost::shared_ptr<boost::thread>  m_pthread;
    bool m_bContinuous;

};


BTimer::IMPL::IMPL(unsigned int nInterval, TimerHandler hander,
                   bool bContinuous) :m_nInterval(nInterval),
    m_handler(hander),m_Timer(m_io)
{
    m_bContinuous = bContinuous;
    m_pWork = BWorkPtr(new boost::asio::io_service::work(m_io));
    m_pStrand = BStrandPtr(new boost::asio::strand(m_io));
    m_pthread =  boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(
                                                                        &boost::asio::io_service::run,&m_io)));
}


BTimer::IMPL::~IMPL()
{
    try
    {
        boost::system::error_code ec;
        m_Timer.cancel(ec);
        m_io.stop();
    }
    catch(boost::system::error_code &e)
    {

    }
}

/**
 * @brief Timer::IMPL::TimerFunction 定时器执行函数
 * @param e
 */
void BTimer::IMPL::TimerFunction(const BSErrorCode &error)
{
    if(error)
    {
        return;
    }

    //未到定时器执行时间，一般是使用了cancel，即stop了定时器
    if (m_Timer.expires_at() > boost::chrono::high_resolution_clock::now())
    {
        return;
    }

    if(m_handler)
    {
        m_handler();
        if(m_bContinuous)
        {
            StartTimer();
        }
    }

}

void BTimer::IMPL::StartTimer()
{
    Milliseconds dur(m_nInterval);
    m_Timer.expires_from_now(dur);
    m_Timer.async_wait(m_pStrand->wrap(
                                       boost::bind(&BTimer::IMPL::TimerFunction,
                                                   this,
                                                   boost::asio::placeholders::error)));
}

void BTimer::IMPL::StopTimer()
{
    try
    {
        boost::system::error_code ec;
        m_Timer.cancel_one(ec);
    }
    catch(boost::system::error_code &e)
    {

    }
}

void BTimer::IMPL::ReStartTimer()
{
    StopTimer();
    StartTimer();
}


/**
 * @brief Timer 构造函数
 * @param nInterval 时间间隔
 * @param hander 执行函数
 */
BTimer::BTimer(unsigned int nInterval, TimerHandler hander, bool bContinuous)
{
    m_pimpl = boost::shared_ptr< IMPL>(new IMPL(nInterval,hander,bContinuous));
}



BTimer::~BTimer()
{

}

/**
 * @brief Timer::StartTimer 执行定时器
 * 定时器到期时执行一次
 */
void BTimer::StartTimer()
{
    m_pimpl->StartTimer();
}

/**
 * @brief Timer::StopTimer 定时器结束
 */
void BTimer::StopTimer()
{
   m_pimpl->StopTimer();

}

/**
 * @brief Timer::ReStartTimer 重启定时器
 */
void BTimer::ReStartTimer()
{
   m_pimpl->ReStartTimer();
}

