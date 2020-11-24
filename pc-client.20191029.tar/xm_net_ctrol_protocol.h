#ifndef _XM_NET_CTROL_PROTOCOL_H_
#define _XM_NET_CTROL_PROTOCOL_H_

#ifdef __cplusplus
extern "C" {
#endif
#pragma pack(push) //让原字节对齐方式入编译器栈
#pragma pack(1)

#include "hi_type.h"

#define XM_SYS_VER (0x01000003)

#define XM_SUCCESS (1)
#define XM_FAILED (-1)

#define XM_FRAME_SEND_PACKET_LEN (10 * 1024)

#define XM_LISTEN_PORT (11666)

        typedef enum {
                IOCTRL_TYPE_UNKN,
                //现场视频音频请求
                IOCTRL_TYPE_LIVE_START_REQ,
                IOCTRL_TYPE_LIVE_START_RESP,
                //现场音视频关闭
                IOCTRL_TYPE_LIVE_STOP_REQ,
                IOCTRL_TYPE_LIVE_STOP_RESP,

                //音频对讲
                IOCTRL_TYPE_LIVE_AUDIO_SPEAK_START_REQ,  //5
                IOCTRL_TYPE_LIVE_AUDIO_SPEAK_START_RESP,
                //对讲关闭
                IOCTRL_TYPE_LIVE_AUDIO_SPEAK_STOP_REQ,
                IOCTRL_TYPE_LIVE_AUDIO_SPEAK_STOP_RESP,

                //无线部分
                //获取设备AP参数
                IOCTRL_TYPE_GET_AP_PARAMETERS_REQ,
                IOCTRL_TYPE_GET_AP_PARAMETERS_RESP,//10
                //设置设备AP参数
                IOCTRL_TYPE_SET_AP_PARAMETERS_REQ,
                IOCTRL_TYPE_SET_AP_PARAMETERS_RESP,
                //获取设备STA参数
                IOCTRL_TYPE_GET_STA_PARAMETERS_REQ,
                IOCTRL_TYPE_GET_STA_PARAMETERS_RESP,
                //设置设备STA参数
                IOCTRL_TYPE_SET_STA_PARAMETERS_REQ,//15
                IOCTRL_TYPE_SET_STA_PARAMETERS_RESP,
                //获取设备扫描的wifi 列表
                IOCTRL_TYPE_LIST_WIFI_AP_REQ,
                IOCTRL_TYPE_LIST_WIFI_AP_RESP,

                //云台部分
                IOCTRL_TYPE_PTZ_COMMAND_REQ,
                IOCTRL_TYPE_PTZ_COMMAND_RESP,//20

                //录像文件回放下载部分
                IOCTRL_TYPE_LIST_RECORDFILES_REQ,
                IOCTRL_TYPE_LIST_RECORDFILES_RESP,

                IOCTRL_TYPE_DOWNLOAD_RECORD_FILE_REQ,
                IOCTRL_TYPE_DOWNLOAD_RECORD_FILE_RESP,

                //视频回放控制
                IOCTRL_TYPE_RECORD_PLAYCONTROL_REQ,//25
                IOCTRL_TYPE_RECORD_PLAYCONTROL_RESP,

                //网络控制设备录像
                IOCTRL_TYPE_NET_TRIGGER_RECORD_START_REQ,
                IOCTRL_TYPE_NET_TRIGGER_RECORD_START_RESP,
                //获取当前录像状态
                IOCTRL_TYPE_NET_TRIGGER_RECORD_CHECK_REQ,
                IOCTRL_TYPE_NET_TRIGGER_RECORD_CHECK_RESP,//30
                //停止录像
                IOCTRL_TYPE_NET_TRIGGER_RECORD_STOP_REQ,
                IOCTRL_TYPE_NET_TRIGGER_RECORD_STOP_RESP,

                //设备推送
                IOCTRL_TYPE_PUSH_EVENT_REQ,
                IOCTRL_TYPE_PUSH_EVENT_RESP,
                //获取设备推送消息类型
                IOCTRL_TYPE_GET_PUSH_PARAMETERS_REQ,//35
                IOCTRL_TYPE_GET_PUSH_PARAMETERS_RESP,
                //设置设备推送消息类型
                IOCTRL_TYPE_SET_PUSH_PARAMETERS_REQ,
                IOCTRL_TYPE_SET_PUSH_PARAMETERS_RESP,


                //设备参数设置

                //获取设备系统版本
                IOCTRL_TYPE_GET_SYSFWVER_REQ,   //App to Device
                IOCTRL_TYPE_GET_SYSFWVER_RESP,  //Device to App //40
                //获取设备时间
                IOCTRL_TYPE_GET_DEV_TIME_REQ,
                IOCTRL_TYPE_GET_DEV_TIME_RESP,
                //设置设备时间
                IOCTRL_TYPE_SET_DEV_TIME_REQ,
                IOCTRL_TYPE_SET_DEV_TIME_RESP,

                IOCTRL_TYPE_CHANGE_DEV_PASSWORD_REQ,//45
                IOCTRL_TYPE_CHANGE_DEV_PASSWORD_RESP,

                IOCTRL_TYPE_GET_DEV_INTERNET_FLAG_REQ,
                IOCTRL_TYPE_GET_DEV_INTERNET_FLAG_RESP,

                IOCTRL_TYPE_SET_DEV_INTERNET_FLAG_REQ,
                IOCTRL_TYPE_SET_DEV_INTERNET_FLAG_RESP,//50


                // 图像参数设置
                IOCTRL_TYPE_GET_DEV_IMAGE_PARAMETERS_REQ,
                IOCTRL_TYPE_GET_DEV_IMAGE_PARAMETERS_RESP,

                IOCTRL_TYPE_SET_DEV_IMAGE_PARAMETERS_REQ,
                IOCTRL_TYPE_SET_DEV_IMAGE_PARAMETERS_RESP,


                // 视频参数设置
                IOCTRL_TYPE_GET_DEV_VIDEO_PARAMETERS_REQ,//55
                IOCTRL_TYPE_GET_DEV_VIDEO_PARAMETERS_RESP,

                IOCTRL_TYPE_SET_DEV_VIDEO_PARAMETERS_REQ,
                IOCTRL_TYPE_SET_DEV_VIDEO_PARAMETERS_RESP,


                //报警设置
                IOCTRL_TYPE_GET_DEVICE_ALARM_REQ,
                IOCTRL_TYPE_GET_DEVICE_ALARM_RESP,//60
                IOCTRL_TYPE_SET_DEVICE_ALARM_REQ,
                IOCTRL_TYPE_SET_DEVICE_ALARM_RESP,


                //录像参数设置
                IOCTRL_TYPE_GET_RECORD_PARAMETERS_REQ,
                IOCTRL_TYPE_GET_RECORD_PARAMETERS_RESP,

                IOCTRL_TYPE_SET_RECORD_PARAMETERS_REQ,//65
                IOCTRL_TYPE_SET_RECORD_PARAMETERS_RESP,


                //验证密码 &  授权
                IOCTRL_TYPE_AUTHORIZE_REQ,
                IOCTRL_TYPE_AUTHORIZE_RESP,

                //设备升级
                IOCTRL_TYPE_UPGRADE_REQ,               //App to Device, data: upgrade file size,
                IOCTRL_TYPE_UPGRADE_READY,      //Device to App, Device is ready for receiving upgrade firmware. App begin to send data
                IOCTRL_TYPE_UPGRADE_QUERY,      //App to Device, Query device to upgrade firmware ok ? after sending upgrade data over
                IOCTRL_TYPE_UPGRADE_OK,         //Device to App
                IOCTRL_TYPE_UPGRADE_FAILED,     //Device to App


                //控制门锁
                IOCTRL_TYPE_DOORBELL_OPEN_REQ,
                IOCTRL_TYPE_DOORBELL_OPEN_RESP,     //75


                // SD卡 TF卡 操作
                IOCTRL_TYPE_SET_DEVICE_SDFORMAT_REQ,
                IOCTRL_TYPE_SET_DEVICE_SDFORMAT_RESP,

                IOCTRL_TYPE_SDFORMAT_QUERY_REQ,
                IOCTRL_TYPE_SDFORMAT_QUERY_RESP,

                IOCTRL_TYPE_GET_SD_INFO_REQ,//80
                IOCTRL_TYPE_GET_SD_INFO_RESP,

                //局域网RTSP 操作
                IOCTRL_TYPE_GET_RTSP_INFO_REQ,
                IOCTRL_TYPE_GET_RTSP_INFO_RESP,


                //SUMMER 方案 a115
                //外设云台& 温度 peripheral equipment生词本
                IOCTRL_TYPE_GET_PERIPHERAL_EQUIPMENT_INFO_REQ,
                IOCTRL_TYPE_GET_PERIPHERAL_EQUIPMENT_INFO_RESP,  //85

                //设置设备工作频率 50hz  or 60hz
                IOCTRL_TYPE_GET_FREQUENCY_INFO_REQ,
                IOCTRL_TYPE_GET_FREQUENCY_INFO_RESP,

                IOCTRL_TYPE_SET_FREQUENCY_INFO_REQ,
                IOCTRL_TYPE_SET_FREQUENCY_INFO_RESP,

                /********************************  行车记录仪部分***************************/
                //文件查询
                IOCTRL_TYPE_DC_GET_FILE_LIST_REQ = 200,
                IOCTRL_TYPE_DC_GET_FILE_LIST_RESP,

                //当月存储信息查询
                IOCTRL_TYPE_DC_CHECK_SAVE_INFO_BY_MONTH_REQ, //查询一个月中哪些天有存储数据
                IOCTRL_TYPE_DC_CHECK_SAVE_INFO_BY_MONTH_RESP,

                //删除文件
                IOCTRL_TYPE_DC_DELETE_FILE_REQ,
                IOCTRL_TYPE_DC_DELETE_FILE_RESP, //5

                //下载文件
                IOCTRL_TYPE_DC_DOWNLOAD_FILE_REQ,
                IOCTRL_TYPE_DC_DOWNLOAD_FILE_RESP,

                //在线回放
                IOCTRL_TYPE_DC_ONLINE_PLAY_FILE_REQ,
                IOCTRL_TYPE_DC_ONLINE_PLAY_FILE_RESP,

                //IOCTRL_TYPE_DC_ONLINE_PLAY_CTRL_REQ,
                //IOCTRL_TYPE_DC_ONLINE_PLAY_CTRL_RESP,

                //实时拍照
                IOCTRL_TYPE_DC_SNAP_REQ,//10
                IOCTRL_TYPE_DC_SNAP_RESP,


                //电子狗
                IOCTRL_TYPE_DC_ELECTRONIC_DOG_GET_REQ,
                IOCTRL_TYPE_DC_ELECTRONIC_DOG_GET_RESP,

                IOCTRL_TYPE_DC_ELECTRONIC_DOG_SET_REQ,
                IOCTRL_TYPE_DC_ELECTRONIC_DOG_SET_RESP,//15

                //设置串口速度
                IOCTRL_TYPE_DC_SET_UART_REQ,
                IOCTRL_TYPE_DC_SET_UART_RESP,

                //请求串口控制
                IOCTRL_TYPE_DC_REQUEST_UART_CTRL_REQ,
                IOCTRL_TYPE_DC_REQUEST_UART_CTRL_RESP,

                //串口数据发送   设备-> app
                IOCTRL_TYPE_DC_SEND_UART_DATA_REQ,//20
                IOCTRL_TYPE_DC_SEND_UART_DATA_RESP,

                //串口数据接收   app -> 设备
                IOCTRL_TYPE_DC_WRITE_UART_DATA_REQ,
                IOCTRL_TYPE_DC_WRITE_UART_DATA_RESP,

                //停止下载文件
                IOCTRL_TYPE_DC_STOP_DOWNLOAD_FILE_REQ,
                IOCTRL_TYPE_DC_STOP_DOWNLOAD_FILE_RESP,

                //获取版本号接口2
                IOCTRL_TYPE_GET_DEVAPPVER_REQ,
                IOCTRL_TYPE_GET_DEVAPPVER_RESP,


                //时间设置接口2
                IOCTRL_TYPE_GET_DEV_TIME2_REQ,
                IOCTRL_TYPE_GET_DEV_TIME2_RESP,

                IOCTRL_TYPE_SET_DEV_TIME2_REQ,//30
                IOCTRL_TYPE_SET_DEV_TIME2_RESP,

                //上传抓拍图片或视频文件的信息 mac -> app
                IOCTRL_TYPE_SEND_SNAP_FILE_INFO_REQ,
                IOCTRL_TYPE_SEND_SNAP_FILE_INFO_RESP,

                //产生临时访问密码
                IOCTRL_TYPE_DC_CREATE_TMP_CONNECT_PASSWORD_REQ,
                IOCTRL_TYPE_DC_CREATE_TMP_CONNECT_PASSWORD_RESP,

                //上传删除文件的信息 mac -> app
                IOCTRL_TYPE_SEND_DEL_FILE_INFO_REQ,
                IOCTRL_TYPE_SEND_DEL_FILE_INFO_RESP,

                //获取tf 卡上具体文件信息
                IOCTRL_TYPE_DC_GET_TFCARD_INFO_REQ,
                IOCTRL_TYPE_DC_GET_TFCARD_INFO_RESP,

                //设置设备对APP 发送实时GPS+OBD的数据
                IOCTRL_TYPE_GET_GPS_OBD_SEND_PARAMETERS_REQ,//40
                IOCTRL_TYPE_GET_GPS_OBD_SEND_PARAMETERS_RESP,

                IOCTRL_TYPE_SET_GPS_OBD_SEND_PARAMETERS_REQ,
                IOCTRL_TYPE_SET_GPS_OBD_SEND_PARAMETERS_RESP,

                //上传GPS OBD 实时数据 mac -> app
                IOCTRL_TYPE_SEND_GPS_OBD_REQ,
                IOCTRL_TYPE_SEND_GPS_OBD_RESP,

                //得到所有相关数据的版本号
                IOCTRL_TYPE_GET_DEV_ALL_APPVER_REQ,
                IOCTRL_TYPE_GET_DEV_ALL_APPVER_RESP,

                //通知APP  是否有人经过 ，展会功能(设备主动发送给APP，APP不需要应答)
                IOCTRL_TYPE_SEND_PIR_INFO_REQ,
                IOCTRL_TYPE_SEND_PIR_INFO_RESP,


                //APP to Device, 停车监控模式变更、获取   //20160303
                IOCTRL_TYPE_SET_PARKING_MONITOR_MODE_REQ,//50
                IOCTRL_TYPE_SET_PARKING_MONITOR_MODE_RESP,
                IOCTRL_TYPE_GET_PARKING_MONITOR_MODE_REQ,
                IOCTRL_TYPE_GET_PARKING_MONITOR_MODE_RESP,

                //APP to Device, 要求设备重启
                IOCTRL_TYPE_REBOOT_DEVICE_REQ,
                IOCTRL_TYPE_REBOOT_DEVICE_RESP,

                //APP to Device, 让设备马达左、右转动
                IOCTRL_TYPE_TURN_MOTO_REQ,
                IOCTRL_TYPE_TURN_MOTO_RESP,

                //APP to Device，令设备恢复出厂
                IOCTRL_TYPE_RESET_DEVICE_REQ,
                IOCTRL_TYPE_RESET_DEVICE_RESP,

                // Device to APP，发送当前听声辩位结论
                IOCTRL_TYPE_SEND_SOUND_POSITION_INFO_REQ,//60
                IOCTRL_TYPE_SEND_SOUND_POSITION_INFO_RESP,
                /*********************************************************************************/

                //APP TO Dev, 设置G-SENSER灵敏度，范围20-255
                IOCTRL_TYPE_SET_GSENSER_SENSE_LEVEL_REQ,
                IOCTRL_TYPE_SET_GSENSER_SENSE_LEVEL_RESP,
                // Dev to APP, 获取G-SENSER灵敏度
                IOCTRL_TYPE_GET_GSENSER_SENSE_LEVEL_REQ,
                IOCTRL_TYPE_GET_GSENSER_SENSE_LEVEL_RESP,

                // Dev to APP，设置听声辩位任意触发版的声音灵敏度，单位为DB值。范围:-100 - 100
                IOCTRL_TYPE_SET_SOUND_POSITION_SENSE_LEVEL_REQ,
                IOCTRL_TYPE_SET_SOUND_POSITION_SENSE_LEVEL_RESP,
                // APP to Dev，获取听声辩位任意触发版的声音灵敏度，单位为DB值。
                IOCTRL_TYPE_GET_SOUND_POSITION_SENSE_LEVEL_REQ,
                IOCTRL_TYPE_GET_SOUND_POSITION_SENSE_LEVEL_RESP,
                /*********************************************************************************/
                // APP to Dev，设置抓拍类型:一般抓拍一次一张；穿越拍照一次多张
                IOCTRL_TYPE_SET_SNAP_TYPE_REQ,//70
                IOCTRL_TYPE_SET_SNAP_TYPE_RESP,
                // APP to Dev，获取抓拍类型:一般抓拍一次一张；穿越拍照一次多张
                IOCTRL_TYPE_GET_SNAP_TYPE_REQ,
                IOCTRL_TYPE_GET_SNAP_TYPE_RESP,

                /*********************************************************************************/
                // APP to Dev, 打开类产测功能: 声音循环播。
                IOCTRL_TYPE_SET_FACT_TEST_AUDIO_LOOP_REQ,
                IOCTRL_TYPE_SET_FACT_TEST_AUDIO_LOOP_RESP,
                // APP to Dev, 获取类产测功能状态: 声音循环播。
                IOCTRL_TYPE_GET_FACT_TEST_AUDIO_LOOP_REQ,
                IOCTRL_TYPE_GET_FACT_TEST_AUDIO_LOOP_RESP,
                // Dev to App 得到哪些天有有效的数据
                IOCTRL_TYPE_DC_GET_DAYS_OF_VALID_DATA_REQ,
                IOCTRL_TYPE_DC_GET_DAYS_OF_VALID_DATA_RESP,
                ///获取和设置ISP信息
                IOCTRL_TYPE_DC_SET_ISP_FUNC_REQ,//80
                IOCTRL_TYPE_DC_SET_ISP_FUNC_RESP,
                IOCTRL_TYPE_DC_GET_ISP_FUNC_REQ,
                IOCTRL_TYPE_DC_GET_ISP_FUNC_RESP,

                /*********************************************************************************/
                IOCTRL_TYPE_SET_SOUND_RECORD_ONOFF_REQ,
                IOCTRL_TYPE_SET_SOUND_RECORD_ONOFF_RESP, // 85

                IOCTRL_TYPE_GET_SOUND_RECORD_ONOFF_REQ,
                IOCTRL_TYPE_GET_SOUND_RECORD_ONOFF_RESP,

                // 设置进入停车监控模式的延迟时间单位秒
                IOCTRL_TYPE_SET_PARKING_DELAY_TIME_REQ,
                IOCTRL_TYPE_SET_PARKING_DELAY_TIME_RESP,
                IOCTRL_TYPE_GET_PARKING_DELAY_TIME_REQ,
                IOCTRL_TYPE_GET_PARKING_DELAY_TIME_RESP,

		// L111
		IOCTRL_TYPE_GET_LED_REQ,
		IOCTRL_TYPE_GET_LED_RESP,
		IOCTRL_TYPE_SET_LED_REQ,
		IOCTRL_TYPE_SET_LED_RESP,
		IOCTRL_TYPE_GET_CBC_REQ,
		IOCTRL_TYPE_GET_CBC_RESP,
                IOCTRL_TYPE_BUTT
        } XM_IOCTRL_TYPE;

        /******************************************************************************************
          视频音频部分
         *******************************************************************************************/

        ///////////相关数据结构////////

        typedef enum {
                FRAME_TYPE_BASE = 0,
                FRAME_TYPE_VIDEO,           //视频数据帧
                FRAME_TYPE_AUDIO,           //音频数据帧
        } XM_FRAME_TYPE;


        typedef enum {
                VIDEO_FRAME_FLAG_I  = 0x00, // Video I Frame
                VIDEO_FRAME_FLAG_P  = 0x01, // Video P Frame
                VIDEO_FRAME_FLAG_B  = 0x02, // Video B Frame
        } XM_VIDEO_FRAME;

        typedef enum {
                ASAMPLE_RATE_8K = 0x00,
                ASAMPLE_RATE_11K = 0x01,
                ASAMPLE_RATE_12K = 0x02,
                ASAMPLE_RATE_16K = 0x03,
                ASAMPLE_RATE_22K = 0x04,
                ASAMPLE_RATE_24K = 0x05,
                ASAMPLE_RATE_32K = 0x06,
                ASAMPLE_RATE_44K = 0x07,
                ASAMPLE_RATE_48K = 0x08,
        } XM_AUDIO_SAMPLERATE;

        typedef enum {
                ADATABITS_8 = 0,
                ADATABITS_16 = 1,
        } XM_AUDIO_DATABITS;

        typedef enum {
                ACHANNEL_MONO   = 0,
                ACHANNEL_STEREO = 1,
        } XM_AUDIO_CHANNEL;

        typedef enum {
                CODECID_UNKN = 1,
                CODECID_V_MJPEG,
                CODECID_V_MPEG4,
                CODECID_V_H264,

                CODECID_A_PCM ,
                CODECID_A_ADPCM,
                CODECID_A_SPEEX,
                CODECID_A_AMR,
                CODECID_A_AAC,
                CODECID_A_G711A,
                CODECID_A_G726,
                CODECID_A_AC3,
                CODECID_A_MP3,
                CODECID_V_H265,
        } XM_CODE_TYPE;

        typedef enum {
                USE_TYPE_UNKN,
                USE_TYPE_LIVE,
                USE_TYPE_PLAYBACK,
                USE_TYPE_SPEAK,
        } XM_FRAME_USE_TYPE;

        ////////网络应答数据结构////////

        //IOCTRL_TYPE_LIVE_START_REQ,
        //IOCTRL_TYPE_LIVE_START_RESP,
        typedef struct _LIVE_START_REQ_ {
                HI_U8 u8EnableVideoSend; // 1: 发送视频，0 : 不发送视频
                HI_U8 u8EnableAudioSend; // 1. 发送音频.    0 : 不发送音频
                HI_U8 u8VideoChan; //请求视频通道数据， 0 为主码流720p，1 为子码流VGA。2 为QVGA
                HI_U8 u8Reserved[9];
        } XM_LIVE_START_REQ;

        typedef struct _LIVE_START_RESP_ {
                HI_S32 s32Result; // XM_SUCCESS: 成功  . 不成功则返回错误值
                HI_U8 u8Reserved[8];
        } XM_LIVE_START_RESP;

        //IOCTRL_TYPE_LIVE_STOP_REQ,
        //IOCTRL_TYPE_LIVE_STOP_RESP,
        typedef struct _LIVE_STOP_REQ_ {
                HI_U8 u8Reserved[8];
        } XM_LIVE_STOP_REQ;

        typedef struct _LIVE_STOP_RESP_ {
                HI_S32 s32Result; // XM_SUCCESS: 成功  . 不成功则返回错误值
                HI_U8 u8Reserved[8];
        } XM_LIVE_STOP_RESP;

        //IOCTRL_TYPE_LIVE_AUDIO_SPEAK_START_REQ,
        //IOCTRL_TYPE_LIVE_AUDIO_SPEAK_START_RESP,
        typedef struct _AUDIO_SPEAK_START_REQ {
                HI_U8 u8Reserved[8];
        } XM_AUDIO_SPEAK_START_REQ;

        typedef struct _AUDIO_SPEAK_START_RESP {
                HI_S32 s32Result; // s32Result = XM_SUCCESS:      授权成功，开始对讲
                // s32Result != XM_SUCCESS: 授权失败，返回错误原因
                HI_U8 u8Reserved[8];
        } XM_AUDIO_SPEAK_START_RESP;

        //IOCTRL_TYPE_LIVE_AUDIO_SPEAK_STOP_REQ,
        //IOCTRL_TYPE_LIVE_AUDIO_SPEAK_STOP_RESP,
        typedef struct _AUDIO_SPEAK_STOP_REQ {
                HI_U8 u8Reserved[8];
        } XM_AUDIO_SPEAK_STOP_REQ;

        typedef struct _AUDIO_SPEAK_STOP_RESP {
                HI_S32 s32Result;
                HI_U8 u8Reserved[8];
        } XM_AUDIO_SPEAK_STOP_RESP;

        ///////////////////////////////////////////////////////////////////////////////////////////

        /******************************************************************************************
          无线部分
         *******************************************************************************************/

        ////////相关数据结构////////
        typedef enum {
                IOCTRL_WIFIAPENC_INVALID = 0x00,
                IOCTRL_WIFIAPENC_NONE = 0x01,
                IOCTRL_WIFIAPENC_WEP_NO_PWD = 0x02,
                IOCTRL_WIFIAPENC_WEP_WITH_PWD = 0x03,
                IOCTRL_WIFIAPENC_WPA_TKIP = 0x04,
                IOCTRL_WIFIAPENC_WPA_AES = 0x05,
                IOCTRL_WIFIAPENC_WPA2_TKIP = 0x06,
                IOCTRL_WIFIAPENC_WPA2_AES = 0x07,
                IOCTRL_WIFIAPENC_WPA_PSK_AES = 0x08,
                IOCTRL_WIFIAPENC_WPA_PSK_TKIP = 0x09,
                IOCTRL_WIFIAPENC_WPA2_PSK_AES = 0x0A,
                IOCTRL_WIFIAPENC_WPA2_PSK_TKIP = 0x0B,
        } XM_WIFIAP_ENC;

#define MAX_NTP_LIST_ITEM_NUM (10)

        typedef struct _NTP_INFO_ST_ {
                HI_S8 ntp_server_url[256]; //ntp 服务器网址如 clock.via.net，机器默认网址为clock.via.net
                HI_S32 ntp_server_port; // ntp 服务器端口默认为123
        } NTP_INFO_ST;

        typedef struct _NTP_ADDR_LIST_ {
                NTP_INFO_ST ntp[MAX_NTP_LIST_ITEM_NUM]; //可以同时设置多组NTP 服务信息
                HI_S32 ntp_available_num; // ntp_available_num表示有几组有效的NTP信息， ntp_available_num 必须小于等于MAX_NTP_LIST_ITEM_NUM
        } NTP_ADDR_LIST;

        typedef struct _XM_LIST_AP_INFO_ {
                HI_S8 strAddress[20]; // 机器的ip地址
                HI_S8 strSsid[64]; // SSID
                HI_U16 u16Channel; // 信道
                HI_U8 u8Enctype; // 加密模式  参考XM_WIFIAP_ENC
                HI_U8 u8SignalLevel; // 信号强度，值的范围[0-100]，值越大信号越强。
                HI_U8 u8Reserve[8]; //保留
        } XM_LIST_AP_INFO;

        typedef struct {
                HI_S8 strSsid[64]; //WiFi ssid
                HI_S8 strPassword[64]; //WiFi 密码
                HI_U8 u8Enctype; // 加密模式 参考 XM_WIFIAP_ENC
                HI_U8 u8SignalChannel; //wifi  信道 [1,14]
                HI_U8 reserved[10];
                HI_S8 strIOTID[64]; // IOTID
        } XM_STA_INFO, XM_AP_INFO;

        ////////网络应答数据结构////////

        //IOCTRL_TYPE_GET_AP_PARAMETERS_REQ,
        //IOCTRL_TYPE_GET_AP_PARAMETERS_RESP,
        typedef struct _GET_AP_PARAMETERS_REQ {
                HI_U8 u8Reserved[8];
        } XM_GET_AP_PARAMETERS_REQ;

        typedef struct _GET_AP_PARAMETERS_RESP {
                HI_S32 s32Result; //s32Result = XM_SUCCESS: 获得参数成功，
                //s32Result != XM_SUCCESS: 获取失败,返回错误原因
                XM_AP_INFO stApInfo;
                HI_U8 u8Reserved[8];
        } XM_GET_AP_PARAMETERS_RESP;

        //IOCTRL_TYPE_SET_AP_PARAMETERS_REQ,
        //IOCTRL_TYPE_SET_AP_PARAMETERS_RESP,
        typedef struct _SET_AP_PARAMETERS_REQ {
                XM_AP_INFO stApInfo;
                HI_U8 u8Reserved[8];
        } XM_SET_AP_PARAMETERS_REQ;

        typedef struct _SET_AP_PARAMETERS_RESP {
                HI_S32 s32Result;
                HI_U8 u8Reserved[8];
        } XM_SET_AP_PARAMETERS_RESP;

        //IOCTRL_TYPE_GET_STA_PARAMETERS_REQ,
        //IOCTRL_TYPE_GET_STA_PARAMETERS_RESP,
        typedef struct _GET_STA_PARAMETERS_REQ {
                HI_U8 u8Reserved[8];
        } XM_GET_STA_PARAMETERS_REQ;

        typedef struct _GET_STA_PARAMETERS_RESP {
                HI_S32 s32Result; //s32Result = XM_SUCCESS: 获得参数成功，
                //s32Result != XM_SUCCESS: 获取失败,返回错误原因
                XM_STA_INFO stStaInfo;
                HI_U8 u8Reserved[8];
        } XM_GET_STA_PARAMETERS_RESP;

        //IOCTRL_TYPE_SET_STA_PARAMETERS_REQ,
        //IOCTRL_TYPE_SET_STA_PARAMETERS_RESP,
        typedef struct _SET_STA_PARAMETERS_REQ {
                XM_STA_INFO stStaInfo;
                HI_U8 u8Reserved[8];
        } XM_SET_STA_PARAMETERS_REQ;

        typedef struct _SET_STA_PARAMETERS_RESP {
                HI_S32 s32Result;
                HI_U8 u8Reserved[8];
        } XM_SET_STA_PARAMETERS_RESP;

        //IOCTRL_TYPE_LIST_WIFI_AP_REQ,
        //IOCTRL_TYPE_LIST_WIFI_AP_RESP,
        typedef struct _LIST_WIFI_AP_REQ {
                HI_U8 u8Reserved[8];
        } XM_LIST_WIFI_AP_REQ;

        typedef struct _LIST_WIFI_AP_RESP {
                HI_S32 s32Result;
                HI_U32 u32ApCount; //查找到的 AP 数量
                HI_U8 u8Reserved[8];
                XM_LIST_AP_INFO stApInfo[0]; //AP INFO 数据地址
        } XM_LIST_WIFI_AP_RESP;

        ///////////////////////////////////////////////////////////////////////////////////////////

        /******************************************************************************************
          云台部分
         *******************************************************************************************/

        ////////相关数据结构////////

        typedef enum {
                IOCTRL_PTZ_STOP,
                IOCTRL_PTZ_UP,
                IOCTRL_PTZ_DOWN,
                IOCTRL_PTZ_LEFT,
                IOCTRL_PTZ_RIGHT,
                IOCTRL_PTZ_BACK,
                IOCTRL_PTZ_FORWARD,
                IOCTRL_LENS_ZOOM_IN,
                IOCTRL_LENS_ZOOM_OUT,
        } XM_PTZ_CMD;

        ////////网络应答数据结构////////
        //IOCTRL_TYPE_PTZ_COMMAND_REQ,
        //IOCTRL_TYPE_PTZ_COMMAND_RESP,
        typedef struct _PTZ_COMMAND_REQ {
                XM_PTZ_CMD enPtzCmd; //PTZ 控制命令
                HI_U8 u8Speed; // PTZ 控制速度范围[1-10] ,值越大速度越快
                HI_U8 u8Reserved[7];
        } XM_PTZ_COMMAND_REQ;

        typedef struct _PTZ_COMMAND_RESP {
                HI_S32 s32Result;
                HI_U8 u8Reserved[8];
        } XM_PTZ_COMMAND_RESP;

        ///////////////////////////////////////////////////////////////////////////////////////////////////

        /******************************************************************************************
          网络录像文件部分
         *******************************************************************************************/

        ////////相关数据结构////////
        typedef struct _DAY_TIME_ {
                HI_U16 u16Year;
                HI_U8 u8Month; //January=0,...
                HI_U8 u8Day;
                HI_U8 u8Hour;
                HI_U8 u8Minute;
                HI_U8 u8Second;
                HI_U8 u8Reserved[1];
        } XM_DAY_TIME;

        typedef struct _XM_RECORD_FILE_ {
                HI_S8 strFileName[40]; //文件名字
                HI_U32 u32FileLen; //文件长度
        } XM_RECORD_FILE;

        // IOCTRL Record Playback Command
        typedef enum {
                IOCTRL_RECORD_PLAY_CMD_START, //command below is valid when IOCTRL_RECORD_PLAY_CMD_START is successful
                IOCTRL_RECORD_PLAY_CMD_PAUSE,
                IOCTRL_RECORD_PLAY_CMD_RESUME,
                IOCTRL_RECORD_PLAY_CMD_STOP,
                IOCTRL_RECORD_PLAY_CMD_END, //这个命令是指，文件播放到结尾点时，设备端向客户端发送命令。
                IOCTRL_RECORD_PLAY_CMD_FAST, // 1X, 2X, 4X
                IOCTRL_RECORD_PLAY_CMD_SEEKTIME,
        } XM_RECORD_PLAY_CMD;

        ////////网络应答数据结构////////
        //IOCTRL_TYPE_LIST_RECORDFILES_REQ,
        //IOCTRL_TYPE_LIST_RECORDFILES_RESP,
        typedef struct _LIST_RECORDFILES_REQ {
                XM_DAY_TIME stStartTime; // 录像搜索的开始时间
                XM_DAY_TIME stEndTime; // 录像搜索的结束时间
                HI_U8 u8Reserved[8];
        } XM_LIST_RECORDFILES_REQ;

        typedef struct _LIST_RECORDFILES_RESP {
                HI_S32 s32Result; //XM_SUCCESS: Success   XM_FAILED: search time don't belong to [stStartTime, stEndTime], within one day.
                HI_U32 u32SearchFileTotal; //符合查询条件的文件的总数量
                HI_U8 u8Count; // 当前包，包含的文件个数
                HI_U8 u8EndFlg; //  u8EndFlg = 1 表示当前包为最后一个发送包。
                HI_U8 u8Index; // 包的序列号
                HI_U8 u8Reserved[5];
                XM_RECORD_FILE stFile[0]; //文件信息的地址
        } XM_LIST_RECORDFILES_RESP;

        //IOCTRL_TYPE_DOWNLOAD_RECORD_FILE_REQ,
        //IOCTRL_TYPE_DOWNLOAD_RECORD_FILE_RESP,
        typedef struct _DOWNLOAD_RECORD_FILE_REQ {
                HI_S8 strFileName[40]; //文件名字
                HI_U8 u8Reserved[8];
        } XM_DOWNLOAD_RECORD_FILE_REQ;

        typedef struct _DOWNLOAD_RECORD_FILE_RESP {
                HI_S32 s32Result; //XM_SUCCESS: 成功 :  失败返回错误原因
                HI_U8 u8Reserved[8];
        } XM_DOWNLOAD_RECORD_FILE_RESP;

        //IOCTRL_TYPE_RECORD_PLAYCONTROL_REQ,
        //IOCTRL_TYPE_RECORD_PLAYCONTROL_RESP,
        typedef struct _RECORD_PLAYCONTROL_REQ {
                HI_U16 u16Command; //回放录像控制命令，参考 XM_RECORD_PLAY_CMD
                HI_U16 us16SeekTimeSec; // 让设备按指定的时间点，来回放文件， 范围 [0 - 文件结束时间点]
                HI_U32 u32Param; //当   u16Command = IOCTRL_RECORD_PLAY_CMD_FAST 时，
                //  u32Param = 1 : 1倍速  u32Param = 2 : 2倍速u32Param = 4 : 4倍速
                HI_S8 strFileName[64]; //文件名字
                HI_U8 u8Reserved[8];
        } XM_RECORD_PLAYCONTROL_REQ;

        typedef struct _RECORD_PLAYCONTROL_RESP {
                HI_S32 s32Result; //XM_SUCCESS 成功:   失败返回错误原因
                HI_U16 u16Command; //回放录像控制命令，参考 XM_RECORD_PLAY_CMD
                HI_U16 s32FilePlaySec; //返回播放文件播放总时间，单位是秒。
                HI_U8 u8Reserved[8];
        } XM_RECORD_PLAYCONTROL_RESP;

        //IOCTRL_TYPE_NET_TRIGGER_RECORD_START_REQ,
        //IOCTRL_TYPE_NET_TRIGGER_RECORD_START_RESP,
        typedef struct _NET_TRIGGER_RECORD_START_REQ {
                HI_U8 u8Reserved[8];
        } XM_NET_TRIGGER_RECORD_START_REQ;

        typedef struct _NET_TRIGGER_RECORD_START_RESP {
                HI_S32 s32Result; //XM_SUCCESS 成功:   失败返回错误原因
                HI_U8 u8Reserved[8];
        } XM_NET_TRIGGER_RECORD_START_RESP;

        //IOCTRL_TYPE_NET_TRIGGER_RECORD_CHECK_REQ,
        //IOCTRL_TYPE_NET_TRIGGER_RECORD_CHECK_RESP,
        typedef struct _NET_TRIGGER_RECORD_CHECK_REQ {
                HI_U8 u8Reserved[8];
        } XM_NET_TRIGGER_RECORD_CHECK_REQ;

        typedef struct _NET_TRIGGER_RECORD_CHECK_RESP {
                HI_S32 s32Result; //XM_SUCCESS 成功:   失败返回错误原因
                HI_U8 u8NowRecord; // 0 没有在录像， 1 表示在录像。
                HI_U8 u8Reserved[7];
        } XM_NET_TRIGGER_RECORD_CHECK_RESP;

        //IOCTRL_TYPE_NET_TRIGGER_RECORD_STOP_REQ,
        //IOCTRL_TYPE_NET_TRIGGER_RECORD_STOP_RESP,
        typedef struct _NET_TRIGGER_RECORD_STOP_REQ {
                HI_U8 u8Reserved[8];
        } XM_NET_TRIGGER_RECORD_STOP_REQ;

        typedef struct _NET_TRIGGER_RECORD_STOP_RESP {
                HI_S32 s32Result; //XM_SUCCESS 成功:   失败返回错误原因
                HI_U8 u8Reserve[8];
        } XM_NET_TRIGGER_RECORD_STOP_RESP;

        //////////////////////////////////////////////////////////////////////////////////////////////////

        /******************************************************************************************
          设备推送私有协议部分
         *******************************************************************************************/

        ////////相关数据结构////////

#define XM_PUSH_NET_VERSION 0x1000
        //消息ID
        typedef enum {
                PUSH_CMD_ID_DEV_REGISTER = 1, //设备注册消息,设备->服务器
                PUSH_CMD_ID_DEV_REGISTER_REPLY = 2, //设备注册回应,服务器->设备
                PUSH_CMD_ID_DEV_EVENT_NOTIFY = 3, //设备消息通知,设备->服务器
                PUSH_CMD_ID_DEV_EVENT_NOTIFY_REPLY = 4, //设备消息通知回应,服务器->设备
                PUSH_CMD_ID_PHONE_REGISTER = 5, //手机注册消息,手机->服务器
                PUSH_CMD_ID_PHONE_REGISTER_REPLY = 6, //手机注册回应,服务器->手机
                PUSH_CMD_ID_HEARTBEAT = 7, //心跳消息,手机<->服务器
                PUSH_CMD_ID_PUSH_EVENT_NOTIFY = 9, //手机设备消息通知,服务器->手机
                PUSH_CMD_ID_PUSH_EVENT_NOTIFY_REPLY = 10, //手机设备消息通知回应,手机->服务器
                PUSH_CMD_ID_PHONE_DEV_REGISTER = 11, //手机关联设备注册,手机->服务器
                PUSH_CMD_ID_PHONE_DEV_REGISTER_REPLY = 12, //手机关联设备注册回应,服务器->手机
                PUSH_CMD_ID_PHONE_DELETE_DEV = 13, //手机删除关联设备,手机->服务器
        } XM_PUSH_CMD_ID;

        //设备类型
        typedef enum {
                DEV_TYPE_DOOR_BELL = 1, //门铃
        } XM_DEV_TYPE;

        //设备事件类型
        typedef enum {
                DEV_EVENT_DOOR_BELL_PUSHED = 1, //门铃被按下
                DEV_EVENT_DOOR_BELL_BROKEN = 2, //门铃被强拆
        } XM_DEV_EVENT_TYPE;

        //========语言类型手机端使用=========
        typedef enum {
                LANGUAGE_ENGLISH = 1,
                LANGUAGE_CHINESE = 2,
        } XM_PUSH_LANGUAGE_TYPE;

        //附加数据类型
        typedef enum {
                EXTRA_DATA_NONE = 0, //无附加数据
                EXTRA_DATA_SPEECH = 1, //语音
                EXTRA_DATA_PICTURE = 2, //图片
        } XM_PUSH_EXTRA_DATA_TYPE;

        //网络消息包头
        typedef struct PushNetPackHdr {
                HI_U16 u16Version; //当前必须为 PUSH_NET_VERSION
                HI_U16 u16Cmd; //消息ID，决定了包数据格式，见 CommandId
                HI_U32 u32PackLen; //包数据长度
        } XM_PUSH_NET_PACK_HDRS;

        //设备注册消息,PUSH_CMD_ID_DEV_REGISTER
        typedef struct PushCmdDevRegister {
                HI_S8 strDevId[64]; //设备唯一ID
                HI_S8 s8DevType; //取值见 DevType
                HI_S8 s8Reserved[7];
        } XM_PUSH_CMD_DEV_REGISTER;

        //设备注册回应,PUSH_CMD_ID_DEV_REGISTER_REPLY
        typedef struct PushCmdDevRegisterReply {
                HI_S8 s8Redirect; //重定向标志。如果不为0，设备需要更新后面的服务器地址和端口，并重新向新服务器地址注册
                HI_S8 s8Reserved[5];
                HI_U16 u16ServerPort; //服务器端口
                HI_S8 strServerIp[16]; //服务器IP
        } XM_PUSH_DEV_REGISTER_REPLY;

        //设备消息通知,PUSH_CMD_ID_DEV_EVENT_NOTIFY
        typedef struct PushCmdDevEventNotify {
                HI_S8 strDevId[64]; //设备唯一ID
                HI_S8 s8DevType; //设备类型,取值见 DevType
                HI_S8 s8EventType; //事件类型,取值见 DevEventType
                HI_S8 s8ExtraDataType; //附加数据类型,取值见 ExtraDataType
                HI_S8 s8Reserved[5];
                HI_U32 u32Timestamp; //事件的UTC时间
                HI_U32 u32ExtraDataLen; //附加数据长度
                HI_S8 s8ExtraData[0]; //附加数据,长度由 extraDataLen 字段决定
        } XM_PUSH_CMD_DEV_EVENT_NOTIFY;

        //手机注册消息,PUSH_CMD_ID_PHONE_REGISTER
        typedef struct PushCmdPhoneRegister {
                HI_S8 strPhoneId[64]; //手机唯一标识(对于iOS为系统分配的device token)
                HI_S8 s8PhoneType; //手机类型，取值见 PhoneType
                HI_S8 s8Language; //手机语言，取值见 LanguageType
                HI_U16 u16EventExpireTime; //设备事件保留天数,该手机未接收到的设备事件在服务器保留的天数
                HI_S8 s8Timezone; //手机时区，取值[-24,24]
                HI_S8 s8AppId; //用于标识不同APP的ID
                HI_S8 s8Reserved[2];
                HI_U64 u64LastEventId; //最后一个从服务器收到的事件ID，没有则填0
        } XM_PUSH_CMD_PHONE_REGISTER;

        //手机注册回应,PUSH_CMD_ID_PHONE_REGISTER_REPLY
        typedef struct PushCmdPhoneRegisterReply {
                HI_S8 s8Redirect; //重定向标志。如果不为0，手机APP需要更新后面的服务器地址和端口，并重新向新服务器地址注册
                HI_S8 s8Reserved[5];
                HI_U16 u16ServerPort; //服务器端口
                HI_S8 strServerIp[16]; //服务器IP
        } XM_PUSH_CMD_PHONE_REGISTER_REPLY;

        //用于手机关联设备注册消息中的设备列表项
        typedef struct PhoneDevEntry {
                HI_S8 strDevId[64]; //设备ID
                HI_S8 strName[32]; //该设备在手机上的显示名称
        } XM_PHONE_DEV_ENTRY;

        //手机关联设备注册,PUSH_CMD_ID_PHONE_DEV_REGISTER
        typedef struct PushCmdPhoneDevRegister {
                HI_U32 u32DevCount; //手机关联的设备个数
                HI_U32 u32Reserved;
                XM_PHONE_DEV_ENTRY devEntries[0]; //手机关联的设备列表，个数由devCount指定
        } XM_PUSH_CMD_PHONE_DEV_REGISTER;

        //手机关联设备注册回应,PUSH_CMD_ID_PHONE_DEV_REGISTER_REPLY
        typedef struct PushCmdPhoneDevRegisterReply {
                HI_U32 u32DevCount; //成功注册的设备个数
        } XM_PUSH_CMD_PHONE_DEV_REGISTER_REPLY;

        //手机设备消息通知,PUSH_CMD_ID_PUSH_EVENT_NOTIFY
        typedef struct PushCmdPushEventNotify {
                HI_S8 strDevId[64]; //设备唯一ID
                HI_S8 s8DevType; //设备类型,取值见 DevType
                HI_S8 s8EventType; //事件类型,取值见 DevEventType
                HI_S8 s8Reserved;
                HI_S8 s8ExtraDataType; //附加数据类型,取值见 ExtraDataType
                HI_U32 u32Timestamp; //事件的UTC时间
                HI_U64 u64EventId; //服务器给事件分配的唯一ID
                HI_U32 u32ExtraDataLen; //附加数据长度
                HI_S8 s8ExtraData[0]; //附加数据,长度由 extraDataLen 字段决定
        } XM_PUSH_CMD_PUSH_EVENT_NOTIFY;

        //手机设备消息通知回应,PUSH_CMD_ID_PUSH_EVENT_NOTIFY_REPLY
        typedef struct PushCmdPushEventNotifyReply {
                HI_U64 u64EventId; //收到的事件ID
        } XM_PUSH_CMD_PUSH_EVENT_NOTIFY_REPLY;

        //手机删除关联设备,手机->服务器,PUSH_CMD_ID_PHONE_DELETE_DEV
        typedef struct PushCmdPhoneDeleteDev {
                char devId[64]; //设备ID
        } XM_PUSH_CMD_PHONE_DELETE_DEV;

        ////////网络应答数据结构////////

        //IOCTRL_TYPE_GET_PUSH_PARAMETERS_REQ,
        //IOCTRL_TYPE_GET_PUSH_PARAMETERS_RESP,

        typedef struct _GET_PUSH_PARAMETERS_REQ {
                HI_U8 u8Reserved[8];
        } XM_GET_PUSH_PARAMETERS_REQ;

        typedef struct _GET_PUSH_PARAMETERS_RESP {
                HI_S32 s32Result; //XM_SUCCESS 成功:   失败返回错误原因
                HI_U8 u8EnablePush; //PUSH 功能使能:   0 关闭  1开启
                HI_U8 u8OpenDoorPush; // 开门推送消息:    0 关闭  1开启
                HI_U8 u8PushExtraDataType; //推送附带信息: 参考 XM_PUSH_EXTRA_DATA_TYPE
                HI_U8 u8Reserved[9];
        } XM_GET_PUSH_PARAMETERS_RESP;

        //IOCTRL_TYPE_SET_PUSH_PARAMETERS_REQ,
        //IOCTRL_TYPE_SET_PUSH_PARAMETERS_RESP,

        typedef struct _SET_PUSH_PARAMETERS_REQ {
                HI_U8 u8EnablePush; //PUSH 功能使能:   0 关闭  1开启
                HI_U8 u8OpenDoorPush; // 开门推送消息:    0 关闭  1开启
                HI_U8 u8PushExtraDataType; //推送附带信息: 参考 XM_PUSH_EXTRA_DATA_TYPE
                HI_U8 u8Reserved[9];
        } XM_SET_PUSH_PARAMETERS_REQ;

        typedef struct _SET_PUSH_PARAMETERS_RESP {
                HI_S32 s32Result; //XM_SUCCESS 成功:   失败返回错误原因
                HI_U8 u8Reserved[8];
        } XM_SET_PUSH_PARAMETERS_RESP;

        //IOCTRL_TYPE_PUSH_EVENT_REQ,
        //IOCTRL_TYPE_PUSH_EVENT_RESP,?
        typedef struct _PUSH_EVENT_REQ {
                // 触发一次设备推送,
                HI_U8 u8Reserved[8];
        } XM_PUSH_EVENT_REQ;

        typedef struct _PUSH_EVENT_RESP {
                HI_S32 s32Result; //XM_SUCCESS 成功:   失败返回错误原因
                HI_U8 u8Reserve[8];
        } XM_PUSH_EVENT_RESP;

        //////////////////////////////////////////////////////////////////////////////////////////////////

        /******************************************************************************************
          设备参数设置部分
         *******************************************************************************************/

        ////////相关数据结构////////

        typedef struct _XM_SYS_TIME_S_ {
                XM_DAY_TIME stSysTime; // 设备时间
                HI_S8 s8TimeZone; // 时区
                HI_U8 u8Reserve[3];
        } XM_SYS_TIME_S;

        typedef struct _XM_SYS_TIME2_S_ {
                XM_DAY_TIME stSysTime; // 设备时间
                HI_S8 strTimeZone[20]; // 时区GMT+8:00
                HI_U8 u8Reserve[3];
        } XM_SYS_TIME2_S;

        typedef enum {
                PASS_TYPE_UNKW,
                PASS_TYPE_DOOR, //开锁密码
                PASS_TYPE_NET_VIEW, //网络观看密码
        } RJONG_PASS_TYPE;

        ////////网络应答数据结构////////

        //IOCTRL_TYPE_GET_SYSFWVER_REQ,
        //IOCTRL_TYPE_GET_SYSFWVER_RESP,
        typedef struct _GET_SYSFWVER_REQ {
                HI_U8 u8Reserved[8];
        } XM_GET_SYSFWVER_REQ;

        typedef struct _GET_SYSFWVER_RESP {
                HI_S32 s32Result; //XM_SUCCESS 成功:   失败返回错误原因
                HI_U32 u32VerFW; //硬件版本号 u32VerFW(UINT)   "Version: %d.%d.%d.%d", (u32VerFW & 0xFF000000)>>24, (u32VerFW & 0x00FF0000)>>16, (u32VerFW & 0x0000FF00)>>8, (u32VerFW & 0x000000FF) >> 0 )
                HI_U32 u32VerSW; //软件版本号 u32VerSW(UINT)   "Version: %d.%d.%d.%d", (u32VerFW & 0xFF000000)>>24, (u32VerFW & 0x00FF0000)>>16, (u32VerFW & 0x0000FF00)>>8, (u32VerFW & 0x000000FF) >> 0 )
                HI_U8 u8DevType; // 参考XM_DEV_TYPE
                HI_U8 u8Reserved[7];
        } XM_GET_SYSFWVER_RESP;

        //IOCTRL_TYPE_GET_DEV_TIME_REQ,
        //IOCTRL_TYPE_GET_DEV_TIME_RESP,
        typedef struct _GET_DEV_TIME_REQ {
                HI_U8 u8Reserved[8];
        } XM_GET_DEV_TIME_REQ;

        typedef struct _GET_DEV_TIME_RESP {
                HI_S32 s32Result; //XM_SUCCESS 成功:   失败返回错误原因
                XM_SYS_TIME_S stSysTime;
                HI_U8 u8Reserved[8];
        } XM_GET_DEV_TIME_RESP;

        //IOCTRL_TYPE_SET_DEV_TIME_REQ,
        //IOCTRL_TYPE_SET_DEV_TIME_RESP,
        typedef struct _SET_DEV_TIME_REQ {
                XM_SYS_TIME_S stSysTime;
                HI_U8 u8Reserved[8];
        } XM_SET_DEV_TIME_REQ;

        typedef struct _SET_DEV_TIME_RESP {
                HI_S32 s32Result; //XM_SUCCESS 成功:   失败返回错误原因
                HI_U8 u8Reserved[8];
        } XM_SET_DEV_TIME_RESP;

        //IOCTRL_TYPE_CHANGE_DEV_PASSWORD_REQ,
        //IOCTRL_TYPE_CHANGE_DEV_PASSWORD_RESP,
        typedef struct _CHANGE_DEV_PASSWORD_REQ {
                HI_S8 strOldPass[48]; // 原密码
                HI_S8 strNewPass[48]; // 新密码
                HI_U8 u8PassType; //密码类型，参考 RJONG_PASS_TYPE
                HI_U8 u8Reserved[7];
        } XM_CHANGE_DEV_PASSWORD_REQ;

        typedef struct _CHANGE_DEV_PASSWORD_RESP {
                HI_S32 s32Result; //XM_SUCCESS 成功:   失败返回错误原因
                HI_U8 u8Reserved[8];
        } XM_CHANGE_DEV_PASSWORD_RESP;

        //IOCTRL_TYPE_GET_DEV_INTERNET_FLAG_REQ,
        //IOCTRL_TYPE_GET_DEV_INTERNET_FLAG_RESP,
        typedef struct _GET_DEV_INTERNET_FLAG_REQ {
                HI_U8 u8Reserved[8];
        } XM_GET_DEV_INTERNET_FLAG_REQ;

        typedef struct _GET_DEV_INTERNET_FLAG_RESP {
                HI_S32 s32Result; //XM_SUCCESS 成功:   失败返回错误原因
                HI_U8 u8EnableInernet; // 0: 不能连internet.   1: 能连internet
                HI_U8 u8Reserved[7];
        } XM_GET_DEV_INTERNET_FLAG_RESP;

        //IOCTRL_TYPE_SET_DEV_INTERNET_FLAG_REQ,
        //IOCTRL_TYPE_SET_DEV_INTERNET_FLAG_RESP,
        typedef struct _SET_DEV_INTERNET_FLAG_REQ {
                HI_U8 u8EnableInernet; // 0: 不能连internet.   1: 能连internet
                HI_U8 u8Reserved[7];
        } XM_SET_DEV_INTERNET_FLAG_REQ;

        typedef struct _SET_DEV_INTERNET_FLAG_RESP {
                HI_S32 s32Result; //XM_SUCCESS 成功:   失败返回错误原因
                HI_U8 u8Reserved[8];
        } XM_SET_DEV_INTERNET_FLAG_RESP;

        /////////////////////////////////////////////////////////////////////////////////////////////////////

        /******************************************************************************************
          图像参数设置
         *******************************************************************************************/

        ////////相关数据结构////////

        typedef enum {
                GAMMA_TYPE_DEFAULT = 0,
                GAMMA_TYPE_0,
                GAMMA_TYPE_1,
                GAMMA_TYPE_2,
                GAMMA_TYPE_AUTO,
        } XM_GAMMA_TYPE;

        ////////网络应答数据结构////////
        //IOCTRL_TYPE_GET_DEV_IMAGE_PARAMETERS_REQ,
        //IOCTRL_TYPE_GET_DEV_IMAGE_PARAMETERS_RESP,
        typedef struct _GET_DEV_IMAGE_PARAMETERS_REQ {
                HI_U8 u8Reserved[8];
        } XM_GET_DEV_IMAGE_PARAMETERS_REQ;

        typedef struct _GET_DEV_IMAGE_PARAMETERS_RESP {
                HI_S32 s32Result; //XM_SUCCESS 成功:   失败返回错误原因
                HI_U8 u8LumaVal; //亮度 [0 ~ 100]  默认值 50.
                HI_U8 u8ContrVal; //对比度 [0 ~ 100]  默认值 50.
                HI_U8 u8HueVal; //色差度 [0 ~ 100]  默认值 50.
                HI_U8 u8SatuVal; //饱和度 [0 ~ 100]  默认值 50.
                HI_U8 u8Gamma; // 参考XM_GAMMA_TYPE
                HI_U8 u8Mirror; // 图像水平翻转u8Mirror=1表示翻转
                HI_U8 u8Flip; // 图像垂直翻转u8Flip =1 表示翻转
                HI_U8 u8Reserved[9];
        } XM_GET_DEV_IMAGE_PARAMETERS_RESP;

        //IOCTRL_TYPE_SET_DEV_IMAGE_PARAMETERS_REQ,
        //IOCTRL_TYPE_SET_DEV_IMAGE_PARAMETERS_RESP,
        typedef struct _SET_DEV_IMAGE_PARAMETERS_REQ {
                HI_U8 u8LumaVal; //亮度 [0 ~ 100]  默认值 50.
                HI_U8 u8ContrVal; //对比度 [0 ~ 100]  默认值 50.
                HI_U8 u8HueVal; //色差度 [0 ~ 100]  默认值 50.
                HI_U8 u8SatuVal; //饱和度 [0 ~ 100]  默认值 50.
                HI_U8 u8Gamma; // 参考XM_GAMMA_TYPE
                HI_U8 u8Mirror; // 图像水平翻转u8Mirror=1表示翻转
                HI_U8 u8Flip; // 图像垂直翻转u8Flip =1 表示翻转
                HI_U8 u8Reserved[9];
        } XM_SET_DEV_IMAGE_PARAMETERS_REQ;

        typedef struct _SET_DEV_IMAGE_PARAMETERS_RESP {
                HI_S32 s32Result; //XM_SUCCESS 成功:   失败返回错误原因
                HI_U8 u8Reserved[8];
        } XM_SET_DEV_IMAGE_PARAMETERS_RESP;

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////

        /******************************************************************************************
          视频参数设置
         *******************************************************************************************/

        ////////相关数据结构////////

        ////////网络应答数据结构////////
        //IOCTRL_TYPE_GET_DEV_VIDEO_PARAMETERS_REQ,
        //IOCTRL_TYPE_GET_DEV_VIDEO_PARAMETERS_RESP,
        typedef struct _GET_DEV_VIDEO_PARAMETERS_REQ {
                HI_U32 u32VideoChn; //视频的通道号，一般 0 为 主码流， 1为辅码流
        } XM_GET_DEV_VIDEO_PARAMETERS_REQ;

        typedef struct _GET_DEV_VIDEO_PARAMETERS_RESP {
                HI_S32 s32Result; //XM_SUCCESS 成功:   失败返回错误原因
                HI_U8 u8VideoChn; //在设置中的通道号。一般 0 为主通道， 1为辅通道。
                HI_U8 u8Code; //压缩格式 参考XM_CODE_TYPE
                HI_U8 u8FrameRate; // 0-20 .   默认值为  20.
                HI_U8 u8Gop; // I 帧间隔 。 默认值为 40
                HI_U32 u32BitRate; //压缩码流.  默认值为 2048 Kb/S
                HI_U32 u32PicWidth; // 视频的宽。
                HI_U32 u32PicHeight; // 视频的高。
        } XM_GET_DEV_VIDEO_PARAMETERS_RESP;

        //IOCTRL_TYPE_SET_DEV_VIDEO_PARAMETERS_REQ,
        //IOCTRL_TYPE_SET_DEV_VIDEO_PARAMETERS_RESP,
        typedef struct _SET_DEV_VIDEO_PARAMETERS_REQ {
                HI_U8 u8VideoChn; //在设置中的通道号。一般 0 为主通道， 1为辅通道。
                HI_U8 u8Code; //压缩格式 参考XM_CODE_TYPE
                HI_U8 u8FrameRate; // 0-20 .   默认值为  20.
                HI_U8 u8Gop; // I 帧间隔 。 默认值为 40
                HI_U32 u32BitRate; //压缩码流.  默认值为 2048 Kb/S
                HI_U32 u32PicWidth; // 视频的宽。
                HI_U32 u32PicHeight; // 视频的高。
                HI_U8 u8Reserved[8];
        } XM_SET_DEV_VIDEO_PARAMETERS_REQ;

        typedef struct _SET_DEV_VIDEO_PARAMETERS_RESP {
                HI_S32 s32Result; //XM_SUCCESS 成功:   失败返回错误原因
                HI_U8 u8Reserved[8];
        } XM_SET_DEV_VIDEO_PARAMETERS_RESP;

        /////////////////////////////////////////////////////////////////////////////////////////////

        /******************************************************************************************
          报警设置
         *******************************************************************************************/

        ////////相关数据结构////////

        ////////网络应答数据结构////////
        //IOCTRL_TYPE_GET_DEVICE_ALARM_REQ,
        //IOCTRL_TYPE_GET_DEVICE_ALARM_RESP,
        typedef struct _GET_DEVICE_ALARM_REQ {
                HI_U8 u8Reserved[8];
        } XM_GET_DEVICE_ALARM_REQ;

        typedef struct _GET_DEVICE_ALARM_RESP {
                HI_S32 s32Result; //XM_SUCCESS 成功:   失败返回错误原因
                HI_U8 u8Enable; //  1: 报警启用 0:报警关闭
                HI_U8 u8AlarmRecord; //  报警后是否启用录像。 1:启用  0:不启用
                HI_U8 u8Snap; // 报警后是否启用抓拍。  1:启用  0:不启用
                HI_U8 u8Push; // 报警后是否启用推送。  1:启用  0:不启用
                HI_U8 u8Reserved[8];
        } XM_GET_DEVICE_ALARM_RESP;

        //IOCTRL_TYPE_SET_DEVICE_ALARM_REQ,
        //IOCTRL_TYPE_SET_DEVICE_ALARM_RESP,
        typedef struct _SET_DEVICE_ALARM_REQ {
                HI_U8 u8Enable; //  1: 报警启用 0:报警关闭
                HI_U8 u8AlarmRecord; //  报警后是否启用录像。 1:启用  0:不启用
                HI_U8 u8Snap; // 报警后是否启用抓拍。  1:启用  0:不启用
                HI_U8 u8Push; // 报警后是否启用推送。  1:启用  0:不启用
                HI_U8 u8Reserved[8];
        } XM_SET_DEVICE_ALARM_REQ;

        typedef struct _SET_DEVICE_ALARM_RESP {
                HI_S32 s32Result; //XM_SUCCESS 成功:   失败返回错误原因
                HI_U8 u8Reserved[8];
        } XM_SET_DEVICE_ALARM_RESP;

        ////////////////////////////////////////////////////////////////////////////////////////////

        /******************************************************************************************
          录像参数设置
         *******************************************************************************************/

        ////////相关数据结构////////

        typedef enum {
                FILE_FORMAT_NONE = 0,
                FILE_FORMAT_AVI,
                FILE_FORMAT_MP4,
        } RJONG_FILE_FORMAT;

        ////////网络应答数据结构////////
        //IOCTRL_TYPE_GET_RECORD_PARAMETERS_REQ,
        //IOCTRL_TYPE_GET_RECORD_PARAMETERS_RESP,
        typedef struct _GET_RECORD_PARAMETERS_REQ {
                HI_U8 u8Reserved[8];
        } XM_GET_RECORD_PARAMETERS_REQ;

        typedef struct _GET_RECORD_PARAMETERS_RESP {
                HI_S32 s32Result; //XM_SUCCESS 成功:   失败返回错误原因
                HI_U32 u32RecordTimeSec; //  有触发事件时，录多少秒的录像。
                HI_U8 u8RecordFileFormat; //  录像文件格式 ，参考 RJONG_FILE_FORMAT
                HI_U8 u8Reserved[7];
        } XM_GET_RECORD_PARAMETERS_RESP;

        //IOCTRL_TYPE_SET_RECORD_PARAMETERS_REQ,
        //IOCTRL_TYPE_SET_RECORD_PARAMETERS_RESP,
        typedef struct _SET_RECORD_PARAMETERS_REQ {
                HI_U32 u32RecordTimeSec; //  有触发事件时，录多少秒的录像。
                HI_U8 u8RecordFileFormat; //  录像文件格式 ，参考 RJONG_FILE_FORMAT
                HI_U8 u8Reserved[7];
        } XM_SET_RECORD_PARAMETERS_REQ;

        typedef struct _SET_RECORD_PARAMETERS_RESP {
                HI_S32 s32Result; //XM_SUCCESS 成功:   失败返回错误原因
                HI_U8 u8Reserved[8];
        } XM_SET_RECORD_PARAMETERS_RESP;

        ///////////////////////////////////////////////////////////////////////////////////////////

        /******************************************************************************************
          验证密码 &  授权
         *******************************************************************************************/

        ////////相关数据结构////////

        ////////网络应答数据结构////////
        //IOCTRL_TYPE_AUTHORIZE_REQ,
        //IOCTRL_TYPE_AUTHORIZE_RESP,
        typedef struct _AUTHORIZE_REQ {
                HI_S8 strPassWord[48]; //填写观看密码
                HI_U8 u8Reserved[8];
        } XM_AUTHORIZE_REQ;

        typedef struct _AUTHORIZE_RESP {
                HI_S32 s32Result; //XM_SUCCESS 成功:   失败返回错误原因
                HI_U8 u8ValueReq; //=IOCTRLSetAuthorizeReq.value
                HI_U8 u8Reserved[7];
        } XM_AUTHORIZE_RESP;

        ///////////////////////////////////////////////////////////////////////////////////////////////

        /******************************************************************************************
          设备升级
         *******************************************************************************************/

        ////////相关数据结构////////
        typedef enum {
                UPGRADE_DATA_FIRST, //App to Device, begin to transfer upgrade pakcet data
                UPGRADE_DATA_MID, //App to Device, middle data of upgrade pakcet
                UPGRADE_DATA_END, //App to Device, complete transfer.
        } XM_ENUM_UPGRADE_DATA_TYPE;

        //IOCTRL_TYPE_UPGRADE_READY,        //Device to App, Device is ready for receiving upgrade firmware. App begin to send data
        typedef struct {
                HI_S32 s32Result; //XM_SUCCESS 成功:   失败返回错误原因
                HI_U8 reserved[4];
        } XM_UPGREDE_RESP;

        //for Upgrade firmware
        typedef struct {
                HI_U16 nUpgradeDataType; //refer to XM_ENUM_UPGRADE_DATA_TYPE
                HI_U16 nUpgradeDataSize;
                HI_U32 nCRC;
                HI_S8 uDevPasswd[24]; //end user upgrad firmware need input passward.
                HI_U8 bNeedDevPasswd; //0: no need, 1:need verfy device password
                HI_U8 reserve[7];
        } XM_UPGRADE_HEAD;

        typedef struct {
                HI_U8 nouse[2]; //default: PK
                HI_U8 dev_type; // 1: single sensor; 2:dual sensor //2013-03-09
                HI_U8 reserve1;
                HI_U8 verThisBin[4]; // 1.2.3.4, verThisBin[0,1,2,3]=0x04,0x03,0x02,0x01
                HI_U32 sizeThisBin;
                HI_U8 flag[4]; //default: P2PU
                HI_U8 reserve2[8];
        } XM_UPGRADE_FILE_HEAD;

        ////////网络应答数据结构////////

        //IOCTRL_TYPE_UPGRADE_REQ,      //App to Device
        // data: upgrade file size

        //IOCTRL_TYPE_UPGRADE_READY,        //Device to App, Device is ready for receiving upgrade firmware. App begin to send data
        // no data

        //IOCTRL_TYPE_UPGRADE_QUERY,    //App to Device, Query device to upgrade firmware ok ? after sending upgrade data over
        // no data

        //IOCTRL_TYPE_UPGRADE_OK,           //Device to App
        // no data

        //IOCTRL_TYPE_UPGRADE_FAILED,       //Device to App
        // no data

        //IOCTRL_TYPE_GET_SN_ETC_REQ    //App to Device
        // no data

        //////////////////////////////////////////////////////////////////////////////////////////////////

        /******************************************************************************************
          控制门铃
         *******************************************************************************************/

        ////////相关数据结构////////

        ////////网络应答数据结构////////
        //IOCTRL_TYPE_DOORBELL_OPEN_REQ,
        //IOCTRL_TYPE_DOORBELL_OPEN_RESP,
        typedef struct _DOORBELL_OPEN_REQ {
                HI_U8 u8DoorBellOpen; // 1. Open   0. No work;
                HI_U8 u8Reserved[7];
        } XM_DOORBELL_OPEN_REQ;

        typedef struct DOORBELL_OPEN_RESP {
                HI_S32 s32Result; //XM_SUCCESS 成功:   失败返回错误原因
                HI_U8 u8Reserved[8];
        } XM_DOORBELL_OPEN_RESP;

        //////////////////////////////////////////////////////////////////////////////////////////////////

        /******************************************************************************************
          SD卡 TF卡 操作
         *******************************************************************************************/

        ////////相关数据结构////////

        ////////网络应答数据结构////////

        //IOCTRL_TYPE_SET_DEVICE_SDFORMAT_REQ,
        //IOCTRL_TYPE_SET_DEVICE_SDFORMAT_RESP,
        typedef struct _SET_DEVICE_SDFORMAT_REQ {
                HI_U8 u8Reserved[8];
        } XM_SET_DEVICE_SDFORMAT_REQ;

        typedef struct SET_DEVICE_SDFORMAT_RESP {
                HI_S32 s32Result; //XM_SUCCESS 成功:   失败返回错误原因
                HI_U8 u8Reserved[8];
        } XM_SET_DEVICE_SDFORMAT_RESP;

        //IOCTRL_TYPE_SDFORMAT_QUERY_REQ,
        //IOCTRL_TYPE_SDFORMAT_QUERY_RESP,
        typedef struct _SDFORMAT_QUERY_REQ {
                HI_U8 u8Reserved[8];
        } XM_SDFORMAT_QUERY_REQ;

        typedef struct _SDFORMAT_QUERY_RESP {
                HI_S32 s32Result; //XM_SUCCESS 成功:   失败返回错误原因
                HI_S8 s8QueryResult; // 1: 正在格式化，-1 格式化失败，0.格式化完成
                HI_U8 u8Reserved[7];
        } XM_SDFORMAT_QUERY_RESP;

        //IOCTRL_TYPE_GET_SD_INFO_REQ,
        //IOCTRL_TYPE_GET_SD_INFO_RESP,
        typedef struct _GET_SD_INFO_REQ {
                HI_U8 u8Reserved[8];
        } XM_GET_SD_INFO_REQ;

        typedef struct GET_SD_INFO_RESP {
                HI_S32 s32Result; //XM_SUCCESS 成功:   失败返回错误原因
                HI_U32 u32SDSize; //  SD 卡的容量大小，单位 K bytes.
                HI_U32 u32UseSize; // SD卡使用了的空间大小  K bytes.
                HI_U32 u32AvailableSize; // SD卡未使用的空间大小  K bytes.
                HI_U8 u8Reserved[8];
        } XM_GET_SD_INFO_RESP;

        //  IOCTRL_TYPE_GET_RTSP_INFO_REQ,
        //  IOCTRL_TYPE_GET_RTSP_INFO_RESP,
        typedef struct _GET_RTSP_INFO_REQ {
                HI_U8 u8Reserved[8];
        } XM_GET_RTSP_INFO_REQ;

        typedef struct GET_RTSP_INFO_RESP {
                HI_S32 s32Result; //XM_SUCCESS 成功:   失败返回错误原因
                HI_S8 stUrl[2][50]; // 两组 rtsp 的地址。
                HI_U8 u8VideoCode; //视频压缩格式 XM_CODE_TYPE
                HI_U8 u8AudioCode; //音频压缩格式 XM_CODE_TYPE
                HI_U8 u8AudioSamplerate; //采样率 XM_AUDIO_SAMPLERATE
                HI_U8 u8AudioBits; //采样位宽XM_AUDIO_DATABITS
                HI_U8 u8AudioChannel; //通道        XM_AUDIO_CHANNEL
                HI_U8 u8Reserved[78];
        } XM_GET_RTSP_INFO_RESP;

        //IOCTRL_TYPE_GET_PERIPHERAL_EQUIPMENT_INFO_REQ,
        //IOCTRL_TYPE_GET_PERIPHERAL_EQUIPMENT_INFO_RESP,
        typedef struct _GET_PERIPHERAL_EQUIPMENT_INFO_REQ {
                HI_U8 u8Reserved[8];
        } XM_GET_PERIPHERAL_EQUIPMENT_INFO_REQ;

        typedef struct _GET_PERIPHERAL_EQUIPMENT_INFO_RESP {
                HI_S32 s32Result; //XM_SUCCESS 成功:   失败返回错误原因
                HI_U8 u8TemperatureF; //华氏温度
                HI_U8 u8TemperatureC; //摄氏温度
                HI_U8 u8PtzHorizontal; //云台水平位移角度 (范围0~120)，现未起作用
                HI_U8 u8PtzVertical; //云台垂直位移角度 (范围0~120).    现未起作用
                HI_U8 u8Reserved[8];
        } XM_GET_PERIPHERAL_EQUIPMENT_INFO_RESP;

        //IOCTRL_TYPE_GET_FREQUENCY_INFO_REQ,
        //IOCTRL_TYPE_GET_FREQUENCY_INFO_RESP,

        //IOCTRL_TYPE_SET_FREQUENCY_INFO_REQ,
        //IOCTRL_TYPE_SET_FREQUENCY_INFO_RESP,

        typedef enum {
                XM_VIDEO_ENCODING_MODE_PAL_50HZ = 0,
                XM_VIDEO_ENCODING_MODE_NTSC_60HZ,
                XM_VIDEO_ENCODING_MODE_AUTO,
                XM_VIDEO_ENCODING_MODE_BUTT
        } XM_VIDEO_NORM_E;

        typedef enum {
                XM_WDR_MODE_NONE = 0,
                XM_WDR_MODE_BUILT_IN,

                XM_WDR_MODE_2To1_LINE,
                XM_WDR_MODE_2To1_FRAME,
                XM_WDR_MODE_2To1_FRAME_FULL_RATE,

                XM_WDR_MODE_3To1_LINE,
                XM_WDR_MODE_3To1_FRAME,
                XM_WDR_MODE_3To1_FRAME_FULL_RATE,

                XM_WDR_MODE_4To1_LINE,
                XM_WDR_MODE_4To1_FRAME,
                XM_WDR_MODE_4To1_FRAME_FULL_RATE,

                XM_WDR_MODE_BUTT,
        } XM_WDR_MODE_E;

        typedef struct _GET_FREQUENCY_INFO_REQ {
                HI_U8 u8Reserved[8];
        } XM_GET_FREQUENCY_INFO_REQ;

        typedef struct _GET_FREQUENCY_INFO_RESP {
                HI_S32 s32Result; //XM_SUCCESS 成功:   失败返回错误原因
                HI_U8 u8Frequency; //参考 XM_VIDEO_NORM_E
                HI_U8 u8Reserved[7];
        } XM_GET_FREQUENCY_INFO_RESP;

        typedef struct _SET_FREQUENCY_INFO_REQ {
                HI_U8 u8Frequency; //参考 XM_VIDEO_NORM_E
                HI_U8 u8Reserved[7];
        } XM_SET_FREQUENCY_INFO_REQ;

        typedef struct _SET_FREQUENCY_INFO_RESP {
                HI_S32 s32Result; //XM_SUCCESS 成功:   失败返回错误原因
                HI_U8 u8Reserved[8];
        } XM_SET_FREQUENCY_INFO_RESP;

        /******************************************************************************************
          行车记录仪参数
         *******************************************************************************************/

        ////////相关数据结构////////

        typedef enum {
                DC_FILE_TYPE_VIDEO,
                DC_FILE_TYPE_GPS,
                DC_FILE_TYPE_SNAP_PIC,
                DC_FILE_TYPE_THUMBNAIL_PIC,
                DC_FILE_TYPE_SNAP_VIDEO,
                DC_FILE_TYPE_EMERGENCY_VIDEO,
                DC_FILE_TYPE_BUFF,
        } XM_DC_ENUM_FILE_TYPE;

        typedef enum {
                DC_PLAY_MODE_FILE,
                DC_PLAY_MODE_TIME,
                DC_PLAY_MODE_BUFF,
        } XM_DC_PLAY_MODE;

        typedef enum {
                DC_PLAY_START,
                DC_PLAY_STOP,
                DC_PLAY_PAUSE,
                DC_PLAY_RESUME,
                DC_PLAY_SEEK_FORWARD,
                DC_PLAY_SEEK_BACK,
                DC_PLAY_SEEK_POS,
        } XM_DC_PLAY_CTRL;

        typedef enum {
                DC_EXTERNAL_GPS_DATA,
                DC_EXTERNAL_OBD_DATA,
                DC_EXTERNAL_BUFF,
        } XM_DC_SEND_EXTERNAL_DATA;

        typedef struct _DC_EXTERNAL_GPS_INFO_ {
                HI_U32 u32TimeDay; // 日期 20151224 -> 2015/12/24
                HI_U32 u32Time; //  时间 115500   -> 11:55:00
                float longitude; //经度
                float latitude; //纬度
                float speed; //速度km/s
                float altitude; //高度m
                HI_U8 u8Reserved[32];
        } XM_DC_EXTERNAL_GPS_INFO;

        typedef struct _DC_EXTERNAL_OBD_INFO_ {
                HI_U32 u32TimeDay; // 日期 20151224 -> 2015/12/24
                HI_U32 u32Time; //  时间 115500   -> 11:55:00
                float oil_temperature; //油温
                float water_temperature; //水温
                float speed; //车速 km/s
                float engine_revolutions; //发电机转速 r/min
                float avg_oil_consumption; //平均油耗   KM/L
                float Instant_oil_consumption; //瞬间油耗   KM/L
                float oil_capacity; //剩余油量  L
                float voltage; //电平电压   V
                float engine_load; //发动机负荷 %
                float engine_failure; //发动机故障，0是表示机器正常，其他值就是故障代码.
                //add 2016.2.15. radar
                float charge_failure; //充电电路异常
                float coolant_temperature; //冷却液温度
                float mileage; //行驶里程
                //end 2016.2.15 append
                HI_U8 u8Reserved[16];
        } XM_DC_EXTERNAL_OBD_INFO;

        typedef struct _DC_EXTERNAL_DATA_INFO {
                HI_U8 u8ExternalDataType; //外部数据的类型，参考 XM_DC_SEND_EXTERNAL_DATA
                HI_U8 u8Reserved[7];
                union {
                        XM_DC_EXTERNAL_GPS_INFO stGpsInfo;
                        XM_DC_EXTERNAL_OBD_INFO stOBDInfo;
                };
        } XM_DC_EXTERNAL_DATA_INFO;

        typedef struct _DC_FILE_INFO_ {
                HI_S8 strFileName[64]; //文件名
                HI_U32 u32FileSize; //文件长度
                HI_U32 u32Duration; //文件有限时长
                HI_U8 u8Reserved[8];
        } XM_DC_FILE_INFO;

        //文件查询
        //IOCTRL_TYPE_DC_GET_FILE_LIST_REQ,
        //IOCTRL_TYPE_DC_GET_FILE_LIST_RESP,

        typedef struct _DC_GET_FILE_LIST_REQ {
                HI_U32 u32SearchTime; //搜索时间 ，例如u32SearchTime = 20150818
                HI_U8 u8FileType; //参考 XM_DC_ENUM_FILE_TYPE
                HI_U8 u8Reserved[7];
        } XM_DC_GET_FILE_LIST_REQ;

        typedef struct _DC_GET_FILE_LIST_RESP {
                HI_S32 s32Result; //XM_SUCCESS 成功:   失败返回错误原因
                HI_U32 u32TotalFileNum; //搜索到的总文件条数
                HI_U8 u8FileType; //参考 XM_DC_ENUM_FILE_TYPE
                HI_U8 u8Reserved[3];
                HI_U32 u32SearchTime; //返回搜索时间
                XM_DC_FILE_INFO stFileInfoArray[0];
        } XM_DC_GET_FILE_LIST_RESP;




        //下载文件
        //  IOCTRL_TYPE_DC_DOWNLOAD_FILE_REQ,
        //  IOCTRL_TYPE_DC_DOWNLOAD_FILE_RESP,

        //暂时还是用以前的下载方式

        typedef struct _DC_DOWNLOAD_FILE_REQ {
                HI_S8 strFileName[64]; //全路径的文件名，例如 /mnt/sdcard/rec/20150816/20150816_152032.mp4
                HI_U32 u32FileStartOffset; //文件偏移点，可以指定从文件哪个位置开始下载，支持文件续传。
                HI_U16 u16FileInTar; //下载tar 文件时，可以指定下载打包文件中的几个文件。
                // u16FileInTar = 0时全部下载, u16FileInTar大于文件总数时，全部下载。
                // u16FileInTar 和u32FileStartOffset 这两个参数不能同时起作用。
                // 当u32FileStartOffset  != 0时，u16FileInTar失效。
                HI_U8 u8Reserved[8];
        } XM_DC_DOWNLOAD_FILE_REQ;

        typedef struct _DC_DOWNLOAD_FILE_RESP {
                HI_S32 s32Result; //XM_SUCCESS 成功:   失败返回错误原因
                HI_S8 strFileName[64]; //全路径的文件名，例如 /mnt/sdcard/rec/20150816/20150816_152032.mp4
                HI_U32 u32FileStartOffset; //文件续传位置。
                HI_U16 u16SessionId; //任务序列号，同一播放任务此标号相同。
                HI_U8 u8Reserved[6];
        } XM_DC_DOWNLOAD_FILE_RESP;

        //在线回放
        //  IOCTRL_TYPE_DC_ONLINE_PLAY_FILE_REQ,
        //  IOCTRL_TYPE_DC_ONLINE_PLAY_FILE_RESP,

        typedef struct _DC_ONLINE_PLAY_FILE_REQ {
                HI_S8 strFileName[64]; //全路径的文件名，例如 /mnt/sdcard/rec/20150816/20150816_152032.mp4  ， u8PlayMode = DC_PLAY_MODE_FILE 时起作用。
                HI_U32 u32TimePos; //以时间点播放视频， u8PlayMode = DC_PLAY_MODE_TIME 时起作用。
                HI_U8 u8PlayMode; //参考XM_DC_PLAY_MODE
                HI_U8 u8Reserved[7];
        } XM_DC_ONLINE_PLAY_FILE_REQ;

        typedef struct _DC_ONLINE_PLAY_FILE_RESP {
                HI_S32 s32Result; //XM_SUCCESS 成功:   失败返回错误原因
                HI_U32 s32FilePlaySec; //返回播放文件播放总时间，单位是秒。
                HI_U16 u16SessionId; //任务序列号，同一播放任务此标号相同。
                HI_U8 u8Reserved[6];
        } XM_DC_ONLINE_PLAY_FILE_RESP;

        //暂时还是用以前的播放方式
        //  IOCTRL_TYPE_DC_ONLINE_PLAY_CTRL_REQ,
        //  IOCTRL_TYPE_DC_ONLINE_PLAY_CTRL_RESP,
        /*
           typedef struct _DC_ONLINE_PLAY_CTRL_REQ
           {
           HI_U8 u8PlayCtrl;           //参考XM_DC_PLAY_CTRL
           HI_U8 u8Reserved[7];
           }XM_DC_ONLINE_PLAY_CTRL_REQ;

           typedef struct _DC_ONLINE_PLAY_CTRL_RESP
           {
           HI_S32  s32Result;           //XM_SUCCESS 成功:   失败返回错误原因
           HI_U8 u8Reserved[8];
           }XM_DC_ONLINE_PLAY_CTRL_RESP;
           */

        //实时拍照
        //  IOCTRL_TYPE_DC_SNAP_REQ,
        //  IOCTRL_TYPE_DC_SNAP_RESP,

        typedef struct _DC_SNAP_REQ {
                HI_U8 u8CreateSnapVideo; //抓拍时是否产生抓拍录像 u8CreateSnapVideo = 1 表示产生录像文件，u8CreateSnapVideo = 0表示不产生录像文件。
                HI_U8 u8Reserved[7];
        } XM_DC_SNAP_REQ;

        typedef struct _DC_SNAP_RESP {
                HI_S32 s32Result; //XM_SUCCESS 成功:   失败返回错误原因
                HI_U8 u8Reserved[8];
        } XM_DC_SNAP_RESP;

        //电子狗
        //IOCTRL_TYPE_DC_ELECTRONIC_DOG_GET_REQ,
        //IOCTRL_TYPE_DC_ELECTRONIC_DOG_GET_RESP,

        //IOCTRL_TYPE_DC_ELECTRONIC_DOG_SET_REQ,
        //IOCTRL_TYPE_DC_ELECTRONIC_DOG_SET_RESP,

        typedef struct _DC_ELECTRONIC_DOG_GET_REQ {
                HI_U8 u8Reserved[8];
        } XM_DC_ELECTRONIC_DOG_GET_REQ;

        typedef struct _DC_ELECTRONIC_DOG_GET_RESP {
                HI_S32 s32Result; //XM_SUCCESS 成功:   失败返回错误原因
                HI_U8 u8ElectronicDowIsWork; //电子狗是否处于工作状态, u8ElectronicDowIsWork = 1 表示正在工作。
                HI_U8 u8Reserved[7];
        } XM_DC_ELECTRONIC_DOG_GET_RESP;

        typedef struct _DC_ELECTRONIC_DOG_SET_REQ {
                HI_U8 u8ElectronicDowIsWork; //设置电子狗工作状态, u8ElectronicDowIsWork = 1 表示工作。
                HI_U8 u8Reserved[7];
        } XM_DC_ELECTRONIC_DOG_SET_REQ;

        typedef struct _DC_ELECTRONIC_DOG_SET_RESP {
                HI_S32 s32Result; //XM_SUCCESS 成功:   失败返回错误原因
                HI_U8 u8Reserved[8];
        } XM_DC_ELECTRONIC_DOG_SET_RESP;

        //设置串口速度
        //IOCTRL_TYPE_DC_SET_UART_REQ,
        //IOCTRL_TYPE_DC_SET_UART_RESP,

        //请求串口控制
        //IOCTRL_TYPE_DC_REQUEST_UART_CTRL_REQ,
        //IOCTRL_TYPE_DC_REQUEST_UART_CTRL_RESP,

        typedef struct _DC_SET_UART_REQ {
                HI_S32 s32Speed; //串口波特率  2400  4800 9600 19200  58400  115200
                HI_U8 u8Reserved[8];
        } XM_DC_SET_UART_REQ;

        typedef struct _DC_SET_UART_RESP {
                HI_S32 s32Result; //XM_SUCCESS 成功:   失败返回错误原因
                HI_U8 u8Reserved[8];
        } XM_DC_SET_UART_RESP;

        typedef struct _DC_REQUEST_UART_CTRL_REQ {
                HI_U8 u8Reserved[8];
        } XM_DC_REQUEST_UART_CTRL_REQ;

        typedef struct _DC_REQUEST_UART_CTRL_RESP {
                HI_S32 s32Result; //XM_SUCCESS 成功:   失败返回错误原因
                HI_U8 u8Reserved[8];
        } XM_DC_REQUEST_UART_CTRL_RESP;

        //串口数据发送   设备-> app
        //IOCTRL_TYPE_DC_SEND_UART_DATA_REQ,
        //IOCTRL_TYPE_DC_SEND_UART_DATA_RESP,

        typedef struct _DC_SEND_UART_DATA_REQ {
                HI_S32 s32DataLen; //串口数据长度
                HI_U8 u8Reserved[8];
                HI_U8 u8UartData[0];
        } XM_DC_SEND_UART_DATA_REQ;

        typedef struct _SEND_UART_DATA_RESP {
                HI_S32 s32Result; //XM_SUCCESS 成功:   失败返回错误原因
                HI_U8 u8Reserved[8];
        } XM_SEND_UART_DATA_RESP;

        //串口数据接收   app -> 设备
        //IOCTRL_TYPE_DC_WRITE_UART_DATA_REQ,
        //IOCTRL_TYPE_DC_WRITE_UART_DATA_RESP,

        typedef struct _DC_WRITE_UART_DATA_REQ {
                HI_S32 s32DataLen; //串口数据长度
                HI_U8 u8Reserved[8];
                HI_U8 u8UartData[0];
        } XM_DC_WRITE_UART_DATA_REQ;

        typedef struct _DC_WRITE_UART_DATA_RESP {
                HI_S32 s32Result; //XM_SUCCESS 成功:   失败返回错误原因
                HI_U8 u8Reserved[8];
        } XM_DC_WRITE_UART_DATA_RESP;

        //IOCTRL_TYPE_DC_STOP_DOWNLOAD_FILE_REQ,
        //IOCTRL_TYPE_DC_STOP_DOWNLOAD_FILE_RESP,
        typedef struct _DC_STOP_DOWNLOAD_FILE_REQ {
                HI_U16 u16SessionId; //任务序列号,此参数未来做扩展用，目前填 u16SessionId = 0 就可以了。
                HI_U8 u8Reserved[6];
        } XM_DC_STOP_DOWNLOAD_FILE_REQ;

        typedef struct _DC_STOP_DOWNLOAD_FILE_RESP {
                HI_S32 s32Result; //XM_SUCCESS 成功:   失败返回错误原因
                HI_U8 u8Reserved[8];
        } XM_DC_STOP_DOWNLOAD_FILE_RESP;

        //获取版本号接口2
        //IOCTRL_TYPE_GET_DEVAPPVER_REQ,
        //IOCTRL_TYPE_GET_DEVAPPVER_RESP,
        typedef struct _GET_DEVAPPVER_REQ {
                HI_U8 u8Reserved[8];
        } XM_GET_DEVAPPVER_REQ;

        typedef struct _GET_DEVAPPVER_RESP {
                HI_S32 s32Result; //XM_SUCCESS 成功:   失败返回错误原因
                HI_U8 u8Ver[64]; //版本号
                HI_U8 u8Reserved[8];
        } XM_GET_DEVAPPVER_RESP;

        //时间设置接口2
        //IOCTRL_TYPE_GET_DEV_TIME2_REQ,
        //IOCTRL_TYPE_GET_DEV_TIME2_RESP,

        //IOCTRL_TYPE_SET_DEV_TIME2_REQ,
        //IOCTRL_TYPE_SET_DEV_TIME2_RESP,

        typedef struct _GET_DEV_TIME2_REQ {
                HI_U8 u8Reserved[8];
        } XM_GET_DEV_TIME2_REQ;

        typedef struct _GET_DEV_TIME2_RESP {
                HI_S32 s32Result; //XM_SUCCESS 成功:   失败返回错误原因
                XM_SYS_TIME2_S stSysTime;
                HI_U8 u8Reserved[8];
        } XM_GET_DEV_TIME2_RESP;

        typedef struct _SET_DEV_TIME2_REQ {
                XM_SYS_TIME2_S stSysTime;
                HI_U8 u8Reserved[8];
        } XM_SET_DEV_TIME2_REQ;

        typedef struct _SET_DEV_TIME2_RESP {
                HI_S32 s32Result; //XM_SUCCESS 成功:   失败返回错误原因
                HI_U8 u8Reserved[8];
        } XM_SET_DEV_TIME2_RESP;

        //上传抓拍图片或视频文件的信息
        //IOCTRL_TYPE_SEND_SNAP_FILE_INFO_REQ,
        //IOCTRL_TYPE_SEND_SNAP_FILE_INFO_RESP,

        typedef struct _SEND_SNAP_FILE_INFO_REQ {
                HI_S8 strFileName[64]; //文件名
                unsigned int u32fileSize; ////// 单位K
                unsigned int u32Duration; /////// 单位秒
                HI_U8 u8FileType; //参考 XM_DC_ENUM_FILE_TYPE
                HI_U8 u8Reserved[7];
        } XM_SEND_SNAP_FILE_INFO_REQ;

        typedef struct _SEND_SNAP_FILE_INFO_RESP {
                HI_S32 s32Result; //XM_SUCCESS 成功:   失败返回错误原因
                HI_U8 u8Reserved[8];
        } XM_SEND_SNAP_FILE_INFO_RESP;

        //产生临时访问密码
        //IOCTRL_TYPE_DC_CREATE_TMP_CONNECT_PASSWORD_REQ,
        //IOCTRL_TYPE_DC_CREATE_TMP_CONNECT_PASSWORD_RESP,

        typedef struct _DC_CREATE_TMP_CONNECT_PASSWORD_REQ {
                HI_S32 s32AllowConnectNum; //产生允许连接多少次的密码
                HI_U32 s32AllowConnectSeconds; //允许连接持续的多少秒钟，超过指定的秒数后，机器会自动断开连接。
                // s32AllowConnectSeconds = 0 时表示不做限制。
                HI_U8 u8Reserved[28];
                ; //文件名
        } XM_DC_CREATE_TMP_CONNECT_PASSWORD_REQ;

        typedef struct _DC_CREATE_TMP_CONNECT_PASSWORD_RESP {
                HI_S32 s32Result; //XM_SUCCESS 成功:   失败返回错误原因
                HI_S8 strTmpAccessPass[64]; //产生的临时密码
                HI_U8 u8Reserved[32];
        } XM_DC_CREATE_TMP_CONNECT_PASSWORD_RESP;

        //上传删除文件的信息 mac -> app
        //IOCTRL_TYPE_SEND_DEL_FILE_INFO_REQ,
        //IOCTRL_TYPE_SEND_DEL_FILE_INFO_RESP,

        typedef struct _SEND_DEL_FILE_INFO_REQ {
                HI_U8 u8FileType; //参考 XM_DC_ENUM_FILE_TYPE
                HI_S8 strFileName[80]; //文件名
        } XM_SEND_DEL_FILE_INFO_REQ;

        typedef struct _SEND_DEL_FILE_INFO_RESP {
                HI_S32 s32Result; //XM_SUCCESS 成功:   失败返回错误原因
                HI_U8 u8Reserved[8];
        } XM_SEND_DEL_FILE_INFO_RESP;

        //获取tf 卡上具体文件信息
        //IOCTRL_TYPE_DC_GET_TFCARD_INFO_REQ,
        //IOCTRL_TYPE_DC_GET_TFCARD_INFO_RESP,

        typedef struct _DC_GET_TFCARD_INFO_REQ {
                HI_U8 u8Reserved[8];
        } XM_DC_GET_TFCARD_INFO_REQ;

        typedef struct _DC_GET_TFCARD_INFO_RESP {
                HI_S32 s32Result; //XM_SUCCESS 成功:   失败返回错误原因,
                HI_U32 u32SDSize; //  SD 卡的容量大小，下面所以单位都是M bytes.
                HI_U32 u32UseSize; // SD卡使用了的空间大小.
                HI_U32 u32AvailableSize; // SD卡未使用的空间大小  .
                HI_U32 u32OtherFileSize; //SD卡中其他文件的大小，这些文件与我们的系统不相关 .
                HI_U32 u32VideoAllowUseSize; //普通录像总存储空间大小 .
                HI_U32 u32VideoUseSize; // 普通录像使用了的空间大小  .
                HI_U32 u32SnapAllowUseSize; //抓拍图片录像总存储空间大小.
                HI_U32 u32SnapPicUseSize; // 抓拍图片使用了的空间大小  .
                HI_U32 u32SnapVideoUseSize; // 抓拍录像使用了的空间大小  .
                HI_U32 u32SnapAvailableSize; // 普通录像未使用的空间大小  .
                HI_U32 u32EmergencyAllowUseSize; //抓拍图片录像总存储空间大小.
                HI_U32 u32EmergencyVideoUseSize; // 抓拍录像使用了的空间大小 .
                HI_U8 u8Reserved[32];
        } XM_DC_GET_TFCARD_INFO_RESP;

        //设置设备对APP 发送实时GPS+OBD的数据
        //IOCTRL_TYPE_GET_GPS_OBD_SEND_PARAMETERS_REQ,
        //IOCTRL_TYPE_GET_GPS_OBD_SEND_PARAMETERS_RESP,

        //IOCTRL_TYPE_SET_GPS_OBD_SEND_PARAMETERS_REQ,
        //IOCTRL_TYPE_SET_GPS_OBD_SEND_PARAMETERS_RESP,

        typedef struct _GET_GPS_OBD_SEND_PARAMETERS_REQ {
                HI_U8 u8Reserved[8];
        } XM_GET_GPS_OBD_SEND_PARAMETERS_REQ;

        typedef struct _GET_GPS_OBD_SEND_PARAMETERS_RESP {
                HI_S32 s32Result; //XM_SUCCESS 成功:   失败返回错误原因
                HI_U32 u32EnalbeAutoSendGPSOBDData; //是否自动发送GPS + OBD 信息  0 不发送  1 发送
                HI_U8 u8Reserved[8];
        } XM_GET_GPS_OBD_SEND_PARAMETERS_RESP;

        typedef struct _SET_GPS_OBD_SEND_PARAMETERS_REQ {
                HI_U32 u32EnalbeAutoSendGPSOBDData; //是否自动发送GPS + OBD 信息  0 不发送  1 发送
                HI_U8 u8Reserved[8];
        } XM_SET_GPS_OBD_SEND_PARAMETERS_REQ;

        typedef struct _SET_GPS_OBD_SEND_PARAMETERS_RESP {
                HI_S32 s32Result; //XM_SUCCESS 成功:   失败返回错误原因
                HI_U8 u8Reserved[8];
        } XM_SET_GPS_OBD_SEND_PARAMETERS_RESP;

        //上传GPS OBD 实时数据 mac -> app  不需要回复
        //IOCTRL_TYPE_SEND_GPS_OBD_REQ,
        //IOCTRL_TYPE_SEND_GPS_OBD_RESP,
        typedef struct _GET_SEND_GPS_OBD_REQ {
                XM_DC_EXTERNAL_DATA_INFO stDataInfo; //外部数据
                HI_U8 u8Reserved[8];
        } XM_SEND_GPS_OBD_REQ;

        //得到所有相关数据的版本号
        //IOCTRL_TYPE_GET_DEV_ALL_APPVER_REQ,
        //IOCTRL_TYPE_GET_DEV_ALL_APPVER_RESP,
        typedef struct _GET_DEV_ALL_APPVER_REQ {
                HI_U8 u8Reserved[8];
        } XM_GET_DEV_ALL_APPVER_REQ;

        typedef struct _GET_DEV_ALL_APPVER_RESP {
                HI_S32 s32Result; //XM_SUCCESS 成功:   失败返回错误原因
                HI_U8 u8SoftVer[64]; //软件版本号
                HI_U8 u8SoundDataVer[32]; //音频数据版本号
                HI_U8 u8Reserved[96];
        } XM_GET_DEV_ALL_APPVER_RESP;

        //通知APP  是否有人经过 ，展会功能(设备主动发送给APP，APP不需要应答)
        //IOCTRL_TYPE_SEND_PIR_INFO_REQ,
        //IOCTRL_TYPE_SEND_PIR_INFO_RESP,
        typedef struct _SEND_PIR_INFO_REQ {
                HI_U8 u8CheckPeople; //u8CheckPeople = 1 表示有人, u8CheckPeople = 0 表示无人,
                HI_U8 u8Reserved[31];
        } XM_SEND_PIR_INFO_REQ;

        typedef struct _COMMON_RESP {
                HI_S32 s32Result; //XM_SUCCESS 成功:   失败返回错误原因
                HI_U8 u8Reserved[12];
        } XM_COMMON_RESP;

        //停车监控模式变更、获取
        //IOCTRL_TYPE_SET_PARKING_MONITOR_MODE_REQ, 填写XM_PARKING_MONITOR_MODE结构
        typedef enum {
                PRKMONTR_MODE_SOUND_POSITION, //声音定位
                PRKMONTR_MODE_CRUISE, //巡航
                PRKMONTR_MODE_OFF, //关闭停车监控
                PRKMONTR_MODE_SOUND_POSITION_ANY_SOUND, //声音定位-听声360度转动
        } XM_PARKING_MONITOR_MODE_TYPE;

        typedef struct _SET_PARKING_MONITOR_MODE_REQ {
                HI_U8 uParkingMode; //matching XM_PARKING_MONITOR_MODE_TYPE
                HI_U8 u8Reserved[7];
        } XM_SET_PARKING_MONITOR_MODE_REQ;

        //IOCTRL_TYPE_SET_PARKING_MONITOR_MODE_RESP
        typedef struct _SET_PARKING_MONITOR_MODE_RESP {
                HI_S32 s32Result; //XM_SUCCESS 成功:   失败返回错误原因
                HI_U8 u8Reserved[12];
        } XM_SET_PARKING_MONITOR_MODE_RESP;

        //IOCTRL_TYPE_GET_PARKING_MONITOR_MODE_REQ,
        typedef struct _GET_PARKING_MONITOR_MODE_REQ {
                HI_U8 u8Reserved[8];
        } XM_GET_PARKING_MONITOR_MODE_REQ;

        //IOCTRL_TYPE_GET_PARKING_MONITOR_MODE_RESP
        typedef struct _GET_PARKING_MONITOR_MODE_RESP {
                HI_S32 s32Result; //XM_SUCCESS 成功:   失败返回错误原因
                HI_U8 uParkingMode; //matching XM_PARKING_MONITOR_MODE_TYPE
                HI_U8 u8Reserved[11];
        } XM_GET_PARKING_MONITOR_MODE_RESP;

        //APP要求设备重启
        //IOCTRL_TYPE_REBOOT_DEVICE_REQ,    无实际结构，设备收到后直接返回IOCTRL_TYPE_REBOOT_DEVICE_RESP
        //IOCTRL_TYPE_REBOOT_DEVICE_RESP,
        typedef XM_COMMON_RESP XM_REBOOT_DEVICE_RESP;

        //APP让设备马达左、右转动
        //IOCTRL_TYPE_TURN_MOTO_RESP,   通用恢复结构
        typedef XM_COMMON_RESP XM_TURN_MOTO_RESP;

        typedef enum {
                TRNMOTO_RIGHT,
                TRNMOTO_LEFT,
                TRNMOTO_CENTER
        } XM_TURN_MOTO_TYPE;

        //IOCTRL_TYPE_TURN_MOTO_REQ,
        typedef struct _TURN_MOTO_REQ {
                HI_U8 uTurnDirection; //matching XM_TURN_MOTO_TYPE
                HI_U8 u8Reserved[7];
        } XM_TURN_MOTO_REQ;

        //APP要求设备恢复出厂
        //IOCTRL_TYPE_RESET_DEVICE_REQ, 恢复出厂，reqtimes为1时为确定恢复出厂
        typedef struct _RESET_DEVICE_REQ {
                HI_U8 uReqTimes; //==0 for first request; ==1 for second request and now is also means confirm to reset.
                HI_U8 u8Reserved[7];
        } XM_RESET_DEVICE_REQ;

        //IOCTRL_TYPE_RESET_DEVICE_RESP,
        typedef XM_COMMON_RESP XM_RESET_DEVICE_RESP;

        // Device to APP，发送当前听声辩位结论
        typedef enum {
                SNDPSTINF_MID,
                SNDPSTINF_LEFT,
                SNDPSTINF_RIGHT
        } XM_SOUND_POSITION_TYPE;

        //IOCTRL_TYPE_SEND_SOUND_POSITION_INFO_REQ,
        typedef struct _SOUND_POSITION_INFO_REQ {
                HI_U8 uSoundPosition; //matching XM_SOUND_POSITION_TYPE
                HI_U8 u8Reserved[15];
        } XM_SOUND_POSITION_INFO_REQ;
        //IOCTRL_TYPE_SEND_SOUND_POSITION_INFO_RESP,

        //APP TO Dev, 设置G-SENSER灵敏度，范围20-255
        //IOCTRL_TYPE_SET_GSENSER_SENSE_LEVEL_REQ,
        typedef struct _SET_GSENSER_SENSE_LEVEL_REQ {
                HI_U8 u8Gsenser_lev; //范围20-255
                HI_U8 gsensor_mode;  // mode = 0 normal   mode = 1  parking mode
                HI_U8 gsensor_x;
                HI_U8 gsensor_y;
                HI_U8 gsensor_z;
                HI_U8 u8Reserved[11];
        } XM_SET_GSENSER_SENSE_LEVEL_REQ;

        //IOCTRL_TYPE_SET_GSENSER_SENSE_LEVEL_RESP,
        typedef XM_COMMON_RESP XM_SET_GSENSER_SENSE_LEVEL_RESP;

        // Dev to APP, 获取G-SENSER灵敏度
        //IOCTRL_TYPE_GET_GSENSER_SENSE_LEVEL_REQ, 无实际结构，设备收到后直接返回IOCTRL_TYPE_GET_GSENSER_SENSE_LEVEL_RESP
        typedef struct _GET_GSENSER_SENSE_LEVEL_REQ_ {
                HI_U8 gsensor_mode;  //  要查询哪个模式的G-sensor参数
                HI_U8 u8Reserved[7];
        } XM_GET_GSENSER_SENSE_LEVEL_REQ;

        //IOCTRL_TYPE_GET_GSENSER_SENSE_LEVEL_RESP,
        typedef struct _GET_GSENSER_SENSE_LEVEL_RESP {
                HI_S32 s32Result; //XM_SUCCESS 成功:   失败返回错误原因
                HI_U8 u8Gsenser_lev; //范围20-255
                HI_U8 gsensor_mode;  // mode = 0 normal   mode = 1  parking mode
                HI_U8 gsensor_x;
                HI_U8 gsensor_y;
                HI_U8 gsensor_z;
                HI_U8 u8Reserved[7];
        } XM_GET_GSENSER_SENSE_LEVEL_RESP;

        // Dev to APP，设置听声辩位任意触发版的声音灵敏度，单位为DB值。范围:-100 - 100
        //IOCTRL_TYPE_SET_SOUND_POSITION_SENSE_LEVEL_REQ,
        typedef struct _SET_SOUND_POSITION_SENSE_LEVEL_REQ {
                HI_S32 sense_lev; //范围-100 - 100
                HI_U8 u8Reserved[12];
        } XM_SET_SOUND_POSITION_SENSE_LEVEL_REQ;
        //IOCTRL_TYPE_SET_SOUND_POSITION_SENSE_LEVEL_RESP,
        typedef XM_COMMON_RESP XM_SET_SOUND_POSITION_SENSE_LEVEL_RESP;

        // APP to Dev，获取听声辩位任意触发版的声音灵敏度，单位为DB值。
        //IOCTRL_TYPE_GET_SOUND_POSITION_SENSE_LEVEL_REQ, 无实际结构，设备收到后直接返回IOCTRL_TYPE_GET_SOUND_POSITION_SENSE_LEVEL_RESP
        //IOCTRL_TYPE_GET_SOUND_POSITION_SENSE_LEVEL_RESP,
        typedef struct _GET_SOUND_POSITION_SENSE_LEVEL_RESP {
                HI_S32 sense_lev; //范围-100 - 100
                HI_U8 u8Reserved[12];
        } XM_GET_SOUND_POSITION_SENSE_LEVEL_RESP;

        typedef enum {
                SNAP_CURRENT, //普通拍照，只拍当前一张
                SNAP_PASSTHROUGH //穿越拍照，包含前4张和当前一张，共5张
        } SNAP_TYPE;

        // APP to Dev，设置抓拍类型:一般抓拍一次一张；穿越拍照一次多张
        //IOCTRL_TYPE_SET_SNAP_TYPE_REQ,
        typedef struct _SET_SNAP_TYPE_REQ {
                HI_U8 u8SnapType; // 参考SNAP_TYPE
                HI_U8 u8Reserved[15];
        } XM_SET_SNAP_TYPE_REQ;

        //IOCTRL_TYPE_SET_SNAP_TYPE_RESP,
        typedef XM_COMMON_RESP XM_SET_SNAP_TYPE_RESP;

        // APP to Dev，获取抓拍类型:一般抓拍一次一张；穿越拍照一次多张
        //IOCTRL_TYPE_GET_SNAP_TYPE_REQ, 无实际结构，设备收到后直接返回IOCTRL_TYPE_GET_SNAP_TYPE_RESP
        //IOCTRL_TYPE_GET_SNAP_TYPE_RESP,
        typedef struct _GET_SNAP_TYPE_RESP {
                HI_U8 u8SnapType; // 参考SNAP_TYPE
                HI_U8 u8Reserved[15];
        } XM_GET_SNAP_TYPE_RESP;

        // APP to Dev, 打开类产测功能: 声音循环播。
        //IOCTRL_TYPE_SET_FACT_TEST_AUDIO_LOOP_REQ,
        typedef struct _SET_FACT_TEST_AUDIO_LOOP_REQ {
                HI_U8 u8On; //0,为OFF；1为ON
                HI_U8 u8Reserved[15];
        } XM_SET_FACT_TEST_AUDIO_LOOP_REQ;

        //IOCTRL_TYPE_SET_FACT_TEST_AUDIO_LOOP_RESP,
        typedef XM_COMMON_RESP XM_SET_FACT_TEST_AUDIO_LOOP_RESP;

        // APP to Dev, 获取类产测功能状态: 声音循环播。
        //IOCTRL_TYPE_GET_FACT_TEST_AUDIO_LOOP_REQ, 无实际结构，设备收到后直接返回IOCTRL_TYPE_GET_FACT_TEST_AUDIO_LOOP_RESP
        //IOCTRL_TYPE_GET_FACT_TEST_AUDIO_LOOP_RESP,
        typedef struct _GET_FACT_TEST_AUDIO_LOOP_RESP {
                HI_U8 u8On; //0,为OFF；1为ON
                HI_U8 u8Reserved[15];
        } XM_GET_FACT_TEST_AUDIO_LOOP_RESP;

        //当月存储信息查询
        //IOCTRL_TYPE_DC_CHECK_SAVE_INFO_BY_MONTH_REQ,   //查询一个月中哪些天有存储数据
        //IOCTRL_TYPE_DC_CHECK_SAVE_INFO_BY_MONTH_RESP,

        typedef struct _DC_CHECK_SAVE_INFO_BY_MONTH_REQ {
                HI_U32 u32SearchTime;         //搜索时间 ，例如u32SearchTime = 20150800 查询2015年8月的数据
                HI_U8 u8FileType;       //参考 XM_DC_ENUM_FILE_TYPE
                HI_U8 u8Reserved[7];
        } XM_DC_CHECK_SAVE_INFO_BY_MONTH_REQ;

        typedef struct _DC_CHECK_SAVE_INFO_BY_MONTH_RESP {
                HI_S32  s32Result;           //XM_SUCCESS 成功:   失败返回错误原因
                HI_U32 u32SaveFileDay;          // 按bit 位来确认当天是否有存储数据，bit位(0 - 30) 这个表示1号-- 31号日期，
                //bit0 ==1 时，表示1号有存储数据,bit30 == 0时表示31号那天没有存储数据。
                HI_U8 u8FileType;       //参考 XM_DC_ENUM_FILE_TYPE
                HI_U8 u8Reserved[3];
                HI_U32 u32SearchTime;       //返回搜索时间
        } XM_DC_CHECK_SAVE_INFO_BY_MONTH_RESP;


        //下载文件
        //  IOCTRL_TYPE_DC_DELETE_FILE_REQ,
        //  IOCTRL_TYPE_DC_DELETE_FILE_RESP,


        typedef struct _DC_DELETE_FILE_REQ {
                HI_S8 strFileName[64];   //全路径的文件名，例如 /mnt/sdcard/rec/20150816/20150816_152032.mp4
                HI_U8 u8Reserved[8];
        } XM_DC_DELETE_FILE_REQ;


        typedef struct _DC_DELETE_FILE_RESP {
                HI_S32  s32Result;           //XM_SUCCESS 成功:   失败返回错误原因
                HI_U8 u8Reserved[8];
        } XM_DC_DELETE_FILE_RESP;




        //IOCTRL_TYPE_DC_GET_DAYS_OF_VALID_DATA_REQ
        //IOCTRL_TYPE_DC_GET_DAYS_OF_VALID_DATA_RESP
        typedef struct _DC_GET_DAYS_OF_VALID_DATA_REQ {
                HI_U8 u8FileType;       //参考 XM_DC_ENUM_FILE_TYPE
                HI_U8 u8Reserved[7];
        } XM_DC_GET_DAYS_OF_VALID_DATA_REQ;

        typedef struct _DC_GET_DAYS_OF_VALID_DATA_RESP {
                HI_S32  s32Result;           //XM_SUCCESS 成功:   失败返回错误原因
                HI_U32 u32ValidDays;        // >= 0
                HI_U8 u8FileType;           //参考 XM_DC_ENUM_FILE_TYPE
                HI_U8 u8Reserved[3];
        } XM_DC_GET_DAYS_OF_VALID_DATA_RESP;


        typedef enum {
                DC_ISP_FUNCATION_DEFOG = 0,
                DC_ISP_FUNCATION_LUMA,
                DC_ISP_FUNCATION_DAY_OR_NIGHT,
                DC_ISP_FUNCATION_AE,
                DC_ISP_FUNCATION_WDR,
                DC_ISP_FUNCATION_WB,
                DC_ISP_FUNCATION_CCM,
                DC_ISP_FUNCATION_CSC,
                DC_ISP_FUNCATION_NR,
                DC_ISP_FUNCATION_BUFF,
        } XM_DC_ISP_FUNC_CTRL;

        //IOCTRL_TYPE_DC_SET_ISP_FUNC_REQ,
        //IOCTRL_TYPE_DC_SET_ISP_FUNC_RESP,
        //IOCTRL_TYPE_DC_GET_ISP_FUNC_REQ,
        //IOCTRL_TYPE_DC_GET_ISP_FUNC_RESP,
        typedef struct _DC_SET_ISP_FUNC_REQ {
                HI_U8 u8ISP_Funcation_Type;  //reference to  XM_DC_ISP_FUNC_CTRL
                HI_U8 enable;  //  1    open    0            1  day   0 night
                HI_U16 parameter_length;
                HI_U8 u8Reserved[4];
        } XM_DC_SET_ISP_FUNC_REQ;

        typedef struct _DC_SET_ISP_FUNC_RESP {
                HI_S32  s32Result;
                HI_U8   u8ISP_Funcation_Type;
                HI_U8 u8Reserved[3];
        } XM_DC_SET_ISP_FUNC_RESP;

        typedef struct _DC_GET_ISP_FUNC_REQ {
                HI_U8 u8ISP_Funcation_Type;     //reference to  XM_DC_ISP_FUNC_CTRL
                HI_U8 u8Reserved[7];

        } XM_DC_GET_ISP_FUNC_REQ;

        typedef struct _DC_GET_ISP_FUNC_RESP {
                HI_S32   s32Result;
                HI_U8   enable;
                HI_U8   u8ISP_Funcation_Type;
                HI_U16  parameter_length;

        } XM_DC_GET_ISP_FUNC_RESP;


        //IOCTRL_TYPE_SET_SOUND_RECORD_ONOFF_REQ
        typedef struct _SET_SOUND_RECORD_ONOFF_REQ {
                HI_S32   u32On;  // 1 for on; 0 for off
                HI_U8 u8Reserved[4];
        } XM_SET_SOUND_RECORD_ONOFF_REQ;

        typedef XM_COMMON_RESP XM_SET_SOUND_RECORD_OFF_RESP;

        //IOCTRL_TYPE_GET_SOUND_RECORD_ONOFF_REQ
        //APP请求不需要结构，设备收到则回复

        //IOCTRL_TYPE_GET_SOUND_RECORD_ONOFF_RESP
        typedef struct _GET_SOUND_RECORD_ONOFF_RESP {
                HI_S32   s32Result;
                HI_S32   u32On;  // 1 for on; 0 for off
        } XM_GET_SOUND_RECORD_ONOFF_RESP;

        // IOCTRL_TYPE_SET_PARKING_DELAY_TIME_REQ,
        // IOCTRL_TYPE_SET_PARKING_DELAY_TIME_RESP,
        typedef struct _XM_SET_PARKING_DELAY_REQ_ {
                HI_U32 u32ParkingDelyTime;  // 进入停车监控模式的延迟时间单位(秒)
                HI_U8 u8Reserve[12]; // 保留数据
        } XM_SET_PARKING_DELAY_REQ;

        typedef XM_COMMON_RESP XM_SET_PARKING_DELAY_RESP;

        // IOCTRL_TYPE_GET_PARKING_DELAY_TIME_REQ,
        // IOCTRL_TYPE_GET_PARKING_DELAY_TIME_RESP,
        typedef struct _XM_GET_PARKING_DELAY_RESP_ {
                HI_S32   s32Result;
                HI_U32 u32ParkingDelyTime;  // 进入停车监控模式的延迟时间单位(秒)
        } XM_GET_PARKING_DELAY_RESP;

        //////////////////////////////////////////////////////////////////////////////////////////////////

        /******************************************************************************************
          局域网内设备广播检测
         *******************************************************************************************/

        ////////相关数据结构////////

#define XM_REQUEST_DEV_INFO 5

#define XM_BROADCAST_RECV_PORT (22356)

        typedef struct _DEV_NET_INFO {
                HI_U32 u32ListenPort; // 设备监听端口
                HI_S8 u8IP[16]; //设备IP
                HI_S8 u8MAC[18]; // 设备 MAC
                HI_U8 u8Reserved[2];
        } XM_DEV_NET_INFO;

        typedef struct _DEV_BROADCAST_ {
                HI_S8 strDevId[64]; //设备唯一ID   ,
                XM_DEV_NET_INFO apInfo; //无线AP 模式，网络信息
                XM_DEV_NET_INFO staInfo; // 无线STA 模式.  网络信息
                XM_DEV_NET_INFO lineInfo; //有线模式，网络信息
                HI_U32 u32SoftwareV; //软件版本
                HI_U32 u32FirewareV; //硬件版本
                HI_U8 u8DevType; // 设备类型
                HI_U8 u8Cmd; // 设备端发送查询信息时，填XM_REQUEST_DEV_INFO 值。
                HI_U8 u8Reserved[6];
        } XM_DEV_SEND_BROADCAST, XM_DEV_RECV_BROADCAST;

        ///////////////////////////////////////////////////////////////////////////////////////////

        /******************************************************************************************
          网络封包
         *******************************************************************************************/

        ////////相关数据结构////////

        typedef enum {
                SIO_TYPE_UNKN,
                SIO_TYPE_VIDEO_AUDIO_FRAME,
                SIO_TYPE_IOCTRL,
                SIO_TYPE_DOWNLOAD_DATA,
                SIO_TYPE_HEART_ALIVE_PACKET,
                SIO_TYPE_UPGRADE_PACKET,
        } XM_STREAM_IO_TYPE;

        //NOTE: struct below is all Little Endian
        typedef struct {
                HI_U8 u8StreamIOType; //填写 XM_STREAM_IO_TYPE  中相关项
                HI_U8 u8Reserved[3];
                HI_U32 u32DataSize; //整个数据包长度
        } XM_STREAM_IO_HEAD;

        //for SIO_TYPE_IOCTRL
        typedef struct {
                HI_U16 u16IOCtrlType; //参考 XM_IOCTRL_TYPE
                HI_U16 u16IOCtrlDataSize;
                HI_U8 u8Reserve[4];
        } XM_IO_CTRL_HEAD;

        typedef struct _VIDEO_FRAME_HEADER_ {
                HI_U32 nTimeStampSec; // 时间戳秒
                HI_U32 nTimeStampUSec; // 时间戳微秒
                HI_U32 u32FrameDataLen; // 帧数据长度
                HI_U8 u8Code; //压缩格式 XM_CODE_TYPE
                HI_U8 u8VideoFrameType; // I帧 or  p帧or b帧  XM_VIDEO_FRAME
                HI_U8 u8FrameIndex; //帧计数，当是I帧时，u8rameIndex = 0，p帧时u8rameIndex=u8rameIndex+1. 这个变量主要用来判断丢帧情况。
                HI_U8 u8Reserved2;
                HI_U16 u16PicWidth; //图像的宽
                HI_U16 u16PicHeight; //图像的高
                HI_U8 u8Reserved[4];
        } XM_VIDEO_FRAME_HEADER;

        typedef struct _AUDIO_FRAME_HEADER_ {
                HI_U8 u8Code; //压缩格式XM_CODE_TYPE
                HI_U8 u8AudioSamplerate; //采样率 XM_AUDIO_SAMPLERATE
                HI_U8 u8AudioBits; //采样位宽XM_AUDIO_DATABITS
                HI_U8 u8AudioChannel; //通道        XM_AUDIO_CHANNEL
                HI_U32 nTimeStampSec; // 时间戳 微秒秒
                HI_U32 nTimeStampUSec; // 时间戳微秒
                HI_U32 u32FrameDataLen; // 帧数据长度
                HI_U8 u8FrameIndex; //帧计数，每来一帧 u8rameIndex=u8rameIndex+1. 这个变量主要用来判断丢帧情况。
                HI_U8 u8Reserved[7];
        } XM_AUDIO_FRAME_HEADER;

        // for SIO_TYPE_VIDEO_AUDIO_FRAME
        typedef struct _FRAME_HEAD_ {
                HI_U8 u8FrameType; // 参考 XM_FRAME_TYPE
                HI_U8 u8FrameUseType; // 参考 FRAME_USE_TYPE
                HI_U16 u16FrameSplitPackTotalNum; // 单帧数据大小超过发送缓冲时，进行分包，u8FramePackNum表示分包的总个数。小于等于1  时，表示未分包。
                HI_U16 u16SplitPackNo; // 当前分包的序号
                HI_U16 u16SessionId;
                union {
                        XM_VIDEO_FRAME_HEADER stVideoFrameHead;
                        XM_AUDIO_FRAME_HEADER stAudioFrameHead;
                };
        } XM_FRAME_HEAD;

        // for SIO_TYPE_SEND_FILE_DATA
        typedef struct _FILE_DOWNLOAD_PACK_HEAD_ {
                HI_U32 u32FileTotalSize; //发送文件的总长度
                HI_U32 u32TotalSendPackNum; //要发送的总包数
                HI_U32 u32CurPackNo; //当前包的序号
                HI_U32 u32CurPackSize; //当前包中录像文件长度
                HI_U32 uCRC; // uCRC=0 时不效验
                HI_U8 u8Endflag; //u8Endflag=1: 表示当前包是结束包
                HI_U8 u8Reserve[3];
                HI_U16 u16SessionId; // 任务ID
                HI_U8 u8Reserve2[2];
                HI_U8 u8Data[0]; //数据的起始地址
        } XM_FILE_DOWNLOAD_PACK_HEAD;


	typedef struct _GET_DEV_LED_REQ {
		HI_U8 u8Reserve[8];
	}XM_GET_DEV_LED_REQ;

	typedef struct _GET_DEV_LED_RESP {
		HI_S32 s32Result;
		HI_U8 u8Enable; // 0 1 2 3
		HI_U8 reserve[7];
	}XM_GET_DEV_LED_RESP;

	typedef struct _SET_DEV_LED_REQ {
		HI_U8 u8Enable; // 0 1 2 3
		HI_U8 reserve[7];
	}XM_SET_DEV_LED_REQ;

	typedef struct _SET_DEV_LED_RESP {
		HI_S32 s32Result;
		HI_U8 reserve[8];
	}XM_SET_DEV_LED_RESP;

	typedef struct _GET_DEV_CBC_REQ {
		HI_U8 reserve[8];
	}XM_GET_DEV_CBC_REQ;

	typedef struct _GET_DEV_CBC_RESP {
		HI_S32 s32Result;
		HI_U8 u8CBCNumber; // 0-4
		HI_U8 reserve[7];
	}XM_GET_DEV_CBC_RESP;

        // 网络包发送格式

        /*   控制数据包
             XM_STREAM_IO_HEAD + XM_IO_CTRL_HEAD + 控制数据
             */

        /*  音视频数据包
            XM_STREAM_IO_HEAD + XM_FRAME_HEAD + 音视频数据
            */

        /* 下载文件数据包
           XM_STREAM_IO_HEAD + XM_FILE_DOWNLOAD_PACK_HEAD + 视频文件数据
           */

        /* 升级文件数据包
           XM_STREAM_IO_HEAD + XM_UPGRADE_HEAD + 升级文件数据
           */

        /*  心跳包发送
            XM_STREAM_IO_HEAD     u8StreamIOType=SIO_TYPE_HEART_ALIVE_PACKET
            */

        ////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif /* _XM_NET_CTROL_PROTOCOL_H_ */
