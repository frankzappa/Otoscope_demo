//
//  client.h
//  earmonitor
//
//  Created by Ming on 2019/11/11.
//  Copyright © 2019 Almoutaz. All rights reserved.
//

#ifndef client_h
#define client_h

int     initDevice();
int     connectDevice();
int     log_in(void);
int     start_recording(const char* sfilepath);
int     stop_recording();
int     start_stream(void);
void    stop_stream(void);

int     isInitDevice();

#endif /* client_h */
