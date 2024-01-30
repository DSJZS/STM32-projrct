/***************************************************************************************
  * 本源文件含有MPU6050相关函数
  * 大部分函数为官方DMP库自带
  ***************************************************************************************
  */
  
#include "MPU6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "math.h"
#include "./systick/bsp_SysTick.h"





/*参数宏定义*********************/

#define DEFAULT_MPU_HZ  ( 80 )              //  获取数据的频率，太大回导致溢出，无法获取数据
#define Q30             ( 1073741824.0f )   //  用于对Q30格式的数据进行转换

/*********************参数宏定义*/





/*全局变量*********************/

struct platform_data_s {                           //  官方DMP库带的，不了解，也不用管
    signed char orientation[9];
};

static struct platform_data_s gyro_pdata = {    //  官方DMP库带的，不了解，也不用管
		
		 .orientation = { 1, 0, 0,
                     0, 1, 0,
                     0, 0, 1}
};

/*********************全局变量*/





/*功能函数*********************/

/**
  * 函    数：官方DMP库带的函数，不了解，也不用管
  * 参    数：不了解，也不用管
  * 返 回 值：不了解，也不用管
  * 说    明：不了解，也不用管
  */
static unsigned short inv_row_2_scale(const signed char *row)
{
    unsigned short b;

    if (row[0] > 0)
        b = 0;
    else if (row[0] < 0)
        b = 4;
    else if (row[1] > 0)
        b = 1;
    else if (row[1] < 0)
        b = 5;
    else if (row[2] > 0)
        b = 2;
    else if (row[2] < 0)
        b = 6;
    else
        b = 7;		// error
    return b;
}

/**
  * 函    数：官方DMP库带的函数，不了解，也不用管
  * 参    数：不了解，也不用管
  * 返 回 值：不了解，也不用管
  * 说    明：不了解，也不用管
  */
unsigned short inv_orientation_matrix_to_scalar(const signed char *mtx)
{

    unsigned short scalar;

    /*
       XYZ  010_001_000 Identity Matrix
       XZY  001_010_000
       YXZ  010_000_001
       YZX  000_010_001
       ZXY  001_000_010
       ZYX  000_001_010
     */

    scalar = inv_row_2_scale(mtx);
    scalar |= inv_row_2_scale(mtx + 3) << 3;
    scalar |= inv_row_2_scale(mtx + 6) << 6;


    return scalar;
}


/**
  * 函    数：MPU6050自检函数
  * 参    数：无
  * 返 回 值：成功返回0，不成功返回非0
  * 说    明：官方DMP库带的函数，不了解，也不用管
  */
static int run_self_test(void)
{
    int result;
    long gyro[3], accel[3];

    result = mpu_run_self_test(gyro, accel);

    if (result == 0x3) {

    	unsigned short accel_sens;
    	float gyro_sens;
        mpu_get_gyro_sens(&gyro_sens);
		gyro[0] = (long) (gyro[0] * gyro_sens);
		gyro[1] = (long) (gyro[1] * gyro_sens);
		gyro[2] = (long) (gyro[2] * gyro_sens);
		dmp_set_gyro_bias(gyro);
		mpu_get_accel_sens(&accel_sens);
		accel[0] *= accel_sens;
		accel[1] *= accel_sens;
		accel[2] *= accel_sens;
		dmp_set_accel_bias(accel);
    }
    else {
           return -1;
     }
    return 0;
}

/**
  * 函    数：配置MPU6050
  * 参    数：无
  * 返 回 值：成功返回0，不成功返回非0
  * 说    明：无
  */
int MPU6050_DMP_Config(void)
{
    int result;
    struct int_param_s int_param;
    
    result = mpu_init(&int_param);  //  mpu初始化
    if(result != 0)
    {
        return -1;
    }
    
    result = mpu_set_sensors(INV_XYZ_GYRO | INV_XYZ_ACCEL); //  设置传感器
    if(result != 0)
    {
        return -2;
    }
    
    result = mpu_configure_fifo(INV_XYZ_GYRO | INV_XYZ_ACCEL);  //  设置fifo
    if(result != 0)
    {
        return -3;
    }
    
    result = mpu_set_sample_rate(DEFAULT_MPU_HZ);   //  设置采样率
    if(result != 0)
    {
        return -4;
    }
    
    result = dmp_load_motion_driver_firmware(); //  加载DMP固件
    if(result != 0)
    {
        return -5;
    }
    
    result = dmp_set_orientation(
     inv_orientation_matrix_to_scalar(gyro_pdata.orientation)); //  设置陀螺仪方向
    if(result != 0)
    {
        return -6;
    }
    
    result = dmp_enable_feature(    
        DMP_FEATURE_6X_LP_QUAT | DMP_FEATURE_TAP |
        DMP_FEATURE_ANDROID_ORIENT | DMP_FEATURE_SEND_RAW_ACCEL | 
        DMP_FEATURE_SEND_CAL_GYRO | DMP_FEATURE_GYRO_CAL);  //  设置dmp功能
    if(result != 0)
    {
        return -7;
    }
    
    result = dmp_set_fifo_rate(DEFAULT_MPU_HZ); //  设置输出速率
    if(result != 0)
    {
        return -8;
    }
    
    run_self_test();    //  自检
    if(result != 0)
    {
        return -9;
    }
    
    result = mpu_set_dmp_state(1);  //  使能dmp功能
    if(result != 0)
    {
        return -10;
    }
    
    return 0;
}

/**
  * 函    数：MPU6050初始化
  * 参    数：无
  * 返 回 值：无
  * 说    明：无
  */
void MPU6050_DMP_Init(void)
{
    int res = 0;
    res = MPU6050_DMP_Config();
    if(res != 0)
    {
        MPU6050_ERROR("MPU6050 Init ERR! res = %d", res);
        while(res)
        {
            delay_ms(1000);
            res = MPU6050_DMP_Config();
            MPU6050_ERROR("MPU6050 Init ERR! res = %d", res);
        }
    }else
    {
        MPU6050_INFO("MPU6050 Init SUCCEED");
    }
}

/**
  * 函    数：获取角度数据
  * 参    数：pitch        存放俯仰角数据的变量的指针
  * 参    数：roll         存放横滚角数据的变量的指针
  * 参    数：yaw          存放偏航角数据的变量的指针
  * 返 回 值：读取成功返回0，否则返回-1
  * 说    明：由于硬件问题，偏航角不准确，误差较大
  */
int MPU6050_DMP_Get_Data( float* pitch, float* roll, float* yaw)
{
    float q0 = 0.0f;
    float q1 = 0.0f;
    float q2 = 0.0f;
    float q3 = 0.0f;
    short gyro[3];
    short accel[3];
    long quat[4];
    unsigned long timestamp;
    short sensors;
    unsigned char more;
    if( dmp_read_fifo( gyro, accel, quat, &timestamp, &sensors, &more) )
    {
        return -1;
    }
    
    if( sensors & INV_WXYZ_QUAT )
    {
        q0 = quat[0] / Q30;
        q1 = quat[1] / Q30;
        q2 = quat[2] / Q30;
        q3 = quat[3] / Q30;
        
        *pitch = asin(-2 * q1* q3 + 2 * q0 * q2) * 57.3;
        *roll = atan2( 2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2 * q2 + 1) * 57.3;
        *yaw  = atan2( 2 * (q1 * q2 + q0 * q3), q0 * q0 + q1 * q1 - q2 * q2 - q3 * q3) * 57.3;
    }
    
    return 0;
}

/*********************功能函数*/
