#ifndef __CARD_H__
#define __CARD_H__

//#include "gctypes.h"

/* slots */
#define CARD_SLOTA					0
#define CARD_SLOTB					1

#define CARD_WORKAREA				(5*8*1024)
#define CARD_READSIZE				512
#define CARD_FILENAMELEN			32
#define CARD_MAXFILES				128

/* Errors */
#define CARD_ERROR_UNLOCKED			1
#define CARD_ERROR_READY            0
#define CARD_ERROR_BUSY            -1
#define CARD_ERROR_WRONGDEVICE     -2
#define CARD_ERROR_NOCARD          -3
#define CARD_ERROR_NOFILE          -4
#define CARD_ERROR_IOERROR         -5
#define CARD_ERROR_BROKEN          -6
#define CARD_ERROR_EXIST           -7
#define CARD_ERROR_NOENT           -8
#define CARD_ERROR_INSSPACE        -9
#define CARD_ERROR_NOPERM          -10
#define CARD_ERROR_LIMIT           -11
#define CARD_ERROR_NAMETOOLONG     -12
#define CARD_ERROR_ENCODING        -13
#define CARD_ERROR_CANCELED        -14
#define CARD_ERROR_FATAL_ERROR     -128


/* File attribute defines */
#define CARD_ATTRIB_PUBLIC			0x04
#define CARD_ATTRIB_NOCOPY			0x08
#define CARD_ATTRIB_NOMOVE			0x10

/* Banner & Icon Attributes */
#define CARD_BANNER_W				96
#define CARD_BANNER_H				32

#define CARD_BANNER_NONE			0x00
#define CARD_BANNER_CI				0x01
#define CARD_BANNER_RGB				0x02
#define CARD_BANNER_MASK			0x03

#define CARD_MAXICONS				8
#define CARD_ICON_W					32
#define CARD_ICON_H					32

#define CARD_ICON_NONE				0x00
#define CARD_ICON_CI				0x01
#define CARD_ICON_RGB				0x02
#define CARD_ICON_MASK				0x03

#define CARD_ANIM_LOOP				0x00
#define CARD_ANIM_BOUNCE			0x04
#define CARD_ANIM_MASK				0x04

#define CARD_SPEED_END				0x00
#define CARD_SPEED_FAST				0x01
#define CARD_SPEED_MIDDLE			0x02
#define CARD_SPEED_SLOW				0x03
#define CARD_SPEED_MASK				0x03

#ifdef __cplusplus
   extern "C" {
#endif /* __cplusplus */

typedef struct _card_file {
	s32 chn;
	s32 filenum;
	s32 offset;
	s32 len;
	u16 iblock;
} card_file;

typedef struct _card_dir { 
      s32 chn; 
      u32 fileno; 
      u8 filename[CARD_FILENAMELEN+1]; 
      u8 gamecode[5]; 
      u8 company[3];
      bool showall;
} card_dir; 

typedef struct _card_stat {
	u8 filename[CARD_FILENAMELEN];
	u32 len;
	u32 time;		//time since 1970 in seconds
	u8 gamecode[4];
	u8 company[2];
	u8 banner_fmt;
	u32 icon_addr;
	u16 icon_fmt;
	u16 icon_speed;
	u32 comment_addr;
	u32 offset_banner;
	u32 offset_banner_tlut;
	u32 offset_icon[CARD_MAXICONS];
	u32 offset_icon_tlut;
	u32 offset_data;
} card_stat;

#define CARD_GetBannerFmt(stat)         (((stat)->banner_fmt)&CARD_BANNER_MASK)
#define CARD_SetBannerFmt(stat,fmt)		((stat)->banner_fmt = (u8)(((stat)->banner_fmt&~CARD_BANNER_MASK)|(fmt)))
#define CARD_GetIconFmt(stat,n)			(((stat)->icon_fmt>>(2*(n)))&CARD_ICON_MASK)
#define CARD_SetIconFmt(stat,n,fmt)		((stat)->icon_fmt = (u16)(((stat)->icon_fmt&~(CARD_ICON_MASK<<(2*(n))))|((fmt)<<(2*(n)))))
#define CARD_GetIconSpeed(stat,n)		(((stat)->icon_speed>>(2*(n)))&~CARD_SPEED_MASK);
#define CARD_SetIconSpeed(stat,n,speed)	((stat)->icon_speed = (u16)(((stat)->icon_fmt&~(CARD_SPEED_MASK<<(2*(n))))|((speed)<<(2*(n)))))
#define CARD_SetIconAddr(stat,addr)		((stat)->icon_addr = (u32)(addr))
#define CARD_SetCommentAddr(stat,addr)	((stat)->comment_addr = (u32)(addr))

typedef void (*cardcallback)(s32 chan,s32 result);

/*new api*/
s32 CARD_Init(const char *gamecode,const char *company);
s32 CARD_Mount(s32 chn,void *workarea,cardcallback detach_cb);
s32 CARD_MountAsync(s32 chn,void *workarea,cardcallback detach_cb,cardcallback attach_cb);
s32 CARD_Unmount(s32 chn);
s32 CARD_Read(card_file *file,void *buffer,u32 len,u32 offset);
s32 CARD_ReadAsync(card_file *file,void *buffer,u32 len,u32 offset,cardcallback callback);
s32 CARD_Open(s32 chn,const char *filename,card_file *file);
s32 CARD_Close(card_file *file);
s32 CARD_Create(s32 chn,const char *filename,u32 size,card_file *file);
s32 CARD_CreateAsync(s32 chn,const char *filename,u32 size,card_file *file,cardcallback callback);
s32 CARD_Delete(s32 chn,const char *filename);
s32 CARD_DeleteAsync(s32 chn,const char *filename,cardcallback callback);
s32 CARD_Write(card_file *file,void *buffer,u32 len,u32 offset);
s32 CARD_WriteAsync(card_file *file,void *buffer,u32 len,u32 offset,cardcallback callback);
s32 CARD_Format(s32 chn);
s32 CARD_FormatAsync(s32 chn,cardcallback callback);
s32 CARD_GetErrorCode(s32 chn);
s32 CARD_FindFirst(s32 chn, card_dir *dir, bool ShowAllFlag); 
s32 CARD_FindNext(card_dir *dir); 
s32 CARD_GetSectorSize(s32 chn,u32 *sector_size);
s32 CARD_GetStatus(s32 chn,s32 fileno,card_stat *stats);
s32 CARD_SetStatus(s32 chn,s32 fileno,card_stat *stats);
s32 CARD_SetStatusAsync(s32 chn,s32 fileno,card_stat *stats,cardcallback callback);
s32 CARD_GetAttributes(s32 chn,s32 fileno,u8 *attr);
s32 CARD_SetAttributes(s32 chn,s32 fileno,u8 attr);
s32 CARD_SetAttributesAsync(s32 chn,s32 fileno,u8 attr,cardcallback callback);

#ifdef __cplusplus
   }
#endif /* __cplusplus */

#endif
