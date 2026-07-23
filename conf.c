/*
 * $Header: /smsa/bsd:sys/conf/conf.c:bsd.riscix1_2  1.38  $
 * $Source: /smsa/bsd:sys/conf/conf.c: $
 *
 * Copyright (c) 1988 Acorn Computers Ltd., Cambridge, England
 *
 * $Desc$
 * $Log:	conf.c,v $
 * Revision 1.38  93/03/23  17:00:26  kwelton
 * Added PPP support.
 * 
 * Revision 1.37  92/05/18  17:03:15  kwelton
 * Dummy definitions for case where NES = 0 should declare esselect(), NOT
 * es_select().
 * 
 * Revision 1.36  92/05/13  20:19:31  kwelton
 * The es driver now has a stop routine.
 * 
 * Revision 1.35  92/03/25  11:48:44  kwelton
 * Restored esselect() to cdevsw - was replaced with ttselect()
 * at some point in the past (this was a Bad Thing).
 * 
 * Revision 1.34  91/10/31  16:19:55  kwelton
 * New cdevsw entry for hawkv12 driver.
 * 
 * Revision 1.33  91/02/22  11:05:48  mark
 * Corrected the #defines for when Nymos driver is disabled (NNYM = 0).
 * nym__tty changed to nym_tty.
 * 
 * Revision 1.32  90/10/19  10:00:16  mark
 * Added support for experimental indirect driver for /dev/tty.
 * 
 * Revision 1.31  90/09/25  18:58:46  kwelton
 * bdevsw[] should only have 7 entries for A680 SCSI driver - opening
 * the eighth entry (major device 7) panics the driver.
 * This should fix bug#4610
 * 
 * Revision 1.30  90/08/29  08:47:02  mark
 * Removed last "no-device-here" entries from both bdevsw and cdevsw;
 * added new no_?dev structs, containing the same field values, to be
 * used to overwrite driver entries for drivers which are configured
 * in the kernel but for which no hardware is found, when the drivers
 * are scaveneged and hence the code and data are (potentially) over-
 * written for some other purpose.
 * 
 * Revision 1.29  90/08/15  15:53:45  mark
 * Added scavenge data pointers for internal (non xcb) drivers.
 * 
 * Revision 1.28  90/06/14  08:05:32  dave
 * added entries for hawk v10 driver
 * 
 * Revision 1.27  90/06/12  22:44:13  mark
 * Added "nym" driver (for Nymos 4-port serial card).
 * 
 * Revision 1.26  90/01/04  16:18:57  mark
 * Added definition of fb_devno, for proper handling of
 * frame buffer access control.
 * 
 * Revision 1.25  89/11/01  11:26:44  mark
 * Fixed up ISO networking interrupt proc conditional compilation.
 * Fixed a few #else/#endif lines for ANSI-compliance.
 * 
 * Revision 1.24  89/10/31  10:55:31  mark
 * Inclusion of the individual driver control headers (eco.h, ps.h, etc.)
 * has been replaced by use of "devconf.h" and (because of slattach())
 * "lineconf.h".  ISO is now a system-wide -Dxxx option (like INET), so
 * "iso.h" is also redundant.  To configure ISO support, this file must be
 * edited to initialise isointrproc to the address of the ISO input routine.
 * 
 * Revision 1.23  89/10/22  20:57:44  keith
 * Preliminary kernel dump routine, is the bdevsw[] entry is 0, then specfs_dump() will
 * attempt to dump via the strategy routine. A bitmap of mapped in pages is placed in
 * page0 at the botttom of the BOOT STACK.
 * 
 * Revision 1.22  89/10/18  11:19:31  keith
 * Major shuffling of the header files, which now occur in ../include/... and no .h
 * files remain in source directories. Header files must now be included by
 * '#include <XYZ/name.h>' where XYZ is a sub-directory. For parametric header
 * files from the build directory use '#include "name.h"'
 * 
 * Revision 1.21  89/10/13  11:56:06  mark
 * Moved centronics device stuff about a bit (no change in 
 * numbering though).  Added sound device entries.
 * 
 * Revision 1.20  89/10/11  10:17:31  mark
 * Removed half of the changes made in the last revision: 
 * loop-back is not trivially de-configurable, so we don't
 * think about it now.
 * 
 * Revision 1.19  89/10/09  11:54:55  mark
 * Added lo_attach and sl_attach definitions, to allow binary-world
 * configuration control on SLIP and loop-back network interfaces.
 * 
 * Revision 1.18  89/08/02  09:44:55  mark
 * Fixed up the declaration of es_init so that the ES driver
 * can be de-configured without the link stage failing because
 * es_init isn't defined (it used to be declared as "extern").
 * 
 * Revision 1.17  89/06/21  11:09:51  mark
 * Major configurability improvements.
 * 
 * Revision 1.16  89/05/02  09:51:17  kwelton
 * Included expansion card SCSI driver.
 * 
 * Revision 1.15  89/04/28  16:40:31  mark
 * Removed expansion card control hooks - this sort of thing is now
 * handled via the XCB manager's facilities.
 * 
 * Revision 1.14  89/04/10  15:17:44  mark
 * Added #define of stclose for case where ST506 is 0.
 * 
 * Revision 1.13  89/03/30  16:59:42  mark
 * Moved #include of "es.h" - this is more sensible for dependency
 * purposes, and also prevents a silly (and faulty) compiler message.
 * 
 * Revision 1.12  88/12/21  14:02:00  mark
 * Add #include of "et.h" for "#if NET > 0" test - cc treated NET as 0.
 * 
 * Revision 1.11  88/12/18  00:59:18  keith
 * Conditionally include the laser-beam-printer stuff on lbp.h
 * 
 * Revision 1.10  88/12/04  05:18:17  keith
 * Added a field to devsw to allow bootstrap to indicate drive+partition pair
 * for translation into the minor device number.
 * 
 * Revision 1.9  88/11/24  01:20:36  keith
 * Fix3268: Return ENOTTY for ioctl()s on inappropriate devices.
 * 
 * Revision 1.8  88/11/17  11:53:37  mark
 * Added fdsize() to bdevsw entry for floppy, to allow block I/O checks
 * for off-end-access to work on floppy.  fdsize() is defined in fdc.c,
 * revisions >= 1.9.
 * 
 * Revision 1.7  88/11/17  05:04:42  keith
 * Factored out network probe() so that this file (source) can be given to clever
 * punters who want their own network device drivers!
 * 
 * Revision 1.6  88/10/24  11:24:04  mark
 * Added raw floppy open/close routines to allow correct
 * driver behaviour (because floppy is de-mountable).
 * 
 * Revision 1.5  88/10/11  11:16:20  hugo
 * Added Econet links
 * 
 * Revision 1.4  88/09/20  17:57:40  keith
 * Name change from wdxxxx to stxxxx
 * 
 * Revision 1.3  88/06/17  20:06:47  beta
 * Acorn Unix initial beta version
 * 
 */

#if defined(KERN_RCS) && !defined(lint)
static char RcsId[] = "$Header: /smsa/bsd:sys/conf/conf.c:bsd.riscix1_2  1.38  $";
#endif
/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)conf.c	7.1 (Berkeley) 6/5/86
 */

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/buf.h>
#include <sys/ioctl.h>
#include <sys/tty.h>
#include <sys/conf.h>
#include <dev/vcminor.h>
#include "lineconf.h"
#include "devconf.h"

int nulldev(), nodev(), noioctl();

int swstrategy(), swread(), swwrite();

/* A680 internal SCSI driver */
#if NSC > 0
int scsiopen(), scsiclose(), scsistrategy(), scsiread(), scsiwrite(), scsiioctl(), 
    scsireset(), scsisize(), scsisecsize();
#define	scsidump	0
extern struct scavenge scsiscavenge;
#else
#define	scsiopen	nodev
#define	scsiclose	nodev
#define	scsistrategy	nodev
#define	scsiread	nodev
#define	scsiwrite	nodev
#define	scsiioctl	noioctl
#define	scsireset	nulldev
#define	scsidump	0
#define	scsisize	0
#define	scsisecsize	nodev
#endif

/* ST506 driver (winc) */
#if NST506 > 0
int stopen(), stclose(), ststrategy(), stread(), stwrite(),
    streset(), stsize(), stsecsize();
#define	stdump		0
#else
#define	stopen		nodev
#define	stclose		nodev
#define	ststrategy	nodev
#define	stread		nodev
#define	stwrite		nodev
#define	streset		nulldev
#define	stdump		0
#define	stsize		0
#define stsecsize	nodev
#endif

/* floppy disc device */
#if NFDC > 0
int fdopen(), fdclose(), /* RAW: */ rfdopen(), rfdclose(),
    fdstrategy(), fdread(), fdwrite(), fdioctl(), fdsize(), fdsecsize();
#define	fddump		0
#else
#define	fdopen		nodev
#define	fdclose		nodev
#define	rfdopen		nodev
#define	rfdclose	nodev
#define	fdstrategy	nodev
#define	fdread		nodev
#define	fdwrite		nodev
#define	fdioctl		noioctl
#define	fddump		0
#define fdsize		nodev
#define	fdsecsize	nodev
#endif

/* "memory disc" */
#if NMD > 0
int mdopen(), mdclose(), mdstrategy();
#else
#define	mdopen		nodev
#define	mdclose		nodev
#define	mdstrategy	nodev
#endif

/* Expansion card SCSI driver */
#if PSC > 0
int scsi_bopen(), scsi_ropen(), scsi_bclose(), scsi_rclose(), scsi_strategy(),
    scsi_read(), scsi_write(), scsi_ioctl(), 
    scsi_reset(), scsi_size(), scsi_secsize();
#define	scsi_dump	0
#else
#define	scsi_bopen	nodev
#define scsi_ropen      nodev
#define	scsi_bclose	nodev
#define	scsi_rclose	nodev
#define	scsi_strategy	nodev
#define	scsi_read	nodev
#define	scsi_write	nodev
#define	scsi_ioctl	nodev
#define	scsi_reset	nulldev
#define	scsi_dump	0
#define	scsi_size	0
#define	scsi_secsize	nodev
#endif

/* Expansion card IDE driver */
#if NIDE > 0
int ecide_open(), ecide_ropen(), ecide_close(), ecide_rclose(), ecide_strategy(),
	ecide_read(), ecide_write(), ecide_ioctl(), ecide_dump(),
	ecide_reset(), ecide_size(), ecide_secsize();
#else
#define	ecide_open	nodev
#define ecide_ropen     nodev
#define	ecide_close	nodev
#define	ecide_rclose	nodev
#define	ecide_strategy	nodev
#define	ecide_read	nodev
#define	ecide_write	nodev
#define	ecide_ioctl	nodev
#define	ecide_dump	0
#define	ecide_size	0
#define	ecide_secsize	nodev
#define ecide_reset     nulldev
#endif

struct bdevsw	bdevsw[] = {
    	/* 7 x A680 SCSI device entries */
	{ scsiopen, scsiclose, scsistrategy, scsidump, scsisize, 0, "sc", 4 }, /* 0 */
	{ scsiopen, scsiclose, scsistrategy, scsidump, scsisize, 0, "sc", 4 }, /* 1 */
	{ scsiopen, scsiclose, scsistrategy, scsidump, scsisize, 0, "sc", 4 }, /* 2 */
	{ scsiopen, scsiclose, scsistrategy, scsidump, scsisize, 0, "sc", 4 }, /* 3 */
	{ scsiopen, scsiclose, scsistrategy, scsidump, scsisize, 0, "sc", 4 }, /* 4 */
	{ scsiopen, scsiclose, scsistrategy, scsidump, scsisize, 0, "sc", 4 }, /* 5 */
	{ scsiopen, scsiclose, scsistrategy, scsidump, scsisize, 0, "sc", 4 }, /* 6 */
	{ nodev, nodev, nodev, nodev, 0, 0, "??", 8 }, /* 7 */

	/* ST506 */
	{ stopen,   stclose,   ststrategy,   stdump,   stsize,   0, "st", 3 }, /* 8 */

	/* floppy disc */
	{ fdopen,   fdclose,   fdstrategy,   fddump,   fdsize,   0, "fd", 8 }, /* 9 */

	/* swap device */
#define SWAP_BLK_MAJOR 10		/* index */
	{ nodev,    nodev,     swstrategy,   nodev,    0,        0, "--", 8 }, /* 10 */

	/* memory disc */
	{ mdopen,   mdclose,   mdstrategy,   nodev,    0,        0, "md", 8 }, /* 11 */

        /* 4*7 expansion card scsi block interface entries */
	{ scsi_bopen, scsi_bclose, scsi_strategy, scsi_dump, scsi_size, 0, "sd", 3 }, /* 12 */
	{ scsi_bopen, scsi_bclose, scsi_strategy, scsi_dump, scsi_size, 0, "sd", 3 }, /* 13 */
	{ scsi_bopen, scsi_bclose, scsi_strategy, scsi_dump, scsi_size, 0, "sd", 3 }, /* 14 */
	{ scsi_bopen, scsi_bclose, scsi_strategy, scsi_dump, scsi_size, 0, "sd", 3 }, /* 15 */
	{ scsi_bopen, scsi_bclose, scsi_strategy, scsi_dump, scsi_size, 0, "sd", 3 }, /* 16 */
	{ scsi_bopen, scsi_bclose, scsi_strategy, scsi_dump, scsi_size, 0, "sd", 3 }, /* 17 */
	{ scsi_bopen, scsi_bclose, scsi_strategy, scsi_dump, scsi_size, 0, "sd", 3 }, /* 18 */
	{ scsi_bopen, scsi_bclose, scsi_strategy, scsi_dump, scsi_size, 0, "sd", 3 }, /* 19 */
	{ scsi_bopen, scsi_bclose, scsi_strategy, scsi_dump, scsi_size, 0, "sd", 3 }, /* 20 */
	{ scsi_bopen, scsi_bclose, scsi_strategy, scsi_dump, scsi_size, 0, "sd", 3 }, /* 21 */
	{ scsi_bopen, scsi_bclose, scsi_strategy, scsi_dump, scsi_size, 0, "sd", 3 }, /* 22 */
	{ scsi_bopen, scsi_bclose, scsi_strategy, scsi_dump, scsi_size, 0, "sd", 3 }, /* 23 */
	{ scsi_bopen, scsi_bclose, scsi_strategy, scsi_dump, scsi_size, 0, "sd", 3 }, /* 24 */
	{ scsi_bopen, scsi_bclose, scsi_strategy, scsi_dump, scsi_size, 0, "sd", 3 }, /* 25 */
	{ scsi_bopen, scsi_bclose, scsi_strategy, scsi_dump, scsi_size, 0, "sd", 3 }, /* 26 */
	{ scsi_bopen, scsi_bclose, scsi_strategy, scsi_dump, scsi_size, 0, "sd", 3 }, /* 27 */
	{ scsi_bopen, scsi_bclose, scsi_strategy, scsi_dump, scsi_size, 0, "sd", 3 }, /* 28 */
	{ scsi_bopen, scsi_bclose, scsi_strategy, scsi_dump, scsi_size, 0, "sd", 3 }, /* 29 */
	{ scsi_bopen, scsi_bclose, scsi_strategy, scsi_dump, scsi_size, 0, "sd", 3 }, /* 30 */
	{ scsi_bopen, scsi_bclose, scsi_strategy, scsi_dump, scsi_size, 0, "sd", 3 }, /* 31 */
	{ scsi_bopen, scsi_bclose, scsi_strategy, scsi_dump, scsi_size, 0, "sd", 3 }, /* 32 */
	{ scsi_bopen, scsi_bclose, scsi_strategy, scsi_dump, scsi_size, 0, "sd", 3 }, /* 33 */
	{ scsi_bopen, scsi_bclose, scsi_strategy, scsi_dump, scsi_size, 0, "sd", 3 }, /* 34 */
	{ scsi_bopen, scsi_bclose, scsi_strategy, scsi_dump, scsi_size, 0, "sd", 3 }, /* 35 */
	{ scsi_bopen, scsi_bclose, scsi_strategy, scsi_dump, scsi_size, 0, "sd", 3 }, /* 36 */
	{ scsi_bopen, scsi_bclose, scsi_strategy, scsi_dump, scsi_size, 0, "sd", 3 }, /* 37 */
	{ scsi_bopen, scsi_bclose, scsi_strategy, scsi_dump, scsi_size, 0, "sd", 3 }, /* 38 */
	{ scsi_bopen, scsi_bclose, scsi_strategy, scsi_dump, scsi_size, 0, "sd", 3 }, /* 39 */

	/* Expansion card IDE block interface */
	{ ecide_open, ecide_close, ecide_strategy, ecide_dump, ecide_size, 0, "id", 3 }, /* 40 */

	/* Add extra block structured devices here */

};

int nblkdev = sizeof (bdevsw) / sizeof (bdevsw[0]);

const struct bdevsw no_bdev = { nodev, nodev, nodev, nodev, 0, 0, "??", 8 };


/*
 * vm_sw_dev is a fake device implemented
 * in vm_sw.c used only internally to get to swstrategy.
 * It cannot be provided to the users, because the
 * swstrategy routine munches the b_dev and b_blkno entries
 * before calling the appropriate driver.  This would horribly
 * confuse, e.g. the hashing routines. Instead, /dev/drum is
 * provided as a character (raw) device.
 */
dev_t	vm_sw_dev = makedev(SWAP_BLK_MAJOR, 0);


int vcopen(), vcclose(), vcread(), vcwrite(), vcioctl();

int snd_open(), snd_close(), snd_write(), snd_ioctl(), snd_secsize();

int syopen(), syread(), sywrite(), syioctl(), syselect();

/* Experimental indirect driver version of /dev/tty */
int nsyopen(), nsyioctl();

#define	mmselect	seltrue

int mmread(), mmwrite();

int logopen(), logclose(), logread(), logioctl(), logselect();

int ttselect(), seltrue();

extern struct tty cons[];

/* Pseudo-ttys */
#if NPTY > 0
int ptsopen(), ptsclose(), ptsread(), ptswrite(), ptsstop();
int ptcopen(), ptcclose(), ptcread(), ptcwrite(), ptcselect();
int ptyioctl();
extern struct tty pt_tty[];
#else
#define ptsopen		nodev
#define ptsclose	nodev
#define ptsread		nodev
#define ptswrite	nodev
#define ptcopen		nodev
#define ptcclose	nodev
#define ptcread		nodev
#define ptcwrite	nodev
#define ptyioctl	noioctl
#define	pt_tty		0
#define	ptcselect	nodev
#define	ptsstop		nulldev
#endif

/* Internal serial line driver(s) */
/* The zilog chip is fitted on the A680 */
#if	NZILOG > 0
int zilog_open (), zilog_close (), zilog_read (),
	zilog_write (), zilog_ioctl ();
extern struct tty zilog_tty[];
extern struct scavenge zilog_scavenge;
#else /*NZILOG*/
#    define zilog_open		nodev
#    define zilog_close		nodev
#    define zilog_read		nodev
#    define zilog_write		nodev
#    define zilog_ioctl		noioctl
#    define zilog_tty		0
#endif/*NZILOG*/

/* The es driver supports the 65C51 used on R140 etc. */
#if	NES > 0
int esopen (), esclose (), esread (), eswrite (), \
    esioctl (), esstop (), esselect ();
extern struct tty es_tty[];
extern struct scavenge es_scavenge;
#else /*NES*/
#    define esopen		nodev
#    define esclose		nodev
#    define esread		nodev
#    define eswrite		nodev
#    define esioctl		noioctl
#    define esstop		nulldev
#    define es_tty		0
#    define esselect		nodev
#endif/*NES*/

struct cdevsw zilog_cdev = 
{
 	zilog_open,	zilog_close,	zilog_read,	zilog_write,
	zilog_ioctl,	nulldev,	nulldev,	zilog_tty,
	ttselect,	nodev,		nulldev
};
struct cdevsw es_cdev = 
{
	esopen,		esclose,	esread,		eswrite,
	esioctl,	esstop,		nulldev,	es_tty,
	esselect,	nodev,		nulldev,
};

#if NHAWK > 0
int hawk_open(), hawk_close(), hawk_read(), hawk_write();
int hawk_ioctl();
#else
#define hawk_open	nodev
#define hawk_close	nodev
#define hawk_read	nodev
#define hawk_write	nodev
#define hawk_ioctl	noioctl
#endif

#if NHAWKV12 > 0
int hawkv12_open(), hawkv12_close(), hawkv12_read(), hawkv12_write();
int hawkv12_ioctl();
#else
#define hawkv12_open	nodev
#define hawkv12_close	nodev
#define hawkv12_read	nodev
#define hawkv12_write	nodev
#define hawkv12_ioctl	noioctl
#endif

#if NNYM > 0
int nym_open(), nym_close(), nym_read(), nym_write();
int nym_ioctl(), nym_stop();
extern struct tty nym_tty[];
#else
#define nym_open	nodev
#define nym_close	nodev
#define nym_read	nodev
#define nym_write	nodev
#define nym_stop	nodev
#define nym_ioctl	noioctl
#define	nym_tty	0
#endif

#if NECO > 0
int eco_open(), eco_close(), eco_ioctl();
#else
#  define eco_open     nodev
#  define eco_close    nodev
#  define eco_ioctl    noioctl
#endif NECO

#if NCENT > 0
int cent_open(), cent_close(), cent_write(), cent_ioctl();
#else
#define	cent_open		nodev
#define	cent_close		nodev
#define	cent_write		nodev
#define	cent_ioctl		noioctl
#endif


#if NLBP > 0
int lbopen(), lbclose(), lbwrite(), lbioctl();
#else
#  define lbopen     nodev
#  define lbclose    nodev
#  define lbwrite    nodev
#  define lbioctl    noioctl
#endif NLBP

struct cdevsw	cdevsw[] = {
	/* /dev/console */
#define VC_MAJOR	0
	vcopen,		vcclose,	vcread,		vcwrite,	/* 0 */
	vcioctl,	nulldev,	nulldev,	cons,
	ttselect,	nodev,		nulldev,

	/* /dev/null, /dev/[k]mem, /dev/cmos */
#define MEM_MAJOR	1
	nulldev,	nulldev,	mmread,		mmwrite,	/* 1 */
	noioctl,	nulldev,	nulldev,	0,
	mmselect,	nodev,		nulldev,

	/* /dev/klog: kernel log device */
	logopen,	logclose,	logread,	nodev,		/* 2 */
	logioctl,	nodev,		nulldev,	0,
	logselect,	nodev,		nulldev,

	/* /dev/tty - universal tty device */
#define SY_MAJOR	3
	syopen,		nulldev,	syread,		sywrite,	/* 3 */
	syioctl,	nulldev,	nulldev,	0,
	syselect,	nodev,		nulldev,

	/* /dev/ttyp? - pseudo tty (slave side) */
	ptsopen,	ptsclose,	ptsread,	ptswrite,	/* 4 */
	ptyioctl,	ptsstop,	nulldev,	pt_tty,
	ttselect,	nodev,		nulldev,

	/* /dev/ptyp? - pseudo tty (master side) */
	ptcopen,	ptcclose,	ptcread,	ptcwrite,	/* 5 */
	ptyioctl,	nulldev,	nulldev,	pt_tty,
	ptcselect,	nodev,		nulldev,

	/*
	 * The following entry is for the serial driver (/dev/serial).  This
	 * is configured according to machine type by the machdep module,
	 * copying zilog_cdev or es_cdev (declared above) into this slot as
	 * appropriate.  It looks at serial_major, declared below, to get the
	 * slot number.
	 */
#define SERIAL_MAJOR 6
	nodev,		nodev,		nodev,		nodev,		/* 6 */
	nodev,		nulldev,	nulldev,	0,
	nodev,		nodev,		nulldev,

	/* /dev/lp - parallel printer interface */
	cent_open,	cent_close,	nodev,		cent_write,	/* 7 */
	cent_ioctl,	nodev,		nodev,		0,
	seltrue,	nodev,		nulldev,

	/* /dev/drum - raw swap device */
	nulldev,	nulldev,	swread,		swwrite,	/* 8 */
	noioctl,	nodev,		nulldev,	0,
	nodev,		nodev,		nulldev,

	/* /dev/eco[0-9a-f] econet device interface */
	eco_open,	eco_close,	nodev,	        nodev,  	/* 9 */
	eco_ioctl,	nodev,		nodev,		0,
	nodev,		nodev,		nodev,

	/* 7 x A680 internal SCSI device entries (one per target # 0..6) */
	scsiopen,	scsiclose,	scsiread,	scsiwrite,	/* 10 */
	scsiioctl,	nodev,		nulldev,	0,
	seltrue,	nodev,		scsisecsize,

	scsiopen,	scsiclose,	scsiread,	scsiwrite,	/* 11 */
	scsiioctl,	nodev,		nulldev,	0,
	seltrue,	nodev,		scsisecsize,

	scsiopen,	scsiclose,	scsiread,	scsiwrite,	/* 12 */
	scsiioctl,	nodev,		nulldev,	0,
	seltrue,	nodev,		scsisecsize,

	scsiopen,	scsiclose,	scsiread,	scsiwrite,	/* 13 */
	scsiioctl,	nodev,		nulldev,	0,
	seltrue,	nodev,		scsisecsize,

	scsiopen,	scsiclose,	scsiread,	scsiwrite,	/* 14 */
	scsiioctl,	nodev,		nulldev,	0,
	seltrue,	nodev,		scsisecsize,

	scsiopen,	scsiclose,	scsiread,	scsiwrite,	/* 15 */
	scsiioctl,	nodev,		nulldev,	0,
	seltrue,	nodev,		scsisecsize,

	scsiopen,	scsiclose,	scsiread,	scsiwrite,	/* 16 */
	scsiioctl,	nodev,		nulldev,	0,
	seltrue,	nodev,		scsisecsize,

	/* Sound output device (via VIDC/MEMC hardware) */
	snd_open,	snd_close,	nodev,		snd_write,	/* 17 */
	snd_ioctl,	nodev,		nulldev,	0,
	seltrue,	nodev,		snd_secsize,

	/* /dev/rst* - raw ST506 winchester */
	stopen,		nodev,		stread,		stwrite,	/* 18 */
	noioctl,	nodev,		streset,	0,
	seltrue,	nodev,		stsecsize,

	/* /dev/rfd[fv]* - raw floppy disc */
	rfdopen,	rfdclose,	fdread, 	fdwrite,	/* 19 */
	fdioctl,	nodev,		nulldev,	0,
	seltrue,	nodev,		fdsecsize,

	/* /dev/lbp - experimental laser beam printer interface */
	lbopen,		lbclose,       	nodev,		lbwrite,	/* 20 */
	lbioctl,	nodev,		nodev,		0,
	seltrue,	nodev,		nodev,

        /* (4 * 7) x raw scsi expansion card driver entries */
 	scsi_ropen,	scsi_rclose,	scsi_read,	scsi_write,	/* 21 */
	scsi_ioctl,	nodev,		nulldev,	0,
	seltrue,	nodev,		scsi_secsize,

 	scsi_ropen,	scsi_rclose,	scsi_read,	scsi_write,	/* 22 */
	scsi_ioctl,	nodev,		nulldev,	0,
	seltrue,	nodev,		scsi_secsize,

 	scsi_ropen,	scsi_rclose,	scsi_read,	scsi_write,	/* 23*/
	scsi_ioctl,	nodev,		nulldev,	0,
	seltrue,	nodev,		scsi_secsize,

 	scsi_ropen,	scsi_rclose,	scsi_read,	scsi_write,	/* 24 */
	scsi_ioctl,	nodev,		nulldev,	0,
	seltrue,	nodev,		scsi_secsize,

 	scsi_ropen,	scsi_rclose,	scsi_read,	scsi_write,	/* 25 */
	scsi_ioctl,	nodev,		nulldev,	0,
	seltrue,	nodev,		scsi_secsize,

 	scsi_ropen,	scsi_rclose,	scsi_read,	scsi_write,	/* 26 */
	scsi_ioctl,	nodev,		nulldev,	0,
	seltrue,	nodev,		scsi_secsize,

 	scsi_ropen,	scsi_rclose,	scsi_read,	scsi_write,	/* 27 */
	scsi_ioctl,	nodev,		nulldev,	0,
	seltrue,	nodev,		scsi_secsize,

 	scsi_ropen,	scsi_rclose,	scsi_read,	scsi_write,	/* 28 */
	scsi_ioctl,	nodev,		nulldev,	0,
	seltrue,	nodev,		scsi_secsize,

 	scsi_ropen,	scsi_rclose,	scsi_read,	scsi_write,	/* 29 */
	scsi_ioctl,	nodev,		nulldev,	0,
	seltrue,	nodev,		scsi_secsize,

 	scsi_ropen,	scsi_rclose,	scsi_read,	scsi_write,	/* 30 */
	scsi_ioctl,	nodev,		nulldev,	0,
	seltrue,	nodev,		scsi_secsize,

 	scsi_ropen,	scsi_rclose,	scsi_read,	scsi_write,	/* 31 */
	scsi_ioctl,	nodev,		nulldev,	0,
	seltrue,	nodev,		scsi_secsize,

 	scsi_ropen,	scsi_rclose,	scsi_read,	scsi_write,	/* 32 */
	scsi_ioctl,	nodev,		nulldev,	0,
	seltrue,	nodev,		scsi_secsize,

 	scsi_ropen,	scsi_rclose,	scsi_read,	scsi_write,	/* 33 */
	scsi_ioctl,	nodev,		nulldev,	0,
	seltrue,	nodev,		scsi_secsize,

 	scsi_ropen,	scsi_rclose,	scsi_read,	scsi_write,	/* 34 */
	scsi_ioctl,	nodev,		nulldev,	0,
	seltrue,	nodev,		scsi_secsize,

 	scsi_ropen,	scsi_rclose,	scsi_read,	scsi_write,	/* 35 */
	scsi_ioctl,	nodev,		nulldev,	0,
	seltrue,	nodev,		scsi_secsize,

 	scsi_ropen,	scsi_rclose,	scsi_read,	scsi_write,	/* 36 */
	scsi_ioctl,	nodev,		nulldev,	0,
	seltrue,	nodev,		scsi_secsize,

 	scsi_ropen,	scsi_rclose,	scsi_read,	scsi_write,	/* 37 */
	scsi_ioctl,	nodev,		nulldev,	0,
	seltrue,	nodev,		scsi_secsize,

 	scsi_ropen,	scsi_rclose,	scsi_read,	scsi_write,	/* 38 */
	scsi_ioctl,	nodev,		nulldev,	0,
	seltrue,	nodev,		scsi_secsize,

 	scsi_ropen,	scsi_rclose,	scsi_read,	scsi_write,	/* 39 */
	scsi_ioctl,	nodev,		nulldev,	0,
	seltrue,	nodev,		scsi_secsize,

 	scsi_ropen,	scsi_rclose,	scsi_read,	scsi_write,	/* 40 */
	scsi_ioctl,	nodev,		nulldev,	0,
	seltrue,	nodev,		scsi_secsize,

 	scsi_ropen,	scsi_rclose,	scsi_read,	scsi_write,	/* 41 */
	scsi_ioctl,	nodev,		nulldev,	0,
	seltrue,	nodev,		scsi_secsize,

 	scsi_ropen,	scsi_rclose,	scsi_read,	scsi_write,	/* 42 */
	scsi_ioctl,	nodev,		nulldev,	0,
	seltrue,	nodev,		scsi_secsize,

 	scsi_ropen,	scsi_rclose,	scsi_read,	scsi_write,	/* 43 */
	scsi_ioctl,	nodev,		nulldev,	0,
	seltrue,	nodev,		scsi_secsize,

 	scsi_ropen,	scsi_rclose,	scsi_read,	scsi_write,	/* 44 */
	scsi_ioctl,	nodev,		nulldev,	0,
	seltrue,	nodev,		scsi_secsize,

 	scsi_ropen,	scsi_rclose,	scsi_read,	scsi_write,	/* 45 */
	scsi_ioctl,	nodev,		nulldev,	0,
	seltrue,	nodev,		scsi_secsize,

 	scsi_ropen,	scsi_rclose,	scsi_read,	scsi_write,	/* 46 */
	scsi_ioctl,	nodev,		nulldev,	0,
	seltrue,	nodev,		scsi_secsize,

 	scsi_ropen,	scsi_rclose,	scsi_read,	scsi_write,	/* 47 */
	scsi_ioctl,	nodev,		nulldev,	0,
	seltrue,	nodev,		scsi_secsize,

 	scsi_ropen,	scsi_rclose,	scsi_read,	scsi_write,	/* 48 */
	scsi_ioctl,	nodev,		nulldev,	0,
	seltrue,	nodev,		scsi_secsize,

	nym_open,	nym_close,	nym_read,	nym_write,	/* 49 */
	nym_ioctl,	nym_stop,	nulldev,	nym_tty,
	ttselect,	nodev,		nulldev,

	hawk_open,	hawk_close,	hawk_read,	hawk_write,	/* 50 */
	hawk_ioctl,	nodev,		nulldev,	0,
	seltrue,	nodev,		nulldev,

	hawkv12_open,	hawkv12_close,	hawkv12_read,	hawkv12_write,	/* 51 */
	hawkv12_ioctl,	nodev,		nulldev,	0,
	seltrue,	nodev,		nulldev,

	ecide_ropen,	ecide_rclose,	ecide_read,	ecide_write,	/* 52 */
	ecide_ioctl,	nodev,		ecide_reset,	0,
	seltrue,	nodev,		ecide_secsize,

	/* Add extra character devices here */
};

const struct cdevsw no_cdev = {
	nodev,		nodev,		nodev,		nodev,
	nodev,		nodev,		nodev,		0,
	nodev,		nodev,		nodev
};

/* Alternate entry for /dev/tty driver */
int sy_major = SY_MAJOR;	/* for boot time configuration */

struct cdevsw new_sy_cdev = 
{
	nsyopen,	nodev,		nodev, 		nodev,
	nsyioctl,	nodev,		nodev,		0,
	nodev,		nodev,		nodev
};

struct scavenge *scavenge_list[] = 
{
#if NSC > 0
    &scsiscavenge,
#endif
#if NES > 0
    &es_scavenge,
#endif
#if NZILOG > 0
    &zilog_scavenge,
#endif
    (struct scavenge *)0
};


int nchrdev = sizeof (cdevsw) / sizeof (cdevsw[0]);

int mem_no = MEM_MAJOR; 	/* major device number of memory special file */

dev_t fb_devno = makedev (VC_MAJOR, VCM_FB);

int serial_major = SERIAL_MAJOR;	/* for serial driver set-up */

/* Internal (de-)configurable device initialisation routines */

/* es_initialise() is called if appropriate on R140's */
#if	NES > 0
extern void es_initialise();
void (*es_init) () = es_initialise;
#else /*NES*/
void (*es_init) ();			/* no es driver - leave 0 */
#endif/*NES*/

#if	NST506 > 0
extern void ist_initialise ();
void (*ist_init) () = ist_initialise;
#else /*NST506 > 0*/
void (*ist_init) ();			/* no ST506: leave 0 */
#endif/*NST506 > 0*/

#if	NECO > 0
extern void eco_initialise ();
void (*eco_init) () = eco_initialise;
#else /*NECO > 0*/
void (*eco_init) ();			/* no econet: leave 0 */
#endif/*NECO > 0*/

#ifdef	ISO
/*
 * ISO Networking support: configure the interface routine address
 * here if you have the relevant module.
 */
void (*isointrproc)() = 0;		/* NOT DEFINED FOR NOW */
#endif/*ISO*/

#if	NSL > 0
extern void slattach ();
void (*sl_attach) () = slattach;
#else /*NSL > 0*/
void (*sl_attach) () = 0;
#endif/*NSL > 0*/

#if	NPPP > 0
extern void pppattach();
void (*ppp_attach)() = pppattach;
#else
void (*ppp_attach)() = 0;
#endif

/* EOF conf.c */
