/* see LICENSE file for copyright and license details. */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#define	LEN(X)  (sizeof(X) / sizeof((X)[0]))
#define MASK(M) (M & (ShiftMask|ControlMask|Mod1Mask|Mod2Mask|Mod3Mask|Mod4Mask|Mod5Mask))

struct bind {
	unsigned int mod;
	KeySym sym;
	char *cmd;
};

static void daemonize(void);
static void grab(void);
static void run(void);
static void setup(void);
static void sigchld(int);
static void spawn(const char *);

static Display *dpy;

#include "config.h"

void daemonize(void) {
	pid_t pid;
	int i;
	pid = fork();
	if (pid < 0)
		perror("unable to fork");
	if (pid > 0)
		exit(0);
	setsid();
	pid = fork();
	if (pid < 0)
		perror("unable to fork");
	if (pid > 0)
		exit(0);
	i = open("/dev/null", O_RDWR);
	dup(i);
	dup(i);
}

void grab(void) {
	for (size_t i = 0; i < LEN(binds); i++)
		for (int ii = 0; ii < ScreenCount(dpy); ii++)
			XGrabKey(dpy, XKeysymToKeycode(dpy, binds[i].sym),
				binds[i].mod, RootWindow(dpy, ii), False,
					GrabModeAsync, GrabModeAsync);
}

void run(void) {
	XEvent e;
	while (1) {
		XNextEvent(dpy, &e);
		for (size_t i = 0; i < LEN(binds); i++)
			if (XKeysymToKeycode(dpy, binds[i].sym) == e.xkey.keycode
				&& MASK(binds[i].mod) == MASK(e.xkey.state))
					spawn(binds[i].cmd);
	}
}

void setup(void) {
	dpy = XOpenDisplay(NULL);
	signal(SIGCHLD, sigchld);
	XAllowEvents(dpy, AsyncBoth, CurrentTime);
	for (int i = 0; i < ScreenCount(dpy); i++)
		XSelectInput(dpy, RootWindow(dpy, i), KeyPressMask);
}

void sigchld(int sig) {
	while ((waitpid(-1, NULL, WNOHANG)) > 0)
		;
	(void)sig;
}

void spawn(const char *cmd) {
	pid_t pid;
	if (!(pid = fork())) {
		setsid();
		if (!fork())
			execlp("sh", "sh", "-c", cmd, (char *)NULL);
		else
			_exit(0);
	}
	if (pid > 0)
		wait(0);
}

int main(void) {
	daemonize();
	setup();
	grab();
	run();
	return 0;
}

