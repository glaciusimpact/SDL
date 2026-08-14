	.file	"simple_windows.c"
	.intel_syntax noprefix
 # GNU C23 (x86_64-posix-seh-rev1, Built by MinGW-Builds project) version 16.1.0 (x86_64-w64-mingw32)
 #	compiled by GNU C version 16.1.0, GMP version 6.2.1, MPFR version 4.1.0, MPC version 1.2.1, isl version isl-0.27-GMP

 # GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
 # options passed: -masm=intel -mwindows -mtune=core2 -march=nocona -Os -fno-ident
	.text
	.globl	"SDL_AppEvent"
	.def	"SDL_AppEvent";	.scl	2;	.type	32;	.endef
	.seh_proc	"SDL_AppEvent"
"SDL_AppEvent":
	.seh_endprologue
 # C:\Users\tcm\Desktop\C\4\simple_windows.c:31:     if (event->type == SDL_EVENT_QUIT) {
	xor	eax, eax	 # _5
	cmp	DWORD PTR [rdx], 256	 # event_4(D)->type,
	sete	al	 #, _5
 # C:\Users\tcm\Desktop\C\4\simple_windows.c:35: }
	ret	
	.seh_endproc
	.globl	"SDL_AppQuit"
	.def	"SDL_AppQuit";	.scl	2;	.type	32;	.endef
	.seh_proc	"SDL_AppQuit"
"SDL_AppQuit":
	.seh_endprologue
 # C:\Users\tcm\Desktop\C\4\simple_windows.c:56: }
	ret	
	.seh_endproc
	.globl	"SDL_main"
	.def	"SDL_main";	.scl	2;	.type	32;	.endef
	.seh_proc	"SDL_main"
"SDL_main":
	sub	rsp, 56	 #,
	.seh_stackalloc	56
	.seh_endprologue
 # C:\SDL\x86_64-w64-mingw32\include/SDL3/SDL_main_impl.h:59:                 return SDL_EnterAppMainCallbacks(argc, argv, SDL_AppInit, SDL_AppIterate, SDL_AppEvent, SDL_AppQuit);
	lea	rax, "SDL_AppQuit"[rip]	 # tmp110,
	lea	r9, "SDL_AppIterate"[rip]	 #,
	lea	r8, "SDL_AppInit"[rip]	 #,
	mov	QWORD PTR 40[rsp], rax	 #, tmp110
	lea	rax, "SDL_AppEvent"[rip]	 # tmp111,
	mov	QWORD PTR 32[rsp], rax	 #, tmp111
	call	"SDL_EnterAppMainCallbacks"	 #
 # C:\SDL\x86_64-w64-mingw32\include/SDL3/SDL_main_impl.h:60:             }
	add	rsp, 56	 #,
	ret	
	.seh_endproc
	.section .rdata,"dr"
.LC0:
	.ascii "com.example.renderer-clear\0"
.LC1:
	.ascii "1.0\0"
.LC2:
	.ascii "Example Renderer Clear\0"
.LC3:
	.ascii "Couldn't initialize SDL: %s\0"
.LC4:
	.ascii "Title\0"
.LC5:
	.ascii "Couldn't create window/renderer: %s\0"
	.text
	.globl	"SDL_AppInit"
	.def	"SDL_AppInit";	.scl	2;	.type	32;	.endef
	.seh_proc	"SDL_AppInit"
"SDL_AppInit":
	sub	rsp, 56	 #,
	.seh_stackalloc	56
	.seh_endprologue
 # C:\Users\tcm\Desktop\C\4\simple_windows.c:12:     SDL_SetAppMetadata("Example Renderer Clear", "1.0", "com.example.renderer-clear");
	lea	r8, .LC0[rip]	 #,
	lea	rdx, .LC1[rip]	 #,
	lea	rcx, .LC2[rip]	 #,
	call	"SDL_SetAppMetadata"	 #
 # C:\Users\tcm\Desktop\C\4\simple_windows.c:14:     if (!SDL_Init(SDL_INIT_VIDEO)) {
	mov	ecx, 32	 #,
	call	"SDL_Init"	 #
 # C:\Users\tcm\Desktop\C\4\simple_windows.c:14:     if (!SDL_Init(SDL_INIT_VIDEO)) {
	test	al, al	 # _1
	jne	.L5	 #,
 # C:\Users\tcm\Desktop\C\4\simple_windows.c:15:         SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
	call	"SDL_GetError"	 #
 # C:\Users\tcm\Desktop\C\4\simple_windows.c:15:         SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
	lea	rcx, .LC3[rip]	 #,
	mov	rdx, rax	 #, _2
.L9:
	call	"SDL_Log"	 #
 # C:\Users\tcm\Desktop\C\4\simple_windows.c:16:         return SDL_APP_FAILURE;
	mov	eax, 2	 # <retval>,
	jmp	.L4	 #
.L5:
 # C:\Users\tcm\Desktop\C\4\simple_windows.c:19:     if (!SDL_CreateWindowAndRenderer("Title", 640, 480, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
	lea	rax, "renderer"[rip]	 # tmp122,
	mov	r9d, 32	 #,
	mov	r8d, 480	 #,
	mov	QWORD PTR 40[rsp], rax	 #, tmp122
	lea	rax, "window"[rip]	 # tmp123,
	mov	edx, 640	 #,
	mov	QWORD PTR 32[rsp], rax	 #, tmp123
	lea	rcx, .LC4[rip]	 #,
	call	"SDL_CreateWindowAndRenderer"	 #
 # C:\Users\tcm\Desktop\C\4\simple_windows.c:19:     if (!SDL_CreateWindowAndRenderer("Title", 640, 480, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
	test	al, al	 # _3
	jne	.L7	 #,
 # C:\Users\tcm\Desktop\C\4\simple_windows.c:20:         SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
	call	"SDL_GetError"	 #
 # C:\Users\tcm\Desktop\C\4\simple_windows.c:20:         SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
	lea	rcx, .LC5[rip]	 #,
	mov	rdx, rax	 #, _4
	jmp	.L9	 #
.L7:
 # C:\Users\tcm\Desktop\C\4\simple_windows.c:23:     SDL_SetRenderLogicalPresentation(renderer, 640, 480, SDL_LOGICAL_PRESENTATION_LETTERBOX);
	mov	rcx, QWORD PTR "renderer"[rip]	 #, renderer
	mov	r9d, 2	 #,
	mov	r8d, 480	 #,
	mov	edx, 640	 #,
	call	"SDL_SetRenderLogicalPresentation"	 #
 # C:\Users\tcm\Desktop\C\4\simple_windows.c:25:     return SDL_APP_CONTINUE;  /* carry on with the program! */
	xor	eax, eax	 # <retval>
.L4:
 # C:\Users\tcm\Desktop\C\4\simple_windows.c:26: }
	add	rsp, 56	 #,
	ret	
	.seh_endproc
	.globl	"SDL_AppIterate"
	.def	"SDL_AppIterate";	.scl	2;	.type	32;	.endef
	.seh_proc	"SDL_AppIterate"
"SDL_AppIterate":
	sub	rsp, 56	 #,
	.seh_stackalloc	56
	.seh_endprologue
 # C:\Users\tcm\Desktop\C\4\simple_windows.c:41:     SDL_SetRenderDrawColorFloat(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE_FLOAT);  /* new color, full alpha. */
	mov	rcx, QWORD PTR "renderer"[rip]	 #, renderer
	xorps	xmm3, xmm3	 #
	movaps	xmm2, xmm3	 #,
	movaps	xmm1, xmm3	 #, tmp22
	mov	DWORD PTR 32[rsp], 0x3f800000	 #,
	call	"SDL_SetRenderDrawColorFloat"	 #
 # C:\Users\tcm\Desktop\C\4\simple_windows.c:44:     SDL_RenderClear(renderer);
	mov	rcx, QWORD PTR "renderer"[rip]	 #, renderer
	call	"SDL_RenderClear"	 #
 # C:\Users\tcm\Desktop\C\4\simple_windows.c:47:     SDL_RenderPresent(renderer);
	mov	rcx, QWORD PTR "renderer"[rip]	 #, renderer
	call	"SDL_RenderPresent"	 #
 # C:\Users\tcm\Desktop\C\4\simple_windows.c:50: }
	xor	eax, eax	 #
	add	rsp, 56	 #,
	ret	
	.seh_endproc
	.globl	"WinMain"
	.def	"WinMain";	.scl	2;	.type	32;	.endef
	.seh_proc	"WinMain"
"WinMain":
	.seh_endprologue
 # C:\SDL\x86_64-w64-mingw32\include/SDL3/SDL_main_impl.h:125:                 return SDL_RunApp(0, NULL, SDL_main, NULL);
	xor	r9d, r9d	 #
	xor	edx, edx	 #
	xor	ecx, ecx	 #
	lea	r8, "SDL_main"[rip]	 #,
 # C:\SDL\x86_64-w64-mingw32\include/SDL3/SDL_main_impl.h:126:             }
 # C:\SDL\x86_64-w64-mingw32\include/SDL3/SDL_main_impl.h:125:                 return SDL_RunApp(0, NULL, SDL_main, NULL);
	jmp	"SDL_RunApp"	 #
	.seh_endproc
.lcomm "renderer",8,8
.lcomm "window",8,8
	.def	"SDL_EnterAppMainCallbacks";	.scl	2;	.type	32;	.endef
	.def	"SDL_SetAppMetadata";	.scl	2;	.type	32;	.endef
	.def	"SDL_Init";	.scl	2;	.type	32;	.endef
	.def	"SDL_GetError";	.scl	2;	.type	32;	.endef
	.def	"SDL_Log";	.scl	2;	.type	32;	.endef
	.def	"SDL_CreateWindowAndRenderer";	.scl	2;	.type	32;	.endef
	.def	"SDL_SetRenderLogicalPresentation";	.scl	2;	.type	32;	.endef
	.def	"SDL_SetRenderDrawColorFloat";	.scl	2;	.type	32;	.endef
	.def	"SDL_RenderClear";	.scl	2;	.type	32;	.endef
	.def	"SDL_RenderPresent";	.scl	2;	.type	32;	.endef
	.def	"SDL_RunApp";	.scl	2;	.type	32;	.endef
