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

#include "xm_net_ctrol_protocol.h"
#include "aes_ende.h"

typedef struct g_state_ {
	unsigned int running;
	unsigned int upgrading;
}g_state_t;

int sock;
g_state_t g_state;
FILE* video_fd = NULL;
#define SAVED_VIDEO_PATH "./1.h264"

unsigned char recv_buff[2048];

void* recv_thread_process(void* data);
void* ctrl_thread_process(void* data);
void handshake_process(void);
void io_process(char* buf);
void media_process(unsigned char* buf, unsigned int length);
void log_in(void);
void get_version(void);
void get_led_status(void);
void set_led_status(unsigned int level);
void video_stream_ctrl(unsigned int on_off);
void get_cbc(void);

unsigned int cal_crc(const unsigned int* buf, int count)
{
	unsigned int i;
	unsigned int sum = 0;

	for (i = 0; i < count; i++) {
		sum ^= buf[i];
	}
	return ~(int)sum;
}

void log_in(void)
{
	printf("log_in check point 1\n");
	XM_STREAM_IO_HEAD head;
	XM_IO_CTRL_HEAD io_ctrl;
	XM_AUTHORIZE_REQ req;
	unsigned char buf[1024];
	unsigned int size = 0;
	printf("log_in check point 2\n");
	head.u8StreamIOType = SIO_TYPE_IOCTRL;
	head.u32DataSize = sizeof(XM_IO_CTRL_HEAD) + sizeof(XM_AUTHORIZE_REQ);
	io_ctrl.u16IOCtrlType = IOCTRL_TYPE_AUTHORIZE_REQ;
	io_ctrl.u16IOCtrlDataSize = sizeof(XM_AUTHORIZE_REQ);
	printf("log_in check point 3\n");
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
	printf("log_in check point 4\n");
	size = sizeof(XM_STREAM_IO_HEAD) + sizeof(XM_IO_CTRL_HEAD) + sizeof(XM_AUTHORIZE_REQ);
	memcpy(buf, &head, sizeof(XM_STREAM_IO_HEAD));
	memcpy(buf+sizeof(XM_STREAM_IO_HEAD), &io_ctrl, sizeof(XM_IO_CTRL_HEAD));
	memcpy(buf+sizeof(XM_STREAM_IO_HEAD)+sizeof(XM_IO_CTRL_HEAD), &req, sizeof(XM_AUTHORIZE_REQ));
	if(send(sock, buf, size, 0) < 0) {
		puts("send authorize failed");
	} else {
		puts("send authorize success");
	}
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
	if(send(sock, buf, size, 0) < 0) {
		puts("send get sysfwver failed");
	} else {
		puts("send get sysfwver success");
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
	if(send(sock, buf, size, 0) < 0) {
		puts("send get led status failed");
	} else {
		puts("send get led status success");
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
	if(send(sock, buf, size, 0) < 0) {
		printf("send set led status %d failed\n", level);
	} else {
		printf("send set led status %d success\n", level);
	}
}

void video_stream_ctrl(unsigned int on_off)
{
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
		printf("send video stream on_off = %d failed\n", on_off);
	} else {
		printf("send video stream on_off = %d success\n", on_off);
	}
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
	if(send(sock, buf, size, 0) < 0) {
		puts("send get cbc failed");
	} else {
		puts("send get cbc success");
	}
}



#define FILE_SLICE   1024

void handshake_process(void)
{
	XM_STREAM_IO_HEAD handshake;
	handshake.u8StreamIOType = SIO_TYPE_HEART_ALIVE_PACKET;
	handshake.u32DataSize = 0;
	if(send(sock, &handshake, sizeof(XM_STREAM_IO_HEAD), 0) < 0) {
		puts("send handshake failed");
	} else {
                /* puts(" handshake ok"); */
        }
}
void io_process(char* buf)
{
	XM_IO_CTRL_HEAD* head;
	XM_IOCTRL_TYPE type;
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

	switch(type) {
	case IOCTRL_TYPE_AUTHORIZE_RESP:
		resp_authorize = (XM_AUTHORIZE_RESP*)(buf + offset);
		if (resp_authorize->s32Result == XM_SUCCESS) {
			printf("authorize success\n");
		} else {
			printf("authorize failed\n");
		}
		break;
	case IOCTRL_TYPE_GET_SYSFWVER_RESP:
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
	case IOCTRL_TYPE_GET_LED_RESP:
		resp_get_led = (XM_GET_DEV_LED_RESP*)(buf + offset);
		if (resp_get_led->s32Result == XM_SUCCESS) {
			printf("get LED = %d\n", resp_get_led->u8Enable);
		} else {
			printf("get LED status failed\n");
		}
		break;
	case IOCTRL_TYPE_SET_LED_RESP:
		resp_set_led = (XM_SET_DEV_LED_RESP*)(buf + offset);
		if (resp_set_led->s32Result == XM_SUCCESS) {
			printf("set led success\n");
		} else {
			printf("set led failed\n");
		}
		break;
	case IOCTRL_TYPE_LIVE_START_RESP:
		resp_live_start = (XM_LIVE_START_RESP*)(buf + offset);
		if (resp_live_start->s32Result == XM_SUCCESS) {
			printf("live start success\n");
		} else {
			printf("live start failed\n");
		}
		break;
	case IOCTRL_TYPE_LIVE_STOP_RESP:
		resp_live_stop = (XM_LIVE_STOP_RESP*)(buf + offset);
		if (resp_live_stop->s32Result == XM_SUCCESS) {
			printf("live stop success\n");
		} else {
			printf("live stop failed\n");
		}
		break;
	case IOCTRL_TYPE_GET_CBC_RESP:
		resp_get_cbc = (XM_GET_DEV_CBC_RESP*)(buf + offset);
		if (resp_get_cbc->s32Result == XM_SUCCESS) {
			printf("get CBC= %d\n", resp_get_cbc->u8CBCNumber);
		} else {
			printf("get CBC failed\n");
		}
		break;
	case IOCTRL_TYPE_REBOOT_DEVICE_RESP:
		resp_reboot = (XM_REBOOT_DEVICE_RESP*)(buf + offset);
		if (resp_reboot->s32Result == XM_SUCCESS) {
			printf("set reboot success\n");
		} else {
			printf("set reboot failed\n");
		}
		break;
	case IOCTRL_TYPE_UPGRADE_READY:
		resp_upgrade = (XM_UPGREDE_RESP*)(buf + offset);
		if (resp_upgrade->s32Result == XM_SUCCESS) {
			printf("device ready for recive upgrade data\n");
			g_state.upgrading = 1; // trig sending upgrade data
		} else {
			printf("device can't upgrade error=%d\n", resp_upgrade->s32Result);
		}
		break;
	case IOCTRL_TYPE_UPGRADE_OK:
		break;
	case IOCTRL_TYPE_UPGRADE_FAILED:
		break;
	case IOCTRL_TYPE_DC_SNAP_REQ:
		printf("device trigger to snap\n");
		break;
	default :
		printf("unused IOCTRL_TYPE %d\n", type);
		break;
	}
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
	//frame = (XM_FRAME_HEAD*)(buf + sizeof(XM_STREAM_IO_HEAD));
	frame = (XM_FRAME_HEAD*)(buf + sizeof(XM_STREAM_IO_HEAD));
	if (!frame) {
		printf("error drop\n");
	}
	/* NALU = *(buf + sizeof(XM_STREAM_IO_HEAD) + sizeof(XM_FRAME_HEAD) + 4) &0x1F; */
        /* there is no audio packet in this project */
	if (frame->u8FrameType == FRAME_TYPE_VIDEO) {
		/* printf("%02d %d:%d %08d len=%08d %X %X %X(%d) ", */
		/* 	frame->stVideoFrameHead.u8FrameIndex, */
		/* 	frame->u16FrameSplitPackTotalNum, */
		/* 	frame->u16SplitPackNo, */
		/* 	frame->stVideoFrameHead.nTimeStampUSec, */
		/* 	frame->stVideoFrameHead.u32FrameDataLen, */
		/* 	(NALU&0x80)>>7, (NALU&0x60)>>5, (NALU&0x1F),(NALU&0x1F)); */
		video = &frame->stVideoFrameHead;

		total_length = video->u32FrameDataLen;
		copy_length = length - head_length;
		data = tmp_buf;
		memcpy(data, buf + head_length, copy_length);
		remain_length = total_length + head_length  - length;

		while(remain_length >= sizeof(recv_buff)){
			tmp_length = recv(sock, recv_buff, sizeof(recv_buff), 0);
			memcpy(data + copy_length, recv_buff, tmp_length);
			copy_length += tmp_length;
			remain_length -= tmp_length;
		}

		if (remain_length) {
			tmp_length = recv(sock, recv_buff, remain_length, 0);
			memcpy(data + copy_length, recv_buff, tmp_length);
			copy_length += tmp_length;
			remain_length -= tmp_length;
		}

	        fwrite(data, 1, total_length, video_fd);
		fflush(video_fd);
	}
}


void* recv_thread_process(void* data)
{
	XM_STREAM_IO_HEAD* head = NULL;
	int len;
	XM_STREAM_IO_TYPE io_type;

	while(g_state.running) {
		/* receive a reply form server */
		len = recv(sock, recv_buff, sizeof(recv_buff), 0);
		if (len < 0) {
			puts("recv failed");
			break;
		}
		head = (XM_STREAM_IO_HEAD*)recv_buff;
		io_type = head->u8StreamIOType;
		switch(io_type) {
			case SIO_TYPE_HEART_ALIVE_PACKET:
				handshake_process();
				break;
			case SIO_TYPE_IOCTRL:
				io_process((char*)recv_buff);
				break;
			case SIO_TYPE_VIDEO_AUDIO_FRAME:
				media_process(recv_buff, len);
				break;
			default:
				/* printf("unused io type %d\n", io_type); */
				break;
		}
	}
	printf("%s exit", __FUNCTION__);
	return NULL;
}

void* ctrl_thread_process(void* data)
{
	char msg[1000];

	while(g_state.running) {
		memset(msg, 0x0, sizeof(msg));
		if (scanf("%s", msg)) {
			/* printf("%s\n", msg); */
			if (strlen(msg) < 1) {
				continue;
			}

			if (strcmp(msg, "quit") == 0) {
				// terminated pc-client
				g_state.running = 0;
			} else if (strcmp(msg, "login") == 0) {
				log_in();
			} else if (strcmp(msg, "led?") == 0) {
				// get led status
				get_led_status();
			} else if (strcmp(msg, "led0") == 0) {
				// set led off
				set_led_status(0);
			} else if (strcmp(msg, "led1") == 0) {
				// set led level 1
				set_led_status(1);
			} else if (strcmp(msg, "led2") == 0) {
				// set led level 2
				set_led_status(2);
			} else if (strcmp(msg, "led3") == 0) {
				// set led level3
				set_led_status(3);
			} else if (strcmp(msg, "start") == 0) {
				// start get live stream
				video_stream_ctrl(1);

				if (video_fd == NULL) {
					buf_index = 0;
					video_fd = fopen(SAVED_VIDEO_PATH, "wb");
					if (video_fd == NULL) {
						printf("can't create video file\n");
					} else {
						printf("create %s\n", SAVED_VIDEO_PATH);
					}
				}
			} else if (strcmp(msg, "stop") == 0) {
				// stop get live stream
				video_stream_ctrl(0);
				if (video_fd) {
					fflush(video_fd);
					fclose(video_fd);
					video_fd = NULL;
					buf_index = 0;
				}
			} else if (strcmp(msg, "cbc") == 0) {
				// get current battery capcity
				get_cbc();
			} else if (strcmp(msg, "version") == 0) {
				// get version
				get_version();
			} else if (strcmp(msg, "snap") == 0) {
				// device request app to capture
			} else {
				printf("unsed:%s\n", msg);
			}

		}
	}
	printf("%s exit", __FUNCTION__);
	return NULL;
}
int main(int argc, char** argv)
{
    struct sockaddr_in server;
    /* char msg[1000], server_reply[2000]; */
    int port=11666;
    char* server_ip = "192.168.12.100";
    pthread_t recv_thread;
    pthread_t ctrl_thread;
    void* retval = NULL;

    if (argc == 3) {
        server_ip = argv[1];
        port = atoi(argv[2]);
    } else if (argc == 2) {
        server_ip = argv[1];
    }
    printf("target server %s %d\n", server_ip, port);
    fflush(stdout);

    puts("\nversion\t-> verify app can connect to device.");
    puts("login\t-> verify app have authority to access device.");
    puts("led?\t-> query the led status");
    puts("led0\t-> set led status to off");
    puts("led1\t-> set led status to level 1");
    puts("start\t-> start pull stream video from device");
    puts("stop\t-> stop pull stream video from device and save to file");
    puts("quit\t-> safely exit");
    puts("\nNOTE: MUST 'login' first then do other things!");

    printf("main function check point 1\n");
    /* create socket */
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        puts("can't create socket.");
        return 1;
    }
    printf("main function check point 2\n");

    /* printf("client socket id = %d\n", sock); */
    /* fill server info */
    server.sin_addr.s_addr = inet_addr(server_ip);
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    printf("main function check point 3\n");

    printf("server.sin_addr.s_addr, server.sin_family, server.sin_port, sock: %u, %u, %u, %d\n", server.sin_addr.s_addr, server.sin_family, server.sin_port, sock);
    /* connect to remote server */
    if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
        puts("connect failed");
        perror(strerror(errno));
        close(sock);
        return 1;
    }
    printf("connect to server done\n");
    printf("main function check point 4\n");

    g_state.running = 1;
    g_state.upgrading = 0;

    pthread_create(&recv_thread, NULL,recv_thread_process, NULL);
    pthread_create(&ctrl_thread, NULL,ctrl_thread_process, NULL);
    printf("main function check point 5\n");

    pthread_join(recv_thread, &retval);
    pthread_join(ctrl_thread, &retval);

    close(sock);
    return 0;
}
