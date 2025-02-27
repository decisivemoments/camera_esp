# ICMP Echo




## Overview


ICMP (Internet Control Message Protocol) is used for diagnostic or control purposes or generated in response to errors in IP operations. The common network util `ping` is implemented based on the ICMP packets with the type field value of 0, also called `Echo Reply`.


During a ping session, the source host firstly sends out an ICMP echo request packet and wait for an ICMP echo reply with specific times. In this way, it also measures the round-trip time for the messages. After receiving a valid ICMP echo reply, the source host will generate statistics about the IP link layer (e.g., packet loss, elapsed time, etc).


It is common that IoT device needs to check whether a remote server is alive or not. The device should show the warnings to users when it got offline. It can be achieved by creating a ping session and sending or parsing ICMP echo packets periodically.


To make this internal procedure much easier for users, ESP-IDF provides some out-of-box APIs.



### Create a New Ping Session


To create a ping session, you need to fill in the `esp_ping_config_t` configuration structure firstly, specifying target IP address, interval times, and etc. Optionally, you can also register some callback functions with the `esp_ping_callbacks_t` structure.


Example method to create a new ping session and register callbacks:



```
static void test_on_ping_success(esp_ping_handle_t hdl, void *args)
{
    // optionally, get callback arguments
    // const char* str = (const char*) args;
    // printf("%s\r\n", str); // "foo"
    uint8_t ttl;
    uint16_t seqno;
    uint32_t elapsed_time, recv_len;
    ip_addr_t target_addr;
    esp_ping_get_profile(hdl, ESP_PING_PROF_SEQNO, &seqno, sizeof(seqno));
    esp_ping_get_profile(hdl, ESP_PING_PROF_TTL, &ttl, sizeof(ttl));
    esp_ping_get_profile(hdl, ESP_PING_PROF_IPADDR, &target_addr, sizeof(target_addr));
    esp_ping_get_profile(hdl, ESP_PING_PROF_SIZE, &recv_len, sizeof(recv_len));
    esp_ping_get_profile(hdl, ESP_PING_PROF_TIMEGAP, &elapsed_time, sizeof(elapsed_time));
    printf("%d bytes from %s icmp_seq=%d ttl=%d time=%d ms\n",
           recv_len, inet_ntoa(target_addr.u_addr.ip4), seqno, ttl, elapsed_time);
}

static void test_on_ping_timeout(esp_ping_handle_t hdl, void *args)
{
    uint16_t seqno;
    ip_addr_t target_addr;
    esp_ping_get_profile(hdl, ESP_PING_PROF_SEQNO, &seqno, sizeof(seqno));
    esp_ping_get_profile(hdl, ESP_PING_PROF_IPADDR, &target_addr, sizeof(target_addr));
    printf("From %s icmp_seq=%d timeout\n", inet_ntoa(target_addr.u_addr.ip4), seqno);
}

static void test_on_ping_end(esp_ping_handle_t hdl, void *args)
{
    uint32_t transmitted;
    uint32_t received;
    uint32_t total_time_ms;

    esp_ping_get_profile(hdl, ESP_PING_PROF_REQUEST, &transmitted, sizeof(transmitted));
    esp_ping_get_profile(hdl, ESP_PING_PROF_REPLY, &received, sizeof(received));
    esp_ping_get_profile(hdl, ESP_PING_PROF_DURATION, &total_time_ms, sizeof(total_time_ms));
    printf("%d packets transmitted, %d received, time %dms\n", transmitted, received, total_time_ms);
}

void initialize_ping()
{
    /* convert URL to IP address */
    ip_addr_t target_addr;
    struct addrinfo hint;
    struct addrinfo *res = NULL;
    memset(&hint, 0, sizeof(hint));
    memset(&target_addr, 0, sizeof(target_addr));
    getaddrinfo("www.espressif.com", NULL, &hint, &res);
    struct in_addr addr4 = ((struct sockaddr_in *) (res->ai_addr))->sin_addr;
    inet_addr_to_ip4addr(ip_2_ip4(&target_addr), &addr4);
    freeaddrinfo(res);

    esp_ping_config_t ping_config = ESP_PING_DEFAULT_CONFIG();
    ping_config.target_addr = target_addr;          // target IP address
    ping_config.count = ESP_PING_COUNT_INFINITE;    // ping in infinite mode, esp_ping_stop can stop it

    /* set callback functions */
    esp_ping_callbacks_t cbs;
    cbs.on_ping_success = test_on_ping_success;
    cbs.on_ping_timeout = test_on_ping_timeout;
    cbs.on_ping_end = test_on_ping_end;
    cbs.cb_args = "foo";  // arguments that feeds to all callback functions, can be NULL
    cbs.cb_args = eth_event_group;

    esp_ping_handle_t ping;
    esp_ping_new_session(&ping_config, &cbs, &ping);
}

```




### Start and Stop Ping Session


You can start and stop ping session with the handle returned by `esp_ping_new_session`. Note that, the ping session does not start automatically after creation. If the ping session is stopped, and restart again, the sequence number in ICMP packets will recount from zero again.




### Delete a Ping Session


If a ping session will not be used any more, you can delete it with `esp_ping_delete_session`. Please make sure the ping session is in stop state (i.e., you have called `esp_ping_stop` before or the ping session has finished all the procedures) when you call this function.




### Get Runtime Statistics


As the example code above, you can call `esp_ping_get_profile` to get different runtime statistics of ping session in the callback function.





## Application Example


ICMP echo example: protocols/icmp_echo




## API Reference



### Header File


* components/lwip/include/apps/ping/ping_sock.h
* This header file can be included with:



> ```
> #include "ping/ping_sock.h"
> 
> ```
* This header file is a part of the API provided by the `lwip` component. To declare that your component depends on `lwip`, add the following to your CMakeLists.txt:



> ```
> REQUIRES lwip
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES lwip
> 
> ```




### Functions


#### esp_ping_new_session


```


esp_err_t esp_ping_new_session(const esp_ping_config_t *config, const esp_ping_callbacks_t *cbs, esp_ping_handle_t *hdl_out)
Create a ping session. 

Parameters

config -- ping configuration 
cbs -- a bunch of callback functions invoked by internal ping task 
hdl_out -- handle of ping session 


Returns

ESP_ERR_INVALID_ARG: invalid parameters (e.g. configuration is null, etc)
ESP_ERR_NO_MEM: out of memory
ESP_FAIL: other internal error (e.g. socket error)
ESP_OK: create ping session successfully, user can take the ping handle to do follow-on jobs 





```

#### esp_ping_delete_session


```


esp_err_t esp_ping_delete_session(esp_ping_handle_t hdl)
Delete a ping session. 

Parameters
hdl -- handle of ping session 

Returns

ESP_ERR_INVALID_ARG: invalid parameters (e.g. ping handle is null, etc)
ESP_OK: delete ping session successfully 





```

#### esp_ping_start


```


esp_err_t esp_ping_start(esp_ping_handle_t hdl)
Start the ping session. 

Parameters
hdl -- handle of ping session 

Returns

ESP_ERR_INVALID_ARG: invalid parameters (e.g. ping handle is null, etc)
ESP_OK: start ping session successfully 





```

#### esp_ping_stop


```


esp_err_t esp_ping_stop(esp_ping_handle_t hdl)
Stop the ping session. 

Parameters
hdl -- handle of ping session 

Returns

ESP_ERR_INVALID_ARG: invalid parameters (e.g. ping handle is null, etc)
ESP_OK: stop ping session successfully 





```

#### esp_ping_get_profile


```


esp_err_t esp_ping_get_profile(esp_ping_handle_t hdl, esp_ping_profile_t profile, void *data, uint32_t size)
Get runtime profile of ping session. 

Parameters

hdl -- handle of ping session 
profile -- type of profile 
data -- profile data 
size -- profile data size 


Returns

ESP_ERR_INVALID_ARG: invalid parameters (e.g. ping handle is null, etc)
ESP_ERR_INVALID_SIZE: the actual profile data size doesn't match the "size" parameter
ESP_OK: get profile successfully 





```



### Structures


#### esp_ping_callbacks_t


```


struct esp_ping_callbacks_t
Type of "ping" callback functions. 

Public Members
cb_args

void *cb_args
arguments for callback functions 

on_ping_success

void (*on_ping_success)(esp_ping_handle_t hdl, void *args)
Invoked by internal ping thread when received ICMP echo reply packet. 

on_ping_timeout

void (*on_ping_timeout)(esp_ping_handle_t hdl, void *args)
Invoked by internal ping thread when receive ICMP echo reply packet timeout. 

on_ping_end

void (*on_ping_end)(esp_ping_handle_t hdl, void *args)
Invoked by internal ping thread when a ping session is finished. 



```

#### esp_ping_config_t


```


struct esp_ping_config_t
Type of "ping" configuration. 

Public Members
count

uint32_t count
A "ping" session contains count procedures 

interval_ms

uint32_t interval_ms
Milliseconds between each ping procedure 

timeout_ms

uint32_t timeout_ms
Timeout value (in milliseconds) of each ping procedure 

data_size

uint32_t data_size
Size of the data next to ICMP packet header 

tos

int tos
Type of Service, a field specified in the IP header 

ttl

int ttl
Time to Live,a field specified in the IP header 

target_addr

ip_addr_t target_addr
Target IP address, either IPv4 or IPv6 

task_stack_size

uint32_t task_stack_size
Stack size of internal ping task 

task_prio

uint32_t task_prio
Priority of internal ping task 

interface

uint32_t interface
Netif index, interface=0 means NETIF_NO_INDEX 



```



### Macros


#### ESP_PING_DEFAULT_CONFIG


```


ESP_PING_DEFAULT_CONFIG()
Default ping configuration. 

```

#### ESP_PING_COUNT_INFINITE


```


ESP_PING_COUNT_INFINITE
Set ping count to zero will ping target infinitely 

```



### Type Definitions


#### esp_ping_handle_t


```


typedef void *esp_ping_handle_t
Type of "ping" session handle. 

```



### Enumerations


#### esp_ping_profile_t


```


enum esp_ping_profile_t
Profile of ping session. 
Values:
ESP_PING_PROF_SEQNO

enumerator ESP_PING_PROF_SEQNO
Sequence number of a ping procedure 

ESP_PING_PROF_TOS

enumerator ESP_PING_PROF_TOS
Type of service of a ping procedure 

ESP_PING_PROF_TTL

enumerator ESP_PING_PROF_TTL
Time to live of a ping procedure 

ESP_PING_PROF_REQUEST

enumerator ESP_PING_PROF_REQUEST
Number of request packets sent out 

ESP_PING_PROF_REPLY

enumerator ESP_PING_PROF_REPLY
Number of reply packets received 

ESP_PING_PROF_IPADDR

enumerator ESP_PING_PROF_IPADDR
IP address of replied target 

ESP_PING_PROF_SIZE

enumerator ESP_PING_PROF_SIZE
Size of received packet 

ESP_PING_PROF_TIMEGAP

enumerator ESP_PING_PROF_TIMEGAP
Elapsed time between request and reply packet 

ESP_PING_PROF_DURATION

enumerator ESP_PING_PROF_DURATION
Elapsed time of the whole ping session 


```