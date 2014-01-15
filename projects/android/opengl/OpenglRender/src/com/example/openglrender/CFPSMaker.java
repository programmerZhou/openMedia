package com.example.openglrender;

import java.text.DecimalFormat;  
/**  
 *  
 */  
/** 
 * <p>Title: LoonFramework</p> 
 * <p>Description:</p> 
 * <p>Copyright: Copyright (c) 2007</p> 
 * <p>Company: LoonFramework</p> 
 * @author chenpeng    
 * @email ceponline@yahoo.com.cn  
 * @version 0.1 
 * 
 */  
public class CFPSMaker  
{  
    /** 
     * 设定动画运行多少帧后统计�?��帧数  
     */  
    public static final int FPS = 8;    
   
    /** 
     * 换算为运行周�?
     * 单位: ns(纳秒) 
     */  
    public static final long PERIOD = (long) (1.0 / FPS * 1000000000);   
    /** 
     * FPS�?��间隔时间，换算为1s = 10^9ns  
     * 单位: ns 
     */  
    public static long FPS_MAX_INTERVAL = 1000000000L;   
      
    /** 
     * 实际的FPS数�? 
     */  
    private double nowFPS = 0.0;  
      
    /** 
     * FPS累计用间距时�?
     * in ns 
     */  
    private long interval = 0L;  
    private long time;  
    /** 
     * 运行桢累�? 
     */  
    private long frameCount = 0;  
      
    /** 
     * 格式化小数位�? 
     */  
    private DecimalFormat df = new DecimalFormat("0.0");   
      
    /** 
     * 制�?FPS数据 
     *  
     */  
    public void makeFPS()  
    {  
        frameCount++;  
        interval += PERIOD;  
        //当实际间隔符合时间时�? 
        if (interval >= FPS_MAX_INTERVAL)  
        {  
            //nanoTime()返回�?��确的可用系统计时器的当前值，以毫微秒为单�? 
            long timeNow = System.nanoTime();  
            // 获得到目前为止的时间距离  
            long realTime = timeNow - time; // 单位: ns  
            //换算为实际的fps数�?  
            nowFPS = ((double) frameCount / realTime) * FPS_MAX_INTERVAL;  
              
            //变更数�?  
            frameCount = 0L;  
            interval = 0L;  
            time = timeNow;  
        }  
    }  
      
    public long getFrameCount()  
    {  
        return frameCount;  
    }  
      
    public void setFrameCount(long frameCount)  
    {  
        this.frameCount = frameCount;  
    }  
      
    public long getInterval()  
    {  
        return interval;  
    }  
      
    public void setInterval(long interval)  
    {  
        this.interval = interval;  
    }  
      
    public double getNowFPS()  
    {  
        return nowFPS;  
    }  
      
    public void setNowFPS(double nowFPS)  
    {  
        this.nowFPS = nowFPS;  
    }  
      
    public long getTime()  
    {  
        return time;  
    }  
      
    public void setTime(long time)  
    {  
        this.time = time;  
    }  
      
    public String getFPS()  
    {  
        return df.format(nowFPS);  
    }  
}  
