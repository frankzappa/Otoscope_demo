#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavfilter/avfilter.h"
#include "libavutil/imgutils.h"
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
//#include <libavutil/timestamp.h>

#ifdef __cplusplus
}
#endif

#define STREAM_WIDTH    640
#define STREAM_HEIGHT   480

#define STREAM_DURATION   1000.0
#define STREAM_FRAME_RATE 25 /* 25 images/s */
#define STREAM_NB_FRAMES  ((int)(STREAM_DURATION * STREAM_FRAME_RATE))
#define STREAM_PIX_FMT    AV_PIX_FMT_YUV420P /* default pix_fmt */


unsigned char recv_buff[2048];

#import "client.h"

#include "client.hpp"
#include "xm_net_ctrol_protocol.hpp"
#include "aes_ende.hpp"

// a wrapper around a single output AVStream
typedef struct OutputStream {
    AVStream *st;
    AVCodecContext *enc;

    /* pts of the next frame that will be generated */
    int64_t next_pts;
    int samples_count;

    AVFrame *frame;
    AVFrame *tmp_frame;

    float t, tincr, tincr2;

    struct SwsContext *sws_ctx;
    struct SwrContext *swr_ctx;
} OutputStream;

int isFFmpegInitialized;

AVFrame* m_pYUVFrame;
AVFrame* m_pRGBFrame;

AVCodecContext* m_pCodecCtx;
AVCodec* m_pAVCodec;
AVCodecParserContext* m_pCodecPaser;
SwsContext* m_pAVConvertCtx;

AVOutputFormat *m_pAVOutputFmt;
AVFormatContext *m_pAVOutputCtx;

AVFrame *m_pOutframe;
AVPicture src_picture, dst_picture;
AVStream *m_pAVOutStream;
AVCodec  *m_pAVOutCodec;

OutputStream m_OutputVideoStream = { 0 };

int g_totalPts;

typedef struct g_state_ {
    unsigned int running;
    unsigned int upgrading;
}g_state_t;

int sock;
g_state_t g_state_main;
FILE* video_fd = NULL;

int g_recording_flag;

int sws_flags = SWS_BICUBIC;
int frame_count;

int g_init_engine = -1;
int g_socket_status = -1;

NSObject *sViewController;

SEL onFrameDataRecvSelector = @selector(onFrameDataRecv:);
SEL onSnapRecvSelector = @selector(onSnapRecv);

void*   recv_thread_process(void* args);
void    handshake_process(int counter);
void    io_process(char* buf);
void    media_process(unsigned char * buf, unsigned int length);
void    get_version(void);
void    get_led_status(void);
void    set_led_status(unsigned int level);
int     video_stream_ctrl(unsigned int on_off);
void    get_cbc(void);
int     initVideoLibrary();
int     parseFrame(unsigned char* buf, int size);
int     decodeFrame(AVCodecContext *dec_ctx, AVFrame *frame, AVPacket *pkt, int width, int height);
void    invokeFrameDataCallback(AVFrame* frame, int width, int height);

unsigned int cal_crc(const unsigned int* buf, int count)
{
    unsigned int i;
    unsigned int sum = 0;

    for (i = 0; i < count; i++) {
        sum ^= buf[i];
    }
    return ~(int)sum;
}


void get_version(void)
{
    XM_STREAM_IO_HEAD head;
    XM_IO_CTRL_HEAD io_ctrl;
    unsigned char buf[1024];
    unsigned int size = 0;

    head.u8StreamIOType = SIO_TYPE_IOCTRL;
    head.u32DataSize = sizeof(XM_IO_CTRL_HEAD) + sizeof(XM_STREAM_IO_HEAD);
    io_ctrl.u16IOCtrlType = IOCTRL_TYPE_GET_SYSFWVER_REQ;
    io_ctrl.u16IOCtrlDataSize = sizeof(XM_STREAM_IO_HEAD);

    size = sizeof(XM_STREAM_IO_HEAD) + sizeof(XM_IO_CTRL_HEAD);
    memcpy(buf, &head, sizeof(XM_STREAM_IO_HEAD));
    memcpy(buf+sizeof(XM_STREAM_IO_HEAD), &io_ctrl, sizeof(XM_IO_CTRL_HEAD));
    printf("----------------- client.mm 149, %d\n", sock);
    if(send(sock, buf, size, 0) < 0) {
        printf("send get sysfwver failed\n");
    } else {
        printf("send get sysfwver success\n");
    }
}

void get_led_status(void)
{
    XM_STREAM_IO_HEAD head;
    XM_IO_CTRL_HEAD io_ctrl;
    unsigned char buf[1024];
    unsigned int size = 0;

    head.u8StreamIOType = SIO_TYPE_IOCTRL;
    head.u32DataSize = sizeof(XM_IO_CTRL_HEAD) + sizeof(XM_STREAM_IO_HEAD);
    io_ctrl.u16IOCtrlType = IOCTRL_TYPE_GET_LED_REQ;
    io_ctrl.u16IOCtrlDataSize = sizeof(XM_STREAM_IO_HEAD);

    size = sizeof(XM_STREAM_IO_HEAD) + sizeof(XM_IO_CTRL_HEAD);
    memcpy(buf, &head, sizeof(XM_STREAM_IO_HEAD));
    memcpy(buf+sizeof(XM_STREAM_IO_HEAD), &io_ctrl, sizeof(XM_IO_CTRL_HEAD));
    printf("------------------ client.mm 172, sock, buf, size: %d, %d, %d\n", sock, buf, size);
    if(send(sock, buf, size, 0) < 0) {
        printf("send get led status failed\n");
    } else {
        printf("send get led status success\n");
    }
}

void set_led_status(unsigned int level)
{
    XM_STREAM_IO_HEAD head;
    XM_IO_CTRL_HEAD io_ctrl;
    XM_SET_DEV_LED_REQ req;
    unsigned char buf[1024];
    unsigned int size = 0;

    head.u8StreamIOType = SIO_TYPE_IOCTRL;
    head.u32DataSize = sizeof(XM_IO_CTRL_HEAD) + sizeof(XM_STREAM_IO_HEAD);
    io_ctrl.u16IOCtrlType = IOCTRL_TYPE_SET_LED_REQ;
    io_ctrl.u16IOCtrlDataSize = sizeof(XM_STREAM_IO_HEAD);
    req.u8Enable = (unsigned char)level;

    size = sizeof(XM_STREAM_IO_HEAD) + sizeof(XM_IO_CTRL_HEAD) + sizeof(XM_SET_DEV_LED_REQ);
    memcpy(buf, &head, sizeof(XM_STREAM_IO_HEAD));
    memcpy(buf+sizeof(XM_STREAM_IO_HEAD), &io_ctrl, sizeof(XM_IO_CTRL_HEAD));
    memcpy(buf+sizeof(XM_STREAM_IO_HEAD) + sizeof(XM_IO_CTRL_HEAD), &req, sizeof(XM_SET_DEV_LED_REQ));
    printf("-------------------- client.mm 198, sock, buf, size: %d, %d, %d\n", sock, buf, size);
    if(send(sock, buf, size, 0) < 0) {
        printf("send set led status %d failed\n", level);
    } else {
        printf("send set led status %d success\n", level);
    }
}

int video_stream_ctrl(unsigned int on_off)
{
    if (on_off == 0){
        printf("------------------- client.mm line 206: video_strem_ctrl -- stop\n");
    }
    int result;
    /*
    socket package data format
    XM_STREAM_IO_HEAD + XM_IO_CTRL_HEAD + CTRL_DATA
    */

    XM_STREAM_IO_HEAD head;
    XM_IO_CTRL_HEAD io_ctrl;
    XM_LIVE_START_REQ start_req;
        /* XM_LIVE_STOP_REQ stop_req; */
    unsigned char buf[1024];
    unsigned int size = 0;

    memset((char*)&head, 0, sizeof(XM_STREAM_IO_HEAD));
    memset((char*)&io_ctrl, 0, sizeof(XM_IO_CTRL_HEAD));
    memset((char*)&start_req, 0, sizeof(XM_LIVE_START_REQ));

    head.u8StreamIOType = SIO_TYPE_IOCTRL;
    if (on_off == 1) {
        head.u32DataSize = sizeof(XM_STREAM_IO_HEAD) + \
                   sizeof(XM_IO_CTRL_HEAD) + \
                   sizeof(XM_LIVE_START_REQ);
        io_ctrl.u16IOCtrlDataSize = sizeof(XM_IO_CTRL_HEAD) + \
                   sizeof(XM_LIVE_START_REQ);
        io_ctrl.u16IOCtrlType = IOCTRL_TYPE_LIVE_START_REQ;

        start_req.u8EnableVideoSend = 1;
        start_req.u8EnableAudioSend = 0;
        start_req.u8VideoChan = 1;

        memcpy(buf, &head, sizeof(XM_STREAM_IO_HEAD));
        memcpy(buf+sizeof(XM_STREAM_IO_HEAD), &io_ctrl, sizeof(XM_IO_CTRL_HEAD));
        memcpy(buf+sizeof(XM_STREAM_IO_HEAD)+sizeof(XM_IO_CTRL_HEAD), &start_req, sizeof(XM_LIVE_START_REQ));
    } else {
        /* xm_live_stop_req no need request data */
        head.u32DataSize = sizeof(XM_STREAM_IO_HEAD) + \
                   sizeof(XM_IO_CTRL_HEAD);
        io_ctrl.u16IOCtrlDataSize = sizeof(XM_STREAM_IO_HEAD);
        io_ctrl.u16IOCtrlType = IOCTRL_TYPE_LIVE_STOP_REQ;

        memcpy(buf, &head, sizeof(XM_STREAM_IO_HEAD));
        memcpy(buf+sizeof(XM_STREAM_IO_HEAD), &io_ctrl, sizeof(XM_IO_CTRL_HEAD));
    }

    size = head.u32DataSize;
    if(send(sock, buf, size, 0) < 0) {
        printf("----------------------c 254 \n");
        printf("send video stream on_off = %d failed\n", on_off);
        result = -1;
    } else {
        printf("----------------------c 258 \n");
        printf("send video stream on_off = %d success\n", on_off);
        result = 1;
    }

    return result;
}

void get_cbc(void)
{
    XM_STREAM_IO_HEAD head;
    XM_IO_CTRL_HEAD io_ctrl;
    unsigned char buf[1024];
    unsigned int size = 0;

    head.u8StreamIOType = SIO_TYPE_IOCTRL;
    head.u32DataSize = sizeof(XM_IO_CTRL_HEAD) + sizeof(XM_STREAM_IO_HEAD);
    io_ctrl.u16IOCtrlType = IOCTRL_TYPE_GET_CBC_REQ;
    io_ctrl.u16IOCtrlDataSize = sizeof(XM_STREAM_IO_HEAD);

    size = sizeof(XM_STREAM_IO_HEAD) + sizeof(XM_IO_CTRL_HEAD);
    memcpy(buf, &head, sizeof(XM_STREAM_IO_HEAD));
    memcpy(buf+sizeof(XM_STREAM_IO_HEAD), &io_ctrl, sizeof(XM_IO_CTRL_HEAD));
    printf("------------------- client.mm 284, sock, buf, size: %d, %d, %d",  sock, buf, size);
    if(send(sock, buf, size, 0) < 0) {
        printf("send get cbc failed\n");
    } else {
        //printf("send get cbc success\n");
    }
}



#define FILE_SLICE   1024

void handshake_process(int counter)
{
    printf("%d\n", counter);
    printf("------------------------ client.mm, line 292, handshake starts \n");
    XM_STREAM_IO_HEAD handshake;
    handshake.u8StreamIOType = SIO_TYPE_HEART_ALIVE_PACKET;
    handshake.u32DataSize = 0;
    int send_result = send(sock, &handshake, sizeof(XM_STREAM_IO_HEAD), 0);
    if (send_result < 0) {
    //if(send(sock, &handshake, sizeof(XM_STREAM_IO_HEAD), 0) < 0) {
        printf("--------********----------- client.mm, line 300, send_result: %s", send_result);
        printf("send handshake failed\n");
    } else {
//        printf(" handshake ok\n");
    }
    printf("------------------------ client.mm, line 305, quit handshake \n");
}
void io_process(char* buf)
{
    XM_IO_CTRL_HEAD* head;
    //XM_IOCTRL_TYPE type;
    HI_U16 type;
    XM_AUTHORIZE_RESP* resp_authorize;
    XM_GET_SYSFWVER_RESP* resp_get_sysfwver;
    XM_GET_DEV_LED_RESP* resp_get_led;
    XM_SET_DEV_LED_RESP* resp_set_led;
    XM_LIVE_START_RESP* resp_live_start;
    XM_LIVE_STOP_RESP* resp_live_stop;
    XM_GET_DEV_CBC_RESP* resp_get_cbc;
    XM_REBOOT_DEVICE_RESP* resp_reboot;
    XM_UPGREDE_RESP* resp_upgrade;

    int offset = 0;

    offset = sizeof(XM_STREAM_IO_HEAD) + sizeof(XM_IO_CTRL_HEAD);

    head = (XM_IO_CTRL_HEAD*)(buf + sizeof(XM_STREAM_IO_HEAD));
    type = head->u16IOCtrlType;
    printf("----------------------- client.mm, 334, io_process(buf), type: %i\n", type);

    switch(type) {
    case IOCTRL_TYPE_AUTHORIZE_RESP:                        // 68
        resp_authorize = (XM_AUTHORIZE_RESP*)(buf + offset);
        if (resp_authorize->s32Result == XM_SUCCESS) {
            printf("authorize success\n");
        } else {
            printf("authorize failed\n");
        }
        break;
    case IOCTRL_TYPE_GET_SYSFWVER_RESP:                     // 40
        resp_get_sysfwver = (XM_GET_SYSFWVER_RESP*)(buf + offset);
        if (resp_get_sysfwver->s32Result == XM_SUCCESS) {
            printf("HW %d.%d.%d.%d \n",
                    (resp_get_sysfwver->u32VerFW&0xFF000000)>>24,
                    (resp_get_sysfwver->u32VerFW&0x00FF0000)>>16,
                    (resp_get_sysfwver->u32VerFW&0x0000FF00)>>8,
                    (resp_get_sysfwver->u32VerFW&0x000000FF));
            printf("SW %d.%d.%d.%d \n",
                    (resp_get_sysfwver->u32VerSW&0xFF000000)>>24,
                    (resp_get_sysfwver->u32VerSW&0x00FF0000)>>16,
                    (resp_get_sysfwver->u32VerSW&0x0000FF00)>>8,
                    (resp_get_sysfwver->u32VerSW&0x000000FF));
        } else {
            printf("get version failed\n");
        }
        break;
    case IOCTRL_TYPE_GET_LED_RESP:                      // 293
        resp_get_led = (XM_GET_DEV_LED_RESP*)(buf + offset);
        if (resp_get_led->s32Result == XM_SUCCESS) {
            printf("get LED = %d\n", resp_get_led->u8Enable);
        } else {
            printf("get LED status failed\n");
        }
        break;
    case IOCTRL_TYPE_SET_LED_RESP:                      // 295
        resp_set_led = (XM_SET_DEV_LED_RESP*)(buf + offset);
        if (resp_set_led->s32Result == XM_SUCCESS) {
            printf("set led success\n");
        } else {
            printf("set led failed\n");
        }
        break;
    case IOCTRL_TYPE_LIVE_START_RESP:                   // 2
        resp_live_start = (XM_LIVE_START_RESP*)(buf + offset);
        if (resp_live_start->s32Result == XM_SUCCESS) {
            printf("live start success\n");
            
        } else {
            printf("live start failed\n");
        }
        break;
    case IOCTRL_TYPE_LIVE_STOP_RESP:                    // 4
        resp_live_stop = (XM_LIVE_STOP_RESP*)(buf + offset);
        if (resp_live_stop->s32Result == XM_SUCCESS) {
            printf("live stop success\n");
        } else {
            printf("live stop failed\n");
        }
        break;
    case IOCTRL_TYPE_GET_CBC_RESP:                      // 297
        resp_get_cbc = (XM_GET_DEV_CBC_RESP*)(buf + offset);
        if (resp_get_cbc->s32Result == XM_SUCCESS) {
            printf("get CBC= %d\n", resp_get_cbc->u8CBCNumber);
        } else {
            printf("get CBC failed\n");
        }
        break;
    case IOCTRL_TYPE_REBOOT_DEVICE_RESP:                   // 255
        resp_reboot = (XM_REBOOT_DEVICE_RESP*)(buf + offset);
        if (resp_reboot->s32Result == XM_SUCCESS) {
            printf("set reboot success\n");
        } else {
            printf("set reboot failed\n");
        }
        break;
    case IOCTRL_TYPE_UPGRADE_READY:                         // 70
        resp_upgrade = (XM_UPGREDE_RESP*)(buf + offset);
        if (resp_upgrade->s32Result == XM_SUCCESS) {
            printf("device ready for recive upgrade data\n");
            printf("--------------------- client 412, g_state_main.running: %d, g_state_main.upgrading: %d", g_state_main.running, g_state_main.upgrading);
            g_state_main.upgrading = 1; // trig sending upgrade data
        } else {
            printf("device can't upgrade error=%d\n", resp_upgrade->s32Result);
        }
        break;
    case IOCTRL_TYPE_UPGRADE_OK:                            // 72
        break;
    case IOCTRL_TYPE_UPGRADE_FAILED:                        // 73
        break;
    case IOCTRL_TYPE_DC_SNAP_REQ:                           // 210
        printf("device trigger to snap\n");
            [sViewController performSelectorOnMainThread:onSnapRecvSelector withObject:nil waitUntilDone:false];
        break;
    default :
        printf("unused IOCTRL_TYPE %d\n", type);
        break;
    }
}

/* Prepare a dummy image. */
void fill_yuv_image(AVFrame *pict, int frame_index,
                           int width, int height)
{
    int x, y, i;
    i = frame_index;
    /* Y */
    for (y = 0; y < height; y++)
        for (x = 0; x < width; x++)
            pict->data[0][y * pict->linesize[0] + x] = x + y + i * 3;
    /* Cb and Cr */
    for (y = 0; y < height / 2; y++) {
        for (x = 0; x < width / 2; x++) {
            pict->data[1][y * pict->linesize[1] + x] = 128 + y + i * 2;
            pict->data[2][y * pict->linesize[2] + x] = 64 + x + i * 5;
        }
    }
}

void log_packet(const AVFormatContext *fmt_ctx, const AVPacket *pkt)
{
    AVRational *time_base = &fmt_ctx->streams[pkt->stream_index]->time_base;

    /*
    printf("pts:%s pts_time:%s dts:%s dts_time:%s duration:%s duration_time:%s stream_index:%d\n",
           av_ts2str(pkt->pts), av_ts2timestr(pkt->pts, time_base),
           av_ts2str(pkt->dts), av_ts2timestr(pkt->dts, time_base),
           av_ts2str(pkt->duration), av_ts2timestr(pkt->duration, time_base),
           pkt->stream_index);*/
}

int write_frame(AVFormatContext *fmt_ctx, const AVRational *time_base, AVStream *st, AVPacket *pkt)
{
    /* rescale output packet timestamp values from codec to stream timebase */
    av_packet_rescale_ts(pkt, *time_base, st->time_base);
    pkt->stream_index = st->index;

    /* Write the compressed frame to the media file. */
    log_packet(fmt_ctx, pkt);
    return av_interleaved_write_frame(fmt_ctx, pkt);

}


void convYuv420spToRGBByte(uint8_t* yuv420sp, int width, int height, uint8_t* rgb)
{
    int frameSize = width * height;
    //int wstep = 3*width;
    int wstep = (24*width+31)/32*4;
    int v1,v2,u1,u2;
    for (int j = 0, yp = 0; j < height; j++) {
        int uvp = frameSize + (j >> 1) * width, u = 0, v = 0;
        
        for (int i = 0; i < width; i++, yp++) {
            int y = (0xff & ((int) yuv420sp[yp])) - 16;
            if (y < 0)
                y = 0;
            if ((i & 1) == 0) {
                v = (0xff & yuv420sp[uvp++]) - 128;
                u = (0xff & yuv420sp[uvp++]) - 128;
                v1 = 1634 * v;
                v2 = 833 * v;
                u1 = 400 * u;
                u2 = 2066 * u;
            }
            
            int y1192 = 1192 * y;
            int r = (y1192 + v1);
            int g = (y1192 - v2 - u1);
            int b = (y1192 + u2);
            
            if (r < 0)
                r = 0;
            else if (r > 262143)
                r = 262143;
            if (g < 0)
                g = 0;
            else if (g > 262143)
                g = 262143;
            if (b < 0)
                b = 0;
            else if (b > 262143)
                b = 262143;
            
            rgb[(height-1-j)*wstep+i*3+2] = (b>>10);
            rgb[(height-1-j)*wstep+i*3+1] = (g>>10);
            rgb[(height-1-j)*wstep+i*3] = (r>>10);//b;
        }
    }
}



void invokeFrameDataCallback(AVFrame* frame, int width, int height)
{
    printf("---------------- client 530, enters invokeFrameDataCallback()");
    CGBitmapInfo bitmapInfo = kCGBitmapByteOrderDefault;
    CFDataRef data = CFDataCreateWithBytesNoCopy(kCFAllocatorDefault, frame->data[0], frame->linesize[0]*height,kCFAllocatorNull);
    
    CGDataProviderRef provider = CGDataProviderCreateWithCFData(data);
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    CGImageRef cgImage = CGImageCreate(width,
                                       height,
                                       8,
                                       24,
                                       frame->linesize[0],
                                       colorSpace,
                                       bitmapInfo,
                                       provider,
                                       NULL,
                                       NO,
                                       kCGRenderingIntentDefault);
    CGColorSpaceRelease(colorSpace);
    UIImage *image = [UIImage imageWithCGImage:cgImage];
    CGImageRelease(cgImage);
    CGDataProviderRelease(provider);
    CFRelease(data);
  
    [sViewController performSelectorOnMainThread:onFrameDataRecvSelector withObject:image waitUntilDone:false];
    printf("---------------- client 554, exiting invokeFrameDataCallback()");
}

int decodeFrame(AVCodecContext *dec_ctx, AVFrame *frame, AVPacket *pkt, int width, int height)
{
    printf("---------------- client 559, enters decodeFrame()\n");
    int ret;

    ret = avcodec_send_packet(dec_ctx, pkt);//crash
    if (ret < 0) {
        printf("Error sending a packet for decoding\n");
        return -1;
    }

    while (ret >= 0) {
        
        ret = avcodec_receive_frame(dec_ctx, frame);//crash
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
            return -1;
        else if (ret < 0) {
            printf("Error during decoding\n");
            return -1;
        }
        
        sws_scale(m_pAVConvertCtx, frame->data, frame->linesize, 0, STREAM_HEIGHT, m_pRGBFrame->data, m_pRGBFrame->linesize);
       
        invokeFrameDataCallback(m_pRGBFrame, width, height);
         
    }
    printf("---------------- client 583, leaving decodeFrame()\n");
    return 1;
}


int parseFrame(unsigned char* buf, int size)
{
    int paserLength_In = size;
    int paserLen;
    int decode_data_length;
    int got_picture = 0;
    unsigned char *pFrameBuff = (unsigned char*) buf;
    while (paserLength_In > 0)
    {
        printf("----------------- client.mm 597, paserLengh_In: %d\n", paserLength_In);
        AVPacket packet;
        av_init_packet(&packet);

        paserLen = av_parser_parse2(m_pCodecPaser, m_pCodecCtx, &packet.data, &packet.size, pFrameBuff,
                                    paserLength_In, AV_NOPTS_VALUE, AV_NOPTS_VALUE, AV_NOPTS_VALUE);

        //LOGD("paserLen = %d",paserLen);
        paserLength_In -= paserLen;
        pFrameBuff += paserLen;
        if (packet.size > 0 && m_pCodecPaser->key_frame > 0){

//            if (packet.size > 0){

//            printf("packet size=%d, width=%d, height=%d, I-frame=%d, frame_num=%d\n",
//                 packet.size,
//                 m_pCodecPaser->width,
//                 m_pCodecPaser->height,
//                 m_pCodecPaser->key_frame,
//                 m_pCodecPaser->output_picture_number
//            );

            decodeFrame(m_pCodecCtx, m_pYUVFrame, &packet, m_pCodecPaser->width, m_pCodecPaser->height);
            
            if(g_recording_flag > 0){

                AVCodecContext *c = m_OutputVideoStream.enc;
                //int ret = write_frame(m_pAVOutputCtx, &c->time_base, m_OutputVideoStream.st, &packet);

                packet.stream_index = m_OutputVideoStream.st->index;

                int duration = STREAM_DURATION;
                g_totalPts += duration;
                packet.pts = g_totalPts;
                packet.dts = g_totalPts;

                int ret = av_write_frame(m_pAVOutputCtx, &packet);
                if (ret < 0)
                    printf("Error while writing video frame: %s\n", av_err2str(ret));
            }
        }
        printf("----------------- client.mm 638, paserLengh_In: %d\n", paserLength_In);
        av_free_packet(&packet);
    }

    return 1;
}


unsigned char tmp_buf[1024*1024*50];
unsigned int buf_index = 0;

void media_process(unsigned char * buf, unsigned int length)
{
    /* audio and video data packet */
    /* XM_STREAM_IO_HEAD + XM_FRAME_HEAD + audio/video packet */
    XM_FRAME_HEAD* frame = NULL;
    unsigned char* data = NULL;
    XM_VIDEO_FRAME_HEADER* video = NULL;
    /* unsigned char NALU; */
    int remain_length = 0;
    int tmp_length = 0;
    int head_length = 0;
    int copy_length = 0;
    int total_length = 0;

    head_length = sizeof(XM_STREAM_IO_HEAD) +  sizeof(XM_FRAME_HEAD);
    printf("------------------------ client.mm 664, head_leghth: %d\n", head_length);
    //frame = (XM_FRAME_HEAD*)(buf + sizeof(XM_STREAM_IO_HEAD));
    frame = (XM_FRAME_HEAD*)(buf + sizeof(XM_STREAM_IO_HEAD));
    if (!frame) {
        printf("error drop\n");
    }
    /* NALU = *(buf + sizeof(XM_STREAM_IO_HEAD) + sizeof(XM_FRAME_HEAD) + 4) &0x1F; */
    /* there is no audio packet in this project */
    if (frame->u8FrameType == FRAME_TYPE_VIDEO) {
        /* printf("%02d %d:%d %08d len=%08d %X %X %X(%d) ", */
        /*     frame->stVideoFrameHead.u8FrameIndex, */
        /*     frame->u16FrameSplitPackTotalNum, */
        /*     frame->u16SplitPackNo, */
        /*     frame->stVideoFrameHead.nTimeStampUSec, */
        /*     frame->stVideoFrameHead.u32FrameDataLen, */
        /*     (NALU&0x80)>>7, (NALU&0x60)>>5, (NALU&0x1F),(NALU&0x1F)); */
        video = &frame->stVideoFrameHead;

        total_length = video->u32FrameDataLen;
        copy_length = length - head_length;
        if(copy_length <= 0)
            return;
        
        data = tmp_buf;//tmp_buf = "" so crash
        memcpy(data, buf + head_length, copy_length);//Crash
        remain_length = total_length + head_length  - length;

        while(remain_length >= sizeof(recv_buff)){
            tmp_length = recv(sock, recv_buff, sizeof(recv_buff), 0);
            if( tmp_length <= 0)
                return;
            
            memcpy(data + copy_length, recv_buff, tmp_length);//Crash
            copy_length += tmp_length;
            remain_length -= tmp_length;
        }

        if (remain_length) {
            tmp_length = recv(sock, recv_buff, remain_length, 0);
            memcpy(data + copy_length, recv_buff, tmp_length);
            copy_length += tmp_length;
            remain_length -= tmp_length;
        }

        parseFrame(data, total_length);

    }
}


void* recv_thread_process(void* context)
{
    XM_STREAM_IO_HEAD* head = NULL;
    int len;
    //XM_STREAM_IO_TYPE io_type;
    HI_U8 io_type;
    int counter = 0;
    printf("------------------------- client.mm, 721, g_socket_status: %d, g_state_main.running: %d\n", g_socket_status, g_state_main.running);
    printf("------------------------- client.mm, 722, sock, recv_buff, sizeof(recv_buff), len: %dd, %d, %d\n", sock, recv_buff, sizeof(recv_buff));
    while(g_state_main.running) {
        
        // receive a reply form server
        len = recv(sock, recv_buff, sizeof(recv_buff), 0);
        if (len < 0) {
            //printf("recv failed\n");
            g_socket_status = -1;
            continue;
        }
        printf("------------------------ client.mm, 732, process \n");
        recv_buff[len] = 0;
        head = (XM_STREAM_IO_HEAD*)recv_buff;
        io_type = head->u8StreamIOType;
        printf("------------------------- client.mm, 735 before switch, io_type: %d\n", io_type);
        switch(io_type) {
            case SIO_TYPE_HEART_ALIVE_PACKET:           // 4
                handshake_process(counter);
                counter = counter + 1;
                printf("======================== client.mm, 741, io_type: %d\n", io_type);
                break;
            case SIO_TYPE_IOCTRL:                       // 2
                io_process((char*)recv_buff);
                printf("======================== client.mm, 745, io_type: %d\n", io_type);
                break;
            case SIO_TYPE_VIDEO_AUDIO_FRAME:            // 1
                media_process(recv_buff,len);
                printf("======================== client.mm, 749, io_type: %d\n", io_type);
                break;
            default:
                //printf("unused io type %d\n", io_type);
                printf("======================== client.mm, 753, io_type: %d\n", io_type);
                break;
        }
        g_socket_status = 1;
    }
    printf("---------------------- client.mm 744, g_state_main.running is set to 0 !!!! What happened?s");
    printf("%s exit\n", __FUNCTION__);
    
    return NULL;
}

/* Add an output stream. */
int add_stream(OutputStream *ost, AVFormatContext *oc,
                       AVCodec **codec,
                       enum AVCodecID codec_id)
{
    AVCodecContext *c;
    int i;

    /* find the encoder */
    *codec = avcodec_find_encoder(codec_id);
    if (!(*codec)) {
        printf("Could not find encoder for '%s'\n",
                avcodec_get_name(codec_id));
        return -1;
    }

    ost->st = avformat_new_stream(oc, NULL);
    if (!ost->st) {
        printf("Could not allocate stream\n");
        return -1;
    }
    ost->st->id = oc->nb_streams-1;
    c = avcodec_alloc_context3(*codec);
    if (!c) {
        printf("Could not alloc an encoding context\n");
        return -1;
    }
    ost->enc = c;

    switch ((*codec)->type) {
        case AVMEDIA_TYPE_AUDIO:
            c->sample_fmt  = (*codec)->sample_fmts ?
                             (*codec)->sample_fmts[0] : AV_SAMPLE_FMT_FLTP;
            c->bit_rate    = 64000;
            c->sample_rate = 44100;
            if ((*codec)->supported_samplerates) {
                c->sample_rate = (*codec)->supported_samplerates[0];
                for (i = 0; (*codec)->supported_samplerates[i]; i++) {
                    if ((*codec)->supported_samplerates[i] == 44100)
                        c->sample_rate = 44100;
                }
            }
            c->channels        = av_get_channel_layout_nb_channels(c->channel_layout);
            c->channel_layout = AV_CH_LAYOUT_STEREO;
            if ((*codec)->channel_layouts) {
                c->channel_layout = (*codec)->channel_layouts[0];
                for (i = 0; (*codec)->channel_layouts[i]; i++) {
                    if ((*codec)->channel_layouts[i] == AV_CH_LAYOUT_STEREO)
                        c->channel_layout = AV_CH_LAYOUT_STEREO;
                }
            }
            c->channels        = av_get_channel_layout_nb_channels(c->channel_layout);
            ost->st->time_base = (AVRational){ 1, c->sample_rate };
            break;

        case AVMEDIA_TYPE_VIDEO:
            c->codec_id = codec_id;

            c->bit_rate = 400000;
            /* Resolution must be a multiple of two. */
            c->width    = 640;
            c->height   = 480;
            /* timebase: This is the fundamental unit of time (in seconds) in terms
             * of which frame timestamps are represented. For fixed-fps content,
             * timebase should be 1/framerate and timestamp increments should be
             * identical to 1. */
            ost->st->time_base = (AVRational){ 1, STREAM_FRAME_RATE };
            c->time_base       = ost->st->time_base;

            c->gop_size      = 12; /* emit one intra frame every twelve frames at most */
            c->pix_fmt       = STREAM_PIX_FMT;
            if (c->codec_id == AV_CODEC_ID_MPEG2VIDEO) {
                /* just for testing, we also add B-frames */
                c->max_b_frames = 2;
            }
            if (c->codec_id == AV_CODEC_ID_MPEG1VIDEO) {
                /* Needed to avoid using macroblocks in which some coeffs overflow.
                 * This does not happen with normal video, it just happens here as
                 * the motion of the chroma plane does not match the luma plane. */
                c->mb_decision = 2;
            }
            break;

        default:
            break;
    }

    /* Some formats want stream headers to be separate. */
    if (oc->oformat->flags & AVFMT_GLOBALHEADER)
        c->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;

    //c->flags |= AV_NOPTS_VALUE;

    return 1;
}


AVFrame *alloc_picture(enum AVPixelFormat pix_fmt, int width, int height)
{
    AVFrame *picture;
    int ret;

    picture = av_frame_alloc();
    if (!picture)
        return NULL;

    picture->format = pix_fmt;
    picture->width  = width;
    picture->height = height;

    /* allocate the buffers for the frame data */
    ret = av_frame_get_buffer(picture, 32);
    if (ret < 0) {
        printf("Could not allocate frame data.\n");
        return NULL;
    }

    return picture;
}

int open_video(AVFormatContext *oc, AVCodec *codec, OutputStream *ost, AVDictionary *opt_arg)
{
    int ret;
    AVCodecContext *c = ost->enc;
    AVDictionary *opt = NULL;

    av_dict_copy(&opt, opt_arg, 0);

    /* open the codec */
    ret = avcodec_open2(c, codec, &opt);
    av_dict_free(&opt);
    if (ret < 0) {
        printf("Could not open video codec: %s\n", av_err2str(ret));
        return -1;
    }

    /* allocate and init a re-usable frame */
    ost->frame = alloc_picture(c->pix_fmt, c->width, c->height);
    if (!ost->frame) {
        printf("Could not allocate video frame\n");
        return -1;
    }

    /* If the output format is not YUV420P, then a temporary YUV420P
     * picture is needed too. It is then converted to the required
     * output format. */
    ost->tmp_frame = NULL;
    if (c->pix_fmt != AV_PIX_FMT_YUV420P) {
        ost->tmp_frame = alloc_picture(AV_PIX_FMT_YUV420P, c->width, c->height);
        if (!ost->tmp_frame) {
            printf("Could not allocate temporary picture\n");
            return -1;
        }
    }

    /* copy the stream parameters to the muxer */
    //ret = avcodec_parameters_from_context(ost->st->codecpar, c);
    if (ret < 0) {
        printf("Could not copy the stream parameters\n");
        return -1;
    }
    return 1;
}

int initVideoLibrary()
{
    if (isFFmpegInitialized == 0)
    {
        avcodec_register_all();
        av_register_all();
        isFFmpegInitialized = 1;
    }
    m_pAVCodec = avcodec_find_decoder(AV_CODEC_ID_H264);
    m_pCodecCtx = avcodec_alloc_context3(m_pAVCodec);
    m_pCodecPaser = av_parser_init(AV_CODEC_ID_H264);
    if (m_pAVCodec == NULL || m_pCodecCtx == NULL)
    {
        printf("m_pAVCodec == NULL||m_pCodecCtx == NULL\n");
        return -1;
    }

    if (m_pAVCodec->capabilities & AV_CODEC_CAP_TRUNCATED)
        m_pCodecCtx->flags |= AV_CODEC_FLAG_TRUNCATED;

    m_pCodecCtx->thread_count = 4;
    m_pCodecCtx->thread_type = FF_THREAD_FRAME;

    if (avcodec_open2(m_pCodecCtx, m_pAVCodec,NULL) < 0)
    {
        m_pAVCodec = NULL;
        return -1;
    }

    m_pAVConvertCtx = sws_getContext(STREAM_WIDTH, STREAM_HEIGHT, AV_PIX_FMT_YUV420P, STREAM_WIDTH, STREAM_HEIGHT, AV_PIX_FMT_RGB24, SWS_FAST_BILINEAR, NULL, NULL, NULL);
    if(!m_pAVConvertCtx){
        printf("impossible to create scale context\n");
        return -1;
    }

    m_pYUVFrame = av_frame_alloc();
    if (m_pYUVFrame == NULL){
        printf(" CDecoder avcodec_alloc_frame() == NULL \n");
        return -1;
    }
    
    m_pRGBFrame = av_frame_alloc();
    int numBytes = avpicture_get_size(AV_PIX_FMT_RGB24, STREAM_WIDTH, STREAM_HEIGHT);
    uint8_t *buffer = (uint8_t *)av_malloc(numBytes*sizeof(uint8_t));
   
    avpicture_fill((AVPicture *)m_pRGBFrame, buffer, AV_PIX_FMT_RGB24, STREAM_WIDTH, STREAM_HEIGHT);
     if (m_pRGBFrame == NULL){
        printf(" CDecoder avcodec_alloc_frame() == NULL \n");
        return -1;
    }
    
    av_log_set_level(AV_LOG_QUIET);
    //av_log_set_level(AV_LOG_DEBUG);

    printf("CDecoder::prepare()2\n");
    return 1;
}

int initDevice()
{
    g_init_engine = -1;
    
    if(initVideoLibrary() < 0){
        printf("initialize video library failed.\n");
        return -1;
    } else{
        printf("initialize video library success.\n");
    }

    g_recording_flag = -1;

    pthread_t recv_thread;
    pthread_attr_t  threadAttr_;

    pthread_attr_init(&threadAttr_);
    pthread_attr_setdetachstate(&threadAttr_, PTHREAD_CREATE_DETACHED);

    void* retval = NULL;

    printf("\nversion\t-> verify app can connect to device.\n");
    printf("login\t-> verify app have authority to access device.\n");
    printf("led?\t-> query the led status\n");
    printf("led0\t-> set led status to off\n");
    printf("led1\t-> set led status to level 1\n");
    printf("start\t-> start pull stream video from device\n");
    printf("stop\t-> stop pull stream video from device and save to file\n");
    printf("quit\t-> safely exit\n");
    printf("\nNOTE: MUST 'login' first then do other things!\n");

    /* create socket */
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        printf("can't create socket.\n");
        return -1;
    }
    printf("client socket id = %d\n", sock);

    g_state_main.running = 1;
    g_state_main.upgrading = 0;
    g_init_engine = 1;
    
    pthread_create(&recv_thread, NULL, recv_thread_process, NULL);
    printf("------------------- client.mm, 1026, process \n");
    pthread_join(recv_thread, &retval);
    printf("------------------- client.mm, 1028, process \n");
    //close(sock);
    //printf("connect to server done 123\n");
    return 1;
}

int connectDevice()
{
    g_socket_status = -1;
    
    struct sockaddr_in server;
    /* char msg[1000], server_reply[2000]; */
    int return_val = -1;
    int port=11666;
    char* server_ip = "192.168.12.100";
    printf("target server %s %d\n", server_ip, port);

    /* fill server info */
    server.sin_addr.s_addr = inet_addr(server_ip);
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    printf("--------------- client.mm 1053, server.sin_addr.s_addr, server.sin_family, server.sin_port, sock: %u, %u, %u, %d\n", server.sin_addr.s_addr, server.sin_family, server.sin_port, sock);
    int counter = 0;
    printf("-------------- client.mm 1055 return_val, counter: %d, %d\n", return_val, counter);
    while (return_val != 1 and counter <= 10000){
        /* connect to remote server */
        int connect_return = connect(sock, (struct sockaddr*)&server, sizeof(server));
        printf("------------------ client.mm 1059, connect_result: ", connect_return);
        //if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
        if (connect_return < 0) {
            printf("--------------- client.mm 1062, server.sin_len, server.sin_family, server.sin_port, server.sin_addr.s_addr, server.sin_zero, sock: %u, %u, %u, %u, %c, %d\n", server.sin_len, server.sin_family, server.sin_port, server.sin_addr.s_addr, server.sin_zero, sock);
            printf("connect failed, %d\n", counter);
            counter++;
            //close(sock);
            //return -1;
        } else {
            return_val = 1;
        }
    }
    if (return_val == 1)
        printf("connect to server done\n");
    
    //return 1;
    return return_val;
}


int log_in(void)
{
   
    int result;
    XM_STREAM_IO_HEAD head;
    XM_IO_CTRL_HEAD io_ctrl;
    XM_AUTHORIZE_REQ req;
    unsigned char buf[1024];
    unsigned int size = 0;

    head.u8StreamIOType = SIO_TYPE_IOCTRL;
    head.u32DataSize = sizeof(XM_IO_CTRL_HEAD) + sizeof(XM_AUTHORIZE_REQ);
    io_ctrl.u16IOCtrlType = IOCTRL_TYPE_AUTHORIZE_REQ;
    io_ctrl.u16IOCtrlDataSize = sizeof(XM_AUTHORIZE_REQ);

    unsigned char sAESKey[32];
    unsigned char encrypt_password[64];
    memset(encrypt_password, 0x00, 64);
    memset(sAESKey, 0x00, 32);
    strcpy((char*)sAESKey, "Greate P2P!!!!!");
    AES_Init();
    AES_Encrypt(128, (unsigned char*)"12345678", 8, sAESKey, 16, encrypt_password);
    strcpy((char*)req.strPassWord, (char*)encrypt_password);
        /* for(int i = 0; i < 64; i++) { */
        /*         if(i%16 == 0) printf("\n"); */
        /*         printf("%02X ", encrypt_password[i]); */
        /* } */

    size = sizeof(XM_STREAM_IO_HEAD) + sizeof(XM_IO_CTRL_HEAD) + sizeof(XM_AUTHORIZE_REQ);
    memcpy(buf, &head, sizeof(XM_STREAM_IO_HEAD));
    memcpy(buf+sizeof(XM_STREAM_IO_HEAD), &io_ctrl, sizeof(XM_IO_CTRL_HEAD));
    memcpy(buf+sizeof(XM_STREAM_IO_HEAD)+sizeof(XM_IO_CTRL_HEAD), &req, sizeof(XM_AUTHORIZE_REQ));
    if(send(sock, buf, size, 0) < 0) {
        printf("send authorize failed\n");
        result = -1;
    } else {
        printf("send authorize success\n");
        result = 1;
    }
    //return result;
    return 1;
}


int start_recording(const char *filepath)
{
    int result = -1;

    if(g_recording_flag > 0)
        return result;

    /* allocate the output media context */
    avformat_alloc_output_context2(&m_pAVOutputCtx, NULL, NULL, filepath);
    if (!m_pAVOutputCtx) {
        printf("Could not deduce output format from file extension: using MPEG.\n");
        avformat_alloc_output_context2(&m_pAVOutputCtx, NULL, "mpeg", filepath);
    }
    if (!m_pAVOutputCtx) {
        g_recording_flag = -1;
        return -1;
    }

    m_pAVOutputFmt = m_pAVOutputCtx->oformat;


    if (m_pAVOutputFmt->video_codec != AV_CODEC_ID_NONE) {
        result = add_stream(&m_OutputVideoStream, m_pAVOutputCtx, &m_pAVOutCodec, m_pAVOutputFmt->video_codec);
    }

    if(result < 0){
        g_recording_flag = -1;
        return -1;
    }

    result = open_video(m_pAVOutputCtx, m_pAVOutCodec, &m_OutputVideoStream, NULL);
    if(result < 0){
        g_recording_flag = -1;
        return -1;
    }

    av_dump_format(m_pAVOutputCtx, 0, filepath, 1);

    /* open the output file, if needed */
    if (!(m_pAVOutputFmt->flags & AVFMT_NOFILE)) {
        result = avio_open(&m_pAVOutputCtx->pb, filepath, AVIO_FLAG_WRITE);
        if (result < 0) {
            printf("Could not open '%s': %s\n", filepath, av_err2str(result));
            g_recording_flag = -1;
            return -1;
        }
    }

    /* Write the stream header, if any. */
    result = avformat_write_header(m_pAVOutputCtx, NULL);
    if (result < 0) {
        printf("Error occurred when opening output file: %s\n",
                av_err2str(result));
        g_recording_flag = -1;
        return -1;
    }

    frame_count = 0;
    g_totalPts = 0;
    g_recording_flag = 1;

    return 1;
}


void close_stream(AVFormatContext *oc, OutputStream *ost)
{
    avcodec_free_context(&ost->enc);
    av_frame_free(&ost->frame);
    av_frame_free(&ost->tmp_frame);
    sws_freeContext(ost->sws_ctx);
    swr_free(&ost->swr_ctx);
}

int stop_recording()
{
    g_recording_flag = -1;

    av_write_trailer(m_pAVOutputCtx);//crash

    close_stream(m_pAVOutputCtx, &m_OutputVideoStream);

    if (!(m_pAVOutputFmt->flags & AVFMT_NOFILE))
        /* Close the output file. */
        avio_closep(&m_pAVOutputCtx->pb);

    /* free the stream */
    avformat_free_context(m_pAVOutputCtx);

    return 1;

}

int start_stream(void)
{
    //connect socket
    printf("c1191 ----------------- g_socket_status: %d\n", g_socket_status);
    if(g_socket_status < 0){
        if(connectDevice() < 0){
            printf("c1194 - connection failed in start_stream: g_socket_status < 0 and connectDevice < 0\n");
            return -1;
        }
    }
    
    //login device
    if(log_in() < 0){
        printf("====================== client.mm 1225, log_in failed\n");
        return -1;
    }
    
    // start get live stream
    int result = video_stream_ctrl(1);

    return result;
}

void stop_stream(void)
{
    // stop get live stream
    video_stream_ctrl(0);
    //g_socket_status = -1;
    //close(sock);
}

int isInitDevice()
{
    return g_init_engine;
}
