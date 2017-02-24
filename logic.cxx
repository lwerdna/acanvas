#include <stdio.h>
#include <pthread.h>

#include "Driver.h"

#include "logic.h"

#include <FL/Fl_Device.H>

Driver *drv;

void *monitor_stdin(void *arg)
{
	printf("%s()\n", __func__);	

	while(1) {
		size_t size;
		char *line;
		if(getline(&line, &size, stdin) == -1) {
			printf("ERROR: getline()\n");
			goto cleanup;
		}

		Fl::lock();
		drv->canvas->processCommand(line);
		Fl::unlock();
		Fl::awake();
	}

	cleanup:
	return 0;
}

void onGuiInit(Driver *drv_, int argc, char **argv)
{
	printf("%s()\n", __func__);	

	printf("%s\n", Fl_Display_Device::display_device()->class_name());

	pthread_t thread;
	pthread_attr_t attr;

	drv = drv_;

	pthread_attr_init(&attr);
	printf("creating thread\n");
	pthread_create(&thread, &attr, monitor_stdin, NULL);
	printf("done creating thread\n");
	
}

void onExit(void)
{
	printf("%s()\n", __func__);
}
