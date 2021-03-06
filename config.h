/* See LICENSE file for copyright and license details. */

/* includes */
#include <X11/XF86keysym.h>

/* need audio keys */
#define XK_VOLM XF86XK_AudioMute
#define XK_VOLU XF86XK_AudioRaiseVolume
#define XK_VOLD XF86XK_AudioLowerVolume


/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const int startwithgaps      = 1;        /* 1 means gaps are ON by default */
static const unsigned int gappx     = 30;       /* default gap value */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int usealtbar          = 0;        /* 1 means use non-dwm status bar */
static const char *altbarclass      = "Polybar"; /* Alternate bar class name */
static const char *alttrayname      = "tray";    /* Polybar tray instance name */
static const char *altbarcmd        = "$HOME/bar.sh"; /* Alternate bar launch command */
static const char *fonts[]          = { "mononoki:pixelsize=15:antialias=true:autohint=false" };
static const char dmenufont[]       = "Calling Code";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_gray5[]       = "#6e6e6e";
static const char col_cyan[]        = "#005577";
static const char col_teal[]        = "#5AF9CE";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray1, col_gray5, col_teal  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     iscentered   isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            0,           1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "[@]",      spiral },
	{ "[\\]",     dwindle },
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
	{ "#",        gaplessgrid },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                           KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY | ControlMask,             KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY | ShiftMask,               KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY | ControlMask | ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char* dmenucmd[] = {"dmenu_run", "-m", dmenumon,
	"-fn", dmenufont, "-l", "10", "-b", "-sb", "#303030", "-sf", "green",
	"-nb", "black", "-nf", "white", NULL};
static const char* termcmd[]  = { "alacritty", NULL };
static const char* cmuscmd[]  = {"st", "-e", "cmus", NULL};
static const char* nnncmd[]   = {"st", "-e", "nnn", NULL};
static const char* htopcmd[]  = {"st", "-e", "htop", "-t", NULL};
#define scrotcmd "scrot --delay 1 -z '%Y-%m-%d-%H%M%S_$wx$h_scrot.png' -e 'mv $f ~/Pictures/scrot/'"
#define volmutecmd "pactl set-sink-mute @DEFAULT_SINK@ toggle"
#define voldowncmd "pactl set-sink-volume @DEFAULT_SINK@ -2%"
#define volupcmd "pactl set-sink-volume @DEFAULT_SINK@ +2%"
#define pausecmd ""
#define nextcmd ""
#define priorcmd ""

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_space,  spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY | ShiftMask,           XK_m,      spawn,          {.v = cmuscmd } },
	{ MODKEY | ShiftMask,           XK_n,      spawn,          {.v = nnncmd } },
	{ MODKEY | ShiftMask,           XK_s,      spawn,          SHCMD(scrotcmd) },
	{ 0,                            XK_VOLM,   spawn,          SHCMD(volmutecmd) },
	{ 0,                            XK_VOLD,   spawn,          SHCMD(voldowncmd) },
	{ 0,                            XK_VOLU,   spawn,          SHCMD(volupcmd) },
	/*{ 0,			        XK_Pause,  spawn,          SHCMD(pausecmd) },*/
	/*{ 0,			        XK_Next,   spawn,          SHCMD(nextcmd) },*/
	/*{ 0,			        XK_Prior,  spawn,          SHCMD(priorcmd) },*/
	{ MODKEY | ShiftMask,           XK_t,      spawn,          {.v = htopcmd } },
	{ MODKEY | ShiftMask,           XK_p,      spawn,          SHCMD("~/scripts/project_screen.sh") },
	{ MODKEY | ShiftMask,           XK_l,      spawn,          SHCMD("slock") },
	{ MODKEY | ShiftMask,           XK_b,      togglebar,      {0} },
	{ MODKEY | ShiftMask,           XK_j,      rotatestack,    {.i = +1 } },
	{ MODKEY | ShiftMask,           XK_k,      rotatestack,    {.i = -1 } },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_p,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY | ShiftMask,           XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY | ShiftMask,           XK_q,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_r,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY | ShiftMask,           XK_r,      setlayout,      {.v = &layouts[4]} },
	{ MODKEY,                       XK_u,      setlayout,      {.v = &layouts[5]} },
	{ MODKEY,                       XK_o,      setlayout,      {.v = &layouts[6]} },
	{ MODKEY,                       XK_g,      setlayout,      {.v = &layouts[7]} },
	{ MODKEY,                       XK_d,  	   setlayout,      {0} },
	{ MODKEY | ShiftMask,           XK_d,      togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY | ShiftMask,           XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY | ShiftMask,           XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY | ShiftMask,           XK_period, tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_n,      shiftview,      {.i = +1 } },
	{ MODKEY,                       XK_b,      shiftview,      {.i = -1 } },
	{ MODKEY,                       XK_minus,  setgaps,        {.i = -5 } },
	{ MODKEY,                       XK_equal,  setgaps,        {.i = +5 } },
	{ MODKEY | ShiftMask,           XK_minus,  setgaps,        {.i = GAP_RESET } },
	{ MODKEY | ShiftMask,           XK_equal,  setgaps,        {.i = GAP_TOGGLE } },
	{ MODKEY | ShiftMask,           XK_f,      togglefullscr,  {0} },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY | ShiftMask,           XK_e,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

