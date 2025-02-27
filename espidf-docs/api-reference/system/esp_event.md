# Event Loop Library




## Overview


The event loop library allows components to declare events so that other components can register handlers -- codes that executes when those events occur. This allows loosely-coupled components to attach desired behavior to state changes of other components without application involvement. This also simplifies event processing by serializing and deferring code execution to another context.


One common case is, if a high-level library is using the Wi-Fi library: it may subscribe to ESP32 Wi-Fi Programming Model directly and act on those events.



Note


Various modules of the Bluetooth stack deliver events to applications via dedicated callback functions instead of via the Event Loop Library.





## Using `esp_event` APIs


There are two objects of concern for users of this library: events and event loops.


An event indicates an important occurrence, such as a successful Wi-Fi connection to an access point. A two-part identifier should be used when referencing events, see declaring and defining events for details. The event loop is the bridge between events and event handlers. The event source publishes events to the event loop using the APIs provided by the event loop library, and event handlers registered to the event loop respond to specific types of events.


Using this library roughly entails the following flow:


1. The user defines a function that should run when an event is posted to a loop. This function is referred to as the event handler, and should have the same signature as `esp_event_handler_t`.
2. An event loop is created using `esp_event_loop_create()`, which outputs a handle to the loop of type `esp_event_loop_handle_t`. Event loops created using this API are referred to as user event loops. There is, however, a special type of event loop called the default event loop which is discussed in default event loop.
3. Components register event handlers to the loop using `esp_event_handler_register_with()`. Handlers can be registered with multiple loops, see notes on handler registration.
4. Event sources post an event to the loop using `esp_event_post_to()`.
5. Components wanting to remove their handlers from being called can do so by unregistering from the loop using `esp_event_handler_unregister_with()`.
6. Event loops that are no longer needed can be deleted using `esp_event_loop_delete()`.


In code, the flow above may look like as follows:



```
// 1. Define the event handler
void run_on_event(void* handler_arg, esp_event_base_t base, int32_t id, void* event_data)
{
    // Event handler logic
}

void app_main()
{
    // 2. A configuration structure of type esp_event_loop_args_t is needed to specify the properties of the loop to be created. A handle of type esp_event_loop_handle_t is obtained, which is needed by the other APIs to reference the loop to perform their operations.
    esp_event_loop_args_t loop_args = {
        .queue_size = ...,
        .task_name = ...
        .task_priority = ...,
        .task_stack_size = ...,
        .task_core_id = ...
    };

    esp_event_loop_handle_t loop_handle;

    esp_event_loop_create(&loop_args, &loop_handle);

    // 3. Register event handler defined in (1). MY_EVENT_BASE and MY_EVENT_ID specify a hypothetical event that handler run_on_event should execute when it gets posted to the loop.
    esp_event_handler_register_with(loop_handle, MY_EVENT_BASE, MY_EVENT_ID, run_on_event, ...);

    ...

    // 4. Post events to the loop. This queues the event on the event loop. At some point, the event loop executes the event handler registered to the posted event, in this case, run_on_event. To simplify the process, this example calls esp_event_post_to from app_main, but posting can be done from any other task (which is the more interesting use case).
    esp_event_post_to(loop_handle, MY_EVENT_BASE, MY_EVENT_ID, ...);

    ...

    // 5. Unregistering an unneeded handler
    esp_event_handler_unregister_with(loop_handle, MY_EVENT_BASE, MY_EVENT_ID, run_on_event);

    ...

    // 6. Deleting an unneeded event loop
    esp_event_loop_delete(loop_handle);
}

```




## Declaring and Defining Events


As mentioned previously, events consist of two-part identifiers: the event base and the event ID. The event base identifies an independent group of events; the event ID identifies the event within that group. Think of the event base and event ID as a person's last name and first name, respectively. A last name identifies a family, and the first name identifies a person within that family.


The event loop library provides macros to declare and define the event base easily.


Event base declaration:



```
ESP_EVENT_DECLARE_BASE(EVENT_BASE);

```


Event base definition:



```
ESP_EVENT_DEFINE_BASE(EVENT_BASE);

```



Note


In ESP-IDF, the base identifiers for system events are uppercase and are postfixed with `_EVENT`. For example, the base for Wi-Fi events is declared and defined as `WIFI_EVENT`, the Ethernet event base `ETHERNET_EVENT`, and so on. The purpose is to have event bases look like constants (although they are global variables considering the definitions of macros `ESP_EVENT_DECLARE_BASE` and `ESP_EVENT_DEFINE_BASE`).



For event IDs, declaring them as enumerations is recommended. Once again, for visibility, these are typically placed in public header files.


Event ID:



```
enum {
    EVENT_ID_1,
    EVENT_ID_2,
    EVENT_ID_3,
    ...
}

```




## Default Event Loop


The default event loop is a special type of loop used for system events (Wi-Fi events, for example). The handle for this loop is hidden from the user, and the creation, deletion, handler registration/deregistration, and posting of events are done through a variant of the APIs for user event loops. The table below enumerates those variants, and the user event loops equivalent.







| User Event Loops | Default Event Loops |
| --- | --- |
| `esp_event_loop_create()` | `esp_event_loop_create_default()` |
| `esp_event_loop_delete()` | `esp_event_loop_delete_default()` |
| `esp_event_handler_register_with()` | `esp_event_handler_register()` |
| `esp_event_handler_unregister_with()` | `esp_event_handler_unregister()` |
| `esp_event_post_to()` | `esp_event_post()` |


If you compare the signatures for both, they are mostly similar except for the lack of loop handle specification for the default event loop APIs.


Other than the API difference and the special designation to which system events are posted, there is no difference in how default event loops and user event loops behave. It is even possible for users to post their own events to the default event loop, should the user opt to not create their own loops to save memory.




## Notes on Handler Registration


It is possible to register a single handler to multiple events individually by using multiple calls to `esp_event_handler_register_with()`. For those multiple calls, the specific event base and event ID can be specified with which the handler should execute.


However, in some cases, it is desirable for a handler to execute on the following situations:


1. all events that get posted to a loop
2. all events of a particular base identifier


This is possible using the special event base identifier `ESP_EVENT_ANY_BASE` and special event ID `ESP_EVENT_ANY_ID`. These special identifiers may be passed as the event base and event ID arguments for `esp_event_handler_register_with()`.


Therefore, the valid arguments to `esp_event_handler_register_with()` are:


1. <event base>, <event ID> - handler executes when the event with base <event base> and event ID <event ID> gets posted to the loop
2. <event base>, ESP_EVENT_ANY_ID - handler executes when any event with base <event base> gets posted to the loop
3. ESP_EVENT_ANY_BASE, ESP_EVENT_ANY_ID - handler executes when any event gets posted to the loop


As an example, suppose the following handler registrations were performed:



```
esp_event_handler_register_with(loop_handle, MY_EVENT_BASE, MY_EVENT_ID, run_on_event_1, ...);
esp_event_handler_register_with(loop_handle, MY_EVENT_BASE, ESP_EVENT_ANY_ID, run_on_event_2, ...);
esp_event_handler_register_with(loop_handle, ESP_EVENT_ANY_BASE, ESP_EVENT_ANY_ID, run_on_event_3, ...);

```


If the hypothetical event `MY_EVENT_BASE`, `MY_EVENT_ID` is posted, all three handlers `run_on_event_1`, `run_on_event_2`, and `run_on_event_3` would execute.


If the hypothetical event `MY_EVENT_BASE`, `MY_OTHER_EVENT_ID` is posted, only `run_on_event_2` and `run_on_event_3` would execute.


If the hypothetical event `MY_OTHER_EVENT_BASE`, `MY_OTHER_EVENT_ID` is posted, only `run_on_event_3` would execute.



### Handler Un-Registering Itself


In general, an event handler run by an event loop is **not allowed to do any registering/unregistering activity on that event loop**. There is one exception, though: un-registering itself is allowed for the handler. E.g., it is possible to do the following:



```
void run_on_event(void* handler_arg, esp_event_base_t base, int32_t id, void* event_data)
{
    esp_event_loop_handle_t *loop_handle = (esp_event_loop_handle_t*) handler_arg;
    esp_event_handler_unregister_with(*loop_handle, MY_EVENT_BASE, MY_EVENT_ID, run_on_event);
}

void app_main(void)
{
    esp_event_loop_handle_t loop_handle;
    esp_event_loop_create(&loop_args, &loop_handle);
    esp_event_handler_register_with(loop_handle, MY_EVENT_BASE, MY_EVENT_ID, run_on_event, &loop_handle);
    // ... post-event MY_EVENT_BASE, MY_EVENT_ID and run loop at some point
}

```




### Handler Registration and Handler Dispatch Order


The general rule is that, for handlers that match a certain posted event during dispatch, those which are registered first also get executed first. The user can then control which handlers get executed first by registering them before other handlers, provided that all registrations are performed using a single task. If the user plans to take advantage of this behavior, caution must be exercised if there are multiple tasks registering handlers. While the 'first registered, first executed' behavior still holds true, the task which gets executed first also gets its handlers registered first. Handlers registered one after the other by a single task are still dispatched in the order relative to each other, but if that task gets pre-empted in between registration by another task that also registers handlers; then during dispatch those handlers also get executed in between.





## Event Loop Profiling


A configuration option CONFIG_ESP_EVENT_LOOP_PROFILING can be enabled in order to activate statistics collection for all event loops created. The function `esp_event_dump()` can be used to output the collected statistics to a file stream. More details on the information included in the dump can be found in the `esp_event_dump()` API Reference.




## Application Example


Examples of using the `esp_event` library can be found in system/esp_event. The examples cover event declaration, loop creation, handler registration and deregistration, and event posting.


Other examples which also adopt `esp_event` library:



> * NMEA Parser , which decodes the statements received from GPS.




## API Reference



### Header File


* components/esp_event/include/esp_event.h
* This header file can be included with:



> ```
> #include "esp_event.h"
> 
> ```
* This header file is a part of the API provided by the `esp_event` component. To declare that your component depends on `esp_event`, add the following to your CMakeLists.txt:



> ```
> REQUIRES esp_event
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES esp_event
> 
> ```




### Functions


#### esp_event_loop_create


```


esp_err_t esp_event_loop_create(const esp_event_loop_args_t *event_loop_args, esp_event_loop_handle_t *event_loop)
Create a new event loop. 

Parameters

event_loop_args -- [in] configuration structure for the event loop to create 
event_loop -- [out] handle to the created event loop


Returns

ESP_OK: Success
ESP_ERR_INVALID_ARG: event_loop_args or event_loop was NULL
ESP_ERR_NO_MEM: Cannot allocate memory for event loops list
ESP_FAIL: Failed to create task loop
Others: Fail 





```

#### esp_event_loop_delete


```


esp_err_t esp_event_loop_delete(esp_event_loop_handle_t event_loop)
Delete an existing event loop. 

Parameters
event_loop -- [in] event loop to delete, must not be NULL

Returns

ESP_OK: Success
Others: Fail 





```

#### esp_event_loop_create_default


```


esp_err_t esp_event_loop_create_default(void)
Create default event loop. 

Returns

ESP_OK: Success
ESP_ERR_NO_MEM: Cannot allocate memory for event loops list
ESP_ERR_INVALID_STATE: Default event loop has already been created
ESP_FAIL: Failed to create task loop
Others: Fail 





```

#### esp_event_loop_delete_default


```


esp_err_t esp_event_loop_delete_default(void)
Delete the default event loop. 

Returns

ESP_OK: Success
Others: Fail 





```

#### esp_event_loop_run


```


esp_err_t esp_event_loop_run(esp_event_loop_handle_t event_loop, TickType_t ticks_to_run)
Dispatch events posted to an event loop. 
This function is used to dispatch events posted to a loop with no dedicated task, i.e. task name was set to NULL in event_loop_args argument during loop creation. This function includes an argument to limit the amount of time it runs, returning control to the caller when that time expires (or some time afterwards). There is no guarantee that a call to this function will exit at exactly the time of expiry. There is also no guarantee that events have been dispatched during the call, as the function might have spent all the allotted time waiting on the event queue. Once an event has been dequeued, however, it is guaranteed to be dispatched. This guarantee contributes to not being able to exit exactly at time of expiry as (1) blocking on internal mutexes is necessary for dispatching the dequeued event, and (2) during dispatch of the dequeued event there is no way to control the time occupied by handler code execution. The guaranteed time of exit is therefore the allotted time + amount of time required to dispatch the last dequeued event.
In cases where waiting on the queue times out, ESP_OK is returned and not ESP_ERR_TIMEOUT, since it is normal behavior.

Note
encountering an unknown event that has been posted to the loop will only generate a warning, not an error.


Parameters

event_loop -- [in] event loop to dispatch posted events from, must not be NULL 
ticks_to_run -- [in] number of ticks to run the loop


Returns

ESP_OK: Success
Others: Fail 





```

#### esp_event_handler_register


```


esp_err_t esp_event_handler_register(esp_event_base_t event_base, int32_t event_id, esp_event_handler_t event_handler, void *event_handler_arg)
Register an event handler to the system event loop (legacy). 
This function can be used to register a handler for either: (1) specific events, (2) all events of a certain event base, or (3) all events known by the system event loop.

specific events: specify exact event_base and event_id
all events of a certain base: specify exact event_base and use ESP_EVENT_ANY_ID as the event_id
all events known by the loop: use ESP_EVENT_ANY_BASE for event_base and ESP_EVENT_ANY_ID as the event_id


Registering multiple handlers to events is possible. Registering a single handler to multiple events is also possible. However, registering the same handler to the same event multiple times would cause the previous registrations to be overwritten.

Note
the event loop library does not maintain a copy of event_handler_arg, therefore the user should ensure that event_handler_arg still points to a valid location by the time the handler gets called


Parameters

event_base -- [in] the base ID of the event to register the handler for 
event_id -- [in] the ID of the event to register the handler for 
event_handler -- [in] the handler function which gets called when the event is dispatched 
event_handler_arg -- [in] data, aside from event data, that is passed to the handler when it is called


Returns

ESP_OK: Success
ESP_ERR_NO_MEM: Cannot allocate memory for the handler
ESP_ERR_INVALID_ARG: Invalid combination of event base and event ID
Others: Fail 





```

#### esp_event_handler_register_with


```


esp_err_t esp_event_handler_register_with(esp_event_loop_handle_t event_loop, esp_event_base_t event_base, int32_t event_id, esp_event_handler_t event_handler, void *event_handler_arg)
Register an event handler to a specific loop (legacy). 
This function behaves in the same manner as esp_event_handler_register, except the additional specification of the event loop to register the handler to.

Note
the event loop library does not maintain a copy of event_handler_arg, therefore the user should ensure that event_handler_arg still points to a valid location by the time the handler gets called


Parameters

event_loop -- [in] the event loop to register this handler function to, must not be NULL 
event_base -- [in] the base ID of the event to register the handler for 
event_id -- [in] the ID of the event to register the handler for 
event_handler -- [in] the handler function which gets called when the event is dispatched 
event_handler_arg -- [in] data, aside from event data, that is passed to the handler when it is called


Returns

ESP_OK: Success
ESP_ERR_NO_MEM: Cannot allocate memory for the handler
ESP_ERR_INVALID_ARG: Invalid combination of event base and event ID
Others: Fail 





```

#### esp_event_handler_instance_register_with


```


esp_err_t esp_event_handler_instance_register_with(esp_event_loop_handle_t event_loop, esp_event_base_t event_base, int32_t event_id, esp_event_handler_t event_handler, void *event_handler_arg, esp_event_handler_instance_t *instance)
Register an instance of event handler to a specific loop. 
This function can be used to register a handler for either: (1) specific events, (2) all events of a certain event base, or (3) all events known by the system event loop.

specific events: specify exact event_base and event_id
all events of a certain base: specify exact event_base and use ESP_EVENT_ANY_ID as the event_id
all events known by the loop: use ESP_EVENT_ANY_BASE for event_base and ESP_EVENT_ANY_ID as the event_id


Besides the error, the function returns an instance object as output parameter to identify each registration. This is necessary to remove (unregister) the registration before the event loop is deleted.
Registering multiple handlers to events, registering a single handler to multiple events as well as registering the same handler to the same event multiple times is possible. Each registration yields a distinct instance object which identifies it over the registration lifetime.

Note
the event loop library does not maintain a copy of event_handler_arg, therefore the user should ensure that event_handler_arg still points to a valid location by the time the handler gets called


Note
Calling this function with instance set to NULL is equivalent to calling esp_event_handler_register_with.


Parameters

event_loop -- [in] the event loop to register this handler function to, must not be NULL 
event_base -- [in] the base ID of the event to register the handler for 
event_id -- [in] the ID of the event to register the handler for 
event_handler -- [in] the handler function which gets called when the event is dispatched 
event_handler_arg -- [in] data, aside from event data, that is passed to the handler when it is called 
instance -- [out] An event handler instance object related to the registered event handler and data, can be NULL. This needs to be kept if the specific callback instance should be unregistered before deleting the whole event loop. Registering the same event handler multiple times is possible and yields distinct instance objects. The data can be the same for all registrations. If no unregistration is needed, but the handler should be deleted when the event loop is deleted, instance can be NULL.


Returns

ESP_OK: Success
ESP_ERR_NO_MEM: Cannot allocate memory for the handler
ESP_ERR_INVALID_ARG: Invalid combination of event base and event ID or instance is NULL
Others: Fail 





```

#### esp_event_handler_instance_register


```


esp_err_t esp_event_handler_instance_register(esp_event_base_t event_base, int32_t event_id, esp_event_handler_t event_handler, void *event_handler_arg, esp_event_handler_instance_t *instance)
Register an instance of event handler to the default loop. 
This function does the same as esp_event_handler_instance_register_with, except that it registers the handler to the default event loop.

Note
the event loop library does not maintain a copy of event_handler_arg, therefore the user should ensure that event_handler_arg still points to a valid location by the time the handler gets called


Note
Calling this function with instance set to NULL is equivalent to calling esp_event_handler_register.


Parameters

event_base -- [in] the base ID of the event to register the handler for 
event_id -- [in] the ID of the event to register the handler for 
event_handler -- [in] the handler function which gets called when the event is dispatched 
event_handler_arg -- [in] data, aside from event data, that is passed to the handler when it is called 
instance -- [out] An event handler instance object related to the registered event handler and data, can be NULL. This needs to be kept if the specific callback instance should be unregistered before deleting the whole event loop. Registering the same event handler multiple times is possible and yields distinct instance objects. The data can be the same for all registrations. If no unregistration is needed, but the handler should be deleted when the event loop is deleted, instance can be NULL.


Returns

ESP_OK: Success
ESP_ERR_NO_MEM: Cannot allocate memory for the handler
ESP_ERR_INVALID_ARG: Invalid combination of event base and event ID or instance is NULL
Others: Fail 





```

#### esp_event_handler_unregister


```


esp_err_t esp_event_handler_unregister(esp_event_base_t event_base, int32_t event_id, esp_event_handler_t event_handler)
Unregister a handler with the system event loop (legacy). 
Unregisters a handler, so it will no longer be called during dispatch. Handlers can be unregistered for any combination of event_base and event_id which were previously registered. To unregister a handler, the event_base and event_id arguments must match exactly the arguments passed to esp_event_handler_register() when that handler was registered. Passing ESP_EVENT_ANY_BASE and/or ESP_EVENT_ANY_ID will only unregister handlers that were registered with the same wildcard arguments.

Note
When using ESP_EVENT_ANY_ID, handlers registered to specific event IDs using the same base will not be unregistered. When using ESP_EVENT_ANY_BASE, events registered to specific bases will also not be unregistered. This avoids accidental unregistration of handlers registered by other users or components.


Parameters

event_base -- [in] the base of the event with which to unregister the handler 
event_id -- [in] the ID of the event with which to unregister the handler 
event_handler -- [in] the handler to unregister


Returns
ESP_OK success 

Returns
ESP_ERR_INVALID_ARG invalid combination of event base and event ID 

Returns
others fail 



```

#### esp_event_handler_unregister_with


```


esp_err_t esp_event_handler_unregister_with(esp_event_loop_handle_t event_loop, esp_event_base_t event_base, int32_t event_id, esp_event_handler_t event_handler)
Unregister a handler from a specific event loop (legacy). 
This function behaves in the same manner as esp_event_handler_unregister, except the additional specification of the event loop to unregister the handler with.

Parameters

event_loop -- [in] the event loop with which to unregister this handler function, must not be NULL 
event_base -- [in] the base of the event with which to unregister the handler 
event_id -- [in] the ID of the event with which to unregister the handler 
event_handler -- [in] the handler to unregister


Returns

ESP_OK: Success
ESP_ERR_INVALID_ARG: Invalid combination of event base and event ID
Others: Fail 





```

#### esp_event_handler_instance_unregister_with


```


esp_err_t esp_event_handler_instance_unregister_with(esp_event_loop_handle_t event_loop, esp_event_base_t event_base, int32_t event_id, esp_event_handler_instance_t instance)
Unregister a handler instance from a specific event loop. 
Unregisters a handler instance, so it will no longer be called during dispatch. Handler instances can be unregistered for any combination of event_base and event_id which were previously registered. To unregister a handler instance, the event_base and event_id arguments must match exactly the arguments passed to esp_event_handler_instance_register() when that handler instance was registered. Passing ESP_EVENT_ANY_BASE and/or ESP_EVENT_ANY_ID will only unregister handler instances that were registered with the same wildcard arguments.

Note
When using ESP_EVENT_ANY_ID, handlers registered to specific event IDs using the same base will not be unregistered. When using ESP_EVENT_ANY_BASE, events registered to specific bases will also not be unregistered. This avoids accidental unregistration of handlers registered by other users or components.


Parameters

event_loop -- [in] the event loop with which to unregister this handler function, must not be NULL 
event_base -- [in] the base of the event with which to unregister the handler 
event_id -- [in] the ID of the event with which to unregister the handler 
instance -- [in] the instance object of the registration to be unregistered


Returns

ESP_OK: Success
ESP_ERR_INVALID_ARG: Invalid combination of event base and event ID
Others: Fail 





```

#### esp_event_handler_instance_unregister


```


esp_err_t esp_event_handler_instance_unregister(esp_event_base_t event_base, int32_t event_id, esp_event_handler_instance_t instance)
Unregister a handler from the system event loop. 
This function does the same as esp_event_handler_instance_unregister_with, except that it unregisters the handler instance from the default event loop.

Parameters

event_base -- [in] the base of the event with which to unregister the handler 
event_id -- [in] the ID of the event with which to unregister the handler 
instance -- [in] the instance object of the registration to be unregistered


Returns

ESP_OK: Success
ESP_ERR_INVALID_ARG: Invalid combination of event base and event ID
Others: Fail 





```

#### esp_event_post


```


esp_err_t esp_event_post(esp_event_base_t event_base, int32_t event_id, const void *event_data, size_t event_data_size, TickType_t ticks_to_wait)
Posts an event to the system default event loop. The event loop library keeps a copy of event_data and manages the copy's lifetime automatically (allocation + deletion); this ensures that the data the handler receives is always valid. 

Parameters

event_base -- [in] the event base that identifies the event 
event_id -- [in] the event ID that identifies the event 
event_data -- [in] the data, specific to the event occurrence, that gets passed to the handler 
event_data_size -- [in] the size of the event data 
ticks_to_wait -- [in] number of ticks to block on a full event queue


Returns

ESP_OK: Success
ESP_ERR_TIMEOUT: Time to wait for event queue to unblock expired, queue full when posting from ISR
ESP_ERR_INVALID_ARG: Invalid combination of event base and event ID
Others: Fail 





```

#### esp_event_post_to


```


esp_err_t esp_event_post_to(esp_event_loop_handle_t event_loop, esp_event_base_t event_base, int32_t event_id, const void *event_data, size_t event_data_size, TickType_t ticks_to_wait)
Posts an event to the specified event loop. The event loop library keeps a copy of event_data and manages the copy's lifetime automatically (allocation + deletion); this ensures that the data the handler receives is always valid. 
This function behaves in the same manner as esp_event_post, except the additional specification of the event loop to post the event to.

Parameters

event_loop -- [in] the event loop to post to, must not be NULL 
event_base -- [in] the event base that identifies the event 
event_id -- [in] the event ID that identifies the event 
event_data -- [in] the data, specific to the event occurrence, that gets passed to the handler 
event_data_size -- [in] the size of the event data 
ticks_to_wait -- [in] number of ticks to block on a full event queue


Returns

ESP_OK: Success
ESP_ERR_TIMEOUT: Time to wait for event queue to unblock expired, queue full when posting from ISR
ESP_ERR_INVALID_ARG: Invalid combination of event base and event ID
Others: Fail 





```

#### esp_event_isr_post


```


esp_err_t esp_event_isr_post(esp_event_base_t event_base, int32_t event_id, const void *event_data, size_t event_data_size, BaseType_t *task_unblocked)
Special variant of esp_event_post for posting events from interrupt handlers. 

Note
this function is only available when CONFIG_ESP_EVENT_POST_FROM_ISR is enabled 


Note
when this function is called from an interrupt handler placed in IRAM, this function should be placed in IRAM as well by enabling CONFIG_ESP_EVENT_POST_FROM_IRAM_ISR


Parameters

event_base -- [in] the event base that identifies the event 
event_id -- [in] the event ID that identifies the event 
event_data -- [in] the data, specific to the event occurrence, that gets passed to the handler 
event_data_size -- [in] the size of the event data; max is 4 bytes 
task_unblocked -- [out] an optional parameter (can be NULL) which indicates that an event task with higher priority than currently running task has been unblocked by the posted event; a context switch should be requested before the interrupt is existed.


Returns

ESP_OK: Success
ESP_FAIL: Event queue for the default event loop full
ESP_ERR_INVALID_ARG: Invalid combination of event base and event ID, data size of more than 4 bytes
Others: Fail 





```

#### esp_event_isr_post_to


```


esp_err_t esp_event_isr_post_to(esp_event_loop_handle_t event_loop, esp_event_base_t event_base, int32_t event_id, const void *event_data, size_t event_data_size, BaseType_t *task_unblocked)
Special variant of esp_event_post_to for posting events from interrupt handlers. 

Note
this function is only available when CONFIG_ESP_EVENT_POST_FROM_ISR is enabled 


Note
when this function is called from an interrupt handler placed in IRAM, this function should be placed in IRAM as well by enabling CONFIG_ESP_EVENT_POST_FROM_IRAM_ISR


Parameters

event_loop -- [in] the event loop to post to, must not be NULL 
event_base -- [in] the event base that identifies the event 
event_id -- [in] the event ID that identifies the event 
event_data -- [in] the data, specific to the event occurrence, that gets passed to the handler 
event_data_size -- [in] the size of the event data 
task_unblocked -- [out] an optional parameter (can be NULL) which indicates that an event task with higher priority than currently running task has been unblocked by the posted event; a context switch should be requested before the interrupt is existed.


Returns

ESP_OK: Success
ESP_FAIL: Event queue for the loop full
ESP_ERR_INVALID_ARG: Invalid combination of event base and event ID, data size of more than 4 bytes
Others: Fail 





```

#### esp_event_dump


```


esp_err_t esp_event_dump(FILE *file)
Dumps statistics of all event loops. 
Dumps event loop info in the format:
      event loop
          handler
          handler
          ...
      event loop
          handler
          handler
          ...

 where:

  event loop
      format: address,name rx:total_received dr:total_dropped
      where:
          address - memory address of the event loop
          name - name of the event loop, 'none' if no dedicated task
          total_received - number of successfully posted events
          total_dropped - number of events unsuccessfully posted due to queue being full

  handler
      format: address ev:base,id inv:total_invoked run:total_runtime
      where:
          address - address of the handler function
          base,id - the event specified by event base and ID this handler executes
          total_invoked - number of times this handler has been invoked
          total_runtime - total amount of time used for invoking this handler



Note
this function is a noop when CONFIG_ESP_EVENT_LOOP_PROFILING is disabled


Parameters
file -- [in] the file stream to output to

Returns

ESP_OK: Success
ESP_ERR_NO_MEM: Cannot allocate memory for event loops list
Others: Fail 





```



### Structures


#### esp_event_loop_args_t


```


struct esp_event_loop_args_t
Configuration for creating event loops. 

Public Members
queue_size

int32_t queue_size
size of the event loop queue 

task_name

const char *task_name
name of the event loop task; if NULL, a dedicated task is not created for event loop 

task_priority

UBaseType_t task_priority
priority of the event loop task, ignored if task name is NULL 

task_stack_size

uint32_t task_stack_size
stack size of the event loop task, ignored if task name is NULL 

task_core_id

BaseType_t task_core_id
core to which the event loop task is pinned to, ignored if task name is NULL 



```



### Header File


* components/esp_event/include/esp_event_base.h
* This header file can be included with:



> ```
> #include "esp_event_base.h"
> 
> ```
* This header file is a part of the API provided by the `esp_event` component. To declare that your component depends on `esp_event`, add the following to your CMakeLists.txt:



> ```
> REQUIRES esp_event
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES esp_event
> 
> ```




### Macros


#### ESP_EVENT_DECLARE_BASE


```


ESP_EVENT_DECLARE_BASE(id)

```

#### ESP_EVENT_DEFINE_BASE


```


ESP_EVENT_DEFINE_BASE(id)

```

#### ESP_EVENT_ANY_BASE


```


ESP_EVENT_ANY_BASE
register handler for any event base 

```

#### ESP_EVENT_ANY_ID


```


ESP_EVENT_ANY_ID
register handler for any event id 

```



### Type Definitions


#### esp_event_base_t


```


typedef const char *esp_event_base_t
unique pointer to a subsystem that exposes events 

```

#### esp_event_loop_handle_t


```


typedef void *esp_event_loop_handle_t
a number that identifies an event with respect to a base 

```

#### esp_event_handler_t


```


typedef void (*esp_event_handler_t)(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
function called when an event is posted to the queue 

```

#### esp_event_handler_instance_t


```


typedef void *esp_event_handler_instance_t
context identifying an instance of a registered event handler 

```




## Related Documents