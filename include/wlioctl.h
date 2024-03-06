/*
 * Custom OID/ioctl definitions for
 *
 *
 * Broadcom 802.11abg Networking Device Driver
 *
 * Definitions subject to change without notice.
 *
 * Copyright (C) 2024, Broadcom.
 *
 *      Unless you and Broadcom execute a separate written software license
 * agreement governing use of this software, this software is licensed to you
 * under the terms of the GNU General Public License version 2 (the "GPL"),
 * available at http://www.broadcom.com/licenses/GPLv2.php, with the
 * following added to such license:
 *
 *      As a special exception, the copyright holders of this software give you
 * permission to link this software with independent modules, and to copy and
 * distribute the resulting executable under terms of your choice, provided that
 * you also meet, for each linked independent module, the terms and conditions of
 * the license of that module.  An independent module is a module which is not
 * derived from this software.  The special exception does not apply to any
 * modifications of the software.
 *
 *
 * <<Broadcom-WL-IPTag/Dual:>>
 */

#ifndef _wlioctl_h_
#define	_wlioctl_h_

#include <typedefs.h>
#include <ethernet.h>
#include <bcmip.h>
#include <bcmeth.h>
#include <bcmip.h>
#include <bcmipv6.h>
#include <bcmevent.h>
#include <802.11.h>
#include <802.11s.h>
#include <802.1d.h>
#include <bcmwifi_channels.h>
#ifdef WL11AX
#include <802.11ax.h>
#endif /* WL11AX */
#ifdef WL11BE
#include <802.11be.h>
#endif /* WL11BE */
#include <bcmwifi_rates.h>
#include <wlioctl_defs.h>
#include <bcmipv6.h>

#include <bcm_mpool_pub.h>
#include <bcmcdc.h>
#define SSSR_NEW_API

/* Include bcmerror.h for error codes or aliases */
#ifdef BCMUTILS_ERR_CODES
#include <bcmerror.h>
#endif	/* BCMUTILS_ERR_CODES */
#include <bcmtlv.h>

#ifndef USE_LEGACY_RSPEC_DEFS
#include <bcmwifi_rspec.h>
#endif

#ifdef FTM
#include <ftm_ioctl.h>
#endif

#include <wlioctl_counters.h>

/* NOTE re: Module specific error codes.
 *
 * BCME_.. error codes are extended by various features - e.g. FTM, NAN, SAE etc.
 * The current process is to allocate a range of 1024 negative 32 bit integers to
 * each module that extends the error codes to indicate a module specific status.
 *
 * The next range to use is below. If that range is used for a new feature, please
 * update the range to be used by the next feature.
 *
 * The error codes -4096 ... -5119 are reserved for firmware signing.
 *
 * Next available (inclusive) range: [-8*1024 + 1, -7*1024]
 *
 * End Note
 */

/* 11ax trigger frame format - versioning info */
#define TRIG_FRAME_FORMAT_11AX_DRAFT_1P1 0
#define TRIG_FRAME_INFO_VER2		1 // support 11AX and 11BE triggers

#define TRIGGER_FRAME_SPEC_11AX		0
#define TRIGGER_FRAME_SPEC_11BE		1

#define WL_NAN_REKEY WL_NAN_CMD_CFG_REKEY
typedef struct {
	uint32 num;
	chanspec_t list[BCM_FLEX_ARRAY];
} chanspec_list_t;

#define RSN_KCK_LENGTH	16
#define RSN_KEK_LENGTH	16
#define TPK_FTM_LEN		16
#ifndef INTF_NAME_SIZ
#define INTF_NAME_SIZ	16
#endif

#define WL_ASSOC_START_EVT_DATA_VERSION_1      1

typedef struct assoc_event_data {
	uint32 version;
	uint32 flags;
	chanspec_t join_chspec;
	uint16 PAD;
} assoc_event_data_t;

/**Used to send ioctls over the transport pipe */
typedef struct remote_ioctl {
	cdc_ioctl_t	msg;
	uint32		data_len;
	char           intf_name[INTF_NAME_SIZ];
} rem_ioctl_t;
#define REMOTE_SIZE	sizeof(rem_ioctl_t)

#define BCM_IOV_XTLV_VERSION_0	0

#define MAX_NUM_D11CORES 2
#define PHY_SAMP_TXTONE_MAX_NUM 2u

/**DFS Forced param */
typedef struct wl_dfs_forced_params {
	chanspec_t chspec;
	uint16 version;
	chanspec_list_t chspec_list;
} wl_dfs_forced_t;

#define DFS_PREFCHANLIST_VER 0x01
#define WL_CHSPEC_LIST_FIXED_SIZE	OFFSETOF(chanspec_list_t, list)
/* size of dfs forced param size given n channels are in the list */
#define WL_DFS_FORCED_PARAMS_SIZE(n) \
	(sizeof(wl_dfs_forced_t) + (((n) < 1) ? (0) : (((n) - 1)* sizeof(chanspec_t))))
#define WL_DFS_FORCED_PARAMS_FIXED_SIZE \
	(WL_CHSPEC_LIST_FIXED_SIZE + OFFSETOF(wl_dfs_forced_t, chspec_list))
#define WL_DFS_FORCED_PARAMS_MAX_SIZE \
	WL_DFS_FORCED_PARAMS_FIXED_SIZE + (WL_NUMCHANNELS * sizeof(chanspec_t))

/**association decision information */
typedef struct {
	uint8		assoc_approved;		/**< (re)association approved */
	uint8		PAD;
	uint16		reject_reason;		/**< reason code for rejecting association */
	struct		ether_addr   da;
	uint8		PAD[6];
#if defined(NDIS) && (NDISVER >= 0x0620)
	LARGE_INTEGER	sys_time;		/**< current system time */
#else
	int64		sys_time;		/**< current system time */
#endif
} assoc_decision_t;

#define DFS_SCAN_S_IDLE		-1
#define DFS_SCAN_S_RADAR_FREE 0
#define DFS_SCAN_S_RADAR_FOUND 1
#define DFS_SCAN_S_INPROGESS 2
#define DFS_SCAN_S_SCAN_ABORTED 3
#define DFS_SCAN_S_SCAN_MODESW_INPROGRESS 4
#define DFS_SCAN_S_MAX 5

#define ACTION_FRAME_SIZE 1800
#define WL_RAND_GAS_MAC (0x01 << 0u)
typedef struct wl_action_frame_v1 {
	struct ether_addr da;
	uint16 len;
	uint32 packetId;
	uint8  data[ACTION_FRAME_SIZE];
} wl_action_frame_v1_t;

typedef struct wl_action_frame_v2 {
	uint16			version;
	uint16			len_total;
	uint16                  data_offset;
	struct ether_addr       da;
	uint32                  packetId;
	struct ether_addr       rand_mac_addr;
	struct ether_addr       rand_mac_mask;
	uint16                  flags;
	uint16                  len_data;
	uint8                   data[];
} wl_action_frame_v2_t;

typedef struct ssid_info
{
	uint8		ssid_len;	/**< the length of SSID */
	uint8		ssid[32];	/**< SSID string */
} ssid_info_t;

typedef struct wl_af_params_v1 {
	uint32			channel;
	int32			dwell_time;
	struct ether_addr	BSSID;
	uint8 PAD[2];
	wl_action_frame_v1_t action_frame;
} wl_af_params_v1_t;

#define WL_WIFI_ACTION_FRAME_SIZE_V1 sizeof(wl_action_frame_v1_t)
#define WL_WIFI_AF_PARAMS_SIZE_V1    sizeof(wl_af_params_v1_t)

typedef struct wl_af_params_v2 {
	uint16			version;
	uint16			length;
	uint32			channel;
	int32			dwell_time;
	struct ether_addr	BSSID;
	uint8                   PAD[2];
	wl_action_frame_v2_t	action_frame;
} wl_af_params_v2_t;

#define WL_WIFI_ACTION_FRAME_SIZE_V2 sizeof(wl_action_frame_v2_t)
#define WL_WIFI_AF_PARAMS_SIZE_V2    sizeof(wl_af_params_v2_t)

/* version of actframe iovar to be returned as part of wl_actframe_version structure */
#define WL_ACTFRAME_VERSION_MAJOR_2        2u

/**< current version of actframe_version structure */
#define WL_ACTFRAME_VERSION_V1             1u

/** actframe interface version */
typedef struct wl_actframe_version_v1 {
	uint16  version;                /**< version of the structure */
	uint16  length;                 /**< length of the entire structure */
	/* actframe interface version numbers */
	uint16  actframe_ver_major;     /**< actframe interface major version number */
} wl_actframe_version_v1_t;

#define MFP_TEST_FLAG_NORMAL	0
#define MFP_TEST_FLAG_ANY_KEY	1
typedef struct wl_sa_query {
	uint32 flag;
	uint8  action;
	uint8  PAD;
	uint16 id;
	struct ether_addr da;
	uint16  PAD;
} wl_sa_query_t;

/* EXT_STA */
/**association information */
typedef struct {
	uint32		assoc_req;	/**< offset to association request frame */
	uint32		assoc_req_len;	/**< association request frame length */
	uint32		assoc_rsp;	/**< offset to association response frame */
	uint32		assoc_rsp_len;	/**< association response frame length */
	uint32		bcn;		/**< offset to AP beacon */
	uint32		bcn_len;	/**< AP beacon length */
	uint32		wsec;		/**< ucast security algo */
	uint32		wpaie;		/**< offset to WPA ie */
	uint8		auth_alg;	/**< 802.11 authentication mode */
	uint8		WPA_auth;	/**< WPA: authenticated key management */
	uint8		ewc_cap;	/**< EWC (MIMO) capable */
	uint8		ofdm;		/**< OFDM */
} assoc_info_t;
/* defined(EXT_STA) */

/* Flags for OBSS IOVAR Parameters */
#define WL_OBSS_DYN_BWSW_FLAG_ACTIVITY_PERIOD        (0x01)
#define WL_OBSS_DYN_BWSW_FLAG_NOACTIVITY_PERIOD      (0x02)
#define WL_OBSS_DYN_BWSW_FLAG_NOACTIVITY_INCR_PERIOD (0x04)
#define WL_OBSS_DYN_BWSW_FLAG_PSEUDO_SENSE_PERIOD    (0x08)
#define WL_OBSS_DYN_BWSW_FLAG_RX_CRS_PERIOD          (0x10)
#define WL_OBSS_DYN_BWSW_FLAG_DUR_THRESHOLD          (0x20)
#define WL_OBSS_DYN_BWSW_FLAG_TXOP_PERIOD            (0x40)

/* OBSS IOVAR Version information */
#define WL_PROT_OBSS_CONFIG_PARAMS_VERSION_1 1

#include <packed_section_start.h>
typedef BWL_PRE_PACKED_STRUCT struct {
	uint8 obss_bwsw_activity_cfm_count_cfg; /**< configurable count in
		* seconds before we confirm that OBSS is present and
		* dynamically activate dynamic bwswitch.
		*/
	uint8 obss_bwsw_no_activity_cfm_count_cfg; /**< configurable count in
		* seconds before we confirm that OBSS is GONE and
		* dynamically start pseudo upgrade. If in pseudo sense time, we
		* will see OBSS, [means that, we false detected that OBSS-is-gone
		* in watchdog] this count will be incremented in steps of
		* obss_bwsw_no_activity_cfm_count_incr_cfg for confirming OBSS
		* detection again. Note that, at present, max 30seconds is
		* allowed like this. [OBSS_BWSW_NO_ACTIVITY_MAX_INCR_DEFAULT]
		*/
	uint8 obss_bwsw_no_activity_cfm_count_incr_cfg; /* see above
		*/
	uint16 obss_bwsw_pseudo_sense_count_cfg; /**< number of msecs/cnt to be in
		* pseudo state. This is used to sense/measure the stats from lq.
		*/
	uint8 obss_bwsw_rx_crs_threshold_cfg; /**< RX CRS default threshold */
	uint8 obss_bwsw_dur_thres; /**< OBSS dyn bwsw trigger/RX CRS Sec */
	uint8 obss_bwsw_txop_threshold_cfg; /**< TXOP default threshold */
} BWL_POST_PACKED_STRUCT wlc_obss_dynbwsw_config_t;
#include <packed_section_end.h>

#include <packed_section_start.h>
typedef BWL_PRE_PACKED_STRUCT struct {
	uint32 version;	/**< version field */
	uint32 config_mask;
	uint32 reset_mask;
	wlc_obss_dynbwsw_config_t config_params;
} BWL_POST_PACKED_STRUCT obss_config_params_t;
#include <packed_section_end.h>

/**bsscfg type */
typedef enum bsscfg_type {
	BSSCFG_TYPE_GENERIC = 0,	/**< Generic AP/STA/IBSS BSS */
	BSSCFG_TYPE_P2P = 1,		/**< P2P BSS */
	/* index 2 earlier used for BTAMP */
	BSSCFG_TYPE_PSTA = 3,
	BSSCFG_TYPE_TDLS = 4,
	BSSCFG_TYPE_SLOTTED_BSS = 5,
	BSSCFG_TYPE_PROXD = 6,
	BSSCFG_TYPE_NAN = 7,
	BSSCFG_TYPE_MESH = 8,
	BSSCFG_TYPE_AIBSS = 9
} bsscfg_type_t;

/* bsscfg subtype */
typedef enum bsscfg_subtype {
	BSSCFG_SUBTYPE_NONE = 0,
	BSSCFG_GENERIC_STA = 1,		/* GENERIC */
	BSSCFG_GENERIC_AP = 2,
	BSSCFG_GENERIC_IBSS = 6,
	BSSCFG_P2P_GC = 3,		/* P2P */
	BSSCFG_P2P_GO = 4,
	BSSCFG_P2P_DISC = 5,
	/* Index 7 & 8 earlier used for BTAMP */
	BSSCFG_SUBTYPE_AWDL = 9, /* SLOTTED_BSS_TYPE */
	BSSCFG_SUBTYPE_NAN_MGMT = 10,
	BSSCFG_SUBTYPE_NAN_DATA = 11,
	BSSCFG_SUBTYPE_NAN_MGMT_DATA = 12
} bsscfg_subtype_t;

typedef struct wlc_bsscfg_info {
	uint32 type;
	uint32 subtype;
} wlc_bsscfg_info_t;

/* ULP SHM Offsets info */
typedef struct ulp_shm_info {
	uint32 m_ulp_ctrl_sdio;
	uint32 m_ulp_wakeevt_ind;
	uint32 m_ulp_wakeind;
} ulp_shm_info_t;

/* Note: Due to unpredictable size, bool type should not be used in any ioctl argument structure
 * Cf PR53622
 */

#define WL_BSS_INFO_VER_109	109
#define WL_BSS_INFO_VER_114	114
#define WL_BSS_INFO_VER_115	115

/**
 * BSS info structure
 * Applications MUST CHECK ie_offset field and length field to access IEs and
 * next bss_info structure in a vector (in wl_scan_results_t)
 */
typedef struct wl_bss_info {
	uint32		version;		/**< version field */
	uint32		length;			/**< byte length of data in this record,
						 * starting at version and including IEs
						 */
	struct ether_addr BSSID;
	uint16		beacon_period;		/**< units are Kusec */
	uint16		capability;		/**< Capability information */
	uint8		SSID_len;
	uint8		SSID[32];
	uint8		bcnflags;		/* additional flags w.r.t. beacon */
	struct {
		uint32	count;			/**< # rates in this set */
		uint8	rates[16];		/**< rates in 500kbps units w/hi bit set if basic */
	} rateset;				/**< supported rates */
	chanspec_t	chanspec;		/**< chanspec for bss */
	uint16		atim_window;		/**< units are Kusec */
	uint8		dtim_period;		/**< DTIM period */
	uint8		accessnet;		/* from beacon interwork IE (if bcnflags) */
	int16		RSSI;			/**< receive signal strength (in dBm) */
	int8		phy_noise;		/**< noise (in dBm) */
	uint8		n_cap;			/**< BSS is 802.11N Capable */
	uint8		freespace1;		/* make implicit padding explicit */
	uint8		load;			/**< BSS Load (channel utilization of BSSLoad IE) */
	uint32		nbss_cap;		/**< 802.11N+AC BSS Capabilities */
	uint8		ctl_ch;			/**< 802.11N BSS control channel number */
	uint8		PAD[3];		/**< explicit struct alignment padding */
	uint16		vht_rxmcsmap;	/**< VHT rx mcs map (802.11ac IE, VHT_CAP_MCS_MAP_*) */
	uint16		vht_txmcsmap;	/**< VHT tx mcs map (802.11ac IE, VHT_CAP_MCS_MAP_*) */
	uint8		flags;			/**< flags */
	uint8		vht_cap;		/**< BSS is vht capable */
	uint8		flags2;		/**< extended flags */
	uint8		reserved;		/**< Reserved for expansion of BSS properties */
	uint8		basic_mcs[MCSSET_LEN];	/**< 802.11N BSS required MCS set */

	uint16		ie_offset;		/**< offset at which IEs start, from beginning */
	uint16		freespace2;		/* making implicit padding explicit */
	uint32		ie_length;		/**< byte length of Information Elements */
	int16		SNR;			/**< average SNR of during frame reception */
	uint16		vht_mcsmap;		/**< STA's Associated vhtmcsmap */
	uint16		vht_mcsmap_prop;	/**< STA's Associated prop vhtmcsmap */
	uint16		vht_txmcsmap_prop;	/**< prop VHT tx mcs prop */
} wl_bss_info_v109_t;

/**
 * BSS info structure
 * Applications MUST CHECK ie_offset field and length field to access IEs and
 * next bss_info structure in a vector (in wl_scan_results_t)
 */
typedef struct wl_bss_info_v109_1 {
	uint32		version;		/**< version field */
	uint32		length;			/**< byte length of data in this record,
						 * starting at version and including IEs
						 */
	struct ether_addr BSSID;
	uint16		beacon_period;		/**< units are Kusec */
	uint16		capability;		/**< Capability information */
	uint8		SSID_len;
	uint8		SSID[32];
	uint8		bcnflags;		/* additional flags w.r.t. beacon */
	struct {
		uint32	count;			/**< # rates in this set */
		uint8	rates[16];		/**< rates in 500kbps units w/hi bit set if basic */
	} rateset;				/**< supported rates */
	chanspec_t	chanspec;		/**< chanspec for bss */
	uint16		atim_window;		/**< units are Kusec */
	uint8		dtim_period;		/**< DTIM period */
	uint8		accessnet;		/* from beacon interwork IE (if bcnflags) */
	int16		RSSI;			/**< receive signal strength (in dBm) */
	int8		phy_noise;		/**< noise (in dBm) */
	uint8		n_cap;			/**< BSS is 802.11N Capable */
	uint8		he_cap;			/**< BSS is he capable */
	uint8		load;			/**< BSS Load (channel utilization of BSSLoad IE) */
	uint32		nbss_cap;		/**< 802.11N+AC BSS Capabilities */
	uint8		ctl_ch;			/**< 802.11N BSS control channel number */
	uint8		PAD[3];		/**< explicit struct alignment padding */
	uint16		vht_rxmcsmap;	/**< VHT rx mcs map (802.11ac IE, VHT_CAP_MCS_MAP_*) */
	uint16		vht_txmcsmap;	/**< VHT tx mcs map (802.11ac IE, VHT_CAP_MCS_MAP_*) */
	uint8		flags;			/**< flags */
	uint8		vht_cap;		/**< BSS is vht capable */
	uint8		flags2;		/**< extended flags */
	uint8		reserved;		/**< Reserved for expansion of BSS properties */
	uint8		basic_mcs[MCSSET_LEN];	/**< 802.11N BSS required MCS set */

	uint16		ie_offset;		/**< offset at which IEs start, from beginning */
	uint16		freespace2;		/* making implicit padding explicit */
	uint32		ie_length;		/**< byte length of Information Elements */
	int16		SNR;			/**< average SNR of during frame reception */
	uint16		vht_mcsmap;		/**< STA's Associated vhtmcsmap */
	uint16		vht_mcsmap_prop;	/**< STA's Associated prop vhtmcsmap */
	uint16		vht_txmcsmap_prop;	/**< prop VHT tx mcs prop */
	uint32		he_mcsmap;	/**< STA's Associated hemcsmap */
	uint32		he_rxmcsmap;	/**< HE rx mcs map (802.11ax IE, HE_CAP_MCS_MAP_*) */
	uint32		he_txmcsmap;	/**< HE tx mcs map (802.11ax IE, HE_CAP_MCS_MAP_*) */
} wl_bss_info_v109_1_t;

/**
 * BSS info structure
 * Applications MUST CHECK ie_offset field and length field to access IEs and
 * next bss_info structure in a vector (in wl_scan_results_t)
 */
typedef struct wl_bss_info_v109_2 {
	uint32		version;		/**< version field */
	uint32		length;			/**< byte length of data in this record,
						 * starting at version and including IEs
						 */
	struct ether_addr BSSID;
	uint16		beacon_period;		/**< units are Kusec */
	uint16		capability;		/**< Capability information */
	uint8		SSID_len;
	uint8		SSID[32];
	uint8		bcnflags;		/* additional flags w.r.t. beacon */
	struct {
		uint32	count;			/**< # rates in this set */
		uint8	rates[16];		/**< rates in 500kbps units w/hi bit set if basic */
	} rateset;				/**< supported rates */
	chanspec_t	chanspec;		/**< chanspec for bss */
	uint16		atim_window;		/**< units are Kusec */
	uint8		dtim_period;		/**< DTIM period */
	uint8		accessnet;		/* from beacon interwork IE (if bcnflags) */
	int16		RSSI;			/**< receive signal strength (in dBm) */
	int8		phy_noise;		/**< noise (in dBm) */
	uint8		n_cap;			/**< BSS is 802.11N Capable */
	uint8		he_cap;			/**< BSS is he capable */
	uint8		load;			/**< BSS Load (channel utilization of BSSLoad IE) */
	uint32		nbss_cap;		/**< 802.11N+AC BSS Capabilities */
	uint8		ctl_ch;			/**< 802.11N BSS control channel number */
	uint8		RSVD1[3];
	uint16		vht_rxmcsmap;	/**< VHT rx mcs map (802.11ac IE, VHT_CAP_MCS_MAP_*) */
	uint16		vht_txmcsmap;	/**< VHT tx mcs map (802.11ac IE, VHT_CAP_MCS_MAP_*) */
	uint8		flags;			/**< flags */
	uint8		vht_cap;		/**< BSS is vht capable */
	uint8		flags2;		/**< extended flags */
	uint8		RSVD2;
	uint8		basic_mcs[MCSSET_LEN];	/**< 802.11N BSS required MCS set */

	uint16		ie_offset;		/**< offset at which IEs start, from beginning */
	uint16		freespace2;		/* making implicit padding explicit */
	uint32		ie_length;		/**< byte length of Information Elements */
	int16		SNR;			/**< average SNR of during frame reception */
	uint16		vht_mcsmap;		/**< STA's Associated vhtmcsmap */
	uint16		vht_mcsmap_prop;	/**< STA's Associated prop vhtmcsmap */
	uint16		vht_txmcsmap_prop;	/**< prop VHT tx mcs prop */
	uint32		he_mcsmap;	/**< STA's Associated hemcsmap */
	uint32		he_rxmcsmap;	/**< HE rx mcs map (802.11ax IE, HE_CAP_MCS_MAP_*) */
	uint32		he_txmcsmap;	/**< HE tx mcs map (802.11ax IE, HE_CAP_MCS_MAP_*) */
	uint32		timestamp[2];  /* Beacon Timestamp for FAKEAP req */
} wl_bss_info_v109_2_t;


/**
 * BSS info structure
 * Applications MUST CHECK ie_offset field and length field to access IEs and
 * next bss_info structure in a vector (in wl_scan_results_t)
 */
typedef struct wl_bss_info_v112 {
	uint32		version;		/**< version field */
	uint32		length;			/**< byte length of data in this record,
						 * starting at version and including IEs
						 */
	struct ether_addr BSSID;
	uint16		beacon_period;		/**< units are Kusec */
	uint16		capability;		/**< Capability information */
	uint8		SSID_len;
	uint8		SSID[32];		/* values can be short ssid or ssid indicates
						 * in flags2 WL_BSS2_FLAGS_SHORT_SSID
						 */
	uint8		bcnflags;		/* additional flags w.r.t. beacon */
	struct {
		uint32	count;			/**< # rates in this set */
		uint8	rates[16];		/**< rates in 500kbps units w/hi bit set if basic */
	} rateset;				/**< supported rates */
	chanspec_t	chanspec;		/**< chanspec for bss */
	uint16		atim_window;		/**< units are Kusec */
	uint8		dtim_period;		/**< DTIM period */
	uint8		accessnet;		/* from beacon interwork IE (if bcnflags) */
	int16		RSSI;			/**< receive signal strength (in dBm) */
	int8		phy_noise;		/**< noise (in dBm) */
	uint8		n_cap;			/**< BSS is 802.11N Capable */
	uint8		he_cap;			/**< BSS is he capable */
	uint8		load;			/**< BSS Load (channel utilization of BSSLoad IE) */
	uint32		nbss_cap;		/**< 802.11N+AC BSS Capabilities */
	uint8		ctl_ch;			/**< 802.11N BSS control channel number */
	uint8		RSVD1[3];
	uint16		vht_rxmcsmap;	/**< VHT rx mcs map (802.11ac IE, VHT_CAP_MCS_MAP_*) */
	uint16		vht_txmcsmap;	/**< VHT tx mcs map (802.11ac IE, VHT_CAP_MCS_MAP_*) */
	uint8		flags;			/**< flags */
	uint8		vht_cap;		/**< BSS is vht capable */
	uint8		flags2;		/**< extended flags */
	uint8		RSVD2;
	uint8		basic_mcs[MCSSET_LEN];	/**< 802.11N BSS required MCS set */
	uint16		ie_offset;		/**< offset at which IEs start, from beginning */
	uint16		freespace2;		/* making implicit padding explicit */
	uint32		ie_length;		/**< byte length of Information Elements */
	int16		SNR;			/**< average SNR of during frame reception */
	uint16		vht_mcsmap;		/**< STA's Associated vhtmcsmap */
	uint16		vht_mcsmap_prop;	/**< STA's Associated prop vhtmcsmap */
	uint16		vht_txmcsmap_prop;	/**< prop VHT tx mcs prop */
	uint32		he_mcsmap;	/**< STA's Associated hemcsmap */
	uint32		he_rxmcsmap;	/**< HE rx mcs map (802.11ax IE, HE_CAP_MCS_MAP_*) */
	uint32		he_txmcsmap;	/**< HE tx mcs map (802.11ax IE, HE_CAP_MCS_MAP_*) */
	uint32		timestamp[2];  /* Beacon Timestamp for FAKEAP req */
	uint8		pad1[20];
} wl_bss_info_v112_t;

/* EHT-MCS Map for the current operating channel width.
 *
 * The map consists of two parts:
 *
 * 1. EHT-MCS Map subfield defined in IEEE P802.11be D1.2 EHT-MCS Map (20MHz-Only Non-AP STA)
 *    for mcs 0 - 13
 * 2. EHT-MCS Bitmap defined in this file for mcs 14 & 15 (See WL_EHT_MCS_BMP_MCS_x_POS)
 */
typedef struct wl_eht_mcsmap {
	uint8		mcs_0_13[4];		/* mcs 0 - 13 mcsmap */
	uint8		mcs_14_15[1];		/* mcs 14 & 15 bitmap */
} wl_eht_mcsmap_t;

/* EHT mcs 14 & 15 bit positions */
#define WL_EHT_MCS_BMP_MCS_14_POS	0u
#define WL_EHT_MCS_BMP_MCS_15_POS	1u
/* EHT mcs 14 & 15 field width */
#define WL_EHT_MCS_BMP_MCS_SZ		1u	/* 1 bit per mcs */

/**
 * BSS info structure
 * Applications MUST CHECK ie_offset field and length field to access IEs and
 * next bss_info structure in a vector (in wl_scan_results_t)
 */
typedef struct wl_bss_info_v114 {
	uint32		version;		/**< version field */
	uint32		length;			/**< byte length of data in this record,
						 * starting at version and including IEs
						 */
	struct ether_addr BSSID;
	uint16		beacon_period;		/**< units are Kusec */
	uint16		capability;		/**< Capability information */
	uint8		SSID_len;
	uint8		SSID[32];		/* values can be short ssid or ssid indicates
						 * in flags2 WL_BSS2_FLAGS_SHORT_SSID
						 */
	uint8		bcnflags;		/* additional flags w.r.t. beacon */
	struct {
		uint32	count;			/**< # rates in this set */
		uint8	rates[16];		/**< rates in 500kbps units w/hi bit set if basic */
	} rateset;				/**< supported rates */
	chanspec_t	chanspec;		/**< chanspec for bss */
	uint16		atim_window;		/**< units are Kusec */
	uint8		dtim_period;		/**< DTIM period */
	uint8		accessnet;		/* from beacon interwork IE (if bcnflags) */
	int16		RSSI;			/**< receive signal strength (in dBm) */
	int8		phy_noise;		/**< noise (in dBm) */
	uint8		n_cap;			/**< BSS is 802.11N Capable */
	uint8		he_cap;			/**< BSS is he capable */
	uint8		load;			/**< BSS Load (channel utilization of BSSLoad IE) */
	uint32		nbss_cap;		/**< 802.11N+AC BSS Capabilities */
	uint8		ctl_ch;			/**< 802.11N BSS control channel number */
	uint8		RSVD1[3];
	uint16		vht_rxmcsmap;		/**< VHT rx mcs map (802.11ac) */
	uint16		vht_txmcsmap;		/**< VHT tx mcs map (802.11ac) */
	uint8		flags;			/**< flags */
	uint8		vht_cap;		/**< BSS is vht capable */
	uint8		flags2;			/**< extended flags */
	uint8		RSVD2[1];
	uint8		basic_mcs[MCSSET_LEN];	/**< 802.11N BSS required MCS set */
	uint16		ie_offset;		/**< offset at which IEs start, from beginning */
	uint8		RSVD3[2];		/* making implicit padding explicit */
	uint32		ie_length;		/**< byte length of Information Elements */
	int16		SNR;			/**< average SNR of during frame reception */
	uint16		vht_mcsmap;		/**< STA's Associated vhtmcsmap */
	uint16		vht_mcsmap_prop;	/**< STA's Associated prop vhtmcsmap */
	uint16		vht_txmcsmap_prop;	/**< prop VHT tx mcs prop */
	uint32		he_mcsmap;		/**< STA's Associated hemcsmap */
	uint32		he_rxmcsmap;		/**< HE rx mcs map (802.11ax) */
	uint32		he_txmcsmap;		/**< HE tx mcs map (802.11ax) */
	uint32		timestamp[2];		/* Beacon Timestamp for FAKEAP req */
	uint8		eht_cap;		/* BSS is EHT capable */
	uint8		RSVD4[1];
	wl_eht_mcsmap_t	eht_mcsmap_sta;		/* EHT-MCS Map for the STA in associated state */
	wl_eht_mcsmap_t	eht_mcsmap;		/* EHT-MCS Map for the BSS operating chan width */
} wl_bss_info_v114_t;

/**
 * BSS info structure
 * Applications MUST CHECK ie_offset field and length field to access IEs and
 * next bss_info structure in a vector (in wl_scan_results_t)
 */
typedef struct wl_bss_info_v115 {
	uint32		version;		/**< version field */
	uint32		length;			/**< byte length of data in this record,
						 * starting at version and including IEs
						 */
	struct ether_addr BSSID;
	uint16		beacon_period;		/**< units are Kusec */
	uint16		capability;		/**< Capability information */
	uint8		SSID_len;
	uint8		SSID[32];		/* values can be short ssid or ssid indicates
						 * in flags2 WL_BSS2_FLAGS_SHORT_SSID
						 */
	uint8		bcnflags;		/* additional flags w.r.t. beacon */
	struct {
		uint32	count;			/**< # rates in this set */
		uint8	rates[16];		/**< rates in 500kbps units w/hi bit set if basic */
	} rateset;				/**< supported rates */
	chanspec_t	chanspec;		/**< chanspec for bss */
	uint16		atim_window;		/**< units are Kusec */
	uint8		dtim_period;		/**< DTIM period */
	uint8		accessnet;		/* from beacon interwork IE (if bcnflags) */
	int16		RSSI;			/**< receive signal strength (in dBm) */
	int8		phy_noise;		/**< noise (in dBm) */
	uint8		n_cap;			/**< BSS is 802.11N Capable */
	uint8		he_cap;			/**< BSS is he capable */
	uint8		load;			/**< BSS Load (channel utilization of BSSLoad IE) */
	uint32		nbss_cap;		/**< 802.11N+AC BSS Capabilities */
	uint8		ctl_ch;			/**< 802.11N BSS control channel number */
	uint8		RSVD1[3];
	uint16		vht_rxmcsmap;		/**< VHT rx mcs map (802.11ac) */
	uint16		vht_txmcsmap;		/**< VHT tx mcs map (802.11ac) */
	uint8		flags;			/**< flags */
	uint8		vht_cap;		/**< BSS is vht capable */
	uint8		flags2;			/**< extended flags */
	uint8		RSVD2[1];
	uint8		basic_mcs[MCSSET_LEN];	/**< 802.11N BSS required MCS set */
	uint16		ie_offset;		/**< offset at which IEs start, from beginning */
	uint8		RSVD3[2];		/* making implicit padding explicit */
	uint32		ie_length;		/**< byte length of Information Elements */
	int16		SNR;			/**< average SNR of during frame reception */
	uint16		vht_mcsmap;		/**< STA's Associated vhtmcsmap */
	uint16		vht_mcsmap_prop;	/**< STA's Associated prop vhtmcsmap */
	uint16		vht_txmcsmap_prop;	/**< prop VHT tx mcs prop */
	uint32		he_mcsmap;		/**< STA's Associated hemcsmap */
	uint32		he_rxmcsmap;		/**< HE rx mcs map (802.11ax) */
	uint32		he_txmcsmap;		/**< HE tx mcs map (802.11ax) */
	uint32		timestamp[2];		/* Beacon Timestamp for FAKEAP req */
	uint8		eht_cap;		/* BSS is EHT capable */
	uint8		RSVD4[1];
	wl_eht_mcsmap_t	eht_mcsmap_sta;		/* EHT-MCS Map for the STA in associated state */
	wl_eht_mcsmap_t	eht_mcsmap;		/* EHT-MCS Map for the BSS operating chan width */
	struct ether_addr mld_addr;		/* AP MLD address */
	uint8		RSVD5[2];
} wl_bss_info_v115_t;

#define WL_GSCAN_FULL_RESULT_VERSION	2	/* current version of wl_gscan_result_t struct */

typedef struct wl_gscan_bss_info_v2 {
	uint32      timestamp[2];
	wl_bss_info_v109_t info;
	/* Do not add any more members below, fixed  */
	/* and variable length Information Elements to follow */
} wl_gscan_bss_info_v2_t;

typedef struct wl_gscan_bss_info_v3 {
	uint32      timestamp[2];
	uint8 info[];	/* var length wl_bss_info_X structures */
	/* Do not add any more members below, fixed  */
	/* and variable length Information Elements to follow */
} wl_gscan_bss_info_v3_t;

typedef struct wl_bsscfg {
	uint32  bsscfg_idx;
	uint32  wsec;
	uint32  WPA_auth;
	uint32  wsec_index;
	uint32  associated;
	uint32  BSS;
	uint32  phytest_on;
	struct ether_addr   prev_BSSID;
	struct ether_addr   BSSID;
	uint32  targetbss_wpa2_flags;
	uint32 assoc_type;
	uint32 assoc_state;
} wl_bsscfg_t;

typedef struct wl_if_add {
	uint32  bsscfg_flags;
	uint32  if_flags;
	uint32  ap;
	struct ether_addr   mac_addr;
	uint16  PAD;
	uint32  wlc_index;
} wl_if_add_t;

typedef struct wl_bss_config {
	uint32	atim_window;
	uint32	beacon_period;
	uint32	chanspec;
} wl_bss_config_t;

/* Number of Bsscolor supported per core */
#ifndef HE_MAX_BSSCOLOR_RES
#define HE_MAX_BSSCOLOR_RES		2
#endif

#ifndef HE_MAX_STAID_PER_BSSCOLOR
#define HE_MAX_STAID_PER_BSSCOLOR	4
#endif

/* BSSColor indices */
#define BSSCOLOR0_IDX	0
#define BSSCOLOR1_IDX	1
#define HE_BSSCOLOR0	0
#define HE_BSSCOLOR_MAX_VAL 63

/* STAID indices */
#define STAID0_IDX	0
#define STAID1_IDX	1
#define STAID2_IDX	2
#define STAID3_IDX	3
#define HE_STAID_MAX_VAL	0x07FF

#define HE_BSSCOLOR_EMLSR_LINKID_VALID	0x80
#define HE_BSSCOLOR_EMLSR_LINKID_MASK	0x7F

typedef struct wl_bsscolor_info {
	uint16 version;		/**< structure version */
	uint16 length;		/**< length of the bsscolor info */
	uint8	bsscolor_index;	/**< bsscolor index 0-1 */
	uint8	bsscolor;	/**<bsscolor value from 0 to 63 */
	uint8	partial_bsscolor_ind;
	uint8	disable_bsscolor_ind;	/**< To disable particular bsscolor */
	/* bsscolor_disable to be added as part of D1.0 */
	uint16	staid_info[HE_MAX_STAID_PER_BSSCOLOR];	/**< 0-3 staid info of each bsscolor */
	uint8	emlsr_linkid;	/**<emlsr linkid of the cfg */
	uint8	PAD[3];
} wl_bsscolor_info_t;

#define WL_BSS_USER_RADAR_CHAN_SELECT	0x1	/**< User application will randomly select
						 * radar channel.
						 */

#define DLOAD_HANDLER_VER		1	/**< Downloader version */
#define DLOAD_FLAG_VER_MASK		0xf000	/**< Downloader version mask */
#define DLOAD_FLAG_VER_SHIFT		12	/**< Downloader version shift */

#define DL_CRC_NOT_INUSE	0x0001u
#define DL_BEGIN		0x0002u /* First BLOB fragment */
#define DL_END			0x0004u /* Last BLOB fragment */
#define DL_FORCE		0x0008u /* Force download */

/* Flags for Major/Minor/Date number shift and mask */
#define EPI_VER_SHIFT     16
#define EPI_VER_MASK      0xFFFF
/** generic download types & flags */
enum {
	DL_TYPE_UCODE = 1,
	DL_TYPE_CLM = 2,
	DL_TYPE_DRRBLOB = 3
};

/** ucode type values */
enum {
	UCODE_FW,
	INIT_VALS,
	BS_INIT_VALS
};

struct wl_dload_data {
	uint16 flag;
	uint16 dload_type;
	uint32 len;
	uint32 crc;
	uint8  data[BCM_FLEX_ARRAY];
};
typedef struct wl_dload_data wl_dload_data_t;

struct wl_ucode_info {
	uint32 ucode_type;
	uint32 num_chunks;
	uint32 chunk_len;
	uint32 chunk_num;
	uint8  data_chunk[BCM_FLEX_ARRAY];
};
typedef struct wl_ucode_info wl_ucode_info_t;

struct wl_clm_dload_info {
	uint32 ds_id;
	uint32 clm_total_len;
	uint32 num_chunks;
	uint32 chunk_len;
	uint32 chunk_offset;
	uint8  data_chunk[BCM_FLEX_ARRAY];
};
typedef struct wl_clm_dload_info wl_clm_dload_info_t;

/* Max size for MSF fw version field */
#define WL_MSFVER_MAX_FW_VERSION_LEN (64u)

/* Indication for an undefined MSF calibration content version */
#define WL_MSFVER_UNDEFINED (0xFFFFFFFFu)

typedef struct wl_msf_ver_s {
	char fw_ver[WL_MSFVER_MAX_FW_VERSION_LEN]; /* FW Version */
	uint32 main_rx_ver;			   /* Main core Rx MSF content version */
	uint32 main_tx_ver;			   /* Main core Tx MSF content version */
	uint32 aux_rx_ver;			   /* Aux core Rx MSF content version */
	uint32 aux_tx_ver;			   /* Aux core Tx MSF content version */
	uint32 scan_rx_ver;			   /* Scan core Rx MSF content version */
	uint8 main_rx_is_generic;		   /* Flag - Main core Rx MSF content is generic */
	uint8 main_tx_is_generic;		   /* Flag - Main core Tx MSF content is generic */
	uint8 aux_rx_is_generic;		   /* Flag - Aux core Rx MSF content is generic */
	uint8 aux_tx_is_generic;		   /* Flag - Aux core Tx MSF content is generic */
	uint8 scan_rx_is_generic;		   /* Flag - Scan core Rx MSF content is generic */
	uint8 PAD[3];
} wl_msf_ver_t;

/* CALLOAD IOVAR - uses bcm iov sub-command framework */
#define WL_CAL_IOV_MAJOR_VER_1 1
#define WL_CAL_IOV_MINOR_VER_1 1
#define WL_CAL_IOV_MAJOR_VER_SHIFT 8
#define WL_CAL_IOV_VERSION_1_1 ((WL_CAL_IOV_MAJOR_VER_1 << WL_CAL_IOV_MAJOR_VER_SHIFT) | \
	WL_CAL_IOV_MINOR_VER_1)

/* CALLOAD subcommand ids */
enum wl_cal_subcmd_ids {
	WL_CAL_SUBCMD_VERSION =	0u,	/* Get cal iovar version */
	WL_CAL_SUBCMD_LOAD =	1u,	/* FW MSF calibration content version */
	WL_CAL_SUBCMD_FW_VER =	2u,	/* FW MSF calibration content version */
	WL_CAL_SUBCMD_STATUS =	3u,	/* MSF load status */
	WL_CAL_SUBCMD_LAST
};

typedef struct wl_cal_status_ver_s {
	uint32 status;		/* calload status */
	uint32 flags;		/* Copy of calload flags - Mainly DL_FORCE */
	wl_msf_ver_t fw_ver;	/* FW MSF content versions for all cores */
	wl_msf_ver_t msf_ver;	/* Downloaded MSF content versions for all cores */
} wl_cal_status_ver_t;

typedef struct wlc_ssid {
	uint32		SSID_len;
	uint8		SSID[DOT11_MAX_SSID_LEN];
} wlc_ssid_t;

typedef struct wlc_ssid_ext {
	uint8      hidden;
	uint8      PAD;
	uint16     flags;
	uint8      SSID_len;
	int8       rssi_thresh;
	uint8      SSID[DOT11_MAX_SSID_LEN];
} wlc_ssid_ext_t;

#define MAX_PREFERRED_AP_NUM     5
typedef struct wlc_fastssidinfo {
	uint32			SSID_channel[MAX_PREFERRED_AP_NUM];
	wlc_ssid_t		SSID_info[MAX_PREFERRED_AP_NUM];
} wlc_fastssidinfo_t;

typedef struct wnm_url {
	uint8   len;
	uint8   data[BCM_FLEX_ARRAY];
} wnm_url_t;

typedef struct chan_scandata {
	uint8		txpower;
	uint8		PAD;
	chanspec_t	channel;		/**< Channel num, bw, ctrl_sb and band */
	uint32		channel_mintime;
	uint32		channel_maxtime;
} chan_scandata_t;

typedef enum wl_scan_type {
	EXTDSCAN_FOREGROUND_SCAN,
	EXTDSCAN_BACKGROUND_SCAN,
	EXTDSCAN_FORCEDBACKGROUND_SCAN
} wl_scan_type_t;

#define WLC_EXTDSCAN_MAX_SSID		5

typedef struct wl_extdscan_params {
	int8		nprobes;			/**< 0, passive, otherwise active */
	int8		split_scan;			/**< split scan */
	int8		band;				/**< band */
	int8		PAD;
	wlc_ssid_t	ssid[WLC_EXTDSCAN_MAX_SSID]; 	/**< ssid list */
	uint32		tx_rate;			/**< in 500ksec units */
	wl_scan_type_t	scan_type;			/**< enum */
	int32		channel_num;
	chan_scandata_t channel_list[BCM_FLEX_ARRAY];	/**< list of chandata structs */
} wl_extdscan_params_t;

#define WL_EXTDSCAN_PARAMS_FIXED_SIZE	(OFFSETOF(wl_extdscan_params_t, channel_list))
#define WL_SCAN_PARAMS_SSID_MAX		10

typedef struct wl_scan_params_v1 {
	wlc_ssid_t ssid;		/**< default: {0, ""} */
	struct ether_addr bssid;	/**< default: bcast */
	int8 bss_type;			/**< default: any,
					 * DOT11_BSSTYPE_ANY/INFRASTRUCTURE/INDEPENDENT
					 */
	uint8 scan_type;		/**< flags, 0 use default */
	int32 nprobes;			/**< -1 use default, number of probes per channel */
	int32 active_time;		/**< -1 use default, dwell time per channel for
					 * active scanning
					 */
	int32 passive_time;		/**< -1 use default, dwell time per channel
					 * for passive scanning
					 */
	int32 home_time;		/**< -1 use default, dwell time for the home channel
					 * between channel scans
					 */
	int32 channel_num;		/**< count of channels and ssids that follow
					 *
					 * low half is count of channels in channel_list, 0
					 * means default (use all available channels)
					 *
					 * high half is entries in wlc_ssid_t array that
					 * follows channel_list, aligned for int32 (4 bytes)
					 * meaning an odd channel count implies a 2-byte pad
					 * between end of channel_list and first ssid
					 *
					 * if ssid count is zero, single ssid in the fixed
					 * parameter portion is assumed, otherwise ssid in
					 * the fixed portion is ignored
					 */
	uint16 channel_list[BCM_FLEX_ARRAY];
} wl_scan_params_v1_t;

/** size of wl_scan_params_v1 not including variable length array */
#define WL_SCAN_PARAMS_V1_FIXED_SIZE	(OFFSETOF(wl_scan_params_v1_t, channel_list))
#define WL_MAX_ROAMSCAN_V1_DATSZ \
	(WL_SCAN_PARAMS_V1_FIXED_SIZE + (WL_NUMCHANNELS * sizeof(uint16)))

/* changes in wl_scan_params_v2 as comapred to wl_scan_params (v1)
* unit8 scantype to uint32
*/
typedef struct wl_scan_params_v2 {
	uint16 version;			/* Version of wl_scan_params, change value of
					 * WL_SCAN_PARAM_VERSION on version update
					 */
	uint16 length;			/* length of structure wl_scan_params_v1_t
					 * without implicit pad
					 */
	wlc_ssid_t ssid;		/**< default: {0, ""} */
	struct ether_addr bssid;	/**< default: bcast */
	int8 bss_type;			/**< default: any,
					 * DOT11_BSSTYPE_ANY/INFRASTRUCTURE/INDEPENDENT
					 */
	uint8 PAD;
	uint32 scan_type;		/**< flags, 0 use default, and flags specified in
					 * WL_SCANFLAGS_XXX
					 */
	int32 nprobes;			/**< -1 use default, number of probes per channel */
	int32 active_time;		/**< -1 use default, dwell time per channel for
					 * active scanning
					 */
	int32 passive_time;		/**< -1 use default, dwell time per channel
					 * for passive scanning
					 */
	int32 home_time;		/**< -1 use default, dwell time for the home channel
					 * between channel scans
					 */
	int32 channel_num;		/**< count of channels and ssids that follow
					 *
					 * low half is count of channels in channel_list, 0
					 * means default (use all available channels)
					 *
					 * high half is entries in wlc_ssid_t array that
					 * follows channel_list, aligned for int32 (4 bytes)
					 * meaning an odd channel count implies a 2-byte pad
					 * between end of channel_list and first ssid
					 *
					 * if ssid count is zero, single ssid in the fixed
					 * parameter portion is assumed, otherwise ssid in
					 * the fixed portion is ignored
					 */
	uint16 channel_list[BCM_FLEX_ARRAY];
} wl_scan_params_v2_t;

#define WL_SCAN_PARAMS_VERSION_V2		2

/** size of wl_scan_params not including variable length array */
#define WL_SCAN_PARAMS_V2_FIXED_SIZE	(OFFSETOF(wl_scan_params_v2_t, channel_list))
#define WL_MAX_ROAMSCAN_V2_DATSZ \
	(WL_SCAN_PARAMS_V2_FIXED_SIZE + (WL_NUMCHANNELS * sizeof(uint16)))


/* changes in wl_scan_params_v3 as comapred to wl_scan_params (v2)
*  pad byte used to differentiate Short SSID and Regular SSID
*/
typedef struct wl_scan_params_v3 {
	uint16 version;			/* Version of wl_scan_params, change value of
					 * WL_SCAN_PARAM_VERSION on version update
					 */
	uint16 length;			/* length of structure wl_scan_params_v1_t
					 * without implicit pad
					 */
	wlc_ssid_t ssid;		/**< default: {0, ""} */
	struct ether_addr bssid;	/**< default: bcast */
	int8 bss_type;			/**< default: any,
					 * DOT11_BSSTYPE_ANY/INFRASTRUCTURE/INDEPENDENT
					 */
	uint8 ssid_type;		/**< ssid_type_flag ,0 use default, and flags specified
					 * WL_SCAN_SSID_FLAGS
					 */
	uint32 scan_type;		/**< flags, 0 use default, and flags specified in
					 * WL_SCANFLAGS_XXX
					 */
	int32 nprobes;			/**< -1 use default, number of probes per channel */
	int32 active_time;		/**< -1 use default, dwell time per channel for
					 * active scanning
					 */
	int32 passive_time;		/**< -1 use default, dwell time per channel
					 * for passive scanning
					 */
	int32 home_time;		/**< -1 use default, dwell time for the home channel
					 * between channel scans
					 */
	int32 channel_num;		/**< count of channels and ssids that follow
					 *
					 * low half is count of channels in channel_list, 0
					 * means default (use all available channels)
					 *
					 * high half is entries in wlc_ssid_t array that
					 * follows channel_list, aligned for int32 (4 bytes)
					 * meaning an odd channel count implies a 2-byte pad
					 * between end of channel_list and first ssid
					 *
					 * if ssid count is zero, single ssid in the fixed
					 * parameter portion is assumed, otherwise ssid in
					 * the fixed portion is ignored
					 */
	uint16 channel_list[];		/**< list of chanspecs */
} wl_scan_params_v3_t;

#define WL_SCAN_PARAMS_VERSION_V3		3

/** size of wl_scan_params not including variable length array */
#define WL_SCAN_PARAMS_V3_FIXED_SIZE	(OFFSETOF(wl_scan_params_v3_t, channel_list))
#define WL_MAX_ROAMSCAN_V3_DATSZ \
	(WL_SCAN_PARAMS_V3_FIXED_SIZE + (WL_NUMCHANNELS * sizeof(uint16)))

/* changes in wl_scan_params_v4 as comapred to wl_scan_params (v3)
 * adding scan_type_ext field.
 */
typedef struct wl_scan_params_v4 {
	uint16 version;			/* Version of wl_scan_params, change value of
					 * WL_SCAN_PARAM_VERSION on version update
					 */
	uint16 length;			/* length of structure wl_scan_params_v1_t
					 * without implicit pad
					 */
	wlc_ssid_t ssid;		/**< default: {0, ""} */
	struct ether_addr bssid;	/**< default: bcast */
	int8 bss_type;			/**< default: any,
					 * DOT11_BSSTYPE_ANY/INFRASTRUCTURE/INDEPENDENT
					 */
	uint8 ssid_type;		/**< ssid_type_flag ,0 use default, and flags specified
					 * WL_SCAN_SSID_FLAGS
					 */
	uint32 scan_type;		/**< flags, 0 use default, and flags specified in
					 * WL_SCANFLAGS_XXX
					 */
	uint32 scan_type_ext;		/**< flags, 0 use default, and flags specified in
					 * WL_SCANFLAGS_EXT_XXX
					 */
	int32 nprobes;			/**< -1 use default, number of probes per channel */
	int32 active_time;		/**< -1 use default, dwell time per channel for
					 * active scanning
					 */
	int32 passive_time;		/**< -1 use default, dwell time per channel
					 * for passive scanning
					 */
	int32 home_time;		/**< -1 use default, dwell time for the home channel
					 * between channel scans
					 */
#ifdef WL_SCAN_TX
					/* This feature does not ever go into ROM.
					 * Therefore, the #ifdef inside this structure definition
					 * does not increase risk of ROM invalidation, and is OK.
					 */
	int32 scan_tx_cnt;		/**< -1 use default, number of mgmt frames per channel */
#endif /* WL_SCAN_TX */
	int32 channel_num;		/**< count of channels and ssids that follow
					 *
					 * low half is count of channels in channel_list, 0
					 * means default (use all available channels)
					 *
					 * high half is entries in wlc_ssid_t array that
					 * follows channel_list, aligned for int32 (4 bytes)
					 * meaning an odd channel count implies a 2-byte pad
					 * between end of channel_list and first ssid
					 *
					 * if ssid count is zero, single ssid in the fixed
					 * parameter portion is assumed, otherwise ssid in
					 * the fixed portion is ignored
					 */
	uint16 channel_list[];		/**< list of chanspecs */
} wl_scan_params_v4_t;

#define WL_SCAN_PARAMS_VERSION_V4		4

/** size of wl_scan_params not including variable length array */
#define WL_SCAN_PARAMS_V4_FIXED_SIZE	(OFFSETOF(wl_scan_params_v4_t, channel_list))
#define WL_MAX_ROAMSCAN_V4_DATSZ \
	(WL_SCAN_PARAMS_V4_FIXED_SIZE + (WL_NUMCHANNELS * sizeof(uint16)))

#define ISCAN_REQ_VERSION_V1 1
#define ISCAN_REQ_VERSION_V2 2

/** incremental scan struct */
typedef struct wl_iscan_params_v1 {
	uint32 version;
	uint16 action;
	uint16 scan_duration;
	struct wl_scan_params_v1 params;
} wl_iscan_params_v1_t;

/** incremental scan struct */
typedef struct wl_iscan_params_v2 {
	uint32 version;
	uint16 action;
	uint16 scan_duration;
	wl_scan_params_v2_t params;
} wl_iscan_params_v2_t;

/** incremental scan struct */
typedef struct wl_iscan_params_v3 {
	uint32 version;
	uint16 action;
	uint16 scan_duration;
	wl_scan_params_v3_t params;
} wl_iscan_params_v3_t;

/** Scan parameter modification for some special case (eg. BT5G) */
#define WL_SCAN_PARAMS_MODS_V1	1u

enum wl_scan_params_mods_xtlv_ids {
	WL_SPM_BT5G_NONE	= 0,
	WL_SPM_BT5G_ASSOC	= 1,
	WL_SPM_BT5G_FIRST	= WL_SPM_BT5G_ASSOC,
	WL_SPM_BT5G_HIPRIO	= 2,
	WL_SPM_BT5G_LOPRIO	= 3,
	WL_SPM_BT5G_LAST	= WL_SPM_BT5G_LOPRIO
};
#define WL_SPM_BT5G_PRIO_CNT	(WL_SPM_BT5G_LAST - WL_SPM_BT5G_FIRST + 1)

typedef struct wl_scan_params_mods {
	uint16 version;		/* IOVAR version */
	uint16 length;		/* Total XTLVs length */
	uint8  spm_xtlvs[];	/* bcm_xtlv_t */
} wl_scan_params_mods_t;

/* The scan dwell time modification is in percentage.
 * The existing scan parameter is multiplied with the modifier percentage and rounded up
 */
#define WL_SCAN_PARAMS_MOD(param, mod)	(uint16)(((param) * (mod) + 99u) / 100u)

typedef struct wl_scan_params_mods_bt5g {
	uint16	active_time_mod;	/* in percentage (can be more than 100%) */
	uint16	passive_time_mod;	/* in percentage (can be more than 100%) */
	uint16	nprobes_mod;		/* in percentage (can be more than 100%) */
	uint16	pad;
} wl_scan_params_mods_bt5g_t;

/** 3 fields + size of wl_scan_params, not including variable length array */
#define WL_ISCAN_PARAMS_FIXED_SIZE	(OFFSETOF(wl_iscan_params_t, params) + sizeof(wlc_ssid_t))
#define WL_ISCAN_PARAMS_V2_FIXED_SIZE \
	(OFFSETOF(wl_iscan_params_v2_t, params) + sizeof(wlc_ssid_t))
#define WL_ISCAN_PARAMS_V3_FIXED_SIZE \
		(OFFSETOF(wl_iscan_params_v3_t, params) + sizeof(wlc_ssid_t))

typedef struct wl_scan_results_v109 {
	uint32 buflen;
	uint32 version;
	uint32 count;
	wl_bss_info_v109_t bss_info[BCM_FLEX_ARRAY];
} wl_scan_results_v109_t;
#define WL_SCAN_RESULTS_V109_FIXED_SIZE (OFFSETOF(wl_scan_results_v109_t, bss_info))

typedef struct wl_scan_results_v2 {
	uint32 buflen;
	uint32 version;
	uint32 count;
	uint8 bss_info[];	/* var length wl_bss_info_X structures */
} wl_scan_results_v2_t;
#define WL_SCAN_RESULTS_V2_FIXED_SIZE (OFFSETOF(wl_scan_results_v2_t, bss_info))

#if defined(SIMPLE_ISCAN)
/** the buf length can be WLC_IOCTL_MAXLEN (8K) to reduce iteration */
#define WLC_IW_ISCAN_MAXLEN   2048
typedef struct iscan_buf {
	struct iscan_buf * next;
	int8   iscan_buf[WLC_IW_ISCAN_MAXLEN];
} iscan_buf_t;
#endif /* SIMPLE_ISCAN */
#define ESCAN_REQ_VERSION_V1 1
#define ESCAN_REQ_VERSION_V2 2
#define ESCAN_REQ_VERSION_V3 3
#define ESCAN_REQ_VERSION_V4 4

typedef struct wl_escan_params_v1 {
	uint32 version;
	uint16 action;
	uint16 sync_id;
	struct wl_scan_params_v1 params;
} wl_escan_params_v1_t;

typedef struct wl_escan_params_v2 {
	uint32 version;
	uint16 action;
	uint16 sync_id;
	wl_scan_params_v2_t params;
} wl_escan_params_v2_t;

typedef struct wl_escan_params_v3 {
	uint32 version;
	uint16 action;
	uint16 sync_id;
	wl_scan_params_v3_t params;
} wl_escan_params_v3_t;

typedef struct wl_escan_params_v4 {
	uint32 version;
	uint16 action;
	uint16 sync_id;
	wl_scan_params_v4_t params;
} wl_escan_params_v4_t;

#define WL_ESCAN_PARAMS_V1_FIXED_SIZE (OFFSETOF(wl_escan_params_v1_t, params) + sizeof(wlc_ssid_t))
#define WL_ESCAN_PARAMS_V2_FIXED_SIZE (OFFSETOF(wl_escan_params_v2_t, params) + sizeof(wlc_ssid_t))
#define WL_ESCAN_PARAMS_V3_FIXED_SIZE (OFFSETOF(wl_escan_params_v3_t, params) + sizeof(wlc_ssid_t))
#define WL_ESCAN_PARAMS_V4_FIXED_SIZE (OFFSETOF(wl_escan_params_v4_t, params) + sizeof(wlc_ssid_t))

/** event scan reduces amount of SOC memory needed to store scan results */
typedef struct wl_escan_result_v109 {
	uint32 buflen;
	uint32 version;
	uint16 sync_id;
	uint16 bss_count;
	wl_bss_info_v109_t bss_info[BCM_FLEX_ARRAY];
} wl_escan_result_v109_t;
#define WL_ESCAN_RESULTS_V109_FIXED_SIZE (OFFSETOF(wl_escan_result_v109_t, bss_info))

/** event scan reduces amount of SOC memory needed to store scan results */
typedef struct wl_escan_result_v2 {
	uint32 buflen;
	uint32 version;
	uint16 sync_id;
	uint16 bss_count;
	uint8 bss_info[];	/* var length wl_bss_info_X structures */
} wl_escan_result_v2_t;
#define WL_ESCAN_RESULTS_V2_FIXED_SIZE (OFFSETOF(wl_escan_result_v2_t, bss_info))

typedef struct wl_gscan_result_v2 {
	uint32 buflen;
	uint32 version;
	uint32 scan_ch_bucket;
	wl_gscan_bss_info_v2_t bss_info[BCM_FLEX_ARRAY];
} wl_gscan_result_v2_t;

typedef struct wl_gscan_result_v2_1 {
	uint32 buflen;
	uint32 version;
	uint32 scan_ch_bucket;
	uint8 bss_info[];	/* var length wl_bss_info_X structures */
} wl_gscan_result_v2_1_t;


/** incremental scan results struct */
typedef struct wl_iscan_results {
	uint32 status;
	wl_scan_results_v109_t results;
} wl_iscan_results_v109_t;

/** incremental scan results struct */
typedef struct wl_iscan_results_v2 {
	uint32 status;
	wl_scan_results_v2_t results;
} wl_iscan_results_v2_t;

typedef struct wl_probe_params {
	wlc_ssid_t ssid;
	struct ether_addr bssid;
	struct ether_addr mac;
} wl_probe_params_t;

#define WL_MAXRATES_IN_SET		16	/**< max # of rates in a rateset */

typedef struct wl_rateset {
	uint32	count;				/**< # rates in this set */
	uint8	rates[WL_MAXRATES_IN_SET];	/**< rates in 500kbps units w/hi bit set if basic */
} wl_rateset_t;

#define WL_VHT_CAP_MCS_MAP_NSS_MAX	8

typedef struct wl_rateset_args_v1 {
	uint32	count;				/**< # rates in this set */
	uint8	rates[WL_MAXRATES_IN_SET];	/**< rates in 500kbps units w/hi bit set if basic */
	uint8   mcs[MCSSET_LEN];	/**< supported mcs index bit map */
	uint16 vht_mcs[WL_VHT_CAP_MCS_MAP_NSS_MAX]; /**< supported mcs index bit map per nss */
} wl_rateset_args_v1_t;

#define RATESET_ARGS_V1		(1)
#define RATESET_ARGS_V2		(2)
#define RATESET_ARGS_V3		(3)

/* RATESET_VERSION_ENABLED is defined in wl.mk post J branch.
 * Guidelines to use wl_rateset_args_t:
 * [a] in wlioctl.h: Add macro RATESET_ARGS_VX where X is the new version number.
 * [b] in wlioctl.h: Add a new structure with wl_rateset_args_vX_t
 * [c] in wlu.c app: Add support to parse new structure under RATESET_ARGS_VX
 * [d] in wlc_types.h: in respective branch and trunk: redefine wl_rateset_args_t with
 *	new wl_rateset_args_vX_t
 */


/* Note: dependent structures: sta_info_vX_t. When any update to this structure happens,
 *	update sta_info_vX_t also.
 */
#define WL_HE_CAP_MCS_MAP_NSS_MAX	8

typedef struct wl_rateset_args_v2 {
	uint16 version;		/**< version. */
	uint16 len;		/**< length */
	uint32	count;		/**< # rates in this set */
	uint8	rates[WL_MAXRATES_IN_SET];	/**< rates in 500kbps units w/hi bit set if basic */
	uint8   mcs[MCSSET_LEN];		/**< supported mcs index bit map */
	uint16 vht_mcs[WL_VHT_CAP_MCS_MAP_NSS_MAX]; /**< supported mcs index bit map per nss */
	uint16 he_mcs[WL_HE_CAP_MCS_MAP_NSS_MAX]; /**< supported he mcs index bit map per nss */
} wl_rateset_args_v2_t;

/* HE Rates BITMAP */
#define WL_HE_CAP_MCS_0_7_MAP		0x00ff
#define WL_HE_CAP_MCS_0_9_MAP		0x03ff
#define WL_HE_CAP_MCS_0_11_MAP		0x0fff

/* Note: dependent structures: sta_info_vX_t. When any update to this structure happens,
 *	update sta_info_vX_t also.
 */
#define WL_EHT_MCS_MAP_NSS_MAX		8u	/* could be max. 16 streams by the spec,
						 * but it's to control our own rateset
						 * so it probably won't exceed 4 streams
						 * anyway...
						 */

typedef struct wl_rateset_args_v3 {
	uint16	version;			/**< version. */
	uint16	len;				/**< length */
	uint32	count;				/**< # rates in 'rates' */
	uint8	rates[WL_MAXRATES_IN_SET];	/**< rates in 500kbps units w/hi bit set if basic */
	uint8   mcs[MCSSET_LEN];		/**< supported mcs index bit map */
	uint16	vht_mcs[WL_VHT_CAP_MCS_MAP_NSS_MAX];	/**< supported VHT mcs per nss */
	uint16	he_mcs[WL_HE_CAP_MCS_MAP_NSS_MAX];	/**< supported HE mcs per nss */
	uint16	eht_mcs[WL_EHT_MCS_MAP_NSS_MAX];	/**< supported EHT mcs bitmap per nss */
} wl_rateset_args_v3_t;

/* 20MHz only EHT-MCS Map mcs range bitmap */
#define WL_EHT_MCS_BMP_MCS_0_7		0x00ffu		/* mcs 0 - 7 */
#define WL_EHT_MCS_BMP_MCS_8_9		0x0300u		/* mcs 8 & 9 */
#define WL_EHT_MCS_BMP_MCS_10_11	0x0c00u		/* mcs 10 & 11 */
#define WL_EHT_MCS_BMP_MCS_12_13	0x3000u		/* mcs 12 & 13 */
#define WL_EHT_MCS_BMP_MCS_14		0x4000u		/* mcs 14 */
#define WL_EHT_MCS_BMP_MCS_15		0x8000u		/* mcs 15 */

#define TXBF_RATE_MCS_ALL		4
#define TXBF_RATE_VHT_ALL		4
#define TXBF_RATE_OFDM_ALL		8

typedef struct wl_txbf_rateset {
	uint8	txbf_rate_mcs[TXBF_RATE_MCS_ALL];	/**< one for each stream */
	uint8	txbf_rate_mcs_bcm[TXBF_RATE_MCS_ALL];	/**< one for each stream */
	uint16	txbf_rate_vht[TXBF_RATE_VHT_ALL];	/**< one for each stream */
	uint16	txbf_rate_vht_bcm[TXBF_RATE_VHT_ALL];	/**< one for each stream */
	uint8	txbf_rate_ofdm[TXBF_RATE_OFDM_ALL]; /**< bitmap of ofdm rates that enables txbf */
	uint8	txbf_rate_ofdm_bcm[TXBF_RATE_OFDM_ALL]; /* bitmap of ofdm rates that enables txbf */
	uint8	txbf_rate_ofdm_cnt;
	uint8	txbf_rate_ofdm_cnt_bcm;
} wl_txbf_rateset_t;

#define NUM_BFGAIN_ARRAY_1RX	2
#define NUM_BFGAIN_ARRAY_2RX	3
#define NUM_BFGAIN_ARRAY_3RX	4
#define NUM_BFGAIN_ARRAY_4RX	5

typedef struct wl_txbf_expgainset {
	/* bitmap for each element: B[4:0]=>c0, B[9:5]=>c1, B[14:10]=>c2, B[19:15]=>c[3-7]
	 * B[24:20]=>c[8-9], B[29:25]=>c[10-11]
	 */
	uint32	bfgain_2x1[NUM_BFGAIN_ARRAY_1RX]; /* exp     1ss, imp 1ss */
	uint32	bfgain_2x2[NUM_BFGAIN_ARRAY_2RX]; /* exp [1-2]ss, imp 1ss */
	uint32	bfgain_3x1[NUM_BFGAIN_ARRAY_1RX];
	uint32	bfgain_3x2[NUM_BFGAIN_ARRAY_2RX];
	uint32	bfgain_3x3[NUM_BFGAIN_ARRAY_3RX]; /* exp [1-3]ss, imp 1ss */
	uint32	bfgain_4x1[NUM_BFGAIN_ARRAY_1RX];
	uint32	bfgain_4x2[NUM_BFGAIN_ARRAY_2RX];
	uint32	bfgain_4x3[NUM_BFGAIN_ARRAY_3RX];
	uint32	bfgain_4x4[NUM_BFGAIN_ARRAY_4RX]; /* exp [1-4]ss, imp 1ss */
} wl_txbf_expgainset_t;

#define OFDM_RATE_MASK			0x0000007f
typedef uint8 ofdm_rates_t;

typedef struct wl_rates_info {
	wl_rateset_t rs_tgt;
	uint32 phy_type;
	int32 bandtype;
	uint8 cck_only;
	uint8 rate_mask;
	uint8 mcsallow;
	uint8 bw;
	uint8 txstreams;
	uint8 PAD[3];
} wl_rates_info_t;

/**uint32 list */
typedef struct wl_uint32_list {
	/** in - # of elements, out - # of entries */
	uint32 count;
	/** variable length uint32 list */
	uint32 element[BCM_FLEX_ARRAY];
} wl_uint32_list_t;
/* Size in bytes for wl_uint32_list_t with 'count' elements */
#define WL_UINT32_LIST_SIZE(count) (((count) + 1) * sizeof(uint32))

#define CHAN_INFO_LIST_ALL_V1 1

typedef struct wl_chanspec_attr_s_v1 {
	uint32 chaninfo;
	uint32 chanspec;
} wl_chanspec_attr_v1_t;

/**chanspecs list */
typedef struct wl_chanspec_list_s_v1 {
	uint16 version;
	/** in - # of chanspecs, out - # of entries */
	uint16 count;
	/** variable length chanspecs list */
	wl_chanspec_attr_v1_t chspecs[BCM_FLEX_ARRAY];
} wl_chanspec_list_v1_t;

/* ML assoc and scan params */
typedef struct wl_ml_assoc_scan_params_v1 {
	uint8 ml_assoc_mode;		/* whether to follow strictly ordered assoc ? */
	uint8 ml_scan_mode;		/* to identify whether ml scan needs to be triggered */
	uint8 pad[2];
} wl_ml_assoc_scan_params_v1_t;

/* WLC_SET_ALLOW_MODE values */
#define ALLOW_MODE_ANY_BSSID		0
#define ALLOW_MODE_ONLY_DESIRED_BSSID	1
#define ALLOW_MODE_NO_BSSID		2

/** used for association with a specific BSSID and chanspec list */
typedef struct wl_assoc_params {
	struct ether_addr bssid;        /**< 00:00:00:00:00:00: broadcast scan */
	uint16 bssid_cnt;               /**< 0: use chanspec_num, and the single bssid,
					  * otherwise count of chanspecs in chanspec_list
					  * AND paired bssids following chanspec_list
					  * also, chanspec_num has to be set to zero
					  * for bssid list to be used
					  */
	int32 chanspec_num;             /**< 0: all available channels,
					  * otherwise count of chanspecs in chanspec_list
					  */
	chanspec_t chanspec_list[BCM_FLEX_ARRAY]; /**< list of chanspecs */

} wl_assoc_params_t;

typedef struct wl_assoc_params_v1 {
	uint16 version;
	uint16 flags;
	struct ether_addr bssid;	/**< 00:00:00:00:00:00: broadcast scan */
	uint16 bssid_cnt;		/**< 0: use chanspec_num, and the single bssid,
					* otherwise count of chanspecs in chanspec_list
					* AND paired bssids following chanspec_list
					* also, chanspec_num has to be set to zero
					* for bssid list to be used
					*/
	int32 chanspec_num;		/**< 0: all available channels,
					* otherwise count of chanspecs in chanspec_list
					*/
	chanspec_t chanspec_list[BCM_FLEX_ARRAY];	/**< list of chanspecs */
} wl_assoc_params_v1_t;

typedef struct wl_assoc_params_v2 {
	uint16				version;
	uint16				flags;
	struct ether_addr		bssid;		/**< 00:00:00:00:00:00: broadcast scan */
	uint16				bssid_cnt;	/**< 0: use chanspec_num, and the single
							 * bssid, otherwise count of chanspecs in
							 * chanspec_list AND paired bssids
							 * following chanspec_list also,
							 * chanspec_num has to be set to zero
							 * for bssid list to be used
							 */
	/* ML assoc and scan params */
	wl_ml_assoc_scan_params_v1_t	ml_assoc_scan_param;
	int32				chanspec_num;		/**< 0: all available channels,
								* otherwise count of chanspecs
								* in chanspec_list
								*/
	chanspec_t			chanspec_list[];	/**< list of chanspecs */
} wl_assoc_params_v2_t;

typedef struct wl_assoc_params_v3 {
	uint16				version;
	uint16				flags;
	struct ether_addr		bssid;		/**< 00:00:00:00:00:00: broadcast scan */
	uint16				bssid_cnt;	/**< 0: use chanspec_num, and the single
							 * bssid, otherwise count of chanspecs in
							 * chanspec_list AND paired bssids
							 * following chanspec_list also,
							 * chanspec_num has to be set to zero
							 * for bssid list to be used
							 */
	struct ether_addr		mld_addr;	/**< 00:00:00:00:00:00: &
							 * broadcast consider invalid
							 */
	uint8				pad[2];
	int32				chanspec_num;		/**< 0: all available channels,
								* otherwise count of chanspecs
								* in chanspec_list
								*/
	chanspec_t			chanspec_list[];	/**< list of chanspecs. Follows
								 * bssid's depending on bssid_cnt
								 */
} wl_assoc_params_v3_t;

/** Assoc params flags */
#define ASSOC_HINT_BSSID_PRESENT		0x0001u
/* FW to delete PMKSA of bssid listed in assoc params */
#define WL_ASSOC_PARAM_FLAG_DEL_PMKSA		0x0002u
#define WL_ASSOC_PARAM_FLAG_ACTIVE6G		0x0004u
/* Flag is set when extra Ml probe needs to be sent */
#define WL_ASSOC_PARAM_FLAG_SEND_EXTRA_ML_PRB	0x0010u

#define WL_ASSOC_PARAMS_FIXED_SIZE      OFFSETOF(wl_assoc_params_t, chanspec_list)
#define WL_ASSOC_PARAMS_FIXED_SIZE_V1   OFFSETOF(wl_assoc_params_v1_t, chanspec_list)
#define WL_ASSOC_PARAMS_FIXED_SIZE_V2   OFFSETOF(wl_assoc_params_v2_t, chanspec_list)
#define WL_ASSOC_PARAMS_FIXED_SIZE_V3   OFFSETOF(wl_assoc_params_v3_t, chanspec_list)

#define WL_ASSOC_ML_SCAN_MODE_MAX_V1	2u	/* Max ml scan mode */
#define WL_ASSOC_ML_SCAN_MODE_0		0u	/* scan ML channels if found in rnr */
#define WL_ASSOC_ML_SCAN_MODE_1		1u	/* Scan only the channels provided in
						 * chanspec list
						 */
#define WL_ASSOC_ML_ASSOC_MODE_MAX_V1	2u	/* Max ml assoc mode */
#define WL_ASSOC_ML_ASSOC_MODE_0	0u	/* use scoring to join */
#define WL_ASSOC_ML_ASSOC_MODE_1	1u	/* Overwrite scoring, perform assoc in the order of
						 * provided chanspec list
						 */

/** used for reassociation/roam to a specific BSSID and channel */
typedef  wl_assoc_params_t wl_reassoc_params_t;
typedef  wl_assoc_params_v1_t wl_reassoc_params_v1_t;
/* reassoc params is jumped to use v3 marking to keep in sync with
 * assoc param revision marking.
 */
typedef  wl_assoc_params_v3_t wl_reassoc_params_v3_t;

#define WL_REASSOC_PARAMS_FIXED_SIZE		WL_ASSOC_PARAMS_FIXED_SIZE
#define WL_REASSOC_PARAMS_FIXED_SIZE_V1		WL_ASSOC_PARAMS_FIXED_SIZE_V1
#define WL_REASSOC_PARAMS_FIXED_SIZE_V3		WL_ASSOC_PARAMS_FIXED_SIZE_V3

#define WL_EXT_REASSOC_VER	1
#define WL_EXT_REASSOC_VER_1	2
#define WL_EXT_REASSOC_VER_3	3

typedef struct wl_ext_reassoc_params {
	uint16 version;
	uint16 length;
	uint32 flags;
	wl_reassoc_params_t params;
} wl_ext_reassoc_params_t;

/* Flags field defined above in wl_ext_reassoc_params
 * The below flag bit is used to specify the type
 * of scan to be used for reassoc
 */

#define WL_SCAN_MODE_HIGH_ACC		0u	/**<  use high accuracy scans for roam */
#define WL_SCAN_MODE_LOW_SPAN		1u	/**< use low span scans for roam */
#define WL_SCAN_MODE_LOW_POWER		2u	/**< use low power scans for roam */
#define WL_SCAN_MODE_NO_6GHZ_FOLLOWUP	4u	/* 6G active scan not done due to RNR or FILS */

#define WL_EXTREASSOC_PARAMS_FIXED_SIZE		(OFFSETOF(wl_ext_reassoc_params_t, params) + \
					 WL_REASSOC_PARAMS_FIXED_SIZE)
typedef struct wl_ext_reassoc_params_v1 {
	uint16 version;
	uint16 length;
	uint32 flags;
	wl_reassoc_params_v1_t params;
} wl_ext_reassoc_params_v1_t;

#define WL_EXTREASSOC_PARAMS_FIXED_SIZE_V1	(OFFSETOF(wl_ext_reassoc_params_v1_t, params) + \
					 WL_REASSOC_PARAMS_FIXED_SIZE_V1)

typedef struct wl_ext_reassoc_params_v3 {
	uint16 version;
	uint16 length;
	uint32 flags;
	wl_reassoc_params_v3_t params;
} wl_ext_reassoc_params_v3_t;

#define WL_EXTREASSOC_PARAMS_FIXED_SIZE_V3	(OFFSETOF(wl_ext_reassoc_params_v3_t, params) + \
					 WL_REASSOC_PARAMS_FIXED_SIZE_V3)
/** used for association to a specific BSSID and channel */
typedef wl_assoc_params_t wl_join_assoc_params_t;
typedef wl_assoc_params_v1_t wl_join_assoc_params_v1_t;
typedef wl_assoc_params_v2_t wl_join_assoc_params_v2_t;
typedef wl_assoc_params_v3_t wl_join_assoc_params_v3_t;
#define WL_JOIN_ASSOC_PARAMS_FIXED_SIZE	WL_ASSOC_PARAMS_FIXED_SIZE
#define WL_JOIN_ASSOC_PARAMS_FIXED_SIZE_V1 WL_ASSOC_PARAMS_FIXED_SIZE_V1
#define WL_JOIN_ASSOC_PARAMS_FIXED_SIZE_V2 WL_ASSOC_PARAMS_FIXED_SIZE_V2
#define WL_JOIN_ASSOC_PARAMS_FIXED_SIZE_V3 WL_ASSOC_PARAMS_FIXED_SIZE_V3
/** used for join with or without a specific bssid and channel list */
typedef struct wl_join_params {
	wlc_ssid_t ssid;
	wl_assoc_params_t params;	/**< optional field, but it must include the fixed portion
					 * of the wl_assoc_params_t struct when it does present.
					 */
} wl_join_params_t;

/** used for join with or without a specific bssid and channel list */
typedef struct wl_join_params_v1 {
	wlc_ssid_t ssid;
	wl_assoc_params_v1_t params;    /**< optional field, but it must include the fixed portion
					* of the wl_assoc_params_t struct when it does present.
					*/
} wl_join_params_v1_t;

/** used for join with or without a specific bssid and channel list */
typedef struct wl_join_params_v2 {
	wlc_ssid_t ssid;
	wl_assoc_params_v2_t params;    /**< optional field, but it must include the fixed portion
					* of the wl_assoc_params_t struct when it does present.
					*/
} wl_join_params_v2_t;

/** used for join with or without a specific bssid and channel list */
typedef struct wl_join_params_v3 {
	wlc_ssid_t ssid;
	wl_assoc_params_v3_t params;    /**< optional field, but it must include the fixed portion
					* of the wl_assoc_params_t struct when it does present.
					*/
} wl_join_params_v3_t;

#define WL_JOIN_PARAMS_FIXED_SIZE	(OFFSETOF(wl_join_params_t, params) + \
					 WL_ASSOC_PARAMS_FIXED_SIZE)
#define WL_JOIN_PARAMS_FIXED_SIZE_V1	(OFFSETOF(wl_join_params_v1_t, params) + \
					WL_ASSOC_PARAMS_FIXED_SIZE_V1)
#define WL_JOIN_PARAMS_FIXED_SIZE_V2	(OFFSETOF(wl_join_params_v2_t, params) + \
					WL_ASSOC_PARAMS_FIXED_SIZE_V2)
#define WL_JOIN_PARAMS_FIXED_SIZE_V3	(OFFSETOF(wl_join_params_v3_t, params) + \
					WL_ASSOC_PARAMS_FIXED_SIZE_V3)
typedef struct wlc_roam_exp_params {
	int8 a_band_boost_threshold;
	int8 a_band_penalty_threshold;
	int8 a_band_boost_factor;
	int8 a_band_penalty_factor;
	int8 cur_bssid_boost;
	int8 alert_roam_trigger_threshold;
	int16 a_band_max_boost;
} wlc_roam_exp_params_t;

#define ROAM_EXP_CFG_VERSION_1     1
#define ROAM_EXP_ENABLE_FLAG             (1 << 0)
#define ROAM_EXP_CFG_PRESENT             (1 << 1)

typedef struct wl_roam_exp_cfg {
	uint16 version;
	uint16 flags;
	wlc_roam_exp_params_t params;
} wl_roam_exp_cfg_t;

typedef struct wl_bssid_pref_list {
	struct ether_addr bssid;
	/* Add this to modify rssi */
	int8 rssi_factor;
	int8 flags;
} wl_bssid_pref_list_t;

#define BSSID_PREF_LIST_VERSION_1        1
#define ROAM_EXP_CLEAR_BSSID_PREF        (1 << 0)

typedef struct wl_bssid_pref_cfg {
	uint16 version;
	uint16 flags;
	uint16 count;
	uint16 reserved;
	wl_bssid_pref_list_t bssids[];
} wl_bssid_pref_cfg_t;

#define SSID_WHITELIST_VERSION_1         1

#define ROAM_EXP_CLEAR_SSID_WHITELIST    (1 << 0)

/* Roam SSID whitelist, ssids in this list are ok to  */
/* be considered as targets to join when considering a roam */

typedef struct wl_ssid_whitelist {

	uint16 version;
	uint16 flags;

	uint8 ssid_count;
	uint8 reserved[3];
	wlc_ssid_t ssids[];
} wl_ssid_whitelist_t;

#define ROAM_EXP_EVENT_VERSION_1       1

typedef struct wl_roam_exp_event {

	uint16 version;
	uint16 flags;
	wlc_ssid_t cur_ssid;
} wl_roam_exp_event_t;

/** scan params for extended join */
typedef struct wl_join_scan_params {
	uint8 scan_type;		/**< 0 use default, active or passive scan */
	uint8 PAD[3];
	int32 nprobes;			/**< -1 use default, number of probes per channel */
	int32 active_time;		/**< -1 use default, dwell time per channel for
					 * active scanning
					 */
	int32 passive_time;		/**< -1 use default, dwell time per channel
					 * for passive scanning
					 */
	int32 home_time;		/**< -1 use default, dwell time for the home channel
					 * between channel scans
					 */
} wl_join_scan_params_t;

/** scan params v1 for extended join */
typedef struct wl_join_scan_params_v1 {
	uint8 scan_type;		/**< 0 use default, active or passive scan */
	uint8 ml_scan_mode;		/* 0 ==> scan ml channels reported in rnr
					 * 1 ==> scan only the provided channels
					 */
	uint8 PAD[2];
	int32 nprobes;			/**< -1 use default, number of probes per channel */
	int32 active_time;		/**< -1 use default, dwell time per channel for
					 * active scanning
					 */
	int32 passive_time;		/**< -1 use default, dwell time per channel
					 * for passive scanning
					 */
	int32 home_time;		/**< -1 use default, dwell time for the home channel
					 * between channel scans
					 */
} wl_join_scan_params_v1_t;

/** scan params v3 for extended join
 * removed the ml scan mode as it is not required
 */
typedef struct wl_join_scan_params_v3 {
	uint8 scan_type;		/**< 0 use default, active or passive scan */
	uint8 PAD[3];
	int32 nprobes;			/**< -1 use default, number of probes per channel */
	int32 active_time;		/**< -1 use default, dwell time per channel for
					 * active scanning
					 */
	int32 passive_time;		/**< -1 use default, dwell time per channel
					 * for passive scanning
					 */
	int32 home_time;		/**< -1 use default, dwell time for the home channel
					 * between channel scans
					 */
} wl_join_scan_params_v3_t;

#define wl_join_assoc_params_t wl_assoc_params_t
#define wl_join_assoc_params_v1_t wl_assoc_params_v1_t
#define wl_join_assoc_params_v2_t wl_assoc_params_v2_t
#define wl_join_assoc_params_v3_t wl_assoc_params_v3_t
/** extended join params */
typedef struct wl_extjoin_params {
	wlc_ssid_t ssid;                /**< {0, ""}: wildcard scan */
	wl_join_scan_params_t scan;
	wl_join_assoc_params_t assoc;   /**< optional field, but it must include the fixed portion
					  * of the wl_join_assoc_params_t struct when it does
					  * present.
					  */
} wl_extjoin_params_t;

typedef struct wl_extjoin_params_v1 {
	uint16 version;
	uint16 PAD;
	wlc_ssid_t ssid;		/**< {0, ""}: wildcard scan */
	wl_join_scan_params_t scan;
	wl_join_assoc_params_v1_t assoc; /**< optional field, but it must include the fixed portion
					 * of the wl_join_assoc_params_t struct when it does
					 * present.
					 */
} wl_extjoin_params_v1_t;

typedef struct wl_extjoin_params_v2 {
	uint16				version;
	uint16				PAD;
	wlc_ssid_t			ssid;		/**< {0, ""}: wildcard scan */
	wl_join_scan_params_v1_t	scan;
	wl_join_assoc_params_v2_t	assoc; /**< optional field, but it must include the fixed
						* portion of the wl_join_assoc_params_t struct
						* when it does present.
						*/
} wl_extjoin_params_v2_t;

typedef struct wl_extjoin_params_v3 {
	uint16				version;
	uint16				PAD;
	wlc_ssid_t			ssid;		/**< {0, ""}: wildcard scan */
	wl_join_scan_params_v3_t	scan;
	wl_join_assoc_params_v3_t	assoc; /**< optional field, but it must include the fixed
						* portion of the wl_join_assoc_params_t struct
						* when it does present.
						*/
} wl_extjoin_params_v3_t;

#define WL_EXTJOIN_PARAMS_FIXED_SIZE	(OFFSETOF(wl_extjoin_params_t, assoc) + \
					 WL_JOIN_ASSOC_PARAMS_FIXED_SIZE)
#define WL_EXTJOIN_PARAMS_FIXED_SIZE_V1    (OFFSETOF(wl_extjoin_params_v1_t, assoc) + \
		                         WL_JOIN_ASSOC_PARAMS_FIXED_SIZE_V1)
#define WL_EXTJOIN_PARAMS_FIXED_SIZE_V2    (OFFSETOF(wl_extjoin_params_v2_t, assoc) + \
		                         WL_JOIN_ASSOC_PARAMS_FIXED_SIZE_V2)
#define WL_EXTJOIN_PARAMS_FIXED_SIZE_V3    (OFFSETOF(wl_extjoin_params_v3_t, assoc) + \
		                         WL_JOIN_ASSOC_PARAMS_FIXED_SIZE_V3)
#define ANT_SELCFG_MAX		4	/**< max number of antenna configurations */
#define MAX_STREAMS_SUPPORTED	4	/**< max number of streams supported */
typedef struct {
	uint8 ant_config[ANT_SELCFG_MAX];	/**< antenna configuration */
	uint8 num_antcfg;			/**< number of available antenna configurations */
} wlc_antselcfg_t;

#define WIFI_RADIO_STAT_VERSION_1  (1u)
#define WIFI_RADIO_STAT_VERSION_2  (2u)

typedef enum wl_radiostats_slice_index {
	WL_RADIOSTAT_SLICE_INDEX_MAIN = 0u,
	WL_RADIOSTAT_SLICE_INDEX_AUX  = 1u,
	WL_RADIOSTAT_SLICE_INDEX_SCAN = 2u,
	WL_RADIOSTAT_SLICE_INDEX_MAX  = 3u
} wl_radiostats_slice_index_t;

#define WIFI_RADIO_STAT_FIXED_LEN	OFFSETOF(wifi_radio_stat, channels)

typedef struct wifi_radio_stat_v1 {
	uint16 version;
	uint16 length;
	uint32 radio;
	uint32 on_time;
	uint32 tx_time;
	uint32 rx_time;
	uint32 on_time_scan;
	uint32 on_time_nbd;
	uint32 on_time_gscan;
	uint32 on_time_roam_scan;
	uint32 on_time_pno_scan;
	uint32 on_time_hs20;
	uint32 num_channels;
	uint8 channels[];
} wifi_radio_stat_v1_t;

typedef struct wifi_radio_stat_v2 {
	uint16 version;
	uint16 length;
	uint32 radio;
	uint32 on_time;
	uint32 tx_time;
	uint32 rx_time;
	uint32 on_time_scan;
	uint32 on_time_nbd;
	uint32 on_time_gscan;
	uint32 on_time_roam_scan;
	uint32 on_time_pno_scan;
	uint32 on_time_hs20;
	uint32 myrx_time;
	uint32 num_channels;
	uint8 channels[];
} wifi_radio_stat_v2_t;

/* per rate statistics */
#define WLC_LINKSTATS_RATESTATS_V1	1u

/* wifi rate */
typedef struct wlc_link_stats_wifi_rate_v1 {
	uint32 preamble;	/* 0: OFDM, 1:CCK, 2:HT 3:VHT 4:HE 5:EHT 6..7 reserved */
	uint32 nss;		/* 0:1x1, 1:2x2, 3:3x3, 4:4x4 */
	uint32 bw;		/* 0:20MHz, 1:40Mhz, 2:80Mhz, 3:160Mhz */
	uint32 rateMcsIdx;	/* OFDM/CCK rate code would be as per ieee std
				 * in the units of 0.5mbps
				 * For HT/VHT/HE/EHT, it would be mcs index
				 */
	uint32 reserved;	/* reserved */
	uint32 bitrate;		/* units of 100 Kbps */
} wlc_link_stats_wifi_rate_v1_t;

typedef struct wlc_link_stats_wifi_rate_stat_v1 {
	uint16 version;
	uint16 length;
	uint32 tx_mpdu;        /* number of successfully transmitted data pkts (ACK rcvd) */
	uint32 rx_mpdu;        /* number of received data pkts */
	uint32 mpdu_lost;      /* number of data packet losses (no ACK) */
	uint32 retries;        /* total number of data pkt retries */
	uint32 retries_short;  /* number of short data pkt retries */
	uint32 retries_long;   /* number of long data pkt retries */
	wlc_link_stats_wifi_rate_v1_t rate; /* rate info */
} wlc_link_stats_wifi_rate_stat_v1_t;

/* This is obsolete.Pls add new fields by extending versioned structure.
 * cca_congest_ext_vX_t [X is latest version]
 */
typedef struct cca_congest {
	uint32 duration;		/**< millisecs spent sampling this channel */
	union {
		uint32 congest_ibss;	/**< millisecs in our bss (presumably this traffic will */
					/**<  move if cur bss moves channels) */
		uint32 congest_me;	/**< millisecs in my own traffic */
	};
	union {
		uint32 congest_obss;	/**< traffic not in our bss */
		uint32 congest_notme;	/**< traffic not from/to me (including bc/mc) */
	};
	uint32 interference;		/**< millisecs detecting a non 802.11 interferer. */
	uint32 timestamp;		/**< second timestamp */
} cca_congest_t;

/* This is obsolete.Pls add new fields by extending versioned structure.
 * cca_congest_ext_channel_req_vX_t [X is latest version]
 */
typedef struct cca_congest_channel_req {
	chanspec_t chanspec;			/**< Which channel? */
	uint16 num_secs;			/**< How many secs worth of data */
	cca_congest_t	secs[BCM_FLEX_ARRAY];	/**< Data */
} cca_congest_channel_req_t;

typedef struct cca_congest_ext {
	uint32 timestamp;		/**< second timestamp */

	/* Base structure of cca_congest_t: CCA statistics all inclusive */
	uint32 duration;		/**< millisecs spent sampling this channel */
	uint32 congest_meonly;		/**< millisecs in my own traffic (TX + RX) */
	uint32 congest_ibss;		/**< millisecs in our bss (presumably this traffic will */
					/**<  move if cur bss moves channels) */
	uint32 congest_obss;		/**< traffic not in our bss */
	uint32 interference;		/**< millisecs detecting a non 802.11 interferer. */

	/* CCA statistics for non PM only */
	uint32 duration_nopm;		/**< millisecs spent sampling this channel */
	uint32 congest_meonly_nopm;	/**< millisecs in my own traffic (TX + RX) */
	uint32 congest_ibss_nopm;	/**< millisecs in our bss (presumably this traffic will */
					/**<  move if cur bss moves channels) */
	uint32 congest_obss_nopm;	/**< traffic not in our bss */
	uint32 interference_nopm;	/**< millisecs detecting a non 802.11 interferer. */

	/* CCA statistics for during PM only */
	uint32 duration_pm;		/**< millisecs spent sampling this channel */
	uint32 congest_meonly_pm;	/**< millisecs in my own traffic (TX + RX) */
	uint32 congest_ibss_pm;		/**< millisecs in our bss (presumably this traffic will */
					/**<  move if cur bss moves channels) */
	uint32 congest_obss_pm;		/**< traffic not in our bss */
	uint32 interference_pm;		/**< millisecs detecting a non 802.11 interferer. */
} cca_congest_ext_t;

typedef struct cca_congest_ext_v2 {
	uint32 timestamp;		/**< second timestamp */

	/* Base structure of cca_congest_t: CCA statistics all inclusive */
	uint32 duration;		/**< millisecs spent sampling this channel */
	uint32 congest_meonly;		/**< millisecs in my own traffic (TX + RX) */
	uint32 congest_ibss;		/**< millisecs in our bss (presumably this traffic will */
					/**<  move if cur bss moves channels) */
	uint32 congest_obss;		/**< traffic not in our bss */
	uint32 interference;		/**< millisecs detecting a non 802.11 interferer. */

	/* CCA statistics for non PM only */
	uint32 duration_nopm;		/**< millisecs spent sampling this channel */
	uint32 congest_meonly_nopm;	/**< millisecs in my own traffic (TX + RX) */
	uint32 congest_ibss_nopm;	/**< millisecs in our bss (presumably this traffic will */
					/**<  move if cur bss moves channels) */
	uint32 congest_obss_nopm;	/**< traffic not in our bss */
	uint32 interference_nopm;	/**< millisecs detecting a non 802.11 interferer. */

	/* CCA statistics for during PM only */
	uint32 duration_pm;		/**< millisecs spent sampling this channel */
	uint32 congest_meonly_pm;	/**< millisecs in my own traffic (TX + RX) */
	uint32 congest_ibss_pm;		/**< millisecs in our bss (presumably this traffic will */
					/**<  move if cur bss moves channels) */
	uint32 congest_obss_pm;		/**< traffic not in our bss */
	uint32 interference_pm;		/**< millisecs detecting a non 802.11 interferer. */
	uint32 radio_on_time;		/* Awake time on this channel */
	uint32 cca_busy_time;		/* CCA is held busy on this channel */
} cca_congest_ext_v2_t;

#define WL_CCA_EXT_REQ_VER		0u
#define WL_CCA_EXT_REQ_VER_V2		2u
#define WL_CCA_EXT_REQ_VER_V3		3u
#define WL_CCA_EXT_REQ_VER_V4		4u

typedef struct cca_congest_ext_channel_req {
	uint16 ver;					/**< version of this struct */
	uint16 len;					/**< len of this structure */
	chanspec_t chanspec;				/**< Which channel? */
	uint16 num_secs;				/**< How many secs worth of data */
	struct cca_congest_ext secs[BCM_FLEX_ARRAY];	/**< Data - 3 sets for ALL - non-PM - PM */
} cca_congest_ext_channel_req_t;

typedef struct cca_congest_ext_channel_req_v2 {
	uint16 ver;					/**< version of this struct */
	uint16 len;					/**< len of this structure */
	chanspec_t chanspec;				/**< Which channel? */
	uint16 num_secs;				/* How many secs worth of data */
	cca_congest_ext_v2_t secs[1];	/* Data - 3 sets for ALL - non-PM - PM */
} cca_congest_ext_channel_req_v2_t;

/* Struct holding all channels cca statistics V3 */
typedef struct cca_congest_ext_channel_req_v3 {
	uint16			ver;
	uint16			len;
	uint8			PAD[2];
	uint16			num_of_entries;
	cca_congest_ext_channel_req_v2_t	per_chan_stats[BCM_FLEX_ARRAY];
} cca_congest_ext_channel_req_v3_t;

#define BANDMASK_2G		BCM_BIT(0)
#define BANDMASK_5G		BCM_BIT(1)
#define BANDMASK_6G		BCM_BIT(2)
/* Struct holding all channels cca statistics V4 */
typedef struct cca_congest_ext_channel_req_v4 {
	uint16			ver;
	uint16			len;
	uint8			bandmask; /* BIT(0)-2G BIT(1)-5G  BIT(2)-6G */
	uint8			PAD[1];
	uint16			num_of_entries;
	cca_congest_ext_channel_req_v2_t	per_chan_stats[BCM_FLEX_ARRAY];
} cca_congest_ext_channel_req_v4_t;

typedef struct {
	uint32 duration;	/**< millisecs spent sampling this channel */
	uint32 congest;		/**< millisecs detecting busy CCA */
	uint32 timestamp;	/**< second timestamp */
} cca_congest_simple_t;

// ED: Energy Detection Thresholds
#define PHY_EDTHRESH_MAX_COUNT 2u /* WLC_API_VERSION_MAJOR >= 17u */

// Desense Reasons
#define BPHY_DESENSE_ACI_MASK (1 << 0u) // Desense due to GBD
#define OFDM_DESENSE_ACI_MASK (1 << 1u)
#define BPHY_DESENSE_BTC_MASK (1 << 2u) // Desense due to BT Coex operation
#define OFDM_DESENSE_BTC_MASK (1 << 3u)
#define BPHY_DESENSE_LTE_MASK (1 << 4u) // Desense due to LTE Coex operation
#define OFDM_DESENSE_LTE_MASK (1 << 5u)
#define DESENSE_ON_LTE_MASK   (1 << 6u)  // Desense due to InitGain or CRS desense
#define DESENSE_ON_BTC_MASK   (1 << 7u)

typedef struct {
	int32 ofdm_desense;
	int32 bphy_desense;
	uint32 reason;		/**< bit flags for desense reasons */
} wl_phy_rxdesense_t;

/* The following two structure must have same first 4 fields.
 * The cca_chan_qual_event_t is used to report CCA in older formats and NF.
 * The cca_only_chan_qual_event_t is used to report CCA only with newer format.
 */
typedef struct {
	uint16 status;					/**< misc status */
	uint16 id;					/**< sub-event ID */
	chanspec_t chanspec;				/**< Which channel? */
	uint16 len;					/**< number of bytes to follow */
	union {
		cca_congest_simple_t  cca_busy;		/**< CCA busy */
		cca_congest_t cca_busy_ext;		/**< Extended CCA report */
		int32 noise;				/**< noise floor */
	};
} cca_chan_qual_event_t;

typedef struct {
	uint16 status;					/**< misc status */
	uint16 id;					/**< sub-event ID */
	chanspec_t chanspec;				/**< Which channel? */
	uint16 len;					/**< number of bytes to follow */
	union {
		cca_congest_simple_t  cca_busy;		/**< CCA busy */
		struct {
			cca_congest_t cca_busy_ext;	/**< Extended CCA report */
			cca_congest_t cca_busy_nopm;	/**< Extedned CCA report (PM awake time) */
			cca_congest_t cca_busy_pm;	/**< Extedned CCA report (PM sleep time) */
		};
	};
} cca_only_chan_qual_event_t;

typedef struct {
	uint16 status;					/**< misc status */
	uint16 id;					/**< sub-event ID */
	/* id is used to indicate the number of bytes to read */
	chanspec_t chanspec;				/**< Which channel? */
	uint16 len;					/**< number of bytes to follow */
	union {
		cca_congest_simple_t  cca_busy;		/**< CCA busy */
		struct {
			cca_congest_t cca_busy_ext;	/**< Extended CCA report */
			cca_congest_t cca_busy_nopm;	/**< Extedned CCA report (PM awake time) */
			cca_congest_t cca_busy_pm;	/**< Extedned CCA report (PM sleep time) */
		};
	};
	int32 ofdm_desense;
} cca_only_chan_qual_event_v2_t;

typedef struct {
	uint16 status;					/**< misc status */
	uint16 id;					/**< sub-event ID */
	chanspec_t chanspec;				/**< Which channel? */
	uint16 len;					/**< number of bytes to follow */
	cca_congest_t cca_busy_ext;			/**< Extended CCA report */
	cca_congest_t cca_busy_nopm;			/**< Extedned CCA report (PM awake time) */
	cca_congest_t cca_busy_pm;			/**< Extedned CCA report (PM sleep time) */
	wl_phy_rxdesense_t desense;			/**< PHY RX desense states & reasons */
} cca_only_chan_qual_event_v3_t;

typedef struct {
	uint32 msrmnt_time;	/**< Time for Measurement (msec) */
	uint32 msrmnt_done;	/**< flag set when measurement complete */
	char buf[];
} cca_stats_n_flags;

typedef struct {
	uint32 msrmnt_query;    /* host to driver query for measurement done */
	uint32 time_req;        /* time required for measurement */
	uint8 report_opt;       /* option to print different stats in report */
	uint8 PAD[3];
} cca_msrmnt_query;

/* interference sources */
enum interference_source {
	ITFR_NONE = 0,			/**< interference */
	ITFR_PHONE,			/**< wireless phone */
	ITFR_VIDEO_CAMERA,		/**< wireless video camera */
	ITFR_MICROWAVE_OVEN,		/**< microwave oven */
	ITFR_BABY_MONITOR,		/**< wireless baby monitor */
	ITFR_BLUETOOTH,			/**< bluetooth */
	ITFR_VIDEO_CAMERA_OR_BABY_MONITOR,	/**< wireless camera or baby monitor */
	ITFR_BLUETOOTH_OR_BABY_MONITOR,	/**< bluetooth or baby monitor */
	ITFR_VIDEO_CAMERA_OR_PHONE,	/**< video camera or phone */
	ITFR_UNIDENTIFIED		/**< interference from unidentified source */
};

/** structure for interference source report */
typedef struct {
	uint32 flags;		/**< flags.  bit definitions below */
	uint32 source;		/**< last detected interference source */
	uint32 timestamp;	/**< second timestamp on interferenced flag change */
} interference_source_rep_t;

#define WLC_CNTRY_BUF_SZ	4		/**< Country string is 3 bytes + NUL */

typedef struct wl_country {
	char country_abbrev[WLC_CNTRY_BUF_SZ];	/**< nul-terminated country code used in
						 * the Country IE
						 */
	int32 rev;				/**< revision specifier for ccode
						 * on set, -1 indicates unspecified.
						 * on get, rev >= 0
						 */
	char ccode[WLC_CNTRY_BUF_SZ];		/**< nul-terminated built-in country code.
						 * variable length, but fixed size in
						 * struct allows simple allocation for
						 * expected country strings <= 3 chars.
						 */
} wl_country_t;

#define CCODE_INFO_VERSION_1	 1

typedef enum wl_ccode_role {
	WLC_CCODE_ROLE_ACTIVE = 0,
	WLC_CCODE_ROLE_HOST,
	WLC_CCODE_ROLE_80211D_ASSOC,
	WLC_CCODE_ROLE_80211D_SCAN,
	WLC_CCODE_ROLE_DEFAULT,
	WLC_CCODE_ROLE_DEFAULT_SROM_BKUP,
	WLC_CCODE_LAST
} wl_ccode_role_t;
#define WLC_NUM_CCODE_INFO WLC_CCODE_LAST

typedef struct wl_ccode_entry {
	uint16 reserved;
	uint8 band;
	uint8 role;
	char	ccode[WLC_CNTRY_BUF_SZ];
} wl_ccode_entry_t;

typedef struct wl_ccode_info {
	uint16 version;
	uint16 count;   /**< Number of ccodes entries in the set */
	wl_ccode_entry_t ccodelist[BCM_FLEX_ARRAY];
} wl_ccode_info_t;
#define WL_CCODE_INFO_FIXED_LEN	OFFSETOF(wl_ccode_info_t, ccodelist)
typedef struct wl_channels_in_country {
	uint32 buflen;
	uint32 band;
	char country_abbrev[WLC_CNTRY_BUF_SZ];
	uint32 count;
	uint32 channel[BCM_FLEX_ARRAY];
} wl_channels_in_country_t;

typedef struct wl_country_list {
	uint32 buflen;
	uint32 band_set;
	uint32 band;
	uint32 count;
	char country_abbrev[BCM_FLEX_ARRAY];
} wl_country_list_t;

typedef struct wl_rm_req_elt {
	int8	type;
	int8	flags;
	chanspec_t	chanspec;
	uint32	token;		/**< token for this measurement */
	uint32	tsf_h;		/**< TSF high 32-bits of Measurement start time */
	uint32	tsf_l;		/**< TSF low 32-bits */
	uint32	dur;		/**< TUs */
} wl_rm_req_elt_t;

typedef struct wl_rm_req {
	uint32	token;				/**< overall measurement set token */
	uint32	count;				/**< number of measurement requests */
	void	*cb;				/**< completion callback function: may be NULL */
	void	*cb_arg;			/**< arg to completion callback function */
	wl_rm_req_elt_t	req[BCM_FLEX_ARRAY];	/**< variable length block of requests */
} wl_rm_req_t;
#define WL_RM_REQ_FIXED_LEN	OFFSETOF(wl_rm_req_t, req)

typedef struct wl_rm_rep_elt {
	int8	type;
	int8	flags;
	chanspec_t	chanspec;
	uint32	token;			/**< token for this measurement */
	uint32	tsf_h;			/**< TSF high 32-bits of Measurement start time */
	uint32	tsf_l;			/**< TSF low 32-bits */
	uint32	dur;			/**< TUs */
	uint32	len;			/**< byte length of data block */
	uint8	data[1];		/**< variable length data block */
} wl_rm_rep_elt_t;
#define WL_RM_REP_ELT_FIXED_LEN	24	/**< length excluding data block */

#define WL_RPI_REP_BIN_NUM 8
typedef struct wl_rm_rpi_rep {
	uint8	rpi[WL_RPI_REP_BIN_NUM];
	int8	rpi_max[WL_RPI_REP_BIN_NUM];
} wl_rm_rpi_rep_t;

typedef struct wl_rm_rep {
	uint32	token;		/**< overall measurement set token */
	uint32	len;		/**< length of measurement report block */
	wl_rm_rep_elt_t rep[BCM_FLEX_ARRAY];
} wl_rm_rep_t;
#define WL_RM_REP_FIXED_LEN	8

#ifdef BCMCCX
#define LEAP_USER_MAX		32
#define LEAP_DOMAIN_MAX		32
#define LEAP_PASSWORD_MAX	32

typedef struct wl_leap_info {
	wlc_ssid_t ssid;
	uint8 user_len;
	uint8 user[LEAP_USER_MAX];
	uint8 password_len;
	uint8 password[LEAP_PASSWORD_MAX];
	uint8 domain_len;
	uint8 domain[LEAP_DOMAIN_MAX];
	uint8 PAD;
} wl_leap_info_t;

typedef struct wl_leap_list {
	uint32 buflen;
	uint32 version;
	uint32 count;
	wl_leap_info_t leap_info[BCM_FLEX_ARRAY];
} wl_leap_list_t;
#endif	/* BCMCCX */

#define WL_SUP_IOV_VERSION_1       0x0001u
enum {
	WL_SUP_CMD_NONE			= 0,
	/* Get SUP IOVAR bersion */
	WL_SUP_CMD_GET_VERSION		= 1u,
	/* IOVAR to send GTK rekey request */
	WL_SUP_CMD_SEND_GTK_REQ		= 2u,
	/* Set/Get skip GTK M1 processing state */
	WL_SUP_CMD_IGNORE_GTK_M1	= 3u
};

typedef uint16 wl_sup_cmd_t;

typedef struct wl_sup_iov_v1 {
	uint16			version;	/* structure version will be incremented
						 * when header is changed.
						 */
	uint16			len;		/* data field lenth. */
	wl_sup_cmd_t		cmd;		/* sub-command id. */
	uint8			data[];		/* variable */
} wl_sup_iov_v1_t;

#define WLC_SUP_TD_POLICY_XTLV_ID		0x1u
#define WLC_SUP_TD_POLICY_XTLV_ELEM_SIZE	0x4u	/* 4B aligned */
#define WLC_SUP_TD_POLICY_XTLV_SIZE	(BCM_XTLV_HDR_SIZE + WLC_SUP_TD_POLICY_XTLV_ELEM_SIZE)

typedef struct wl_wsec_key {
	uint32		index;		/**< key index */
	uint32		len;		/**< key length */
	uint8		data[DOT11_MAX_KEY_SIZE];	/**< key data */
	uint32		PAD[18];
	uint32		algo;		/**< CRYPTO_ALGO_AES_CCM, CRYPTO_ALGO_WEP128, etc */
	uint32		flags;		/**< misc flags */
	uint32		PAD[2];
	int32		PAD;
	int32		iv_initialized;	/**< has IV been initialized already? */
	int32		PAD;
	/* Rx IV */
	struct {
		uint32	hi;		/**< upper 32 bits of IV */
		uint16	lo;		/**< lower 16 bits of IV */
		uint16	PAD;
	} rxiv;
	uint32		PAD[2];
	struct ether_addr ea;		/**< per station */
	uint16	    PAD;
} wl_wsec_key_t;

/* Min length for PSK passphrase */
#define WSEC_MIN_PSK_LEN	8
/* Max length of supported passphrases for PSK */
#define WSEC_MAX_PSK_LEN	64
/* Max length of supported passphrases for SAE */
#define WSEC_MAX_PASSPHRASE_LEN 256u
/* Max length of SAE password ID */
#define WSEC_MAX_SAE_PASSWORD_ID 255u

/* Flag for key material needing passhash'ing */
#define WSEC_PASSPHRASE		1u
/* Flag indicating an SAE passphrase */
#define WSEC_SAE_PASSPHRASE	2u

/**receptacle for WLC_SET_WSEC_PMK parameter */

typedef struct wsec_pmk {
	uint16  key_len;			/* octets in key material */
	uint16  flags;				/* key handling qualification */
	uint8	key[WSEC_MAX_PASSPHRASE_LEN];	/* PMK material */
	uint16	opt_len;			/* optional field length */
	uint8	opt_tlvs[BCM_FLEX_ARRAY];	/* optional filed in bcm_xtlv_t format */
} wsec_pmk_t;

typedef enum {
	WL_PMK_TLV_PASSWORD_ID  = 1,
	WL_PMK_TLV_SSID		= 2,
	WL_PMK_TLV_BSSID	= 3
} wl_pmk_tlv_types_t;

#define WL_AUTH_EVENT_DATA_V1		0x1
#define WL_AUTH_EVENT_DATA_V2		0x2

/* tlv ids for auth event */
#define WL_AUTH_PMK_TLV_ID		1u
#define WL_AUTH_PMKID_TLV_ID		2u
#define WL_AUTH_PMKID_TYPE_TLV_ID	3u
#define WL_AUTH_SSID_TLV_ID		4u

#define WL_AUTH_PMKID_TYPE_BSSID	1u
#define WL_AUTH_PMKID_TYPE_SSID		2u
/* AUTH event data
* pmk and pmkid in case of SAE auth
* xtlvs will be 32 bit alligned
*/
typedef struct wl_auth_event {
	uint16 version;
	uint16 length;
	uint8 xtlvs[];
} wl_auth_event_t;

#define WL_AUTH_EVENT_FIXED_LEN_V1	OFFSETOF(wl_auth_event_t, xtlvs)
#define WL_AUTH_EVENT_FIXED_LEN_V2	OFFSETOF(wl_auth_event_t, xtlvs)

#define WL_PMKSA_EVENT_DATA_V1	1u

/* tlv ids for PMKSA event */
#define WL_PMK_TLV_ID		1u
#define WL_PMKID_TLV_ID		2u
#define WL_PEER_ADDR_TLV_ID	3u

/* PMKSA event data structure */
typedef struct wl_pmksa_event {
	uint16 version;
	uint16 length;
	uint8 xtlvs[];
} wl_pmksa_event_t;

#define WL_PMKSA_EVENT_FIXED_LEN_V1	OFFSETOF(wl_pmksa_event_t, xtlvs)

#define FILS_CACHE_ID_LEN	2u
#define PMK_LEN_MAX		48u

typedef struct _pmkid_v1 {
	struct ether_addr	BSSID;
	uint8			PMKID[WPA2_PMKID_LEN];
} pmkid_v1_t;

#define PMKID_ELEM_V2_LENGTH (sizeof(struct ether_addr) + WPA2_PMKID_LEN + PMK_LEN_MAX + \
	sizeof(ssid_info_t) + FILS_CACHE_ID_LEN)

typedef struct _pmkid_v2 {
	uint16			length; /* Should match PMKID_ELEM_VX_LENGTH */
	struct ether_addr	BSSID;
	uint8			PMKID[WPA2_PMKID_LEN];
	uint8			pmk[PMK_LEN_MAX]; /* for FILS key deriviation */
	uint16			pmk_len;
	ssid_info_t		ssid;
	uint8			fils_cache_id[FILS_CACHE_ID_LEN];
	uint8			PAD;
} pmkid_v2_t;

#define PMKID_LIST_VER_2	2

typedef struct _pmkid_v3 {
	struct ether_addr	bssid;
	uint8			pmkid[WPA2_PMKID_LEN];
	uint8			pmkid_len;
	uint8			pmk[PMK_LEN_MAX];
	uint8			pmk_len;
	uint16			fils_cache_id; /* 2-byte length */
	uint8			akm;
	uint8			ssid_len;
	uint8			ssid[DOT11_MAX_SSID_LEN]; /* For FILS, to save ESSID */
							  /* one pmkid used in whole ESS */
	uint32			time_left; /* remaining time until expirary in sec. */
					   /* 0 means expired, all 0xFF means never expire */
} pmkid_v3_t;

#define PMKID_LIST_VER_3	3
typedef struct _pmkid_list_v1 {
	uint32	npmkid;
	pmkid_v1_t	pmkid[BCM_FLEX_ARRAY];
} pmkid_list_v1_t;

typedef struct _pmkid_list_v2 {
	uint16  version;
	uint16	length;
	pmkid_v2_t	pmkid[BCM_FLEX_ARRAY];
} pmkid_list_v2_t;

#define PMKDB_SET_IOVAR		0x0001u
#define PMKDB_GET_IOVAR		0x0002u
#define PMKDB_CLEAR_IOVAR	0x0004u
#define PMKDB_ALL_CFGS		0x0008u /* option for PMKDB_GET_IOVAR */

typedef struct _pmkid_list_v3 {
	uint16		version;
	uint16		length;
	uint16		count;
	uint16		flag;
	pmkid_v3_t	pmkid[];
} pmkid_list_v3_t;


typedef struct _pmkid_cand {
	struct ether_addr	BSSID;
	uint8			preauth;
} pmkid_cand_t;

typedef struct _pmkid_cand_list {
	uint32	npmkid_cand;
	pmkid_cand_t	pmkid_cand[BCM_FLEX_ARRAY];
} pmkid_cand_list_t;

#define WL_STA_ANT_MAX		4	/**< max possible rx antennas */

typedef struct wl_assoc_info {
	uint32		req_len;
	uint32		resp_len;
	uint32		flags;
	struct dot11_assoc_req req;
	struct ether_addr reassoc_bssid; /**< used in reassoc's */
	struct dot11_assoc_resp resp;
	uint32		state;
} wl_assoc_info_t;

/** srom read/write struct passed through ioctl */
typedef struct {
	uint32	byteoff;	/**< byte offset */
	uint32	nbytes;		/**< number of bytes */
	uint16	buf[];
} srom_rw_t;

#define CISH_FLAG_PCIECIS	(1 << 15)	/**< write CIS format bit for PCIe CIS */

/** similar cis (srom or otp) struct [iovar: may not be aligned] */
typedef struct {
	uint16	source;		/**< cis source */
	uint16	flags;		/**< flags */
	uint32	byteoff;	/**< byte offset */
	uint32	nbytes;		/**< number of bytes */
	/* data follows here */
} cis_rw_t;

/** R_REG and W_REG struct passed through ioctl */
typedef struct {
	uint32	byteoff;	/**< byte offset of the field in d11regs_t */
	uint32	val;		/**< read/write value of the field */
	uint32	size;		/**< sizeof the field */
	uint32	band;		/**< band (optional) */
} rw_reg_t;

/**
 * Structure used by GET/SET_ATTEN ioctls - it controls power in b/g-band
 * PCL - Power Control Loop
 */
typedef struct {
	uint16	auto_ctrl;	/**< WL_ATTEN_XX */
	uint16	bb;		/**< Baseband attenuation */
	uint16	radio;		/**< Radio attenuation */
	uint16	txctl1;		/**< Radio TX_CTL1 value */
} atten_t;

/** Per-AC retry parameters */
struct wme_tx_params_s {
	uint8  short_retry;
	uint8  short_fallback;
	uint8  long_retry;
	uint8  long_fallback;
	uint16 max_rate;  /**< In units of 512 Kbps */
};

typedef struct wme_tx_params_s wme_tx_params_t;

#define WL_WME_TX_PARAMS_IO_BYTES (sizeof(wme_tx_params_t) * AC_COUNT)

/**Used to get specific link/ac parameters */
typedef struct {
	int32 ac;
	uint8 val;
	struct ether_addr ea;
	uint8 PAD;
} link_val_t;


#define WL_PM_MUTE_TX_VER 1

typedef struct wl_pm_mute_tx {
	uint16 version;		/**< version */
	uint16 len;		/**< length */
	uint16 deadline;	/**< deadline timer (in milliseconds) */
	uint8  enable;		/**< set to 1 to enable mode; set to 0 to disable it */
	uint8 PAD;
} wl_pm_mute_tx_t;

/*
 * Pay attention to version if structure changes.
 */

/* sta_info_t version 4 */
typedef struct {
	uint16			ver;		/**< version of this struct */
	uint16			len;		/**< length in bytes of this structure */
	uint16			cap;		/**< sta's advertised capabilities */
	uint16			PAD;
	uint32			flags;		/**< flags defined below */
	uint32			idle;		/**< time since data pkt rx'd from sta */
	struct ether_addr	ea;		/**< Station address */
	uint16			PAD;
	wl_rateset_t	rateset;	/**< rateset in use */
	uint32			in;		/**< seconds elapsed since associated */
	uint32			listen_interval_inms; /**< Min Listen interval in ms for this STA */
	uint32			tx_pkts;	/**< # of user packets transmitted (unicast) */
	uint32			tx_failures;	/**< # of user packets failed */
	uint32			rx_ucast_pkts;	/**< # of unicast packets received */
	uint32			rx_mcast_pkts;	/**< # of multicast packets received */
	uint32			tx_rate;	/**< Rate used by last tx frame */
	uint32			rx_rate;	/**< Rate of last successful rx frame */
	uint32			rx_decrypt_succeeds;	/**< # of packet decrypted successfully */
	uint32			rx_decrypt_failures;	/**< # of packet decrypted unsuccessfully */
	uint32			tx_tot_pkts;	/**< # of user tx pkts (ucast + mcast) */
	uint32			rx_tot_pkts;	/**< # of data packets recvd (uni + mcast) */
	uint32			tx_mcast_pkts;	/**< # of mcast pkts txed */
	uint64			tx_tot_bytes;	/**< data bytes txed (ucast + mcast) */
	uint64			rx_tot_bytes;	/**< data bytes recvd (ucast + mcast) */
	uint64			tx_ucast_bytes;	/**< data bytes txed (ucast) */
	uint64			tx_mcast_bytes;	/**< # data bytes txed (mcast) */
	uint64			rx_ucast_bytes;	/**< data bytes recvd (ucast) */
	uint64			rx_mcast_bytes;	/**< data bytes recvd (mcast) */
	int8			rssi[WL_STA_ANT_MAX]; /**< average rssi per antenna
							* of data frames
							*/
	int8			nf[WL_STA_ANT_MAX];	/**< per antenna noise floor */
	uint16			aid;			/**< association ID */
	uint16			ht_capabilities;	/**< advertised ht caps */
	uint16			vht_flags;		/**< converted vht flags */
	uint16			PAD;
	uint32			tx_pkts_retried;	/**< # of frames where a retry was
							 * necessary
							 */
	uint32			tx_pkts_retry_exhausted; /**< # of user frames where a retry
							  * was exhausted
							  */
	int8			rx_lastpkt_rssi[WL_STA_ANT_MAX]; /**< Per antenna RSSI of last
								  * received data frame.
								  */
	/* TX WLAN retry/failure statistics:
	 * Separated for host requested frames and WLAN locally generated frames.
	 * Include unicast frame only where the retries/failures can be counted.
	 */
	uint32			tx_pkts_total;		/**< # user frames sent successfully */
	uint32			tx_pkts_retries;	/**< # user frames retries */
	uint32			tx_pkts_fw_total;	/**< # FW generated sent successfully */
	uint32			tx_pkts_fw_retries;	/**< # retries for FW generated frames */
	uint32			tx_pkts_fw_retry_exhausted;	/**< # FW generated where a retry
								 * was exhausted
								 */
	uint32			rx_pkts_retried;	/**< # rx with retry bit set */
	uint32			tx_rate_fallback;	/**< lowest fallback TX rate */
	/* Fields above this line are common to sta_info_t versions 4 and 5 */

	uint32			rx_dur_total;	/* total user RX duration (estimated) */

	chanspec_t		chanspec;       /** chanspec this sta is on */
	uint16			PAD;
	wl_rateset_args_v1_t	rateset_adv;	/* rateset along with mcs index bitmap */
	uint32			PAD;
} sta_info_v4_t;

/* Note: Version 4 is the latest version of sta_info_t.  Version 5 is abandoned.
 * Please add new fields to version 4, not version 5.
 */
/* sta_info_t version 5 */
typedef struct {
	uint16			ver;		/**< version of this struct */
	uint16			len;		/**< length in bytes of this structure */
	uint16			cap;		/**< sta's advertised capabilities */
	uint16			PAD;
	uint32			flags;		/**< flags defined below */
	uint32			idle;		/**< time since data pkt rx'd from sta */
	struct ether_addr	ea;		/**< Station address */
	uint16			PAD;
	wl_rateset_t		rateset;	/**< rateset in use */
	uint32			in;		/**< seconds elapsed since associated */
	uint32			listen_interval_inms; /**< Min Listen interval in ms for this STA */
	uint32			tx_pkts;	/**< # of user packets transmitted (unicast) */
	uint32			tx_failures;	/**< # of user packets failed */
	uint32			rx_ucast_pkts;	/**< # of unicast packets received */
	uint32			rx_mcast_pkts;	/**< # of multicast packets received */
	uint32			tx_rate;	/**< Rate used by last tx frame */
	uint32			rx_rate;	/**< Rate of last successful rx frame */
	uint32			rx_decrypt_succeeds;	/**< # of packet decrypted successfully */
	uint32			rx_decrypt_failures;	/**< # of packet decrypted unsuccessfully */
	uint32			tx_tot_pkts;	/**< # of user tx pkts (ucast + mcast) */
	uint32			rx_tot_pkts;	/**< # of data packets recvd (uni + mcast) */
	uint32			tx_mcast_pkts;	/**< # of mcast pkts txed */
	uint64			tx_tot_bytes;	/**< data bytes txed (ucast + mcast) */
	uint64			rx_tot_bytes;	/**< data bytes recvd (ucast + mcast) */
	uint64			tx_ucast_bytes;	/**< data bytes txed (ucast) */
	uint64			tx_mcast_bytes;	/**< # data bytes txed (mcast) */
	uint64			rx_ucast_bytes;	/**< data bytes recvd (ucast) */
	uint64			rx_mcast_bytes;	/**< data bytes recvd (mcast) */
	int8			rssi[WL_STA_ANT_MAX]; /**< average rssi per antenna
							* of data frames
							*/
	int8			nf[WL_STA_ANT_MAX];	/**< per antenna noise floor */
	uint16			aid;			/**< association ID */
	uint16			ht_capabilities;	/**< advertised ht caps */
	uint16			vht_flags;		/**< converted vht flags */
	uint16			PAD;
	uint32			tx_pkts_retried;	/**< # of frames where a retry was
							 * necessary
							 */
	uint32			tx_pkts_retry_exhausted; /**< # of user frames where a retry
							  * was exhausted
							  */
	int8			rx_lastpkt_rssi[WL_STA_ANT_MAX]; /**< Per antenna RSSI of last
								  * received data frame.
								  */
	/* TX WLAN retry/failure statistics:
	 * Separated for host requested frames and WLAN locally generated frames.
	 * Include unicast frame only where the retries/failures can be counted.
	 */
	uint32			tx_pkts_total;		/**< # user frames sent successfully */
	uint32			tx_pkts_retries;	/**< # user frames retries */
	uint32			tx_pkts_fw_total;	/**< # FW generated sent successfully */
	uint32			tx_pkts_fw_retries;	/**< # retries for FW generated frames */
	uint32			tx_pkts_fw_retry_exhausted;	/**< # FW generated where a retry
								 * was exhausted
								 */
	uint32			rx_pkts_retried;	/**< # rx with retry bit set */
	uint32			tx_rate_fallback;	/**< lowest fallback TX rate */
	/* Fields above this line are common to sta_info_t versions 4 and 5 */

	chanspec_t		chanspec;       /** chanspec this sta is on */
	uint16			PAD;
	wl_rateset_args_v1_t	rateset_adv;	/* rateset along with mcs index bitmap */
} sta_info_v5_t;

/*
 * Pay attention to version if structure changes.
 */

/* sta_info_t version 6
	changes to wl_rateset_args_t is leading to update this struct version as well.
 */
typedef struct sta_info_v6 {
	uint16			ver;		/**< version of this struct */
	uint16			len;		/**< length in bytes of this structure */
	uint16			cap;		/**< sta's advertised capabilities */
	uint16			PAD;
	uint32			flags;		/**< flags defined below */
	uint32			idle;		/**< time since data pkt rx'd from sta */
	struct ether_addr	ea;		/**< Station address */
	uint16			PAD;
	wl_rateset_t	rateset;	/**< rateset in use */
	uint32			in;		/**< seconds elapsed since associated */
	uint32			listen_interval_inms; /**< Min Listen interval in ms for this STA */
	uint32			tx_pkts;	/**< # of user packets transmitted (unicast) */
	uint32			tx_failures;	/**< # of user packets failed */
	uint32			rx_ucast_pkts;	/**< # of unicast packets received */
	uint32			rx_mcast_pkts;	/**< # of multicast packets received */
	uint32			tx_rate;	/**< Rate used by last tx frame */
	uint32			rx_rate;	/**< Rate of last successful rx frame */
	uint32			rx_decrypt_succeeds;	/**< # of packet decrypted successfully */
	uint32			rx_decrypt_failures;	/**< # of packet decrypted unsuccessfully */
	uint32			tx_tot_pkts;	/**< # of user tx pkts (ucast + mcast) */
	uint32			rx_tot_pkts;	/**< # of data packets recvd (uni + mcast) */
	uint32			tx_mcast_pkts;	/**< # of mcast pkts txed */
	uint64			tx_tot_bytes;	/**< data bytes txed (ucast + mcast) */
	uint64			rx_tot_bytes;	/**< data bytes recvd (ucast + mcast) */
	uint64			tx_ucast_bytes;	/**< data bytes txed (ucast) */
	uint64			tx_mcast_bytes;	/**< # data bytes txed (mcast) */
	uint64			rx_ucast_bytes;	/**< data bytes recvd (ucast) */
	uint64			rx_mcast_bytes;	/**< data bytes recvd (mcast) */
	int8			rssi[WL_STA_ANT_MAX]; /**< average rssi per antenna
							* of data frames
							*/
	int8			nf[WL_STA_ANT_MAX];	/**< per antenna noise floor */
	uint16			aid;			/**< association ID */
	uint16			ht_capabilities;	/**< advertised ht caps */
	uint16			vht_flags;		/**< converted vht flags */
	uint16			PAD;
	uint32			tx_pkts_retried;	/**< # of frames where a retry was
							 * necessary
							 */
	uint32			tx_pkts_retry_exhausted; /**< # of user frames where a retry
							  * was exhausted
							  */
	int8			rx_lastpkt_rssi[WL_STA_ANT_MAX]; /**< Per antenna RSSI of last
								  * received data frame.
								  */
	/* TX WLAN retry/failure statistics:
	 * Separated for host requested frames and WLAN locally generated frames.
	 * Include unicast frame only where the retries/failures can be counted.
	 */
	uint32			tx_pkts_total;		/**< # user frames sent successfully */
	uint32			tx_pkts_retries;	/**< # user frames retries */
	uint32			tx_pkts_fw_total;	/**< # FW generated sent successfully */
	uint32			tx_pkts_fw_retries;	/**< # retries for FW generated frames */
	uint32			tx_pkts_fw_retry_exhausted;	/**< # FW generated where a retry
								 * was exhausted
								 */
	uint32			rx_pkts_retried;	/**< # rx with retry bit set */
	uint32			tx_rate_fallback;	/**< lowest fallback TX rate */
	/* Fields above this line are common to sta_info_t versions 4 and 5 */

	uint32			rx_dur_total;	/* total user RX duration (estimated) */

	chanspec_t		chanspec;       /** chanspec this sta is on */
	uint16			PAD;
	wl_rateset_args_v2_t	rateset_adv;	/* rateset along with mcs index bitmap */
} sta_info_v6_t;

/*
 * Pay attention to version if structure changes.
 */

/* sta_info_t version 7
	changes to wl_rateset_args_t is leading to update this struct version as well.
 */
typedef struct sta_info_v7 {
	uint16			ver;		/**< version of this struct */
	uint16			len;		/**< length in bytes of this structure */
	uint16			cap;		/**< sta's advertised capabilities */
	uint16			PAD;
	uint32			flags;		/**< flags defined below */
	uint32			idle;		/**< time since data pkt rx'd from sta */
	struct ether_addr	ea;		/**< Station address */
	uint16			PAD;
	wl_rateset_t		rateset;	/**< rateset in use */
	uint32			in;		/**< seconds elapsed since associated */
	uint32			listen_interval_inms; /**< Min Listen interval in ms for this STA */
	uint32			tx_pkts;	/**< # of user packets transmitted (unicast) */
	uint32			tx_failures;	/**< # of user packets failed */
	uint32			rx_ucast_pkts;	/**< # of unicast packets received */
	uint32			rx_mcast_pkts;	/**< # of multicast packets received */
	uint32			tx_rate;	/**< Rate used by last tx frame */
	uint32			rx_rate;	/**< Rate of last successful rx frame */
	uint32			rx_decrypt_succeeds;	/**< # of packet decrypted successfully */
	uint32			rx_decrypt_failures;	/**< # of packet decrypted unsuccessfully */
	uint32			tx_tot_pkts;	/**< # of user tx pkts (ucast + mcast) */
	uint32			rx_tot_pkts;	/**< # of data packets recvd (uni + mcast) */
	uint32			tx_mcast_pkts;	/**< # of mcast pkts txed */
	uint64			tx_tot_bytes;	/**< data bytes txed (ucast + mcast) */
	uint64			rx_tot_bytes;	/**< data bytes recvd (ucast + mcast) */
	uint64			tx_ucast_bytes;	/**< data bytes txed (ucast) */
	uint64			tx_mcast_bytes;	/**< # data bytes txed (mcast) */
	uint64			rx_ucast_bytes;	/**< data bytes recvd (ucast) */
	uint64			rx_mcast_bytes;	/**< data bytes recvd (mcast) */
	int8			rssi[WL_STA_ANT_MAX]; /**< average rssi per antenna
							* of data frames
							*/
	int8			nf[WL_STA_ANT_MAX];	/**< per antenna noise floor */
	uint16			aid;			/**< association ID */
	uint16			ht_capabilities;	/**< advertised ht caps */
	uint16			vht_flags;		/**< converted vht flags */
	uint16			PAD;
	uint32			tx_pkts_retried;	/**< # of frames where a retry was
							 * necessary
							 */
	uint32			tx_pkts_retry_exhausted; /**< # of user frames where a retry
							  * was exhausted
							  */
	int8			rx_lastpkt_rssi[WL_STA_ANT_MAX]; /**< Per antenna RSSI of last
								  * received data frame.
								  */
	/* TX WLAN retry/failure statistics:
	 * Separated for host requested frames and WLAN locally generated frames.
	 * Include unicast frame only where the retries/failures can be counted.
	 */
	uint32			tx_pkts_total;		/**< # user frames sent successfully */
	uint32			tx_pkts_retries;	/**< # user frames retries */
	uint32			tx_pkts_fw_total;	/**< # FW generated sent successfully */
	uint32			tx_pkts_fw_retries;	/**< # retries for FW generated frames */
	uint32			tx_pkts_fw_retry_exhausted;	/**< # FW generated where a retry
								 * was exhausted
								 */
	uint32			rx_pkts_retried;	/**< # rx with retry bit set */
	uint32			tx_rate_fallback;	/**< lowest fallback TX rate */
	/* Fields above this line are common to sta_info_t versions 4 and 5 */

	uint32			rx_dur_total;	/* total user RX duration (estimated) */

	chanspec_t		chanspec;       /** chanspec this sta is on */
	uint16			PAD;
	wl_rateset_args_v3_t	rateset_adv;	/* rateset along with mcs index bitmap */
} sta_info_v7_t;


/* define to help support one version older sta_info_t from user level
 * applications.
 */
#define WL_OLD_STAINFO_SIZE	OFFSETOF(sta_info_t, tx_tot_pkts)

#define WL_STA_VER_4		4u
#define WL_STA_VER_5		5u
#define WL_STA_VER_6		6u
#define WL_STA_VER_7		7u

#define	WLC_NUMRATES	16	/**< max # of rates in a rateset */

/**Used to get specific STA parameters */
typedef struct {
	uint32	val;
	struct ether_addr ea;
	uint16	PAD;
} scb_val_t;

/**Used by iovar versions of some ioctls, i.e. WLC_SCB_AUTHORIZE et al */
typedef struct {
	uint32 code;
	scb_val_t ioctl_args;
} authops_t;

/** channel encoding */
typedef struct channel_info {
	int32 hw_channel;
	int32 target_channel;
	int32 scan_channel;
} channel_info_t;

/** For ioctls that take a list of MAC addresses */
typedef struct maclist {
	uint32 count;				/**< number of MAC addresses */
	struct ether_addr ea[BCM_FLEX_ARRAY];	/**< variable length array of MAC addresses */
} maclist_t;

typedef struct wds_client_info {
	char	ifname[INTF_NAME_SIZ];	/* WDS ifname */
	struct	ether_addr ea;		/* WDS client MAC address */
} wds_client_info_t;

#define WDS_MACLIST_MAGIC	0xFFFFFFFF
#define WDS_MACLIST_VERSION	1

/* For wds MAC list ioctls */
typedef struct wds_maclist {
	uint32 count;						/* Number of WDS clients */
	uint32 magic;						/* Magic number */
	uint32 version;						/* Version number */
	struct wds_client_info client_list[BCM_FLEX_ARRAY];	/* Var len array of WDS clients */
} wds_maclist_t;

/**get pkt count struct passed through ioctl */
typedef struct get_pktcnt {
	uint32 rx_good_pkt;
	uint32 rx_bad_pkt;
	uint32 tx_good_pkt;
	uint32 tx_bad_pkt;
	uint32 rx_ocast_good_pkt; /**< unicast packets destined for others */
} get_pktcnt_t;

/* NINTENDO2 */
#define LQ_IDX_MIN              0
#define LQ_IDX_MAX              1
#define LQ_IDX_AVG              2
#define LQ_IDX_SUM              2
#define LQ_IDX_LAST             3
#define LQ_STOP_MONITOR         0
#define LQ_START_MONITOR        1

/** Get averages RSSI, Rx PHY rate and SNR values */
/* Link Quality */
typedef struct {
	int32 rssi[LQ_IDX_LAST];  /**< Array to keep min, max, avg rssi */
	int32 snr[LQ_IDX_LAST];   /**< Array to keep min, max, avg snr */
	int32 isvalid;            /**< Flag indicating whether above data is valid */
} wl_lq_t;

typedef enum wl_wakeup_reason_type {
	LCD_ON = 1,
	LCD_OFF,
	DRC1_WAKE,
	DRC2_WAKE,
	REASON_LAST
} wl_wr_type_t;

typedef struct {
	/** Unique filter id */
	uint32	id;
	/** stores the reason for the last wake up */
	uint8	reason;
	uint8	PAD[3];
} wl_wr_t;

/** Get MAC specific rate histogram command */
typedef struct {
	struct	ether_addr ea;	/**< MAC Address */
	uint8	ac_cat;	/**< Access Category */
	uint8	num_pkts;	/**< Number of packet entries to be averaged */
} wl_mac_ratehisto_cmd_t;
/** Get MAC rate histogram response */
/* deprecated after JAGUAR branch */
typedef struct {
	uint32	rate[DOT11_RATE_MAX + 1];	/**< Rates */
	uint32	mcs[WL_RATESET_SZ_HT_IOCTL * WL_TX_CHAINS_MAX];	/**< MCS counts */
	uint32	vht[WL_RATESET_SZ_VHT_MCS][WL_TX_CHAINS_MAX];	/**< VHT counts */
	uint32	tsf_timer[2][2];	/**< Start and End time for 8bytes value */
	uint32	prop11n_mcs[WLC_11N_LAST_PROP_MCS - WLC_11N_FIRST_PROP_MCS + 1]; /** MCS counts */
} wl_mac_ratehisto_res_t;

/* sta_info ecounters */
typedef struct {
	struct ether_addr   ea;				/* Station MAC addr */
	struct ether_addr   BSSID;			/* BSSID of the BSS */
	uint32              tx_pkts_fw_total;		/* # FW generated sent successfully */
	uint32              tx_pkts_fw_retries;		/* # retries for FW generated frames */
	uint32              tx_pkts_fw_retry_exhausted;	/* # FW generated which
							 * failed after retry
							 */
} sta_info_ecounters_t;

#define STAMON_MODULE_VER		1

/**Linux network driver ioctl encoding */
typedef struct wl_ioctl {
	uint32 cmd;	/**< common ioctl definition */
	void *buf;	/**< pointer to user buffer */
	uint32 len;	/**< length of user buffer */
	uint8 set;		/**< 1=set IOCTL; 0=query IOCTL */
	uint8 PAD[3];
	uint32 used;	/**< bytes read or written (optional) */
	uint32 needed;	/**< bytes needed (optional) */
} wl_ioctl_t;

#ifdef CONFIG_COMPAT
typedef struct compat_wl_ioctl {
	uint32 cmd;	/**< common ioctl definition */
	uint32 buf;	/**< pointer to user buffer */
	uint32 len;	/**< length of user buffer */
	uint8 set;		/**< 1=set IOCTL; 0=query IOCTL */
	uint8 PAD[3];
	uint32 used;	/**< bytes read or written (optional) */
	uint32 needed;	/**< bytes needed (optional) */
} compat_wl_ioctl_t;
#endif /* CONFIG_COMPAT */

#define WL_NUM_RATES_CCK		4 /**< 1, 2, 5.5, 11 Mbps */
#define WL_NUM_RATES_OFDM		8 /**< 6, 9, 12, 18, 24, 36, 48, 54 Mbps SISO/CDD */
#define WL_NUM_RATES_MCS_1STREAM	8 /**< MCS 0-7 1-stream rates - SISO/CDD/STBC/MCS */
#define WL_NUM_RATES_EXTRA_VHT		2 /**< Additional VHT 11AC rates */
#define WL_NUM_RATES_VHT		10
#define WL_NUM_RATES_VHT_ALL		(WL_NUM_RATES_VHT + WL_NUM_RATES_EXTRA_VHT)
#define WL_NUM_RATES_HE			12
#define WL_NUM_RATES_EHT		16u
#define WL_NUM_RATES_MCS32		1
#define UC_PATH_LEN			128u /**< uCode path length */


/*
 * Structure for passing hardware and software
 * revision info up from the driver.
 */
typedef struct wlc_rev_info {
	uint32		vendorid;	/**< PCI vendor id */
	uint32		deviceid;	/**< device id of chip */
	uint32		radiorev;	/**< radio revision */
	uint32		chiprev;	/**< chip revision */
	uint32		corerev;	/**< core revision */
	uint32		boardid;	/**< board identifier (usu. PCI sub-device id) */
	uint32		boardvendor;	/**< board vendor (usu. PCI sub-vendor id) */
	uint32		boardrev;	/**< board revision */
	uint32		driverrev;	/**< driver version */
	uint32		ucoderev;	/**< uCode version */
	uint32		bus;		/**< bus type */
	uint32		chipnum;	/**< chip number */
	uint32		phytype;	/**< phy type */
	uint32		phyrev;		/**< phy revision */
	uint32		anarev;		/**< anacore rev */
	uint32		chippkg;	/**< chip package info */
	uint32		nvramrev;	/**< nvram revision number */
	uint32		phyminorrev;	/**< phy minor rev */
	uint32		coreminorrev;	/**< core minor rev */
	uint32		drvrev_major;	/**< driver version: major */
	uint32		drvrev_minor;	/**< driver version: minor */
	uint32		drvrev_rc;	/**< driver version: rc */
	uint32		drvrev_rc_inc;	/**< driver version: rc incremental */
	uint16		ucodeprebuilt;	/**< uCode prebuilt flag */
	uint16		ucodediffct;	/**< uCode diff count */
	uchar		ucodeurl[128u]; /* obsolete, kept for ROM compatiblity */
	uchar		ucodepath[UC_PATH_LEN]; /**< uCode URL or path */
} wlc_rev_info_t;

#define WL_REV_INFO_LEGACY_LENGTH	48

#define WL_BRAND_MAX 10
typedef struct wl_instance_info {
	uint32 instance;
	int8 brand[WL_BRAND_MAX];
	int8 PAD[4-(WL_BRAND_MAX%4)];
} wl_instance_info_t;

/** structure to change size of tx fifo */
typedef struct wl_txfifo_sz {
	uint16	magic;
	uint16	fifo;
	uint16	size;
} wl_txfifo_sz_t;

/* Transfer info about an IOVar from the driver */
/**Max supported IOV name size in bytes, + 1 for nul termination */
#define WLC_IOV_NAME_LEN	(32 + 1)

typedef struct wlc_iov_trx_s {
	uint8 module;
	uint8 type;
	char name[WLC_IOV_NAME_LEN];
} wlc_iov_trx_t;

/** bump this number if you change the ioctl interface */
#define WLC_IOCTL_VERSION	2
#define WLC_IOCTL_VERSION_LEGACY_IOTYPES	1
/* ifdef EXT_STA */
typedef struct _wl_assoc_result {
	ulong associated;
	ulong NDIS_auth;
	ulong NDIS_infra;
} wl_assoc_result_t;
/* EXT_STA */

#define WL_PHY_PAVARS_LEN	64	/**< Phytype, Bandrange, chain, a[0], b[0], c[0], d[0] .. */


#define WL_PHY_PAVAR_VER	1	/**< pavars version */
#define WL_PHY_PAVARS2_NUM	3	/**< a1, b0, b1 */
typedef struct wl_pavars2 {
	uint16 ver;		/**< version of this struct */
	uint16 len;		/**< len of this structure */
	uint16 inuse;		/**< driver return 1 for a1,b0,b1 in current band range */
	uint16 phy_type;	/**< phy type */
	uint16 bandrange;
	uint16 chain;
	uint16 inpa[WL_PHY_PAVARS2_NUM];	/**< phy pavars for one band range */
} wl_pavars2_t;

typedef struct wl_po {
	uint16	phy_type;	/**< Phy type */
	uint16	band;
	uint16	cckpo;
	uint16	PAD;
	uint32	ofdmpo;
	uint16	mcspo[8];
} wl_po_t;

#define WL_NUM_RPCALVARS 5	/**< number of rpcal vars */

typedef struct wl_rpcal {
	uint16 value;
	uint16 update;
} wl_rpcal_t;

#define WL_NUM_RPCALPHASEVARS 5	/* number of rpcal phase vars */

typedef struct wl_rpcal_phase {
	uint16 value;
	uint16 update;
} wl_rpcal_phase_t;

typedef struct wl_aci_args {
	int32 enter_aci_thresh; /* Trigger level to start detecting ACI */
	int32 exit_aci_thresh; /* Trigger level to exit ACI mode */
	int32 usec_spin; /* microsecs to delay between rssi samples */
	int32 glitch_delay; /* interval between ACI scans when glitch count is consistently high */
	uint16 nphy_adcpwr_enter_thresh;	/**< ADC power to enter ACI mitigation mode */
	uint16 nphy_adcpwr_exit_thresh;	/**< ADC power to exit ACI mitigation mode */
	uint16 nphy_repeat_ctr;		/**< Number of tries per channel to compute power */
	uint16 nphy_num_samples;	/**< Number of samples to compute power on one channel */
	uint16 nphy_undetect_window_sz;	/**< num of undetects to exit ACI Mitigation mode */
	uint16 nphy_b_energy_lo_aci;	/**< low ACI power energy threshold for bphy */
	uint16 nphy_b_energy_md_aci;	/**< mid ACI power energy threshold for bphy */
	uint16 nphy_b_energy_hi_aci;	/**< high ACI power energy threshold for bphy */
	uint16 nphy_noise_noassoc_glitch_th_up; /**< wl interference 4 */
	uint16 nphy_noise_noassoc_glitch_th_dn;
	uint16 nphy_noise_assoc_glitch_th_up;
	uint16 nphy_noise_assoc_glitch_th_dn;
	uint16 nphy_noise_assoc_aci_glitch_th_up;
	uint16 nphy_noise_assoc_aci_glitch_th_dn;
	uint16 nphy_noise_assoc_enter_th;
	uint16 nphy_noise_noassoc_enter_th;
	uint16 nphy_noise_assoc_rx_glitch_badplcp_enter_th;
	uint16 nphy_noise_noassoc_crsidx_incr;
	uint16 nphy_noise_assoc_crsidx_incr;
	uint16 nphy_noise_crsidx_decr;
} wl_aci_args_t;

#define WL_ACI_ARGS_LEGACY_LENGTH	16	/**< bytes of pre NPHY aci args */

#define	WL_MACFIFO_PLAY_ARGS_T_VERSION	1u	/* version of wl_macfifo_play_args_t struct */

enum wl_macfifo_play_flags {
	WL_MACFIFO_PLAY_STOP =		0x00u,	/* stop playing samples */
	WL_MACFIFO_PLAY_START =		0x01u,	/* start playing samples */
	WL_MACFIFO_PLAY_LOAD =		0x02u,	/* for set: load samples
						   for get: samples are loaded
						 */
	WL_MACFIFO_PLAY_CAL =		0x08u,	/* macfifo play for calibration */
	WL_MACFIFO_PLAY_GET_MAX_SIZE =	0x10u,	/* get the macfifo buffer size */
	WL_MACFIFO_PLAY_GET_STATUS =	0x20u,	/* get macfifo play status */
};

typedef struct wl_macfifo_play_args {
	uint16 version;		/* structure version */
	uint16 len;		/* size of structure */
	uint16 flags;
	uint8 PAD[2];
	uint32 data_len;	/* data length */
} wl_macfifo_play_args_t;

#define	WL_MACFIFO_PLAY_DATA_T_VERSION	1u	/* version of wl_macfifo_play_data_t struct */

typedef struct wl_macfifo_play_data {
	uint16 version;		/* structure version */
	uint16 len;		/* size of structure */
	uint32 data_len;	/* data length */
} wl_macfifo_play_data_t;

#define	WL_SAMPLECOLLECT_T_VERSION	2	/**< version of wl_samplecollect_args_t struct */
typedef struct wl_samplecollect_args {
	/* version 0 fields */
	uint8 coll_us;
	uint8 PAD[3];
	int32 cores;
	/* add'l version 1 fields */
	uint16 version;     /**< see definition of WL_SAMPLECOLLECT_T_VERSION */
	uint16 length;      /**< length of entire structure */
	int8 trigger;
	uint8 PAD;
	uint16 timeout;
	uint16 mode;
	uint16 PAD;
	uint32 pre_dur;
	uint32 post_dur;
	uint8 gpio_sel;
	uint8 downsamp;
	uint8 be_deaf;
	uint8 agc;		/**< loop from init gain and going down */
	uint8 filter;		/**< override high pass corners to lowest */
	/* add'l version 2 fields */
	uint8 trigger_state;
	uint8 module_sel1;
	uint8 module_sel2;
	uint16 nsamps;
	uint16 PAD;
	int32 bitStart;
	uint32 gpioCapMask;
	uint8 gpio_collection;
	uint8 PAD[3];
} wl_samplecollect_args_t;

#define	WL_SAMPLEDATA_T_VERSION		1	/**< version of wl_samplecollect_args_t struct */
/* version for unpacked sample data, int16 {(I,Q),Core(0..N)} */
#define	WL_SAMPLEDATA_T_VERSION_SPEC_AN 2

typedef struct wl_sampledata {
	uint16 version;	/**< structure version */
	uint16 size;	/**< size of structure */
	uint16 tag;	/**< Header/Data */
	uint16 length;	/**< data length */
	uint32 flag;	/**< bit def */
} wl_sampledata_t;


/* WL_OTA START */
/* OTA Test Status */
enum {
	WL_OTA_TEST_IDLE = 0,		/**< Default Idle state */
	WL_OTA_TEST_ACTIVE = 1,		/**< Test Running */
	WL_OTA_TEST_SUCCESS = 2,	/**< Successfully Finished Test */
	WL_OTA_TEST_FAIL = 3		/**< Test Failed in the Middle */
};

/* OTA SYNC Status */
enum {
	WL_OTA_SYNC_IDLE = 0,	/**< Idle state */
	WL_OTA_SYNC_ACTIVE = 1,	/**< Waiting for Sync */
	WL_OTA_SYNC_FAIL = 2	/**< Sync pkt not recieved */
};

/* Various error states dut can get stuck during test */
enum {
	WL_OTA_SKIP_TEST_CAL_FAIL = 1,		/**< Phy calibration failed */
	WL_OTA_SKIP_TEST_SYNCH_FAIL = 2,	/**< Sync Packet not recieved */
	WL_OTA_SKIP_TEST_FILE_DWNLD_FAIL = 3,	/**< Cmd flow file download failed */
	WL_OTA_SKIP_TEST_NO_TEST_FOUND = 4,	/**< No test found in Flow file */
	WL_OTA_SKIP_TEST_WL_NOT_UP = 5,		/**< WL UP failed */
	WL_OTA_SKIP_TEST_UNKNOWN_CALL		/**< Unintentional scheduling on ota test */
};

/* Differentiator for ota_tx and ota_rx */
enum {
	WL_OTA_TEST_TX = 0,		/**< ota_tx */
	WL_OTA_TEST_RX = 1,		/**< ota_rx */
};

/* Catch 3 modes of operation: 20Mhz, 40Mhz, 20 in 40 Mhz */
enum {
	WL_OTA_TEST_BW_20_IN_40MHZ = 0,		/**< 20 in 40 operation */
	WL_OTA_TEST_BW_20MHZ = 1,		/**< 20 Mhz operation */
	WL_OTA_TEST_BW_40MHZ = 2,		/**< full 40Mhz operation */
	WL_OTA_TEST_BW_80MHZ = 3		/* full 80Mhz operation */
};
#define HT_MCS_INUSE	0x00000080	/* HT MCS in use,indicates b0-6 holds an mcs */
#define VHT_MCS_INUSE	0x00000100	/* VHT MCS in use,indicates b0-6 holds an mcs */
#define OTA_RATE_MASK 0x0000007f	/* rate/mcs value */
#define OTA_STF_SISO	0
#define OTA_STF_CDD		1
#define OTA_STF_STBC	2
#define OTA_STF_SDM		3

typedef struct ota_rate_info {
	uint8 rate_cnt;					/**< Total number of rates */
	uint8 PAD;
	uint16 rate_val_mbps[WL_OTA_TEST_MAX_NUM_RATE];	/**< array of rates from 1mbps to 130mbps */
							/**< for legacy rates : ratein mbps * 2 */
							/**< for HT rates : mcs index */
} ota_rate_info_t;

typedef struct ota_power_info {
	int8 pwr_ctrl_on;	/**< power control on/off */
	int8 start_pwr;		/**< starting power/index */
	int8 delta_pwr;		/**< delta power/index */
	int8 end_pwr;		/**< end power/index */
} ota_power_info_t;

typedef struct ota_packetengine {
	uint16 delay;           /**< Inter-packet delay */
				/**< for ota_tx, delay is tx ifs in micro seconds */
				/* for ota_rx, delay is wait time in milliseconds */
	uint16 nframes;         /**< Number of frames */
	uint16 length;          /**< Packet length */
} ota_packetengine_t;

/*
 * OTA txant/rxant parameter
 *    bit7-4: 4 bits swdiv_tx/rx_policy bitmask, specify antenna-policy for SW diversity
 *    bit3-0: 4 bits TxCore bitmask, specify cores used for transmit frames
 *            (maximum spatial expansion)
 */
#define WL_OTA_TEST_ANT_MASK	0xF0
#define WL_OTA_TEST_CORE_MASK	0x0F

/* OTA txant/rxant 'ant_mask' field; map to Tx/Rx antenna policy for SW diversity */
enum {
	WL_OTA_TEST_FORCE_ANT0 = 0x10,	/* force antenna to Ant 0 */
	WL_OTA_TEST_FORCE_ANT1 = 0x20,	/* force antenna to Ant 1 */
};

/* antenna/core fields access */
#define WL_OTA_TEST_GET_ANT(_txant) ((_txant) & WL_OTA_TEST_ANT_MASK)
#define WL_OTA_TEST_GET_CORE(_txant) ((_txant) & WL_OTA_TEST_CORE_MASK)

/** Test info vector */
typedef struct wl_ota_test_args {
	uint8 cur_test;			/**< test phase */
	uint8 chan;			/**< channel */
	uint8 bw;			/**< bandwidth */
	uint8 control_band;		/**< control band */
	uint8 stf_mode;			/**< stf mode */
	uint8 PAD;
	ota_rate_info_t rt_info;	/**< Rate info */
	ota_packetengine_t pkteng;	/**< packeteng info */
	uint8 txant;			/**< tx antenna */
	uint8 rxant;			/**< rx antenna */
	ota_power_info_t pwr_info;	/**< power sweep info */
	uint8 wait_for_sync;		/**< wait for sync or not */
	uint8 ldpc;
	uint8 sgi;
	uint8 PAD;
	/* Update WL_OTA_TESTVEC_T_VERSION for adding new members to this structure */
} wl_ota_test_args_t;

#define WL_OTA_TESTVEC_T_VERSION		1	/* version of wl_ota_test_vector_t struct */
typedef struct wl_ota_test_vector {
	uint16 version;
	wl_ota_test_args_t test_arg[WL_OTA_TEST_MAX_NUM_SEQ];	/**< Test argument struct */
	uint16 test_cnt;					/**< Total no of test */
	uint8 file_dwnld_valid;					/**< File successfully downloaded */
	uint8 sync_timeout;					/**< sync packet timeout */
	int8 sync_fail_action;					/**< sync fail action */
	struct ether_addr sync_mac;				/**< macaddress for sync pkt */
	struct ether_addr tx_mac;				/**< macaddress for tx */
	struct ether_addr rx_mac;				/**< macaddress for rx */
	int8 loop_test;					/**< dbg feature to loop the test */
	uint16 test_rxcnt;
	/* Update WL_OTA_TESTVEC_T_VERSION for adding new members to this structure */
} wl_ota_test_vector_t;


/** struct copied back form dongle to host to query the status */
typedef struct wl_ota_test_status {
	int16 cur_test_cnt;		/**< test phase */
	int8 skip_test_reason;		/**< skip test reasoin */
	uint8 PAD;
	wl_ota_test_args_t test_arg;	/**< cur test arg details */
	uint16 test_cnt;		/**< total no of test downloaded */
	uint8 file_dwnld_valid;		/**< file successfully downloaded ? */
	uint8 sync_timeout;		/**< sync timeout */
	int8 sync_fail_action;		/**< sync fail action */
	struct ether_addr sync_mac;	/**< macaddress for sync pkt */
	struct ether_addr tx_mac;	/**< tx mac address */
	struct ether_addr rx_mac;	/**< rx mac address */
	uint8  test_stage;		/**< check the test status */
	int8 loop_test;			/**< Debug feature to puts test enfine in a loop */
	uint8 sync_status;		/**< sync status */
} wl_ota_test_status_t;

/* FOR ioctl that take the sta monitor information */
typedef struct stamon_data {
	struct ether_addr  ea;
	uint8 PAD[2];
	int32 rssi;
} stamon_data_t;

typedef struct stamon_info {
	int32 version;
	uint32 count;
	stamon_data_t sta_data[BCM_FLEX_ARRAY];
} stamon_info_t;

typedef struct wl_ota_rx_rssi {
	uint16	pktcnt;		/* Pkt count used for this rx test */
	chanspec_t chanspec;	/* Channel info on which the packets are received */
	int16	rssi;		/* Average RSSI of the first 50% packets received */
} wl_ota_rx_rssi_t;

#define	WL_OTARSSI_T_VERSION		1	/* version of wl_ota_test_rssi_t struct */
#define WL_OTA_TEST_RSSI_FIXED_SIZE	OFFSETOF(wl_ota_test_rssi_t, rx_rssi)

typedef struct wl_ota_test_rssi {
	uint8	version;
	uint8	testcnt;				/* total RSSI values, valid on o/p only */
	wl_ota_rx_rssi_t rx_rssi[BCM_FLEX_ARRAY];	/* Variable len array */
} wl_ota_test_rssi_t;

/* WL_OTA END */

/**wl_radar_args_t */
typedef struct {
	int32 npulses;	/**< required number of pulses at n * t_int */
	int32 ncontig;	/**< required number of pulses at t_int */
	int32 min_pw;	/**< minimum pulse width (20 MHz clocks) */
	int32 max_pw;	/**< maximum pulse width (20 MHz clocks) */
	uint16 thresh0;	/**< Radar detection, thresh 0 */
	uint16 thresh1;	/**< Radar detection, thresh 1 */
	uint16 blank;	/**< Radar detection, blank control */
	uint16 fmdemodcfg;	/**< Radar detection, fmdemod config */
	int32 npulses_lp;  /**< Radar detection, minimum long pulses */
	int32 min_pw_lp; /**< Minimum pulsewidth for long pulses */
	int32 max_pw_lp; /**< Maximum pulsewidth for long pulses */
	int32 min_fm_lp; /**< Minimum fm for long pulses */
	int32 max_span_lp;  /**< Maximum deltat for long pulses */
	int32 min_deltat; /**< Minimum spacing between pulses */
	int32 max_deltat; /**< Maximum spacing between pulses */
	uint16 autocorr;	/**< Radar detection, autocorr on or off */
	uint16 st_level_time;	/**< Radar detection, start_timing level */
	uint16 t2_min; /**< minimum clocks needed to remain in state 2 */
	uint8 PAD[2];
	uint32 version; /**< version */
	uint32 fra_pulse_err;	/**< sample error margin for detecting French radar pulsed */
	int32 npulses_fra;  /**< Radar detection, minimum French pulses set */
	int32 npulses_stg2;  /**< Radar detection, minimum staggered-2 pulses set */
	int32 npulses_stg3;  /**< Radar detection, minimum staggered-3 pulses set */
	uint16 percal_mask;	/**< defines which period cal is masked from radar detection */
	uint8 PAD[2];
	int32 quant;	/**< quantization resolution to pulse positions */
	uint32 min_burst_intv_lp;	/**< minimum burst to burst interval for bin3 radar */
	uint32 max_burst_intv_lp;	/**< maximum burst to burst interval for bin3 radar */
	int32 nskip_rst_lp;	/**< number of skipped pulses before resetting lp buffer */
	int32 max_pw_tol; /* maximum tolerance allowd in detected pulse width for radar detection */
	uint16 feature_mask; /**< 16-bit mask to specify enabled features */
	uint16 thresh0_sc;	/**< Radar detection, thresh 0 */
	uint16 thresh1_sc;	/**< Radar detection, thresh 1 */
	uint8 PAD[2];
} wl_radar_args_t;

#define WL_RADAR_ARGS_VERSION_2 2

typedef struct {
	uint32 version; /**< version */
	uint16 thresh0_20_lo;	/**< Radar detection, thresh 0 (range 5250-5350MHz) for BW 20MHz */
	uint16 thresh1_20_lo;	/**< Radar detection, thresh 1 (range 5250-5350MHz) for BW 20MHz */
	uint16 thresh0_40_lo;	/**< Radar detection, thresh 0 (range 5250-5350MHz) for BW 40MHz */
	uint16 thresh1_40_lo;	/**< Radar detection, thresh 1 (range 5250-5350MHz) for BW 40MHz */
	uint16 thresh0_80_lo;	/**< Radar detection, thresh 0 (range 5250-5350MHz) for BW 80MHz */
	uint16 thresh1_80_lo;	/**< Radar detection, thresh 1 (range 5250-5350MHz) for BW 80MHz */
	uint16 thresh0_20_hi;	/**< Radar detection, thresh 0 (range 5470-5725MHz) for BW 20MHz */
	uint16 thresh1_20_hi;	/**< Radar detection, thresh 1 (range 5470-5725MHz) for BW 20MHz */
	uint16 thresh0_40_hi;	/**< Radar detection, thresh 0 (range 5470-5725MHz) for BW 40MHz */
	uint16 thresh1_40_hi;	/**< Radar detection, thresh 1 (range 5470-5725MHz) for BW 40MHz */
	uint16 thresh0_80_hi;	/**< Radar detection, thresh 0 (range 5470-5725MHz) for BW 80MHz */
	uint16 thresh1_80_hi;	/**< Radar detection, thresh 1 (range 5470-5725MHz) for BW 80MHz */
	uint16 thresh0_160_lo;	/**< Radar detection, thresh 0 (range 5250-5350MHz) for BW 160MHz */
	uint16 thresh1_160_lo;	/**< Radar detection, thresh 1 (range 5250-5350MHz) for BW 160MHz */
	uint16 thresh0_160_hi;	/**< Radar detection, thresh 0 (range 5470-5725MHz) for BW 160MHz */
	uint16 thresh1_160_hi;	/**< Radar detection, thresh 1 (range 5470-5725MHz) for BW 160MHz */
} wl_radar_thr_t;

typedef struct {
	uint32 version; /* version */
	uint16 thresh0_sc_20_lo;
	uint16 thresh1_sc_20_lo;
	uint16 thresh0_sc_40_lo;
	uint16 thresh1_sc_40_lo;
	uint16 thresh0_sc_80_lo;
	uint16 thresh1_sc_80_lo;
	uint16 thresh0_sc_20_hi;
	uint16 thresh1_sc_20_hi;
	uint16 thresh0_sc_40_hi;
	uint16 thresh1_sc_40_hi;
	uint16 thresh0_sc_80_hi;
	uint16 thresh1_sc_80_hi;
	uint16 fc_varth_sb;
	uint16 fc_varth_bin5_sb;
	uint16 notradar_enb;
	uint16 max_notradar_lp;
	uint16 max_notradar;
	uint16 max_notradar_lp_sc;
	uint16 max_notradar_sc;
	uint16 highpow_war_enb;
	uint16 highpow_sp_ratio;	//unit is 0.5
} wl_radar_thr2_t;

#define WL_RADAR_THR_VERSION	2

typedef struct {
	uint32 ver;
	uint32 len;
	int32  rssi_th[3];
	uint8  rssi_gain_80[4];
	uint8  rssi_gain_160[4];
} wl_dyn_switch_th_t;

#define WL_PHY_DYN_SWITCH_TH_VERSION	1

/** RSSI per antenna */
typedef struct {
	uint32	version;		/**< version field */
	uint32	count;			/**< number of valid antenna rssi */
	int8 rssi_ant[WL_RSSI_ANT_MAX];	/**< rssi per antenna */
	int8 rssi_sum;			/**< summed rssi across all antennas */
	int8 PAD[3];
} wl_rssi_ant_t;

/* SNR per antenna */
typedef struct {
	uint32  version;		/* version field */
	uint32  count;			/* number of valid antenna snr */
	int8 snr_ant[WL_RSSI_ANT_MAX];	/* snr per antenna */
} wl_snr_ant_t;

/* Noise per antenna */
typedef struct {
	uint32	version;		/* version field */
	uint32	count;			/* number of valid antenna NF */
	int8 noise_ant[WL_RSSI_ANT_MAX]; /* NF per antenna */
} wl_noise_ant_t;

/* Weighted average support */
#define	WL_WA_VER		0	/* Initial version - Basic WA algorithm only */

#define WL_WA_ALGO_BASIC	0	/* Basic weighted average algorithm (all 4 metrics) */
#define WL_WA_TYPE_RSSI		0
#define WL_WA_TYPE_SNR		1
#define WL_WA_TYPE_TXRATE	2
#define WL_WA_TYPE_RXRATE	3
#define WL_WA_TYPE_MAX		4

typedef struct {			/* payload of subcmd in xtlv */
	uint8 id;
	uint8 n_total;			/* Total number of samples (n_total >= n_recent) */
	uint8 n_recent;			/* Number of samples denoted as recent */
	uint8 w_recent;			/* Total weight for the recent samples (as percentage) */
} wl_wa_basic_params_t;

typedef struct {
	uint16 ver;
	uint16 len;
	uint8 subcmd[];			/* sub-cmd in bcm_xtlv_t */
} wl_wa_cmd_t;

/** data structure used in 'dfs_status' wl interface, which is used to query dfs status */
typedef struct {
	uint32 state;		/**< noted by WL_DFS_CACSTATE_XX. */
	uint32 duration;		/**< time spent in ms in state. */
	/**
	 * as dfs enters ISM state, it removes the operational channel from quiet channel
	 * list and notes the channel in channel_cleared. set to 0 if no channel is cleared
	 */
	chanspec_t chanspec_cleared;
	/** chanspec cleared used to be a uint32, add another to uint16 to maintain size */
	uint16 PAD;
} wl_dfs_status_t;

typedef struct {
	uint32 state;		/* noted by WL_DFS_CACSTATE_XX */
	uint32 duration;		/* time spent in ms in state */
	chanspec_t chanspec;	/* chanspec of this core */
	chanspec_t chanspec_last_cleared; /* chanspec last cleared for operation by scanning */
	uint16 sub_type;	/* currently just the index of the core or the respective PLL */
	uint16 PAD;
} wl_dfs_sub_status_t;

#define WL_DFS_STATUS_ALL_VERSION	(1)
typedef struct {
	uint16 version;		/* version field; current max version 1 */
	uint16 num_sub_status;
	wl_dfs_sub_status_t  dfs_sub_status[BCM_FLEX_ARRAY]; /* Array of len num_sub_status */
} wl_dfs_status_all_t;

#define WL_DFS_AP_MOVE_VERSION	(1)

struct wl_dfs_ap_move_status_v1 {
	int16 dfs_status;	/* DFS scan status */
	chanspec_t chanspec;	/* New AP Chanspec */
	wl_dfs_status_t cac_status;	/* CAC status */
};

typedef struct wl_dfs_ap_move_status_v2 {
	int8 version;            /* version field; current max version 1 */
	int8 move_status;        /* DFS move status */
	chanspec_t chanspec;     /* New AP Chanspec */
	wl_dfs_status_all_t scan_status; /* status; see dfs_status_all for wl_dfs_status_all_t */
} wl_dfs_ap_move_status_v2_t;

#define WL_DFS_AP_MOVE_ABORT -1		/* Abort any dfs_ap_move in progress immediately */
#define WL_DFS_AP_MOVE_STUNT -2		/* Stunt move but continue background CSA if in progress */

/** data structure used in 'radar_status' wl interface, which is use to query radar det status */
typedef struct {
	uint8 detected;
	uint8 PAD[3];
	int32 count;
	uint8 pretended;
	uint8 PAD[3];
	uint32 radartype;
	uint32 timenow;
	uint32 timefromL;
	int32  lp_csect_single;
	int32  detected_pulse_index;
	int32  nconsecq_pulses;
	chanspec_t ch;
	uint8 PAD[2];
	int32  pw[10];
	int32  intv[10];
	int32  fm[10];
} wl_radar_status_t;

#define NUM_PWRCTRL_RATES 12

typedef struct {
	uint8 txpwr_band_max[NUM_PWRCTRL_RATES];	/**< User set target */
	uint8 txpwr_limit[NUM_PWRCTRL_RATES];		/**< reg and local power limit */
	uint8 txpwr_local_max;				/**< local max according to the AP */
	uint8 txpwr_local_constraint;			/**< local constraint according to the AP */
	uint8 txpwr_chan_reg_max;			/**< Regulatory max for this channel */
	uint8 txpwr_target[2][NUM_PWRCTRL_RATES];	/**< Latest target for 2.4 and 5 Ghz */
	uint8 txpwr_est_Pout[2];			/**< Latest estimate for 2.4 and 5 Ghz */
	uint8 txpwr_opo[NUM_PWRCTRL_RATES];		/**< On G phy, OFDM power offset */
	uint8 txpwr_bphy_cck_max[NUM_PWRCTRL_RATES];	/**< Max CCK power for this band (SROM) */
	uint8 txpwr_bphy_ofdm_max;			/**< Max OFDM power for this band (SROM) */
	uint8 txpwr_aphy_max[NUM_PWRCTRL_RATES];	/**< Max power for A band (SROM) */
	int8  txpwr_antgain[2];				/**< Ant gain for each band - from SROM */
	uint8 txpwr_est_Pout_gofdm;			/**< Pwr estimate for 2.4 OFDM */
} tx_power_legacy_t;

#define WL_TX_POWER_RATES_LEGACY    45
#define WL_TX_POWER_MCS20_FIRST         12
#define WL_TX_POWER_MCS20_NUM           16
#define WL_TX_POWER_MCS40_FIRST         28
#define WL_TX_POWER_MCS40_NUM           17

typedef struct {
	uint32 flags;
	chanspec_t chanspec;                 /**< txpwr report for this channel */
	chanspec_t local_chanspec;           /**< channel on which we are associated */
	uint8 local_max;                 /**< local max according to the AP */
	uint8 local_constraint;              /**< local constraint according to the AP */
	int8  antgain[2];                /**< Ant gain for each band - from SROM */
	uint8 rf_cores;                  /**< count of RF Cores being reported */
	uint8 est_Pout[4];                           /**< Latest tx power out estimate per RF
							  * chain without adjustment
							  */
	uint8 est_Pout_cck;                          /**< Latest CCK tx power out estimate */
	uint8 user_limit[WL_TX_POWER_RATES_LEGACY];  /**< User limit */
	uint8 reg_limit[WL_TX_POWER_RATES_LEGACY];   /**< Regulatory power limit */
	uint8 board_limit[WL_TX_POWER_RATES_LEGACY]; /**< Max power board can support (SROM) */
	uint8 target[WL_TX_POWER_RATES_LEGACY];      /**< Latest target power */
	uint8 PAD[2];
} tx_power_legacy2_t;

#define WL_NUM_2x2_ELEMENTS		4
#define WL_NUM_3x3_ELEMENTS		6
#define WL_NUM_4x4_ELEMENTS		10

typedef struct {
	uint16 ver;				/**< version of this struct */
	uint16 len;				/**< length in bytes of this structure */
	uint32 flags;
	chanspec_t chanspec;			/**< txpwr report for this channel */
	chanspec_t local_chanspec;		/**< channel on which we are associated */
	uint32     buflen;			/**< ppr buffer length */
	uint8      pprbuf[BCM_FLEX_ARRAY];	/**< Latest target power buffer */
} wl_txppr_t;

#define WL_TXPPR_VERSION	1
#define WL_TXPPR_LENGTH	(sizeof(wl_txppr_t))
#define TX_POWER_T_VERSION	45
#define TX_POWER_T_VERSION_V2	46
#define TX_POWER_T_VERSION_V3	47

/* curpower ppr types */
enum {
	PPRTYPE_TARGETPOWER         = 1u,
	PPRTYPE_BOARDLIMITS         = 2u,
	PPRTYPE_REGLIMITS           = 3u,
	PPRTYPE_RU_REGLIMITS        = 4u,
	PPRTYPE_RU_BOARDLIMITS      = 5u,
	PPRTYPE_RU_TARGETPOWER      = 6u,
	PPRTYPE_DYNAMIC_INFO        = 7u,
	PPRTYPE_TGT_PWR_PSU         = 8u,
	PPRTYPE_BOARDLIMITS_PSU     = 9u,
	PPRTYPE_REGLIMITS_PSU       = 10u,
	PPRTYPE_TGT_PWR_MRU         = 11u,
	PPRTYPE_BOARDLIMITS_MRU     = 12u,
	PPRTYPE_REGLIMITS_MRU       = 13u,
	PPRTYPE_LAST
};

/** number of ppr serialization buffers, it should be reg, board and target */
#define WL_TXPPR_SER_BUF_NUM	(PPRTYPE_LAST - 1)


typedef struct chanspec_txpwr_max {
	chanspec_t chanspec;   /**< chanspec */
	int8 txpwr_max;        /**< max txpwr in all the rates */
	uint8 txpwr_cat;       /**< txpwr category for 6g */
} chanspec_txpwr_max_t;

typedef struct  wl_chanspec_txpwr_max {
	uint16 ver;					/**< version of this struct */
	uint16 len;					/**< length in bytes of this structure */
	uint32 count;					/**< number of (chanspec, txpwr_max) pair */
	chanspec_txpwr_max_t txpwr[BCM_FLEX_ARRAY];	/**< array of (chanspec, max_txpwr) pair */
} wl_chanspec_txpwr_max_t;

#define WL_CHANSPEC_TXPWR_MAX_VER	1
#define WL_CHANSPEC_TXPWR_MAX_LEN \
	(OFFSETOF(wl_chanspec_txpwr_max_t, txpwr) + sizeof(chanspec_txpwr_max_t))

typedef struct tx_inst_power {
	uint8 txpwr_est_Pout[2];			/**< Latest estimate for 2.4 and 5 Ghz */
	uint8 txpwr_est_Pout_gofdm;			/**< Pwr estimate for 2.4 OFDM */
} tx_inst_power_t;

#define WL_NUM_TXCHAIN_MAX	4
#if WL_NUM_TXCHAIN_MAX > 4
#error "WL_NUM_TXCHAIN_MAX is exceeded more than MCSSQSNR defs (4)"
#endif

typedef struct wl_txchain_pwr_offsets {
	int8 offset[WL_NUM_TXCHAIN_MAX];	/**< quarter dBm signed offset for each chain */
} wl_txchain_pwr_offsets_t;

/** maximum channels returned by the get valid channels iovar */
#define WL_NUMCHANNELS		64
#define WL_NUMCHANNELS_MANY_CHAN 10
#define WL_ITER_LIMIT_MANY_CHAN 5

#define WL_MIMO_PS_CFG_VERSION_1 1

typedef struct wl_mimops_cfg {
	uint8 version;
	/* active_chains: 0 for all, 1 for 1 chain. */
	uint8 active_chains;
	/* static (0) or dynamic (1).or disabled (3) Mode applies only when active_chains = 0. */
	uint8 mode;
	/* bandwidth = Full (0), 20M (1), 40M (2), 80M (3). */
	uint8 bandwidth;
	uint8 applychangesafterlearning;
	uint8 PAD[3];
} wl_mimops_cfg_t;

/* This event is for tracing MIMO PS metrics snapshot calls.
 * It is helpful to debug out-of-sync issue between
 * ucode SHM values and FW snapshot calculation.
 * It is part of the EVENT_LOG_TAG_MIMO_PS_TRACE.
 */
#define WL_MIMO_PS_METRICS_SNAPSHOT_TRACE_TYPE	0
typedef struct wl_mimo_ps_metrics_snapshot_trace {
	/* type field for this TLV: */
	uint16  type;
	/* length field for this TLV */
	uint16  len;
	uint32  idle_slotcnt_mimo;	/* MIMO idle slotcnt raw SHM value */
	uint32  last_idle_slotcnt_mimo;	/* stored value snapshot */
	uint32  idle_slotcnt_siso;	/* SISO idle slotcnt raw SHM value */
	uint32  last_idle_slotcnt_siso;	/* stored value snapshot */
	uint32	rx_time_mimo;		/* Rx MIMO raw SHM value */
	uint32	last_rx_time_mimo;	/* stored value snapshot */
	uint32	rx_time_siso;		/* RX SISO raw SHM value */
	uint32	last_rx_time_siso;	/* stored value snapshot */
	uint32  tx_time_1chain;		/* Tx 1-chain raw SHM value */
	uint32  last_tx_time_1chain;	/* stored value snapshot */
	uint32	tx_time_2chain;		/* Tx 2-chain raw SHM value */
	uint32	last_tx_time_2chain;	/* stored value snapshot */
	uint32  tx_time_3chain;		/* Tx 3-chain raw SHM value */
	uint32  last_tx_time_3chain;	/* stored value snapshot */
	uint16	reason;			/* reason for snapshot call, see below */
	/* Does the call reset last values after delta calculation */
	uint16	reset_last;
} wl_mimo_ps_metrics_snapshot_trace_t;
/* reason codes for mimo ps metrics snapshot function calls */
#define WL_MIMOPS_METRICS_SNAPSHOT_REPORT		1
#define WL_MIMOPS_METRICS_SNAPSHOT_RXCHAIN_SET		2
#define WL_MIMOPS_METRICS_SNAPSHOT_ARBI			3
#define WL_MIMOPS_METRICS_SNAPSHOT_SLOTUPD		4
#define WL_MIMOPS_METRICS_SNAPSHOT_PMBCNRX		5
#define WL_MIMOPS_METRICS_SNAPSHOT_BMACINIT		6
#define WL_MIMOPS_METRICS_SNAPSHOT_HT_COMPLETE		7
#define WL_MIMOPS_METRICS_SNAPSHOT_OCL                  8

#define WL_MIMO_PS_STATUS_VERSION_2	2
typedef struct wl_mimo_ps_status {
	uint8 version;
	uint8 ap_cap;			/* The associated AP's capability (BW, MIMO/SISO). */
	uint8 association_status;	/* How we are associated to the AP (MIMO/SISO). */
	uint8 mimo_ps_state;		/* mimo_ps_cfg states: [0-5]. See below for values */
	uint8 mrc_state;		/* MRC state: NONE (0), ACTIVE(1) */
	uint8 bss_rxchain;		/* bss rxchain bitmask */
	uint8 bss_txchain;		/* bss txchain bitmask */
	uint8 bss_bw;			/* bandwidth: Full (0), 20M (1), 40M (2), 80M (3), etc */
	uint16 hw_state;		/* bitmask of hw state. See below for values */
	uint8 hw_rxchain;		/* actual HW rxchain bitmask */
	uint8 hw_txchain;		/* actual HW txchain bitmask */
	uint8 hw_bw;			/* bandwidth: Full (0), 20M (1), 40M (2), 80M (3), etc */
	uint8 pm_bcnrx_state;		/* actual state of ucode flag */
	uint8 basic_rates_present;	/* internal flag to trigger siso bcmc rx */
	uint8 siso_bcmc_rx_state;	/* actual state of ucode flag */
} wl_mimo_ps_status_t;

#define WL_MIMO_PS_STATUS_VERSION_1	1
typedef struct wl_mimo_ps_status_v1 {
	uint8 version;
	uint8 ap_cap;			/* The associated AP's capability (BW, MIMO/SISO). */
	uint8 association_status;	/* How we are associated to the AP (MIMO/SISO). */
	uint8 mimo_ps_state;		/* mimo_ps_cfg states: [0-5]. See below for values */
	uint8 mrc_state;		/* MRC state: NONE (0), ACTIVE(1) */
	uint8 bss_rxchain;		/* bss rxchain bitmask */
	uint8 bss_txchain;		/* bss txchain bitmask */
	uint8 bss_bw;			/* bandwidth: Full (0), 20M (1), 40M (2), 80M (3), etc */
	uint16 hw_state;		/* bitmask of hw state. See below for values */
	uint8 hw_rxchain;		/* actual HW rxchain bitmask */
	uint8 hw_txchain;		/* actual HW txchain bitmask */
	uint8 hw_bw;			/* bandwidth: Full (0), 20M (1), 40M (2), 80M (3), etc */
	uint8 PAD[3];
} wl_mimo_ps_status_v1_t;

#define WL_MIMO_PS_STATUS_AP_CAP(ap_cap)	(ap_cap & 0x0F)
#define WL_MIMO_PS_STATUS_AP_CAP_BW(ap_cap)	(ap_cap >> 4)
#define WL_MIMO_PS_STATUS_ASSOC_BW_SHIFT 4

/* version 3: assoc status: low nibble is status enum, high other flags */
#define WL_MIMO_PS_STATUS_VERSION_3			3
#define WL_MIMO_PS_STATUS_ASSOC_STATUS_MASK		0x0F
#define WL_MIMO_PS_STATUS_ASSOC_STATUS_VHT_WITHOUT_OMN	0x80

/* mimo_ps_status: ap_cap/association status */
enum {
	WL_MIMO_PS_STATUS_ASSOC_NONE	= 0,
	WL_MIMO_PS_STATUS_ASSOC_SISO	= 1,
	WL_MIMO_PS_STATUS_ASSOC_MIMO	= 2,
	WL_MIMO_PS_STATUS_ASSOC_LEGACY	= 3
};

/* mimo_ps_status: mimo_ps_cfg states */
enum {
	WL_MIMO_PS_CFG_STATE_NONE			= 0,
	WL_MIMO_PS_CFG_STATE_INFORM_AP_INPROGRESS	= 1,
	WL_MIMO_PS_CFG_STATE_INFORM_AP_DONE		= 2,
	WL_MIMO_PS_CFG_STATE_LEARNING			= 3,
	WL_MIMO_PS_CFG_STATE_HW_CONFIGURE		= 4,
	WL_MIMO_PS_CFG_STATE_INFORM_AP_PENDING		= 5
};

/* mimo_ps_status: hw_state values */
#define WL_MIMO_PS_STATUS_HW_STATE_NONE			0
#define WL_MIMO_PS_STATUS_HW_STATE_LTECOEX		(0x1 << 0)
#define WL_MIMO_PS_STATUS_HW_STATE_MIMOPS_BSS		(0x1 << 1)


#define WL_MIMO_PS_STATUS_HW_STATE_SCAN			(0x1 << 3)
#define WL_MIMO_PS_STATUS_HW_STATE_TXPPR		(0x1 << 4)
#define WL_MIMO_PS_STATUS_HW_STATE_PWRTHOTTLE		(0x1 << 5)
#define WL_MIMO_PS_STATUS_HW_STATE_TMPSENSE		(0x1 << 6)
#define WL_MIMO_PS_STATUS_HW_STATE_IOVAR		(0x1 << 7)
#define WL_MIMO_PS_STATUS_HW_STATE_AP_BSS		(0x1 << 8)

/* mimo_ps_status: mrc states */
#define WL_MIMO_PS_STATUS_MRC_NONE	0
#define WL_MIMO_PS_STATUS_MRC_ACTIVE	1

/* mimo_ps_status: core flag states for single-core beacon and siso-bcmc rx */
#define WL_MIMO_PS_STATUS_MHF_FLAG_NONE		0
#define WL_MIMO_PS_STATUS_MHF_FLAG_ACTIVE	1
#define WL_MIMO_PS_STATUS_MHF_FLAG_COREDOWN	2
#define WL_MIMO_PS_STATUS_MHF_FLAG_INVALID	3

/* Type values for the REASON */
#define WL_MIMO_PS_PS_LEARNING_ABORTED          (1 << 0)
#define WL_MIMO_PS_PS_LEARNING_COMPLETED        (1 << 1)
#define WL_MIMO_PS_PS_LEARNING_ONGOING          (1 << 2)

typedef struct wl_mimo_ps_learning_event_data {
	uint32 startTimeStamp;
	uint32 endTimeStamp;
	uint16 reason;
	struct ether_addr BSSID;
	uint32 totalSISO_below_rssi_threshold;
	uint32 totalMIMO_below_rssi_threshold;
	uint32 totalSISO_above_rssi_threshold;
	uint32 totalMIMO_above_rssi_threshold;
} wl_mimo_ps_learning_event_data_t;

#define WL_MIMO_PS_PS_LEARNING_CFG_ABORT	(1 << 0)
#define WL_MIMO_PS_PS_LEARNING_CFG_STATUS	(1 << 1)
#define WL_MIMO_PS_PS_LEARNING_CFG_CONFIG	(1 << 2)
#define WL_MIMO_PS_PS_LEARNING_CFG_MASK		(0x7)

#define WL_MIMO_PS_PS_LEARNING_CFG_V1 1

typedef struct wl_mimops_learning_cfg {
	/* flag:  bit 0 for abort */
	/* flag:  bit 1 for status */
	/* flag:  bit 2 for configuring no of packets and rssi */
	uint8                  flag;
	/* mimo ps learning version, compatible version is 0 */
	uint8                  version;
	/* if version is 0 or rssi is 0, ignored */
	int8                   learning_rssi_threshold;
	uint8                  reserved;
	uint32                 no_of_packets_for_learning;
	wl_mimo_ps_learning_event_data_t mimops_learning_data;
} wl_mimops_learning_cfg_t;


/* ocl status is reported per slice */
#define WL_OCL_STATUS_VERSION_1 1
typedef struct ocl_status_info {
	uint8  version;
	uint8  len;
	uint16 fw_status;		/* Bits representing FW disable reasons */
	uint8  hw_status;		/* Bits for actual HW config and SISO/MIMO coremask */
	uint8  coremask;		/* The ocl core mask (indicating listening core) */
} ocl_status_info_t;

#define WL_OCL_STATUS_VERSION_2 2	/* v2 supports disable stats */
typedef struct ocl_status_info_v2 {
	uint8  version;
	uint8  len;
	uint8  hw_status;		/* Bits for actual HW config and SISO/MIMO coremask */
	uint8  coremask;		/* The ocl core mask (indicating listening core) */
	uint32 fw_status;		/* Bits representing FW disable reasons */
	uint32 disable_start_req;	/* disable reason which triggered the ocl state change
		                         * from enable to disable, afterwards there could be more
		                         * disable reason(s) active before ocl state is enabled
		                         * again. All current disable reason(s) are
		                         * indicated in fw_status and at snapshot maynot include
		                         * the disable reason which actually triggered ocl disable
		                         * as reasons become active/inactive independently.
		                         */
	uint32 enable_start_req;	/* disable reason which triggered the ocl state change
		                         * from disable to enable. This is the reason which very
		                         * recently turned off which caused the ocl to enable.
		                         */
	uint32 cur_ts;			/* current timestamp (ms) */
	uint32 disable_start_ts;	/* disable_start_req timestamp (ms) */
	uint32 enable_start_ts;		/* enable_start_req timestamp (ms) */
	uint32 disable_count_as;	/* total effective-disable (i.e., ocl state changing
		                         * from enable-to-disable) count since last assoc
		                         */
	uint32 disable_reqs_count_as;	/* total disable requests since last assoc */
	uint32 disable_dur_as;		/* total disable duration (ms) since last assoc */
	uint32 disable_count;		/* total effective-disable count */
	uint32 disable_reqs_count;	/* total disable requests */
	uint32 disable_dur;		/* total disable duration (ms) */
} ocl_status_info_v2_t;

/* v3 shows per-chanspec OCL status, active link type
 * on that chanspec, and user's ocl configuration
 */
#define WL_OCL_STATUS_VERSION_3 3
typedef struct ocl_status_info_v3 {
	uint8  version;
	uint8  len;
	uint8  hw_status;		/* Bits for actual HW config and SISO/MIMO coremask */
	uint8  coremask;		/* The ocl core mask (indicating listening core) */
	uint32 fw_status;		/* Bits representing FW disable reasons */
	uint32 disable_start_req;	/* disable reason which triggered the ocl state change
		                         * from enable to disable, afterwards there could be more
		                         * disable reason(s) active before ocl state is enabled
		                         * again. All current disable reason(s) are
		                         * indicated in fw_status and at snapshot maynot include
		                         * the disable reason which actually triggered ocl disable
		                         * as reasons become active/inactive independently.
		                         */
	uint32 enable_start_req;	/* disable reason which triggered the ocl state change
		                         * from disable to enable. This is the reason which very
		                         * recently turned off which caused the ocl to enable.
		                         */
	uint32 cur_ts;			/* current timestamp (ms) */
	uint32 disable_start_ts;	/* disable_start_req timestamp (ms) */
	uint32 enable_start_ts;		/* enable_start_req timestamp (ms) */
	uint32 disable_count_as;	/* total effective-disable (i.e., ocl state changing
		                         * from enable-to-disable) count since last assoc
		                         */
	uint32 disable_reqs_count_as;	/* total disable requests since last assoc */
	uint32 disable_dur_as;		/* total disable duration (ms) since last assoc */
	uint32 disable_count;		/* total effective-disable count */
	uint32 disable_reqs_count;	/* total disable requests */
	uint32 disable_dur;		/* total disable duration (ms) */
	uint16 chanspec;		/* chanspec on which ocl_status is reported */
	uint8  active_link;		/* Bits showing which link is active */
	uint8  ocl_en;			/* user's configuation */
} ocl_status_info_v3_t;

#define OCL_SET_INFRA	(1u << 0u) /* host OCL enable/disable for INFRA */
#define OCL_SET_NAN	(1u << 1u) /* host OCL enable/disable for NAN */
#define OCL_SET_AWDL	(1u << 2u) /* host OCL enable/disable for AWDL */

/* MWS OCL map */
#define WL_MWS_OCL_OVERRIDE_VERSION_1	 1
typedef struct wl_mws_ocl_override {
	uint16  version;		/* Structure version */
	uint16	bitmap_2g;		/* bitmap for 2.4G channels bits 1-13 */
	uint16	bitmap_5g_lo;		/* bitmap for 5G low channels by 2:
					 * 34-48, 52-56, 60-64, 100-102
					 */
	uint16	bitmap_5g_mid;		/* bitmap for 5G mid channels by 2:
					 * 104, 108-112, 116-120, 124-128,
					 * 132-136, 140, 149-151
					 */
	uint16	bitmap_5g_high;		/* bitmap for 5G high channels by 2
					 * 153, 157-161, 165
					 */
} wl_mws_ocl_override_t;


/* Bits for fw_status and disable/enable start request */
#define OCL_DISABLED_HOST		(1u << 0u)   /* Disabled by host through ocl_enable */
#define OCL_DISABLED_RSSI		(1u << 1u)   /* Disabled because of ocl_rssi_threshold */
#define OCL_DISABLED_LTEC		(1u << 2u)   /* Disabled due to LTE Coex activity */
#define OCL_DISABLED_SISO		(1u << 3u)   /* Disabled while in SISO mode */
#define OCL_DISABLED_CAL		(1u << 4u)   /* Disabled during active calibration */
#define OCL_DISABLED_CHANSWITCH		(1u << 5u)   /* Disabled during channel switch */
#define OCL_DISABLED_ASPEND		(1u << 6u)   /* Disabled due to assoc pending */
#define OCL_DISABLED_SEQ_RANGE		(1u << 7u)   /* Disabled during SEQ Ranging */
#define OCL_DISABLED_RXIQ_EST_BTLOWAR	(1u << 8u)   /* Disabled if the bt-lo-war is active */
#define OCL_DISABLED_IDLE_TSSICAL	(1u << 9u)   /* Disabled if TSSI Cal in progress */
#define OCL_DISABLED_TONE		(1u << 10u)  /* Disabled if the tone is active */
#define OCL_DISABLED_NOISECAL		(1u << 11u)  /* Disabled if the noise cal is active */
#define OCL_DISABLED_INIT		(1u << 12u)  /* Disabled during phy init */
#define OCL_DISABLED_AZ			(1u << 13u)  /* Disabled during 802.11az ranging */
#define OCL_DISABLED_PHYTS		(1u << 14u)  /* Disabled during PHYTS */
#define OCL_DISABLED_SCPEND		(1u << 15u)  /* Disabled due to scan pending */
#define OCL_DISABLED_EMLSR		(1u << 16u)  /* Disabled due to EMLSR enabled */
#define OCL_DISABLED_BTBPHYWAR		(1u << 17u)  /* Disabled during BT eSCO traffic */
#define OCL_DISABLED_SCCA_MIT		(1u << 18u)  /* Disabled when SCCA enabled, sccatxlbt=1 */


/* Bits for hw_status */
#define OCL_HWCFG			0x01u   /* State of OCL config bit in phy HW */
#define OCL_HWMIMO			0x02u   /* Set if current coremask is > 1 bit */
#define OCL_COREDOWN			0x80u   /* Set if core is currently down */

#define WL_OPS_CFG_VERSION_1  1
/* Common IOVAR struct */
typedef struct wl_ops_cfg_v1 {
	uint16  version;
	uint16 len;		/* total length includes fixed fields and variable data[] */
	uint16 subcmd_id;	/* subcommand id */
	uint16 padding;		/* reserved / padding for 4 byte align */
	uint8 data[];		/* subcommand data; could be empty */
} wl_ops_cfg_v1_t;


/* subcommands ids */
enum {
	WL_OPS_CFG_SUBCMD_ENABLE =		0,	/* OPS enable/disable mybss and obss
				                         * for nav and plcp options
				                         */
	WL_OPS_CFG_SUBCMD_MAX_SLEEP_DUR =	1,	/* Max sleep duration used for OPS */
	WL_OPS_CFG_SUBCMD_RESET_STATS =		2	/* Reset stats part of ops_status
			                                 * on both slices
				                         */
};

#define WL_OPS_CFG_MASK				0xffff
#define WL_OPS_CFG_CAP_MASK			0xffff0000
#define WL_OPS_CFG_CAP_SHIFT			16	/* Shift bits to locate the OPS CAP */
#define WL_OPS_MAX_SLEEP_DUR			12500	/* max ops duration in us */
#define WL_OPS_MINOF_MAX_SLEEP_DUR		512	/* minof max ops duration in us */
#define WL_OPS_SUPPORTED_CFG			(WL_OPS_MYBSS_PLCP_DUR | WL_OPS_MYBSS_NAV_DUR \
		                                 | WL_OPS_OBSS_PLCP_DUR | WL_OPS_OBSS_NAV_DUR)
#define WL_OPS_DEFAULT_CFG		        WL_OPS_SUPPORTED_CFG

/* WL_OPS_CFG_SUBCMD_ENABLE */
typedef struct wl_ops_cfg_enable {
	uint32   bits;   /* selectively enable ops for mybss and obss */
} wl_ops_cfg_enable_t;
/* Bits for WL_OPS_CFG_SUBCMD_ENABLE Parameter */
#define WL_OPS_MYBSS_PLCP_DUR		0x1	/* OPS based on mybss 11b & 11n mixed HT frames
		                                 * PLCP header duration
		                                 */
#define WL_OPS_MYBSS_NAV_DUR		0x2	/* OPS based on mybss RTS-CTS duration */
#define WL_OPS_OBSS_PLCP_DUR		0x4	/* OPS based on obss 11b & 11n mixed HT frames
		                                 * PLCP header duration
		                                 */
#define WL_OPS_OBSS_NAV_DUR		0x8	/* OPS based on obss RTS-CTS duration */


/* WL_OPS_CFG_SUBCMD_MAX_SLEEP_DUR */
typedef struct wl_ops_cfg_max_sleep_dur {
	uint32  val;   /* maximum sleep duration (us) used for OPS */
} wl_ops_cfg_max_sleep_dur_t;

/* WL_OPS_CFG_SUBCMD_RESET_STATS */
typedef struct wl_ops_cfg_reset_stats {
	uint32  val;   /* bitmap of slices, 0 means all slices */
} wl_ops_cfg_reset_stats_t;


#define WL_OPS_STATUS_VERSION_1 1
#define WL_OPS_STATUS_VERSION_2	2	/* V1 + a few C RX CCA raw stats */
#define OPS_DUR_HIST_BINS	5	/* number of bins used, 0-1, 1-2, 2-4, 4-8, >8 msec */

typedef struct wl_ops_status_v1 {
	uint16  version;
	uint16  len;			/* Total length including all fixed fields */
	uint8	slice_index;		/* Slice for which status is reported */
	uint8   disable_obss;		/* indicate if obss cfg is disabled */
	uint8   pad[2];			/* 4-byte alignment */
	uint32  disable_reasons;	/* FW disable reasons */
	uint32  disable_duration;	/* ops disable time(ms) due to disable reasons */
	uint32  applied_ops_config;	/* currently applied ops config */
	uint32  partial_ops_dur;	/* Total time (in usec) of partial ops duration */
	uint32  full_ops_dur;		/* Total time (in usec) of full ops duration */
	uint32  count_dur_hist[OPS_DUR_HIST_BINS];	/* ops occurrence histogram */
	uint32  nav_cnt;		/* number of times ops triggered based NAV duration */
	uint32  plcp_cnt;		/* number of times ops triggered based PLCP duration */
	uint32  mybss_cnt;		/* number of times mybss ops trigger */
	uint32  obss_cnt;		/* number of times obss ops trigger */
	uint32  miss_dur_cnt;		/* number of times ops couldn't happen
		                         * due to insufficient duration
		                         */
	uint32  miss_premt_cnt;		/* number of times ops couldn't happen due
		                         * to not meeting Phy preemption thresh
		                         */
	uint32  max_dur_cnt;		/* number of times ops did not trigger due to
		                         * frames exceeding max sleep duration
		                         */
	uint32	wake_cnt;		/* number of ops miss due to wake reason */
	uint32	bcn_wait_cnt;		/* number of ops miss due to waiting for bcn */
} wl_ops_status_v1_t;

typedef struct wl_ops_status_v2 {
	uint16  version;
	uint16  len;			/* Total length including all fixed fields */
	uint8	slice_index;		/* Slice for which status is reported */
	uint8   disable_obss;		/* indicate if obss cfg is disabled */
	uint8   pad[2];			/* 4-byte alignment */
	uint32  disable_reasons;	/* FW disable reasons */
	uint32  disable_duration;	/* ops disable time(ms) due to disable reasons */
	uint32  applied_ops_config;	/* currently applied ops config */
	uint32  partial_ops_dur;	/* Total time (in usec) of partial ops duration */
	uint32  full_ops_dur;		/* Total time (in usec) of full ops duration */
	uint32  count_dur_hist[OPS_DUR_HIST_BINS];	/* ops occurrence histogram */
	uint32  nav_cnt;		/* number of times ops triggered based NAV duration */
	uint32  plcp_cnt;		/* number of times ops triggered based PLCP duration */
	uint32  mybss_cnt;		/* number of times mybss ops trigger */
	uint32  obss_cnt;		/* number of times obss ops trigger */
	uint32  miss_dur_cnt;		/* number of times ops couldn't happen
		                         * due to insufficient duration
		                         */
	uint32  miss_premt_cnt;		/* number of times ops couldn't happen due
		                         * to not meeting Phy preemption thresh
		                         */
	uint32  max_dur_cnt;		/* number of times ops did not trigger due to
		                         * frames exceeding max sleep duration
		                         */
	uint32	wake_cnt;		/* number of ops miss due to wake reason */
	uint32	bcn_wait_cnt;		/* number of ops miss due to waiting for bcn */

	/* Total RX time measured by CCA (help info for OPS efficiency analysis) */
	uint32 rx_time_mybss;		/* Total RX time of IBSS intended to local DUT */
	uint32 rx_time_ibss;		/* Total RX time of IBSS */
	uint32 rx_time_obss;		/* Total RX time of OBSS */
} wl_ops_status_v2_t;

/* Bits for disable_reasons */
#define OPS_DISABLED_HOST	0x01	/* Host has disabled through ops_cfg */
#define OPS_DISABLED_UNASSOC	0x02	/* Disabled because the slice is in unassociated state */
#define OPS_DISABLED_SCAN	0x04	/* Disabled because the slice is in scan state */
#define OPS_DISABLED_BCN_MISS	0x08	/* Disabled because beacon missed for a duration */
#define OPS_DISABLED_OBSS_MMT	0x10	/* Disabled because of OBSS measurement */

#define WL_PSBW_CFG_VERSION_1  1
/* Common IOVAR struct */
typedef struct wl_psbw_cfg_v1 {
	uint16  version;
	uint16 len;			/* total length includes fixed fields and variable data[] */
	uint16 subcmd_id;		/* subcommand id */
	uint16 pad;			/* reserved / padding for 4 byte align */
	uint8 data[];			/* subcommand data */
} wl_psbw_cfg_v1_t;

/* subcommands ids */
enum {
	/* PSBW enable/disable */
	WL_PSBW_CFG_SUBCMD_ENABLE =                0,
	/* override psbw disable requests */
	WL_PSBW_CFG_SUBCMD_OVERRIDE_DISABLE_MASK = 1,
	/* Reset stats part of psbw status */
	WL_PSBW_CFG_SUBCMD_RESET_STATS =           2
};

#define WL_PSBW_OVERRIDE_DISA_CFG_MASK			0x0000ffff
#define WL_PSBW_OVERRIDE_DISA_CAP_MASK			0xffff0000
#define WL_PSBW_OVERRIDE_DISA_CAP_SHIFT			16  /* shift bits for cap */

/* WL_PSBW_CFG_SUBCMD_ENABLE */
typedef struct wl_psbw_cfg_enable {
	bool   enable;		/* enable or disable */
} wl_psbw_cfg_enable_t;

/* WL_PSBW_CFG_SUBCMD_OVERRIDE_DISABLE_MASK */
typedef struct wl_psbw_cfg_override_disable_mask {
	uint32  mask;		/* disable requests to override, cap and current cfg */
} wl_psbw_cfg_override_disable_mask_t;

/* WL_PSBW_CFG_SUBCMD_RESET_STATS */
typedef struct wl_psbw_cfg_reset_stats {
	uint32  val;		/* infra interface index, 0 */
} wl_psbw_cfg_reset_stats_t;

#define WL_PSBW_STATUS_VERSION_1 1
typedef struct wl_psbw_status_v1 {
	uint16  version;
	uint16  len;			/* total length including all fixed fields */
	uint8   curr_slice_index;	/* current slice index of the interface */
	uint8   associated;		/* interface associatd */
	chanspec_t   chspec;		/* radio chspec */
	uint32	state;			/* psbw state */
	uint32  disable_reasons;	/* FW disable reasons */
	uint32  slice_enable_dur;	/* time(ms) psbw remains enabled on this slice */
	uint32  total_enable_dur;	/* time(ms) psbw remains enabled total */
	uint32  enter_cnt;		/* total cnt entering PSBW active */
	uint32  exit_cnt;		/* total cnt exiting PSBW active */
	uint32  exit_imd_cnt;		/* total cnt imd exit when waited N tbtts */
	uint32  enter_skip_cnt;		/* total cnt entering PSBW active skipped */
} wl_psbw_status_v1_t;

/* Bit for state */
#define PSBW_ACTIVE				0x1 /* active 20MHz */
#define PSBW_TTTT_PEND				0x2 /* waiting for TTTT intr */
#define PSBW_WAIT_ENTER				0x4 /* in wait period before entering */
#define PSBW_CAL_DONE				0x8 /* 20M channel cal done */

/* Bits for disable_reasons */
#define WL_PSBW_DISA_HOST			0x00000001 /* Host has disabled through psbw_cfg */
#define WL_PSBW_DISA_AP20M			0x00000002 /* AP is operating on 20 MHz */
#define WL_PSBW_DISA_SLOTTED_BSS		0x00000004 /* slot_bss active */
#define WL_PSBW_DISA_NOT_PMFAST			0x00000008 /* Not PM_FAST */
#define WL_PSBW_DISA_BASICRATESET		0x00000010 /* BasicRateSet is empty */
#define WL_PSBW_DISA_NOT_D3			0x00000020 /* PCIe not in D3 */
#define WL_PSBW_DISA_CSA			0x00000040 /* CSA IE is present */
#define WL_PSBW_DISA_ASSOC			0x00000080 /* assoc state is active/or unassoc */
#define WL_PSBW_DISA_SCAN			0x00000100 /* scan state is active */
#define WL_PSBW_DISA_CAL			0x00000200 /* cal pending or active */
#define WL_PSBW_DISA_BCN_OFFLOAD		0x00000400 /* PSBW disabled due to scan
							    * core beacon offload
							    */
#define WL_PSBW_DISA_DISASSOC			0x00000800 /* STA is disassociated */
/* following exit reasons are reserved, for internal use & not part of disable_reasons */
#define WL_PSBW_EXIT_PM				0x00001000 /* Out of PM */
#define WL_PSBW_EXIT_TIM			0x00002000 /* unicast TIM bit present */
#define WL_PSBW_EXIT_DATA			0x00004000 /* Data for transmission */
#define WL_PSBW_EXIT_MGMTDATA			0x00008000 /* management frame for transmission */
#define WL_PSBW_EXIT_BW_UPD			0x00010000 /* BW being updated */
/* Bits for disable_reasons */
#define WL_PSBW_DISA_NON_PREF			0x40000000 /* STA interface non-pref */
#define WL_PSBW_DISA_NONE			0x80000000 /* reserved for internal use only */

/* DVFS */
#define DVFS_CMND_VERSION_1 1
/* Common IOVAR struct */
typedef struct dvfs_cmnd_v1 {
	uint16 ver;				/* version of this structure */
	uint16 len;				/* includes both fixed and variable data[] fields */
	uint32 subcmd;				/* subcommand id */
	uint8 data[];				/* subcommand data */
} dvfs_cmnd_v1_t;

/* subcommand ids */
enum {
	DVFS_SUBCMD_ENABLE = 0,			/* DVFS enable/disable, 1-byte data
						 * DVFS enable:1, disable:0
						 */
	DVFS_SUBCMD_LDV =    1,			/* DVFS force arm state to LDV, 1-byte data
						 * DVFS force LDV ON 1, LDV OFF 0
						 */
	DVFS_SUBCMD_STATUS = 2,			/* DVFS status, data[] contains dvfs_status */
	DVFS_SUBCMD_HIST =   3,			/* DVFS history, data[] contains
			                         * history of dvfs state change
			                         */
	DVFS_SUBCMD_LAST
};

/* DVFS Status */
/* current DVFS state request for ARM */
#define DVFS_STATE_BIT_MASK			0x0Fu
#define DVFS_STATE_BIT_SHIFT			0u
/* Bit value for DVFS state request */
#define DVFS_STATE_LDV				0u
#define DVFS_STATE_NDV				1u
/* current DVFS status */
#define DVFS_STATUS_BIT_MASK			0xF0u
#define DVFS_STATUS_BIT_SHIFT			4u
/* Bit value for DVFS status */
#define DVFS_STATUS_LDV				0u
#define DVFS_STATUS_NDV				1u
#define DVFS_STATUS_HDV				3u
/* DVFS bits are for status, raw request and active request */
/* 4387b0 supports only status bits for aux, main, and bt */
/* 4387c0 supports all eight status and request bits */
#define DVFS_BIT_AUX_MASK			0x0001u
#define DVFS_BIT_AUX_SHIFT			0u
#define DVFS_BIT_AUX_VAL(_val)			(((_val) & DVFS_BIT_AUX_MASK) \
						>> DVFS_BIT_AUX_SHIFT)
#define DVFS_BIT_MAIN_MASK			0x0002u
#define DVFS_BIT_MAIN_SHIFT			1u
#define DVFS_BIT_MAIN_VAL(_val)			(((_val) & DVFS_BIT_MAIN_MASK) \
						>> DVFS_BIT_MAIN_SHIFT)
#define DVFS_BIT_BT_MASK			0x0004u
#define DVFS_BIT_BT_SHIFT			2u
#define DVFS_BIT_BT_VAL(_val)			(((_val) & DVFS_BIT_BT_MASK) \
						>> DVFS_BIT_BT_SHIFT)
#define DVFS_BIT_CHIPC_MASK			0x0008u
#define DVFS_BIT_CHIPC_SHIFT			3u
#define DVFS_BIT_CHIPC_VAL(_val)		(((_val) & DVFS_BIT_CHIPC_MASK) \
						>> DVFS_BIT_CHIPC_SHIFT)
#define DVFS_BIT_PCIE_MASK			0x0010u
#define DVFS_BIT_PCIE_SHIFT			4u
#define DVFS_BIT_PCIE_VAL(_val)			(((_val) & DVFS_BIT_PCIE_MASK) \
						>> DVFS_BIT_PCIE_SHIFT)
#define DVFS_BIT_ARM_MASK			0x0020u
#define DVFS_BIT_ARM_SHIFT			5u
#define DVFS_BIT_ARM_VAL(_val)			(((_val) & DVFS_BIT_ARM_MASK) \
						>> DVFS_BIT_ARM_SHIFT)
#define DVFS_BIT_SCAN_MASK			0x0040u
#define DVFS_BIT_SCAN_SHIFT			6u
#define DVFS_BIT_SCAN_VAL(_val)			(((_val) & DVFS_BIT_SCAN_MASK) \
						>> DVFS_BIT_SCAN_SHIFT)
#define DVFS_BIT_BTSCAN_MASK			0x0080u
#define DVFS_BIT_BTSCAN_SHIFT			7u
#define DVFS_BIT_BTSCAN_VAL(_val)		(((_val) & DVFS_BIT_BTSCAN_MASK) \
						>> DVFS_BIT_BTSCAN_SHIFT)
#define DVFS_BIT_HWA_MASK			0x0100u
#define DVFS_BIT_HWA_SHIFT			8u
#define DVFS_BIT_HWA_VAL(_val)			(((_val) & DVFS_BIT_HWA_MASK) \
						>> DVFS_BIT_HWA_SHIFT)
#define DVFS_BIT_SYSMEM_MASK			0x0200u
#define DVFS_BIT_SYSMEM_SHIFT			9u
#define DVFS_BIT_SYSMEM_VAL(_val)		(((_val) & DVFS_BIT_SYSMEM_MASK) \
						>> DVFS_BIT_SYSMEM_SHIFT)

#define DVFS_BIT_SAQM_SHIFT			10u

/* to convert voltage to volt from multiple of 10mVolt */
#define DVFS_CONVERT_TO_VOLT			100u

/* status version for 4387b0 */
#define DVFS_STATUS_VERSION_1			1
typedef struct dvfs_status_v1 {
	uint16  version;			/* version of dvfs_status */
	uint16  len;				/* total length including all fixed fields */
	uint8   info;				/* current dvfs state request and status */
	uint8   voltage;			/* voltage (multiple of 10mV) */
	uint16  freq;				/* arm clock frequency (in MHz) */
	uint32  state_change_count;		/* total state (LDV/NDV) transition count */
	uint32  ldv_duration;			/* total time (ms) in LDV */
	uint32  ndv_duration;			/* total time (ms) in NDV */
	uint16	status;				/* status bits */
	uint16	PAD;				/* word aligned for size */
} dvfs_status_v1_t;
#define DVFS_STATUS_VER_1_LEN			(sizeof(dvfs_status_v1_t))
/* status version for 4387c0 */
#define DVFS_STATUS_VERSION_2			2
#define DVFS_STATUS_VERSION_3			3
typedef struct dvfs_status_v2 {
	uint16  version;			/* version of dvfs_status */
	uint16  len;				/* total length including all fixed fields */
	uint8   info;				/* current dvfs state request and status */
	uint8   voltage;			/* voltage (multiple of 10mV) */
	uint16  freq;				/* arm clock frequency (in MHz) */
	uint32  state_change_count;		/* total state (LDV/NDV) transition count */
	uint32  ldv_duration;			/* total time (ms) in LDV */
	uint32  ndv_duration;			/* total time (ms) in NDV */
	uint16	status;				/* status bits */
	uint16	raw_request;			/* raw request bits */
	uint16	active_request;			/* active request bits */
	/* DVFS_STATUS_VERSION_3 for pmurev >= 40 */
	uint16	valid_cores;			/* bitmap to indicate valid cores status */
} dvfs_status_v2_t;
#define DVFS_STATUS_V2_VALID_CORES		(0xFFu)
#define DVFS_STATUS_VER_3_LEN			(sizeof(dvfs_status_v2_t))
#define DVFS_STATUS_VER_2_LEN			(DVFS_STATUS_VER_3_LEN - (sizeof(uint16)))

#define DVFS_STATUS_VERSION_4			4
typedef struct dvfs_status_v4 {
	uint16  version;			/* version of dvfs_status */
	uint16  len;				/* total length including all fixed fields */
	uint8   info;				/* current dvfs state request and status */
	uint8   voltage;			/* voltage (multiple of 10mV) */
	uint16  freq;				/* arm clock frequency (in MHz) */
	uint32  state_change_count;		/* total state (LDV/NDV) transition count */
	uint32  ldv_duration;			/* total time (ms) in LDV */
	uint32  ndv_duration;			/* total time (ms) in NDV */
	uint16	status;				/* status bits */
	uint16	raw_request;			/* raw request bits */
	uint16	active_request;			/* active request bits */
	/* DVFS_STATUS_VERSION_3 for pmurev >= 40 */
	uint16	valid_cores;			/* bitmap to indicate valid cores status */
	/* DVFS_STATUS_VERSION_4 for pmurev >= 43 */
	uint32  state_change_count_hw_ldv;	/* total state (LDV/NDV) transition count by HW */
	uint32  state_change_count_hw_ndv;	/* total state (LDV/NDV) transition count by HW */
	uint32  ldv_duration_hw;		/* total time (ms) in LDV reported by HW */
	uint32  ndv_duration_hw;		/* total time (ms) in NDV reported by HW */
} dvfs_status_v4_t;
#define DVFS_STATUS_VER_4_LEN			(sizeof(dvfs_status_v4_t))

typedef struct dvfs_supply_stats_v5 {
	uint16  version;
	uint16  len;

	uint16  supply_num;			/* Supply num that has this dvfs supply stats. */
	uint8   dvfs_status;			/* current dvfs status LDV/NDV/HDV */
	uint8   voltage;			/* voltage (multiple of 10mV) */

	uint32  state_change_count;		/* total state (LDV/NDV) transition count */

	uint32  fw_ldv_duration;		/* total time (ms) in LDV */
	uint32  hw_ldv_duration;		/* total time (ms) in LDV reported by HW */
	uint32  state_change_count_hw_ldv;	/* HDV transition count by HW */

	uint32  fw_ndv_duration;		/* total time (ms) in LDV */
	uint32  hw_ndv_duration;		/* total time (ms) in LDV reported by HW */
	uint32  state_change_count_hw_ndv;	/* HDV transition count by HW */

	uint32  fw_hdv_duration;		/* total time (ms) in LDV */
	uint32  hw_hdv_duration;		/* total time (ms) in LDV reported by HW */
	uint32  state_change_count_hw_hdv;	/* HDV transition count by HW */
} dvfs_supply_stats_v5_t;

#define DVFS_STATUS_CORE_NAME_MASK		0x3Fu
#define DVFS_STATUS_CORE_NAME_SHIFT		0u
#define DVFS_STATUS_CORE_STATUS_MASK		0x3u
#define DVFS_STATUS_CORE_STATUS_SHIFT		0x6u
#define DVFS_STATUS_CORE_ACTIVE_REQ_MASK	0x3u
#define DVFS_STATUS_CORE_ACTIVE_REQ_SHIFT	8u
#define DVFS_STATUS_CORE_RAW_REQ_MASK		0x3u
#define DVFS_STATUS_CORE_RAW_REQ_SHIFT		10u
#define DVFS_STATUS_CORE_SUPPLY_NUM_MASK	0x3u
#define DVFS_STATUS_CORE_SUPPLY_NUM_SHIFT	12u

typedef struct dvfs_core_info_v5 {
	uint16  version;
	uint16  len;
	uint8	num_cores;
	uint8   PAD;
	uint16	powercontrol;			/* PowerControl from chipcommon */
	uint16	dvfs_core_stats[];		/* Per core statistics,
						 *  BITs 5:0 dvfs_core_name
						 *  BITs 7:6 DVFS Status
						 *  BITs 9:8 Active Request
						 *  BITs 11:10 RAW Request
						 *  BITs 13:12 Supply Num
						 *  BIT  14:14 if set CoreReady rsrc is active
						 *  BITs 15 reserved
						 */
} dvfs_core_info_v5_t;

typedef struct dvfs_freq_info_v5 {
	uint16 version;
	uint16 len;
	uint32 freq_common_bp;
	uint32 freq_wlan_bp;
	uint32 freq_main_mac;
	uint32 freq_aux_mac;
	uint32 freq_scan_mac;
	uint32 freq_saqm;
} dvfs_freq_info_v5_t;

typedef enum dvfs_tlv_id {
	DVFS_SUPPLY_STATS_ID = 0u,
	DVFS_CORE_STATUS_ID = 1u,
	DVFS_FREQ_STATS_ID = 2u,
	DVFS_XTLV_MAX = 3u
} dvfs_tlv_id_s;

#define DVFS_STATUS_CORE_INFO_VER		0
#define DVFS_STATUS_FREQ_INFO_VER		0
#define DVFS_STATUS_VERSION_5			5

typedef struct dvfs_status_v5 {
	uint16  version;	/* version of dvfs_status */
	uint16  len;		/* total length including all fixed fields */

	uint16  armfreq;	/* arm clock frequency (in MHz) */
	uint16  num_dvfs_supplies;	/* Total number of supplies */
	uint8   tlv_params[];     /* xtlvs for variable ext params, id dvfs_tlv_id_s */

} dvfs_status_v5_t;
#define DVFS_STATUS_VER_5_LEN			(sizeof(dvfs_status_v5_t))


/* DVFS_SUBCMD_HIST */
#define DVFS_HIST_CMD_VERSION_1 1
typedef struct dvfs_hist_cmd_v1 {
	uint16 version;				/* version of this structure */
	uint16 len;				/* includes both fixed and variable data[] fields */
	uint8 data[];				/* subcommand data : array of dvfs_hist_v1_t */
} dvfs_hist_cmd_v1_t;

/* DVFS_SUBCMD_HIST data[] payload */
typedef struct dvfs_hist_v1 {
	uint8 old_state;			/* old state */
	uint8 new_state;			/* new state */
	uint16 reason;				/* reason for state change */
	uint32 timestamp;			/* timestamp of state change */
} dvfs_hist_v1_t;

/* Bits for DVFS state change reason */
#define WL_DVFS_REASON_NOTPM			0x0001u /* Not PM */
#define WL_DVFS_REASON_MPC			0x0002u /* MPC */
#define WL_DVFS_REASON_TX_ACTIVE		0x0004u /* TX Active */
#define WL_DVFS_REASON_DBGST_ACTIVE		0x0008u /* Power state active */
#define WL_DVFS_REASON_DBGST_ASLEEP		0x0010u /* Power state asleep */
#define WL_DVFS_REASON_LTR_ACTIVE		0x0020u /* LTR Active */
#define WL_DVFS_REASON_HOST			0x0040u /* Host disabled */
#define WL_DVFS_REASON_SCAN			0x0080u /* Scan */
#define WL_DVFS_REASON_SLOTTED_BSS		0x0100u /* Slotted BSS */
#define WL_DVFS_REASON_CHAN			0x0200u /* Channel Change */
#define WL_DVFS_REASON_CAL			0x0400u /* CAL */
#define WL_DVFS_REASON_ASSOC			0x0800u /* ASSOC */
#define WL_DVFS_REASON_WD			0x1000u /* WD */
#define WL_DVFS_REASON_SOFTAP			0x2000u /* SoftAP */
#define WL_DVFS_REASON_PHYBW			0x4000u /* Channel BW Change */
#define WL_DVFS_REASON_MCHAN_ACTIVE		0x8000u /* Mchan Active */
#define WL_DVFS_REASON_MONITOR			0x10000u /* Monitor Mode */
#define WL_DVFS_REASON_COEX			0x20000u /* BT COEX  */

/*
 * Join preference iovar value is an array of tuples. Each tuple has a one-byte type,
 * a one-byte length, and a variable length value.  RSSI type tuple must be present
 * in the array.
 *
 * Types are defined in "join preference types" section.
 *
 * Length is the value size in octets. It is reserved for WL_JOIN_PREF_WPA type tuple
 * and must be set to zero.
 *
 * Values are defined below.
 *
 * 1. RSSI - 2 octets
 * offset 0: reserved
 * offset 1: reserved
 *
 * 2. WPA - 2 + 12 * n octets (n is # tuples defined below)
 * offset 0: reserved
 * offset 1: # of tuples
 * offset 2: tuple 1
 * offset 14: tuple 2
 * ...
 * offset 2 + 12 * (n - 1) octets: tuple n
 *
 * struct wpa_cfg_tuple {
 *   uint8 akm[DOT11_OUI_LEN+1];     akm suite
 *   uint8 ucipher[DOT11_OUI_LEN+1]; unicast cipher suite
 *   uint8 mcipher[DOT11_OUI_LEN+1]; multicast cipher suite
 * };
 *
 * multicast cipher suite can be specified as a specific cipher suite or WL_WPA_ACP_MCS_ANY.
 *
 * 3. BAND - 2 octets
 * offset 0: reserved
 * offset 1: see "band preference" and "band types"
 *
 * 4. BAND RSSI - 2 octets
 * offset 0: band types
 * offset 1: +ve RSSI boost value in dB
 */

struct tsinfo_arg {
	uint8 octets[3];
};

#define RATE_CCK_1MBPS 0
#define RATE_CCK_2MBPS 1
#define RATE_CCK_5_5MBPS 2
#define RATE_CCK_11MBPS 3

#define RATE_LEGACY_OFDM_6MBPS 0
#define RATE_LEGACY_OFDM_9MBPS 1
#define RATE_LEGACY_OFDM_12MBPS 2
#define RATE_LEGACY_OFDM_18MBPS 3
#define RATE_LEGACY_OFDM_24MBPS 4
#define RATE_LEGACY_OFDM_36MBPS 5
#define RATE_LEGACY_OFDM_48MBPS 6
#define RATE_LEGACY_OFDM_54MBPS 7

#define WL_BSSTRANS_RSSI_RATE_MAP_VERSION_V1	1
#define WL_BSSTRANS_RSSI_RATE_MAP_VERSION_V2	2
#define WL_BSSTRANS_RSSI_RATE_MAP_VERSION_V3	3
#define WL_BSSTRANS_RSSI_RATE_MAP_VERSION_V4	4

#define WL_BSSTRANS_RSSI_VERSION_V1 1u

#ifdef WL_BAND6G
#define	MAXBANDS		3	/**< Maximum # of bands */
#else
#define	MAXBANDS		2	/**< Maximum # of bands */
#endif /* WL_BAND6G */

typedef struct wl_bsstrans_rssi_v1 {
	int8 rssi[MAXBANDS];	/**< RSSI in dbm */
} wl_bsstrans_rssi_v1_t;

typedef struct wl_bsstrans_rssi {
	int8 rssi_2g;	/**< RSSI in dbm for 2.4 G */
	int8 rssi_5g;	/**< RSSI in dbm for 5G, unused for cck */
} wl_bsstrans_rssi_t;

#define RSSI_RATE_MAP_MAX_STREAMS 4	/**< max streams supported */


/** RSSI to rate mapping, all 20Mhz, no SGI */
typedef struct wl_bsstrans_rssi_rate_map_v4 {
	uint16 ver;
	uint16 len; /**< length of entire structure */
	wl_bsstrans_rssi_v1_t cck[WL_NUM_RATES_CCK]; /**< 2.4G only */
	wl_bsstrans_rssi_v1_t ofdm[WL_NUM_RATES_OFDM]; /**< 6 to 54mbps */
	wl_bsstrans_rssi_v1_t phy_n[RSSI_RATE_MAP_MAX_STREAMS][WL_NUM_RATES_MCS_1STREAM]; /* MCS7 */
	wl_bsstrans_rssi_v1_t phy_ac[RSSI_RATE_MAP_MAX_STREAMS][WL_NUM_RATES_VHT_ALL]; /* MCS0-11 */
	wl_bsstrans_rssi_v1_t phy_ax[RSSI_RATE_MAP_MAX_STREAMS][WL_NUM_RATES_HE]; /* MCS0-11 */
	wl_bsstrans_rssi_v1_t phy_be[RSSI_RATE_MAP_MAX_STREAMS][WL_NUM_RATES_EHT]; /* MCS0-15 */
} wl_bsstrans_rssi_rate_map_v4_t;

/** RSSI to rate mapping, all 20Mhz, no SGI */
typedef struct wl_bsstrans_rssi_rate_map_v3 {
	uint16 ver;
	uint16 len; /**< length of entire structure */
	wl_bsstrans_rssi_t cck[WL_NUM_RATES_CCK]; /**< 2.4G only */
	wl_bsstrans_rssi_t ofdm[WL_NUM_RATES_OFDM]; /**< 6 to 54mbps */
	wl_bsstrans_rssi_t phy_n[RSSI_RATE_MAP_MAX_STREAMS][WL_NUM_RATES_MCS_1STREAM]; /* MCS0-7 */
	wl_bsstrans_rssi_t phy_ac[RSSI_RATE_MAP_MAX_STREAMS][WL_NUM_RATES_VHT_ALL];    /* MCS0-11 */
	wl_bsstrans_rssi_t phy_ax[RSSI_RATE_MAP_MAX_STREAMS][WL_NUM_RATES_HE];         /* MCS0-11 */
	wl_bsstrans_rssi_t phy_be[RSSI_RATE_MAP_MAX_STREAMS][WL_NUM_RATES_EHT];        /* MCS0-15 */
} wl_bsstrans_rssi_rate_map_v3_t;

/** RSSI to rate mapping, all 20Mhz, no SGI */
typedef struct wl_bsstrans_rssi_rate_map_v2 {
	uint16 ver;
	uint16 len; /**< length of entire structure */
	wl_bsstrans_rssi_t cck[WL_NUM_RATES_CCK]; /**< 2.4G only */
	wl_bsstrans_rssi_t ofdm[WL_NUM_RATES_OFDM]; /**< 6 to 54mbps */
	wl_bsstrans_rssi_t phy_n[RSSI_RATE_MAP_MAX_STREAMS][WL_NUM_RATES_MCS_1STREAM]; /* MCS0-7 */
	wl_bsstrans_rssi_t phy_ac[RSSI_RATE_MAP_MAX_STREAMS][WL_NUM_RATES_VHT_ALL]; /**< MCS0-11 */
	wl_bsstrans_rssi_t phy_ax[RSSI_RATE_MAP_MAX_STREAMS][WL_NUM_RATES_HE]; /**< MCS0-11 */
} wl_bsstrans_rssi_rate_map_v2_t;

/** RSSI to rate mapping, all 20Mhz, no SGI */
typedef struct wl_bsstrans_rssi_rate_map_v1 {
	uint16 ver;
	uint16 len; /**< length of entire structure */
	wl_bsstrans_rssi_t cck[WL_NUM_RATES_CCK]; /**< 2.4G only */
	wl_bsstrans_rssi_t ofdm[WL_NUM_RATES_OFDM]; /**< 6 to 54mbps */
	wl_bsstrans_rssi_t phy_n[RSSI_RATE_MAP_MAX_STREAMS][WL_NUM_RATES_MCS_1STREAM]; /* MCS0-7 */
	wl_bsstrans_rssi_t phy_ac[RSSI_RATE_MAP_MAX_STREAMS][WL_NUM_RATES_VHT]; /**< MCS0-9 */
} wl_bsstrans_rssi_rate_map_v1_t;

/** RSSI to rate mapping, all 20Mhz, no SGI */
typedef struct wl_bsstrans_rssi_rate_map {
	uint16 ver;
	uint16 len; /**< length of entire structure */
	wl_bsstrans_rssi_t cck[WL_NUM_RATES_CCK]; /**< 2.4G only */
	wl_bsstrans_rssi_t ofdm[WL_NUM_RATES_OFDM]; /**< 6 to 54mbps */
	wl_bsstrans_rssi_t phy_n[RSSI_RATE_MAP_MAX_STREAMS][WL_NUM_RATES_MCS_1STREAM]; /* MCS0-7 */
	wl_bsstrans_rssi_t phy_ac[RSSI_RATE_MAP_MAX_STREAMS][WL_NUM_RATES_VHT]; /**< MCS0-9 */
} wl_bsstrans_rssi_rate_map_t;

#define WL_CCA_CHAN_LOAD_VERSION_1	 1
#define CCA_LOAD_MAP_MAX_TABLE_ENTRY 16u

typedef struct wl_cca_chnl_load {
	uint8 cca; /* CCA value */
	uint8 chnl_load; /* Channel load percent */
} wl_cca_chnl_load_t;

/* CCA to Channel Load mapping  */
typedef struct wl_cca_chnl_load_map {
	uint16 ver;
	uint16 len; /**<  No of cca load levels */
	wl_cca_chnl_load_t cca_load[CCA_LOAD_MAP_MAX_TABLE_ENTRY];
} wl_cca_chnl_load_map_t;

#define WL_BSSTRANS_ROAMTHROTTLE_VERSION_1 1

/** Configure number of scans allowed per throttle period */
typedef struct wl_bsstrans_roamthrottle {
	uint16 ver;
	uint16 period;
	uint16 scans_allowed;
} wl_bsstrans_roamthrottle_t;

/* tlv IDs uniquely identifies periodic state component */
enum wl_periodic_slice_state_xtlv_id {
	WL_STATE_COMPACT_COUNTERS = 0x1,
	WL_STATE_TXBF_COUNTERS = 0x2,
	WL_STATE_COMPACT_HE_COUNTERS = 0x3
};

/* tlv IDs uniquely identifies periodic state component */
enum wl_periodic_if_state_xtlv_id {
	WL_STATE_IF_COMPACT_STATE = 0x1,
	WL_STATE_IF_ADPS_STATE = 0x02,
	WL_STATE_IF_ADPS_ENERGY_GAIN = 0x03
};

enum wl_periodic_tdls_if_state_xtlv_id {
	WL_STATE_IF_TDLS_STATE = 0x1
};

#define TDMTX_CNT_VERSION_V1      1
#define TDMTX_CNT_VERSION_V2      2

/* structure holding tdm counters that interface to iovar */
typedef struct tdmtx_cnt_v1 {
	uint16 ver;
	uint16 length; /* length of this structure */
	uint16 wlc_idx; /* index for wlc */
	uint16 enabled; /* tdmtx is enabled on slice */
	uint32 tdmtx_txa_on; /* TXA on requests */
	uint32 tdmtx_txa_tmcnt; /* Total number of TXA timeout */
	uint32 tdmtx_por_on; /* TXA POR requests */
	uint32 tdmtx_txpuen; /* Path enable requests */
	uint32 tdmtx_txpudis; /* Total number of times Tx path is muted on the slice */
	uint32 tdmtx_txpri_on; /* Total number of times Tx priority was obtained by the slice */
	uint32 tdmtx_txdefer; /* Total number of times Tx was deferred on the slice */
	uint32 tdmtx_txmute; /* Total number of times active Tx muted on the slice */
	uint32 tdmtx_actpwrboff; /* Total number of times TX power is backed off by the slice */
	uint32 tdmtx_txa_dur; /* Total time txa on */
	uint32 tdmtx_txpri_dur; /* Total time TXPri */
	uint32 tdmtx_txdefer_dur; /* Total time txdefer */
	/* TDMTX input fields */
	uint32 tdmtx_txpri;
	uint32 tdmtx_defer;
	uint32 tdmtx_threshold;
	uint32 tdmtx_rssi_threshold;
	uint32 tdmtx_txpwrboff;
	uint32 tdmtx_txpwrboff_dt;
} tdmtx_cnt_v1_t;

typedef struct {
	uint16  ver;
	uint16	length; /* length of the data portion */
	uint16  cnt;
	uint16	PAD; /* pad to align to 32 bit */
	uint8   data[]; /* array of tdmtx_cnt_v1_t */
} tdmtx_status_t;

/* structure holding counters that match exactly shm field sizes */
typedef struct tdmtx_cnt_shm_v1 {
	uint16 tdmtx_txa_on; /* TXA on requests */
	uint16 tdmtx_tmcnt; /* TXA on requests */
	uint16 tdmtx_por_on; /* TXA POR requests */
	uint16 tdmtx_txpuen; /* Path enable requests */
	uint16 tdmtx_txpudis; /* Total number of times Tx path is muted on the slice */
	uint16 tdmtx_txpri_on; /* Total number of times Tx priority was obtained by the slice */
	uint16 tdmtx_txdefer; /* Total number of times Tx was defered by the slice */
	uint16 tdmtx_txmute; /* Total number of times active Tx muted on the slice */
	uint16 tdmtx_actpwrboff; /* Total number of times TX power is backed off by the slice */
	uint16 tdmtx_txa_dur_l; /* Total time (low 16 bits) txa on */
	uint16 tdmtx_txa_dur_h; /* Total time (low 16 bits) txa on */
	uint16 tdmtx_txpri_dur_l; /* Total time (low 16 bits) TXPri */
	uint16 tdmtx_txpri_dur_h; /* Total time (high 16 bits) TXPri */
	uint16 tdmtx_txdefer_dur_l; /* Total time (low 16 bits)  txdefer */
	uint16 tdmtx_txdefer_dur_h; /* Total time (high 16 bits) txdefer */
} tdmtx_cnt_shm_v1_t;


/* structure holding tdm counters that interface to iovar for version 2 */
typedef struct tdmtx_cnt_v2 {
	uint16 ver;
	uint16 length; /* length of this structure */
	uint16 wlc_idx; /* index for wlc */
	uint16 enabled; /* tdmtx is enabled on slice */
	uint32 tdmtx_txa_on; /* TXA on requests */
	uint32 tdmtx_txa_tmcnt; /* Total number of TXA timeout */
	uint32 tdmtx_porhi_on; /* TXA PORHI requests */
	uint32 tdmtx_porlo_on; /* TXA PORLO requests */
	uint32 tdmtx_txpuen; /* Path enable requests */
	uint32 tdmtx_txpudis; /* Total number of times Tx path is muted on the slice */
	uint32 tdmtx_txpri_on; /* Total number of times Tx priority was obtained by the slice */
	uint32 tdmtx_txdefer; /* Total number of times Tx was deferred on the slice */
	uint32 tdmtx_txmute; /* Total number of times active Tx muted on the slice */
	uint32 tdmtx_actpwrboff; /* Total number of times TX power is backed off by the slice */
	uint32 tdmtx_txa_dur; /* Total time txa on */
	uint32 tdmtx_txpri_dur; /* Total time TXPri */
	uint32 tdmtx_txdefer_dur; /* Total time txdefer */
	/* TDMTX input fields */
	uint32 tdmtx_txpri;
	uint32 tdmtx_defer;
	uint32 tdmtx_threshold;
	uint32 tdmtx_rssi_threshold;
	uint32 tdmtx_txpwrboff;
	uint32 tdmtx_txpwrboff_dt;
} tdmtx_cnt_v2_t;

/* structure holding counters that match exactly shm field sizes */
typedef struct tdmtx_cnt_shm_v2 {
	uint16 tdmtx_txa_on; /* TXA on requests */
	uint16 tdmtx_tmcnt; /* TXA on requests */
	uint16 tdmtx_porhi_on; /* TXA PORHI requests */
	uint16 tdmtx_porlo_on; /* TXA PORLO requests */
	uint16 tdmtx_txpuen; /* Path enable requests */
	uint16 tdmtx_txpudis; /* Total number of times Tx path is muted on the slice */
	uint16 tdmtx_txpri_on; /* Total number of times Tx priority was obtained by the slice */
	uint16 tdmtx_txdefer; /* Total number of times Tx was defered by the slice */
	uint16 tdmtx_txmute; /* Total number of times active Tx muted on the slice */
	uint16 tdmtx_actpwrboff; /* Total number of times TX power is backed off by the slice */
	uint16 tdmtx_txa_dur_l; /* Total time (low 16 bits) txa on */
	uint16 tdmtx_txa_dur_h; /* Total time (low 16 bits) txa on */
	uint16 tdmtx_txpri_dur_l; /* Total time (low 16 bits) TXPri */
	uint16 tdmtx_txpri_dur_h; /* Total time (high 16 bits) TXPri */
	uint16 tdmtx_txdefer_dur_l; /* Total time (low 16 bits)  txdefer */
	uint16 tdmtx_txdefer_dur_h; /* Total time (high 16 bits) txdefer */
} tdmtx_cnt_shm_v2_t;

typedef struct wl_tdmtx_ioc {
	uint16 id;	/* ID of the sub-command */
	uint16 len;	/* total length of all data[] */
	uint8  data[];	/* var len payload */
} wl_tdmtx_ioc_t;

/*
 * iovar subcommand ids
 */
enum {
	IOV_TDMTX_ENB = 1,
	IOV_TDMTX_STATUS = 2,
	IOV_TDMTX_TXPRI = 3,
	IOV_TDMTX_DEFER = 4,
	IOV_TDMTX_TXA = 5,
	IOV_TDMTX_CFG = 6,
	IOV_TDMTX_LAST
};


/* SFLASH iovar sub commands */
#define WL_SFLASH_IOV_VERSION_V1 1u

enum wl_sflash_subcmd_id {
	WL_SFLASH_SUBCMD_CALWRITE = 0,
	WL_SFLASH_SUBCMD_CALERASE = 1u,
	WL_SFLASH_SUBCMD_WRITE = 2u,
	WL_SFLASH_SUBCMD_ERASE = 3u,
	WL_SFLASH_SUBCMD_DUMP = 4u,
	WL_SFLASH_SUBCMD_DATASIZE = 5u,
	WL_SFLASH_SUBCMD_SIZE = 6u,
	WL_SFLASH_SUBCMD_READ = 7u
};

struct wl_msglevel2 {
	uint32 low;
	uint32 high;
};

/* A versioned structure for setting and retrieving debug message levels. */
#define WL_MSGLEVEL_STRUCT_VERSION_1	1

typedef struct wl_msglevel_v1 {
	uint16 version;
	uint16 length;
	uint32 msglevel1;
	uint32 msglevel2;
	uint32 msglevel3;
	/* add another uint32 when full */
} wl_msglevel_v1_t;

#define WL_ICMP_IPV6_CFG_VERSION_1       1u
#define WL_ICMP_IPV6_CLEAR_ALL           (1u << 0u)

typedef struct wl_icmp_ipv6_cfg {
	uint16 version;
	uint16 length;
	uint16 fixed_length;
	uint16 flags;
	uint32 num_ipv6;
	/* num_ipv6 to follow */
	struct ipv6_addr host_ipv6[];
} wl_icmp_ipv6_cfg_t;

#define WL_ICMP_CFG_IPV6_FIXED_LEN      OFFSETOF(wl_icmp_ipv6_cfg_t, host_ipv6)
#define WL_ICMP_CFG_IPV6_LEN(count)    (WL_ICMP_CFG_IPV6_FIXED_LEN + \
					((count) * sizeof(struct ipv6_addr)))

typedef struct wl_mkeep_alive_pkt_v1 {
	uint16	version; /* Version for mkeep_alive */
	uint16	length; /* length of fixed parameters in the structure */
	uint32	period_msec; /* high bit on means immediate send */
	uint16	len_bytes;
	uint8	keep_alive_id; /* 0 - 3 for N = 4 */
	uint8	data[BCM_FLEX_ARRAY];
} wl_mkeep_alive_pkt_v1_t;

#define WL_MKEEP_ALIVE_VERSION_1	1u
#define WL_MKEEP_ALIVE_VERSION_2	2u
/* 1/2 second precision since idle time is a seconds counter anyway */
#define WL_MKEEP_ALIVE_PRECISION	500
#define WL_MKEEP_ALIVE_PERIOD_MASK  0x7FFFFFFF
#define WL_MKEEP_ALIVE_IMMEDIATE    0x80000000


#define WL_MKEEP_ALIVE_FIXED_LEN	OFFSETOF(wl_mkeep_alive_pkt_v1_t, data)

typedef struct wl_mkeep_alive_pkt_v2 {
	uint16	version;	/* Version for mkeep_alive */
	uint16	length;		/* length of fixed parameters in the structure */
	uint32	period_msec;	/* high bit on means immediate send */
	uint16	len_bytes;
	uint16	retry_cnt;	/* keep alive alive pkt retry count */
	uint8	PAD[2];
	uint8	keep_alive_id;	/* 0 - 3 for N = 4 */
	uint8	data[BCM_FLEX_ARRAY];
} wl_mkeep_alive_pkt_v2_t;


typedef struct wl_mkeep_alive_hist_info_v1 {
	uint32 first_pktsend_ts;	/**< timestamp(ms): packet was sent */
	uint32 first_txs_ts;		/**< timestamp(ms): received the first txstatus */
	uint32 last_retry_txs_ts;	/**< timestamp(ms): received the last txstatus */
	uint32 first_retry_ts;		/**< timestamp(ms): resent the packet first time */
	uint32 last_retry_ts;		/**< timestamp(ms): resent the packet last time */
	uint32 first_txs;		/**< txstatus when dongle received first time */
	uint32 last_retry_txs;		/**< txstatus when dongle received last time */
	uint32 retry_cnt;		/**< number of retries for the packet */
} wl_mkeep_alive_hist_info_v1_t;

typedef struct wl_mkeep_alive_hist_req_v1 {
	uint16 version;		/**< version of structure */
	uint16 length;		/**< length of this structure */
	uint16 flags;		/**< mkeepalive idx, operation codes */
	uint16 count;		/**< number of results */
	uint16 max;		/**< maximum number of history */
	uint16 PAD;
	wl_mkeep_alive_hist_info_v1_t info[]; /**< struct array of length count */
} wl_mkeep_alive_hist_req_v1_t;

/* version of the mkeep_alive_hist IOVAR */
#define WL_MKEEP_ALIVE_HIST_REQ_VER_1		1u
/* Fixed length of wl_mkeep_alive_hist_req_v1_t */
#define WL_MKEEP_ALIVE_HIST_REQ_FIXED_LEN_VER_1	OFFSETOF(wl_mkeep_alive_hist_req_v1_t, info)
/* Keepalive ID */
#define WL_MKEEP_ALIVE_HIST_ID_MASK		0xFF00u	/**< ID mask */
#define WL_MKEEP_ALIVE_HIST_ID_SHIFT		8u	/**< Offset of keepalive ID */
/* OP Codes */
#define WL_MKEEP_ALIVE_HIST_OP_MASK		0x00FFu	/**< OP code mask */
#define WL_MKEEP_ALIVE_HIST_RESET		(0x1u << 0u) /**< Clear history of specified ID */
#define WL_MKEEP_ALIVE_HIST_RESET_ALL		(0x1u << 1u) /**< Clear all history */

/** TCP Keep-Alive conn struct */
typedef struct wl_mtcpkeep_alive_conn_pkt {
	struct ether_addr saddr;		/**< src mac address */
	struct ether_addr daddr;		/**< dst mac address */
	struct ipv4_addr sipaddr;		/**< source IP addr */
	struct ipv4_addr dipaddr;		/**< dest IP addr */
	uint16 sport;				/**< src port */
	uint16 dport;				/**< dest port */
	uint32 seq;				/**< seq number */
	uint32 ack;				/**< ACK number */
	uint16 tcpwin;				/**< TCP window */
	uint16 PAD;
} wl_mtcpkeep_alive_conn_pkt_t;

/** TCP Keep-Alive interval struct */
typedef struct wl_mtcpkeep_alive_timers_pkt {
	uint16 interval;		/**< interval timer */
	uint16 retry_interval;		/**< retry_interval timer */
	uint16 retry_count;		/**< retry_count */
} wl_mtcpkeep_alive_timers_pkt_t;

typedef struct wake_info {
	uint32 wake_reason;
	uint32 wake_info_len;		/**< size of packet */
	uint8  packet[];
} wake_info_t;

typedef struct wake_pkt {
	uint32 wake_pkt_len;		/**< size of packet */
	uint8  packet[];
} wake_pkt_t;


#define WL_MTCPKEEP_ALIVE_VERSION		1

/** structure for per-tid ampdu control */
struct ampdu_tid_control {
	uint8 tid;			/* tid */
	uint8 enable;			/* enable/disable */
};

/** Support for ampdu_tx_ba_window_cfg */
#define WL_AMPDU_TX_BA_WINDOW_CFG_VER_1		1u

/* 16 bits Config (5 bits reserved) and Status (2 bits reserved) */
#define WL_AMPDU_TX_BA_WINDOW_CFG_BA_WSIZE_IDX		0u
#define WL_AMPDU_TX_BA_WINDOW_CFG_BA_WSIZE_FSZ		9u

#define WL_AMPDU_TX_BA_WINDOW_CFG_ASSOC_TYPE_IDX	9u
#define WL_AMPDU_TX_BA_WINDOW_CFG_ASSOC_TYPE_FSZ	2u

#define WL_AMPDU_TX_BA_WINDOW_CFG_STATE_IDX		11u
#define WL_AMPDU_TX_BA_WINDOW_CFG_STATE_FSZ		3u

#define WL_AMPDU_TX_BA_WINDOW_CFG_BA_WSIZE_MASK		\
	(MAXBITVAL(WL_AMPDU_TX_BA_WINDOW_CFG_BA_WSIZE_FSZ) << \
		WL_AMPDU_TX_BA_WINDOW_CFG_BA_WSIZE_IDX)

#define WL_AMPDU_TX_BA_WINDOW_CFG_ASSOC_TYPE_MASK	\
	(MAXBITVAL(WL_AMPDU_TX_BA_WINDOW_CFG_ASSOC_TYPE_FSZ) << \
		WL_AMPDU_TX_BA_WINDOW_CFG_ASSOC_TYPE_IDX)

#define WL_AMPDU_TX_BA_WINDOW_CFG_STATE_MASK	\
	(MAXBITVAL(WL_AMPDU_TX_BA_WINDOW_CFG_STATE_FSZ) << \
		WL_AMPDU_TX_BA_WINDOW_CFG_STATE_IDX)

/* code for config assoc_type */
enum {
	WL_AMPDU_TX_BA_WINDOW_CFG_ASSOC_TYPE_AX			= 0,
	WL_AMPDU_TX_BA_WINDOW_CFG_ASSOC_TYPE_UNIVERSAL		= 1,
	WL_AMPDU_TX_BA_WINDOW_CFG_ASSOC_TYPE_MAX		= 2
};

/* ampdu_tx_ba_window_cfg states */
enum {
	WL_AMPDU_TX_BA_WINDOW_CFG_STATE_OFF			= 0,
	WL_AMPDU_TX_BA_WINDOW_CFG_STATE_NEGOTIATING		= 1,
	WL_AMPDU_TX_BA_WINDOW_CFG_STATE_NEGOTIATED		= 2,
	WL_AMPDU_TX_BA_WINDOW_CFG_STATE_MAX			= 3
};

/** structure for per tid ampdu BA window configuration */
typedef struct wl_ampdu_tx_ba_window_cfg_v1 {
	uint16	version;
	uint16	length;		/* length of the entire structure ver+len+payload. */
	/* tid bitmap:
	 * input (SET): select tid to configure.
	 * output (GET): TID that is currently configured.
	 */
	uint8   tidbmp;
	uint8	flag;		/* currently not used. Reserved. 32-bit alignment. */
	uint16	PAD;

	/* Per-tid configuration tuple (tx_ba_wsize, assoctype). Used for GET and SET.
	 *	bit0 - bit8:	User configured TX BA window size. Range {0, max. FW supported}.
	 *	bit9 - bit10:	User configured association type. 0: 11ax association, 1: universal.
	 *	bit11 - bit15:	Reserved.
	 */
	uint16	config[NUMPRIO];

	/* Status of the per-tid configuration: GET only
	 *	bit0 - bit8:	Resulted TX BA window size.
	 *	bit9 - bit10:	Reserved.
	 *	bit11 - bit13:	TX BA configuration state.
	 *	bit14 - bit15:	Reserved.
	 */
	uint16	status[NUMPRIO];
} wl_ampdu_tx_ba_window_cfg_v1_t;

/** Support for ampdu_rx_ba_window_cfg */
#define WL_AMPDU_RX_BA_WINDOW_CFG_VER_1                1u

/** structure for per tid ampdu BA window configuration */
typedef struct wl_ampdu_rx_ba_window_cfg_v1 {
	uint16  version;
	uint16  length;         /* length of the entire structurer+len+payload. */
	/* tid bitmap:0-7 bit 0 is zero TID and follow on.
	 * input (SET): select tid to configure.
	 */
	uint8   tidbmp;
	uint8   PAD[3];

	/*  Per-tid configuration. Used for GET and SET.
	 *  * User configured RX BA window size. Range {0, max. FW supported}.
	 */
	uint16 ba_wsize[NUMPRIO];
} wl_ampdu_rx_ba_window_cfg_v1_t;

/** struct for ampdu tx/rx aggregation control */
struct ampdu_aggr {
	int8 aggr_override;	/**< aggr overrided by dongle. Not to be set by host. */
	uint16 conf_TID_bmap;	/**< bitmap of TIDs to configure */
	uint16 enab_TID_bmap;	/**< enable/disable per TID */
};

/** structure for identifying ea/tid for sending addba/delba */
struct ampdu_ea_tid {
	struct ether_addr ea;		/**< Station address */
	uint8 tid;			/**< tid */
	uint8 initiator;	/**< 0 is recipient, 1 is originator */
};

/** structure for identifying retry/tid for retry_limit_tid/rr_retry_limit_tid */
struct ampdu_retry_tid {
	uint8 tid;	/**< tid */
	uint8 retry;	/**< retry value */
};

struct ampdu_timeout_tid {
	uint8 timeout[NUMPRIO];	/* Timeout per TID */
	uint8 bitmap;		/* Bitmap for each TID */
};

#define BDD_FNAME_LEN       32  /**< Max length of friendly name */
typedef struct bdd_fname {
	uint8 len;          /**< length of friendly name */
	uchar name[BDD_FNAME_LEN];  /**< friendly name */
} bdd_fname_t;

/* structure for addts arguments */
/** For ioctls that take a list of TSPEC */
struct tslist {
	int32 count;			/**< number of tspecs */
	struct tsinfo_arg tsinfo[];	/**< variable length array of tsinfo */
};

/* WLTDLS */
/**structure for tdls iovars */
typedef struct tdls_iovar {
	struct ether_addr ea;		/**< Station address */
	uint8 mode;			/**< mode: depends on iovar */
	uint8 PAD;
	chanspec_t chanspec;
	uint8 PAD[6];
} tdls_iovar_t;

#define TDLS_WFD_IE_SIZE		512
/**structure for tdls wfd ie */
typedef struct tdls_wfd_ie_iovar {
	struct ether_addr ea;		/**< Station address */
	uint8 mode;
	uint8 PAD;
	uint16 length;
	uint8 data[TDLS_WFD_IE_SIZE];
} tdls_wfd_ie_iovar_t;
/* #endif WLTDLS */

/** structure for addts/delts arguments */
typedef struct tspec_arg {
	uint16 version;			/**< see definition of TSPEC_ARG_VERSION */
	uint16 length;			/**< length of entire structure */
	uint32 flag;			/**< bit field */
	/* TSPEC Arguments */
	struct tsinfo_arg tsinfo;	/**< TS Info bit field */
	uint8  PAD;
	uint16 nom_msdu_size;		/**< (Nominal or fixed) MSDU Size (bytes) */
	uint16 max_msdu_size;		/**< Maximum MSDU Size (bytes) */
	uint32 min_srv_interval;		/**< Minimum Service Interval (us) */
	uint32 max_srv_interval;		/**< Maximum Service Interval (us) */
	uint32 inactivity_interval;	/**< Inactivity Interval (us) */
	uint32 suspension_interval;	/**< Suspension Interval (us) */
	uint32 srv_start_time;		/**< Service Start Time (us) */
	uint32 min_data_rate;		/**< Minimum Data Rate (bps) */
	uint32 mean_data_rate;		/**< Mean Data Rate (bps) */
	uint32 peak_data_rate;		/**< Peak Data Rate (bps) */
	uint32 max_burst_size;		/**< Maximum Burst Size (bytes) */
	uint32 delay_bound;		/**< Delay Bound (us) */
	uint32 min_phy_rate;		/**< Minimum PHY Rate (bps) */
	uint16 surplus_bw;		/**< Surplus Bandwidth Allowance (range 1.0 to 8.0) */
	uint16 medium_time;		/**< Medium Time (32 us/s periods) */
	uint8 dialog_token;		/**< dialog token */
	uint8  PAD[3];
} tspec_arg_t;

/** tspec arg for desired station */
typedef	struct tspec_per_sta_arg {
	struct ether_addr ea;
	uint8  PAD[2];
	struct tspec_arg ts;
} tspec_per_sta_arg_t;

/** structure for max bandwidth for each access category */
typedef	struct wme_max_bandwidth {
	uint32	ac[AC_COUNT];	/**< max bandwidth for each access category */
} wme_max_bandwidth_t;

#define WL_WME_MBW_PARAMS_IO_BYTES (sizeof(wme_max_bandwidth_t))

/* current version of wl_tspec_arg_t struct */
#define	TSPEC_ARG_VERSION		2	/**< current version of wl_tspec_arg_t struct */
#define TSPEC_ARG_LENGTH		55	/**< argument length from tsinfo to medium_time */
#define TSPEC_DEFAULT_DIALOG_TOKEN	42	/**< default dialog token */
#define TSPEC_DEFAULT_SBW_FACTOR	0x3000	/**< default surplus bw */


#define WL_WOWL_KEEPALIVE_MAX_PACKET_SIZE  80
#define WLC_WOWL_MAX_KEEPALIVE	2

/** Packet lifetime configuration per ac */
typedef struct wl_lifetime {
	uint32 ac;	        /**< access class */
	uint32 lifetime;    /**< Packet lifetime value in ms */
} wl_lifetime_t;

/** Management time configuration */
typedef struct wl_lifetime_mg {
	uint32 mgmt_bitmap;	/**< Mgmt subtype */
	uint32 lifetime;    /**< Packet lifetime value in us */
} wl_lifetime_mg_t;

/* MAC Sample Capture related */
#define	WL_MACCAPTR_DEFSTART_PTR	0xA00
#define	WL_MACCAPTR_DEFSTOP_PTR		0xA3F
#define	WL_MACCAPTR_DEFSZ		0x3F

#define WL_MACCAPTR_DEF_MASK		0xFFFFFFFF

typedef enum {
	WL_MACCAPT_TRIG		= 0,
	WL_MACCAPT_STORE	= 1,
	WL_MACCAPT_TRANS	= 2,
	WL_MACCAPT_MATCH	= 3
} maccaptr_optn;

typedef enum {
	WL_MACCAPT_STRT	= 1,
	WL_MACCAPT_STOP	= 2,
	WL_MACCAPT_RST	= 3
} maccaptr_cmd_t;

/* MAC Sample Capture Set-up Paramters */
typedef struct wl_maccapture_params {
	uint8	gpio_sel;
	uint8	la_mode;	/* TRUE: GPIO Out Enabled */
	uint8	PAD[2];
	uint32	start_ptr;	/* Start address to store */
	uint32	stop_ptr;	/* Stop address to store */
	uint8	optn_bmp;	/* Options */
	uint8	PAD[3];
	/* Don't change the order after this nor
	 * add anything in betw. Code uses offsets to populate
	 * registers
	 */
	uint32	tr_mask;	/* Trigger Mask */
	uint32	tr_val;		/* Trigger Value */
	uint32	s_mask;		/* Store Mode Mask */
	uint32	x_mask;		/* Trans. Mode Mask */
	uint32	m_mask;		/* Match Mode Mask */
	uint32	m_val;		/* Match Value */
	maccaptr_cmd_t cmd;	/* Start / Stop */
} wl_maccapture_params_t;

/** Channel Switch Announcement param */
typedef struct wl_chan_switch {
	uint8 mode;		/**< value 0 or 1 */
	uint8 count;		/**< count # of beacons before switching */
	chanspec_t chspec;	/**< chanspec */
	uint8 reg;		/**< regulatory class */
	uint8 frame_type;		/**< csa frame type, unicast or broadcast */
} wl_chan_switch_t;

enum {
	PFN_LIST_ORDER,
	PFN_RSSI
};

enum {
	DISABLE,
	ENABLE
};

enum {
	OFF_ADAPT,
	SMART_ADAPT,
	STRICT_ADAPT,
	SLOW_ADAPT
};

#define SORT_CRITERIA_BIT		0
#define AUTO_NET_SWITCH_BIT		1
#define ENABLE_BKGRD_SCAN_BIT		2
#define IMMEDIATE_SCAN_BIT		3
#define	AUTO_CONNECT_BIT		4
#define	ENABLE_BD_SCAN_BIT		5
#define ENABLE_ADAPTSCAN_BIT		6
#define IMMEDIATE_EVENT_BIT		8
#define SUPPRESS_SSID_BIT		9
#define ENABLE_NET_OFFLOAD_BIT		10
/** report found/lost events for SSID and BSSID networks seperately */
#define REPORT_SEPERATELY_BIT		11
#define PFN_FULL_SCAN_RESULT_BIT	12

#define SORT_CRITERIA_MASK	0x0001
#define AUTO_NET_SWITCH_MASK	0x0002
#define ENABLE_BKGRD_SCAN_MASK	0x0004
#define IMMEDIATE_SCAN_MASK	0x0008
#define AUTO_CONNECT_MASK	0x0010

#define ENABLE_BD_SCAN_MASK	0x0020
#define ENABLE_ADAPTSCAN_MASK	0x00c0
#define IMMEDIATE_EVENT_MASK	0x0100
#define SUPPRESS_SSID_MASK	0x0200
#define ENABLE_NET_OFFLOAD_MASK	0x0400
/** report found/lost events for SSID and BSSID networks seperately */
#define REPORT_SEPARATELY_MASK	0x0800
#define REPORT_SEPERATELY_MASK	REPORT_SEPARATELY_MASK // Legacy typo
#define PFNFULLSCAN_RESULT_MASK	0x1000

#define PFN_COMPLETE			1
#define PFN_INCOMPLETE			0

#define DEFAULT_BESTN			2
#define DEFAULT_MSCAN			0
#define DEFAULT_REPEAT			10
#define DEFAULT_EXP			2

#define PFN_SWC_RSSI_WINDOW_MAX   8
#define PFN_SWC_MAX_NUM_APS       16
#define PFN_HOTLIST_MAX_NUM_APS   64

#define MAX_EPNO_HIDDEN_SSID    8
#define MAX_WHITELIST_SSID      2

#define WL_PFN_SHORTSSID_LEN			4u
#define WL_PFN_SUBNET_FLAGS_IS_SHORT_SSID	0x01u	/* subnet flags bit 0 */
#define WL_PFN_SUBNET_FLAGS_MATCH_SHORT_SSID	0x02u	/* subnet flags bit 1 */

/* Version 1 and 2 for various scan results structures defined below */
#define PFN_SCANRESULTS_VERSION_V1	1u
#define PFN_SCANRESULTS_VERSION_V2	2u
#define PFN_SCANRESULTS_VERSION_V3	3u
#define PFN_SCANRESULTS_VERSION_V4	4u
#define PFN_SCANRESULTS_VERSION_V5	5u

/** PFN network info structure */
typedef struct wl_pfn_subnet_info_v1 {
	struct ether_addr BSSID;
	uint8	channel; /**< channel number only */
	uint8	SSID_len;
	uint8	SSID[32];
} wl_pfn_subnet_info_v1_t;

typedef struct wl_pfn_subnet_info_v2 {
	struct ether_addr BSSID;
	uint8   channel; /**< channel number only */
	uint8   SSID_len;
	union {
		uint8   SSID[32];
		uint16 index;
	} u;
} wl_pfn_subnet_info_v2_t;

typedef struct wl_pfn_subnet_info_v3 {
	struct ether_addr BSSID;
	chanspec_t	chanspec; /**< with 6G chanspec only */
	uint8		SSID_len;
	uint8		PAD;
	uint16		flags;
	union {
		uint8   SSID[32];
		uint16 index;
	} u;
} wl_pfn_subnet_info_v3_t;

typedef struct wl_pfn_net_info_v1 {
	wl_pfn_subnet_info_v1_t pfnsubnet;
	int16	RSSI; /**< receive signal strength (in dBm) */
	uint16	timestamp; /**< age in seconds */
} wl_pfn_net_info_v1_t;

typedef struct wl_pfn_net_info_v2 {
	wl_pfn_subnet_info_v2_t pfnsubnet;
	int16   RSSI; /**< receive signal strength (in dBm) */
	uint16  timestamp; /**< age in seconds */
} wl_pfn_net_info_v2_t;

typedef struct wl_pfn_net_info_v3 {
	wl_pfn_subnet_info_v3_t pfnsubnet;
	int16   RSSI; /**< receive signal strength (in dBm) */
	uint16  timestamp; /**< age in seconds */
} wl_pfn_net_info_v3_t;

/* Version 1 and 2 for various lbest scan results structures below */
#define PFN_LBEST_SCAN_RESULT_VERSION_V1 1
#define PFN_LBEST_SCAN_RESULT_VERSION_V2 2
#define PFN_LBEST_SCAN_RESULT_VERSION_V3 3

#define MAX_CHBKT_PER_RESULT		4

typedef struct wl_pfn_lnet_info_v1 {
	wl_pfn_subnet_info_v1_t pfnsubnet; /**< BSSID + channel + SSID len + SSID */
	uint16	flags; /**< partial scan, etc */
	int16	RSSI; /**< receive signal strength (in dBm) */
	uint32	timestamp; /**< age in miliseconds */
	uint16	rtt0; /**< estimated distance to this AP in centimeters */
	uint16	rtt1; /**< standard deviation of the distance to this AP in centimeters */
} wl_pfn_lnet_info_v1_t;

typedef struct wl_pfn_lnet_info_v2 {
	wl_pfn_subnet_info_v2_t pfnsubnet; /**< BSSID + channel + SSID len + SSID */
	uint16  flags; /**< partial scan, etc */
	int16   RSSI; /**< receive signal strength (in dBm) */
	uint32  timestamp; /**< age in miliseconds */
	uint16  rtt0; /**< estimated distance to this AP in centimeters */
	uint16  rtt1; /**< standard deviation of the distance to this AP in centimeters */
} wl_pfn_lnet_info_v2_t;

typedef struct wl_pfn_lnet_info_v3 {
	wl_pfn_subnet_info_v3_t pfnsubnet; /**< BSSID + channel + SSID len + SSID */
	uint16  flags; /**< partial scan, etc */
	int16   RSSI; /**< receive signal strength (in dBm) */
	uint32  timestamp; /**< age in miliseconds */
	uint16  rtt0; /**< estimated distance to this AP in centimeters */
	uint16  rtt1; /**< standard deviation of the distance to this AP in centimeters */
} wl_pfn_lnet_info_v3_t;

typedef struct wl_pfn_lscanresults_v1 {
	uint32 version;
	uint32 status;
	uint32 count;
	wl_pfn_lnet_info_v1_t netinfo[BCM_FLEX_ARRAY];
} wl_pfn_lscanresults_v1_t;

typedef struct wl_pfn_lscanresults_v2 {
	uint32 version;
	uint16 status;
	uint16 count;
	uint32 scan_ch_buckets[MAX_CHBKT_PER_RESULT];
	wl_pfn_lnet_info_v2_t netinfo[BCM_FLEX_ARRAY];
} wl_pfn_lscanresults_v2_t;

typedef struct wl_pfn_lscanresults_v3 {
	uint32 version;
	uint16 status;
	uint16 count;
	uint32 scan_ch_buckets[MAX_CHBKT_PER_RESULT];
	wl_pfn_lnet_info_v3_t netinfo[BCM_FLEX_ARRAY];
} wl_pfn_lscanresults_v3_t;

/**this is used to report on 1-* pfn scan results */
typedef struct wl_pfn_scanresults_v1 {
	uint32 version;
	uint32 status;
	uint32 count;
	wl_pfn_net_info_v1_t netinfo[BCM_FLEX_ARRAY];
} wl_pfn_scanresults_v1_t;

typedef struct wl_pfn_scanresults_v2 {
	uint32 version;
	uint32 status;
	uint32 count;
	uint32 scan_ch_bucket;
	wl_pfn_net_info_v2_t netinfo[BCM_FLEX_ARRAY];
} wl_pfn_scanresults_v2_t;

typedef struct wl_pfn_scanresults_v3 {
	uint32 version;
	uint32 status;
	uint32 count;
	uint32 scan_ch_bucket;
	wl_pfn_net_info_v3_t netinfo[BCM_FLEX_ARRAY];
} wl_pfn_scanresults_v3_t;

#define WL_PFN_SCANRESULTS_SCAN_TYPE_HA		0u
#define WL_PFN_SCANRESULTS_SCAN_TYPE_LP		1u

/* In version 4, the status field is split between status and flags from version 2.
 * This does not include changes from version 3.
 */
typedef struct wl_pfn_scanresults_v4 {
	uint32 version;
	uint16 status;
	uint16 flags;
	uint32 count;
	uint32 scan_ch_bucket;
	wl_pfn_net_info_v2_t netinfo[BCM_FLEX_ARRAY];
} wl_pfn_scanresults_v4_t;

typedef struct wl_pfn_scanresults_v5 {
	uint16 version;
	uint16 length;
	uint16 status;
	uint16 flags;
	uint32 count;
	uint32 scan_ch_bucket;
	wl_pfn_net_info_v3_t netinfo[BCM_FLEX_ARRAY];
} wl_pfn_scanresults_v5_t;

typedef struct wl_pfn_significant_net {
	uint16 flags;
	uint16 channel;
	struct ether_addr BSSID;
	int8 rssi[PFN_SWC_RSSI_WINDOW_MAX];
} wl_pfn_significant_net_t;

#define PFN_SWC_SCANRESULT_VERSION_1     1

typedef struct wl_pfn_swc_results {
	uint32 version;
	uint32 pkt_count;   /**< No. of results in current frame */
	uint32 total_count; /**< Total expected results */
	wl_pfn_significant_net_t list[];
} wl_pfn_swc_results_t;

typedef struct wl_pfn_net_info_bssid_v1 {
	struct ether_addr BSSID;
	uint8 channel;	/**< channel number only */
	int8  RSSI;	/**< receive signal strength (in dBm) */
	uint16 flags;	/**< (e.g. partial scan, off channel) */
	uint16 timestamp; /**< age in seconds */
} wl_pfn_net_info_bssid_v1_t;

typedef struct wl_pfn_scanhist_bssid_v1 {
	uint32 version;
	uint32 status;
	uint32 count;
	wl_pfn_net_info_bssid_v1_t netinfo[BCM_FLEX_ARRAY];
} wl_pfn_scanhist_bssid_v1_t;

/* v2 for this struct is skiped to match with other struct v3 version */
typedef struct wl_pfn_net_info_bssid_v3 {
	struct ether_addr BSSID;
	chanspec_t	chanspec;	/**<with 6G chanspec only */
	uint16		flags;		/**< (e.g. partial scan, off channel) */
	uint16		timestamp;	/**< age in seconds */
	int8		RSSI;		/**< receive signal strength (in dBm) */
	uint8		PAD[3];
} wl_pfn_net_info_bssid_v3_t;

typedef struct wl_pfn_scanhist_bssid_v3 {
	uint32 version;
	uint32 status;
	uint32 count;
	wl_pfn_net_info_bssid_v3_t netinfo[BCM_FLEX_ARRAY];
} wl_pfn_scanhist_bssid_v3_t;


/* Version 1 and 2 for various single scan result */
#define PFN_SCANRESULT_VERSION_V1	1
#define PFN_SCANRESULT_VERSION_V2	2
#define PFN_SCANRESULT_VERSION_V3	3


/* used to report exactly one scan result */
/* plus reports detailed scan info in bss_info */
typedef struct wl_pfn_scanresult_v1 {
	uint32 version;
	uint32 status;
	uint32 count;
	wl_pfn_net_info_v1_t netinfo;
	wl_bss_info_v109_t bss_info;
} wl_pfn_scanresult_v1_t;

typedef struct wl_pfn_scanresult_v2 {
	uint32 version;
	uint32 status;
	uint32 count;
	wl_pfn_net_info_v2_t netinfo;
	wl_bss_info_v109_t bss_info;
} wl_pfn_scanresult_v2_t;

typedef struct wl_pfn_scanresult_v2_1 {
	uint32 version;
	uint32 status;
	uint32 count;
	wl_pfn_net_info_v2_t netinfo;
	uint8 bss_info[];	/* var length wl_bss_info_X structures */
} wl_pfn_scanresult_v2_1_t;

typedef struct wl_pfn_scanresult_v3_1 {
	uint32 version;
	uint32 status;
	uint32 count;
	wl_pfn_net_info_v3_t netinfo;
	uint8 bss_info[];	/* var length wl_bss_info_X structures */
} wl_pfn_scanresult_v3_1_t;

#define PFN_SCAN_ALLGONE_VERSION_V1	1u

typedef struct wl_pfn_scan_all_gone_event_v1 {
	uint16 version;
	uint16 length;
	uint16 flags;
	uint16 PAD;
} wl_pfn_scan_all_gone_event_v1_t;

#define WL_PFN_MAX_RAND_LIMIT		20u
#define PFN_VERSION_V2			2u
/**PFN data structure */
typedef struct wl_pfn_param_v2 {
	int32 version;			/**< PNO parameters version */
	int32 scan_freq;		/**< Scan frequency */
	int32 lost_network_timeout;	/**< Timeout in sec. to declare
					* discovered network as lost
					*/
	int16 flags;			/**< Bit field to control features
					* of PFN such as sort criteria auto
					* enable switch and background scan
					*/
	int16 rssi_margin;		/**< Margin to avoid jitter for choosing a
					* PFN based on RSSI sort criteria
					*/
	uint8 bestn;			/**< number of best networks in each scan */
	uint8 mscan;			/**< number of scans recorded */
	uint8 repeat;			/**< Minimum number of scan intervals
					*before scan frequency changes in adaptive scan
					*/
	uint8 exp;			/**< Exponent of 2 for maximum scan interval */
	int32 slow_freq;		/**< slow scan period */
} wl_pfn_param_v2_t;

#define PFN_VERSION_V3			3u
typedef struct wl_pfn_param_v3 {
	int16 version;			/**< PNO parameters version */
	int16 length;			/* length of the structure */
	int32 scan_freq;		/**< Scan frequency */
	int32 lost_network_timeout;	/**< Timeout in sec. to declare
					* discovered network as lost
					*/
	int16 flags;			/**< Bit field to control features
					* of PFN such as sort criteria auto
					* enable switch and background scan
					*/
	int16 rssi_margin;		/**< Margin to avoid jitter for choosing a
					* PFN based on RSSI sort criteria
					*/
	uint8 bestn;			/**< number of best networks in each scan */
	uint8 mscan;			/**< number of scans recorded */
	uint8 repeat;			/**< Minimum number of scan intervals
					*before scan frequency changes in adaptive scan
					*/
	uint8 exp;			/**< Exponent of 2 for maximum scan interval */
	int32 slow_freq;		/**< slow scan period */
	uint8 min_bound;		/**< pfn scan period randomization - lower bound % */
	uint8 max_bound;		/**< pfn scan period randomization - upper bound % */
	uint8 pfn_lp_scan_disable;	/**< unused */
	uint8 pfn_lp_scan_cnt;		/* add support to insert LP PNO scan */
					/* interleaved with HP PNO scan */
} wl_pfn_param_v3_t;


typedef struct wl_pfn_bssid {
	struct ether_addr  macaddr;
	/* Bit4: suppress_lost, Bit3: suppress_found */
	uint16             flags;
} wl_pfn_bssid_t;
typedef struct wl_pfn_significant_bssid {
	struct ether_addr	macaddr;
	int8    rssi_low_threshold;
	int8    rssi_high_threshold;
} wl_pfn_significant_bssid_t;
#define WL_PFN_SUPPRESSFOUND_MASK	0x08
#define WL_PFN_SUPPRESSLOST_MASK	0x10
#define WL_PFN_SSID_IMPRECISE_MATCH	0x80
#define WL_PFN_SSID_SAME_NETWORK	0x10000
#define WL_PFN_SUPPRESS_AGING_MASK	0x20000
#define WL_PFN_FLUSH_ALL_SSIDS		0x40000

#define WL_PFN_IOVAR_FLAG_MASK		0xFFFF00FF
#define WL_PFN_RSSI_MASK		0xff00
#define WL_PFN_RSSI_SHIFT		8

typedef struct wl_pfn_cfg {
	uint32	reporttype;
	int32	channel_num;
	uint16	channel_list[WL_NUMCHANNELS];
	uint32	flags;
} wl_pfn_cfg_t;

#define WL_PFN_SSID_CFG_VERSION		1
#define WL_PFN_SSID_CFG_CLEAR		0x1

typedef struct wl_pfn_ssid_params {
	int8 min5G_rssi;           /* minimum 5GHz RSSI for a BSSID to be considered      */
	int8 min2G_rssi;           /* minimum 2.4GHz RSSI for a BSSID to be considered   */
	int16 init_score_max;     /* The maximum score that a network can have before bonuses  */

	int16 cur_bssid_bonus;    /* Add to current bssid                                      */
	int16 same_ssid_bonus;    /* score bonus for all networks with the same network flag   */
	int16 secure_bonus;       /* score bonus for networks that are not open          */
	int16 band_5g_bonus;
} wl_pfn_ssid_params_t;

typedef struct wl_ssid_ext_params {
	int8 min5G_rssi;	/* minimum 5GHz RSSI for a BSSID to be considered      */
	int8 min2G_rssi;	/* minimum 2.4GHz RSSI for a BSSID to be considered   */
	int16 init_score_max;	/* The maximum score that a network can have before bonuses  */
	int16 cur_bssid_bonus;	/* Add to current bssid                                      */
	int16 same_ssid_bonus;	/* score bonus for all networks with the same network flag   */
	int16 secure_bonus;	/* score bonus for networks that are not open                */
	int16 band_5g_bonus;
} wl_ssid_ext_params_t;

typedef struct wl_pfn_ssid_cfg {
	uint16 version;
	uint16 flags;
	wl_ssid_ext_params_t params;
} wl_pfn_ssid_cfg_t;

#define CH_BUCKET_REPORT_NONE                   0
#define CH_BUCKET_REPORT_SCAN_COMPLETE_ONLY     1
#define CH_BUCKET_REPORT_FULL_RESULT            2
#define CH_BUCKET_REPORT_SCAN_COMPLETE    (CH_BUCKET_REPORT_SCAN_COMPLETE_ONLY | \
								CH_BUCKET_REPORT_FULL_RESULT)
#define CH_BUCKET_REPORT_REGULAR            0
#define CH_BUCKET_GSCAN                     4

typedef struct wl_pfn_gscan_ch_bucket_cfg {
	uint8 bucket_end_index;
	uint8 bucket_freq_multiple;
	uint8 flag;
	uint8 reserved;
	uint16 repeat;
	uint16 max_freq_multiple;
} wl_pfn_gscan_ch_bucket_cfg_t;

typedef struct wl_pfn_capabilities {
	uint16 max_mscan;
	uint16 max_bestn;
	uint16 max_swc_bssid;
	uint16 max_hotlist_bssid;
} wl_pfn_capabilities_t;

#define GSCAN_SEND_ALL_RESULTS_MASK          (1 << 0)
#define GSCAN_ALL_BUCKETS_IN_FIRST_SCAN_MASK (1 << 3)
#define GSCAN_CFG_FLAGS_ONLY_MASK            (1 << 7)
#define WL_GSCAN_CFG_VERSION_1                     1
typedef struct wl_pfn_gscan_cfg {
	uint16 version;
	/**
	 * BIT0 1 = send probes/beacons to HOST
	 * BIT1 Reserved
	 * BIT2 Reserved
	 * Add any future flags here
	 * BIT7 1 = no other useful cfg sent
	 */
	uint8  flags;
	/** Buffer filled threshold in % to generate an event */
	uint8   buffer_threshold;
	/**
	 * No. of BSSIDs with "change" to generate an evt
	 * change - crosses rssi threshold/lost
	 */
	uint8   swc_nbssid_threshold;
	/* Max=8 (for now) Size of rssi cache buffer */
	uint8  swc_rssi_window_size;
	uint8  count_of_channel_buckets;
	uint8  retry_threshold;
	uint16  lost_ap_window;
	wl_pfn_gscan_ch_bucket_cfg_t channel_bucket[BCM_FLEX_ARRAY];
} wl_pfn_gscan_cfg_t;

#define WL_PFN_REPORT_ALLNET    0
#define WL_PFN_REPORT_SSIDNET   1
#define WL_PFN_REPORT_BSSIDNET  2

#define WL_PFN_CFG_FLAGS_PROHIBITED	0x00000001	/* Accept and use prohibited channels */
#define WL_PFN_CFG_FLAGS_RESERVED	0xfffffffe	/**< Remaining reserved for future use */

typedef struct wl_pfn {
	wlc_ssid_t		ssid;			/**< ssid name and its length */
	int32			flags;			/**< bit2: hidden */
	int32			infra;			/**< BSS Vs IBSS */
	int32			auth;			/**< Open Vs Closed */
	int32			wpa_auth;		/**< WPA type */
	int32			wsec;			/**< wsec value */
} wl_pfn_t;

typedef struct wl_pfn_list {
	uint32		version;
	uint32		enabled;
	uint32		count;
	wl_pfn_t	pfn[BCM_FLEX_ARRAY];
} wl_pfn_list_t;

#define PFN_SSID_EXT_VERSION_1   1

typedef struct wl_pfn_ext {
	uint8 flags;
	int8 rssi_thresh; /* RSSI threshold, track only if RSSI > threshold */
	uint16 wpa_auth; /* Match the wpa auth type defined in wlioctl_defs.h */
	uint8 ssid[DOT11_MAX_SSID_LEN];
	uint8 ssid_len;
	uint8 PAD;
} wl_pfn_ext_t;
typedef struct wl_pfn_ext_list {
	uint16 version;
	uint16 count;
	wl_pfn_ext_t pfn_ext[BCM_FLEX_ARRAY];
} wl_pfn_ext_list_t;

#define WL_PFN_SSID_EXT_FOUND   0x1
#define WL_PFN_SSID_EXT_LOST    0x2
typedef struct wl_pfn_result_ssid {
	uint8 flags;
	int8 rssi;
	/* channel number */
	uint16 channel;
	/* Assume idx in order of cfg */
	uint32 index;
} wl_pfn_result_ssid_crc32_t;

typedef struct wl_pfn_ssid_ext_result {
	uint16 version;
	uint16 count;
	wl_pfn_result_ssid_crc32_t net[BCM_FLEX_ARRAY];
} wl_pfn_ssid_ext_result_t;

#define PFN_EXT_AUTH_CODE_OPEN   1 /* open */
#define PFN_EXT_AUTH_CODE_PSK   2 /* WPA_PSK or WPA2PSK */
#define PFN_EXT_AUTH_CODE_EAPOL 4 /* any EAPOL  */

#define WL_PFN_HIDDEN_BIT		2
#define WL_PFN_HIDDEN_MASK		0x4

#ifndef BESTN_MAX
#define BESTN_MAX			10
#endif

#ifndef MSCAN_MAX
#define MSCAN_MAX			32
#endif

/* Dynamic scan configuration for motion profiles */

#define WL_PFN_MPF_VERSION_1	 1

/* Valid group IDs, may be expanded in the future */
#define WL_PFN_MPF_GROUP_SSID 0
#define WL_PFN_MPF_GROUP_BSSID 1
#define WL_PFN_MPF_MAX_GROUPS 2

/* Max number of MPF states supported in this time */
#define WL_PFN_MPF_STATES_MAX	4u
#define WL_PFN_MPF_LP_CNT_MAX	7u

/* Flags for the mpf-specific stuff */
#define WL_PFN_MPF_ADAPT_ON_BIT		0u
#define WL_PFN_MPF_ADAPTSCAN_BIT	1u
#define WL_PFN_MPF_LP_SCAN_BIT		3u

#define WL_PFN_MPF_ADAPT_ON_MASK       0x0001  /* Bit  0     */
#define WL_PFN_MPF_ADAPTSCAN_MASK      0x0006  /* Bits [2:1] */
#define WL_PFN_MPF_LP_SCAN_CNT_MASK    0x0038  /* Bits [5:3] */

/* Per-state timing values */
typedef struct wl_pfn_mpf_state_params {
	int32  scan_freq;		/* Scan frequency (secs) */
	int32  lost_network_timeout;	/* Timeout to declare net lost (secs) */
	int16  flags;			/* Space for flags: ADAPT, LP_SCAN cnt etc */
	uint8  exp;			/* Exponent of 2 for max interval for SMART/STRICT_ADAPT */
	uint8  repeat;			/* Number of scans before changing adaptation level */
	int32  slow_freq;		/* Slow scan period for SLOW_ADAPT */
} wl_pfn_mpf_state_params_t;

typedef struct wl_pfn_mpf_param {
	uint16 version;		/* Structure version */
	uint16 groupid;		/* Group ID: 0 (SSID), 1 (BSSID), other: reserved */
	wl_pfn_mpf_state_params_t state[WL_PFN_MPF_STATES_MAX];
} wl_pfn_mpf_param_t;

/* Structure for setting pfn_override iovar */
typedef struct wl_pfn_override_param {
	uint16 version;         /* Structure version */
	uint16 start_offset;    /* Seconds from now to apply new params */
	uint16 duration;        /* Seconds to keep new params applied */
	uint16 reserved;
	wl_pfn_mpf_state_params_t override;
} wl_pfn_override_param_t;
#define WL_PFN_OVERRIDE_VERSION	1

/*
 * Definitions for base MPF configuration
 */

#define WL_MPF_VERSION_1	1
#define WL_MPF_MAX_BITS		3
#define WL_MPF_MAX_STATES	(1 << WL_MPF_MAX_BITS)

#define WL_MPF_STATE_NAME_MAX 12

typedef struct wl_mpf_val {
	uint16 val;		/* Value of GPIO bits */
	uint16 state;		/* State identifier */
	char name[WL_MPF_STATE_NAME_MAX]; /* Optional name */
} wl_mpf_val_t;

typedef struct wl_mpf_map {
	uint16 version;
	uint16 type;
	uint16 mask;		/* Which GPIO bits to use */
	uint8  count;		/* Count of state/value mappings */
	uint8  PAD;
	wl_mpf_val_t vals[WL_MPF_MAX_STATES];
} wl_mpf_map_t;

#define WL_MPF_STATE_AUTO (0xFFFF) /* (uint16)-1) */

typedef struct wl_mpf_state {
	uint16 version;
	uint16 type;
	uint16 state;		/* Get/Set */
	uint8 force;		/* 0 - auto (HW) state, 1 - forced state */
	char name[WL_MPF_STATE_NAME_MAX]; /* Get/Set: Optional/actual name */
	uint8  PAD;
} wl_mpf_state_t;
/*
 * WLFCTS definition
 */
typedef struct wl_txstatus_additional_info {
	ratespec_t rspec;
	uint32 enq_ts;
	uint32 last_ts;
	uint32 entry_ts;
	uint16 seq;
	uint8  rts_cnt;
	uint8  tx_cnt;
} wl_txstatus_additional_info_t;


/** Service discovery */
typedef struct {
	uint8	transaction_id;	/**< Transaction id */
	uint8	protocol;	/**< Service protocol type */
	uint16	query_len;	/**< Length of query */
	uint16	response_len;	/**< Length of response */
	uint8	qrbuf[];
} wl_p2po_qr_t;

typedef struct {
	uint16			period;			/**< extended listen period */
	uint16			interval;		/**< extended listen interval */
	uint16                  count;                  /* count to repeat */
	uint16                  PAD;                    /* pad for 32bit align */
} wl_p2po_listen_t;

/** GAS state machine tunable parameters.  Structure field values of 0 means use the default. */
typedef struct wl_gas_config {
	uint16 max_retransmit;		/**< Max # of firmware/driver retransmits on no Ack
					 * from peer (on top of the ucode retries).
					 */
	uint16 response_timeout;	/**< Max time to wait for a GAS-level response
					 * after sending a packet.
					 */
	uint16 max_comeback_delay;	/**< Max GAS response comeback delay.
					 * Exceeding this fails the GAS exchange.
					 */
	uint16 max_retries;		/**< Max # of GAS state machine retries on failure
					 * of a GAS frame exchange.
					 */
} wl_gas_config_t;

/** P2P Find Offload parameters */
typedef struct wl_p2po_find_config {
	uint16 version;			/**< Version of this struct */
	uint16 length;			/**< sizeof(wl_p2po_find_config_t) */
	int32 search_home_time;		/**< P2P search state home time when concurrent
					 * connection exists.  -1 for default.
					 */
	uint8 num_social_channels;
			/**< Number of social channels up to WL_P2P_SOCIAL_CHANNELS_MAX.
			 * 0 means use default social channels.
			 */
	uint8 flags;
	uint16 social_channels[BCM_FLEX_ARRAY];	/**< Variable length array of social channels */
} wl_p2po_find_config_t;

#define WL_P2PO_FIND_CONFIG_VERSION_2	2u	/**< value for version field */

/** wl_p2po_find_config_t flags */
#define P2PO_FIND_FLAG_SCAN_ALL_APS	0x01u	/**< Whether to scan for all APs in the p2po_find
						 * periodic scans of all channels.
						 * 0 means scan for only P2P devices.
						 * 1 means scan for P2P devices plus non-P2P APs.
						 */


/** For adding a WFDS service to seek */
typedef struct {
	uint32 seek_hdl;		/**< unique id chosen by host */
	uint8 addr[6];			/**< Seek service from a specific device with this
					 * MAC address, all 1's for any device.
					 */
	uint8 service_hash[P2P_WFDS_HASH_LEN];
	uint8 service_name_len;
	uint8 service_name[MAX_WFDS_SEEK_SVC_NAME_LEN];
					/**< Service name to seek, not null terminated */
	uint8 service_info_req_len;
	uint8 service_info_req[BCM_FLEX_ARRAY];
					/**< Service info request, not null terminated.
					 * Variable length specified by service_info_req_len.
					 * Maximum length is MAX_WFDS_SEEK_SVC_INFO_LEN.
					 */
} wl_p2po_wfds_seek_add_t;

/** For deleting a WFDS service to seek */
typedef struct {
	uint32 seek_hdl;		/**< delete service specified by id */
} wl_p2po_wfds_seek_del_t;


/** For adding a WFDS service to advertise */
#include <packed_section_start.h>
typedef BWL_PRE_PACKED_STRUCT struct {
	uint32 advertise_hdl;		/**< unique id chosen by host */
	uint8 service_hash[P2P_WFDS_HASH_LEN];
	uint32 advertisement_id;
	uint16 service_config_method;
	uint8 service_name_len;
	uint8 service_name[MAX_WFDS_SVC_NAME_LEN];
					/**< Service name , not null terminated */
	uint8 service_status;
	uint16 service_info_len;
	uint8 service_info[BCM_FLEX_ARRAY];
					/**< Service info, not null terminated.
					 * Variable length specified by service_info_len.
					 * Maximum length is MAX_WFDS_ADV_SVC_INFO_LEN.
					 */
} BWL_POST_PACKED_STRUCT wl_p2po_wfds_advertise_add_t;
#include <packed_section_end.h>

/** For deleting a WFDS service to advertise */
typedef struct {
	uint32 advertise_hdl;	/**< delete service specified by hdl */
} wl_p2po_wfds_advertise_del_t;

/** P2P Offload discovery mode for the p2po_state iovar */
typedef enum {
	WL_P2PO_DISC_STOP,
	WL_P2PO_DISC_LISTEN,
	WL_P2PO_DISC_DISCOVERY
} disc_mode_t;

/* ANQP offload */

#define ANQPO_MAX_QUERY_SIZE		256
typedef struct {
	uint16 max_retransmit;		/**< ~0 use default, max retransmit on no ACK from peer */
	uint16 response_timeout; 	/**< ~0 use default, msec to wait for resp after tx pkt */
	uint16 max_comeback_delay;	/**< ~0 use default, max comeback delay in resp else fail */
	uint16 max_retries;		/**< ~0 use default, max retries on failure */
	uint16 query_len;		/**< length of ANQP query */
	uint8 query_data[BCM_FLEX_ARRAY];  /**< ANQP encoded query (max ANQPO_MAX_QUERY_SIZE) */
} wl_anqpo_set_t;

#define WL_ANQPO_FLAGS_BSSID_WILDCARD		0x0001
#define WL_ANQPO_PEER_LIST_VERSION_2		2
typedef struct {
	uint16 channel;				/**< channel of the peer */
	struct ether_addr addr;			/**< addr of the peer */
} wl_anqpo_peer_v1_t;
typedef struct {
	uint16 channel;				/**< channel of the peer */
	struct ether_addr addr;			/**< addr of the peer */
	uint32 flags;				/**< 0x01-Peer is MBO Capable */
} wl_anqpo_peer_v2_t;

#define WL_ANQPO_PEER_LIST_VERSION_3		3
typedef struct {
	uint16 chanspec;			/**< chanspec of the peer */
	struct ether_addr addr;			/**< addr of the peer */
	uint32 flags;				/**< 0x01-Peer is MBO Capable */
} wl_anqpo_peer_v3_t;


#define ANQPO_MAX_PEER_LIST			64
typedef struct {
	uint16 count;					/**< number of peers in list */
	wl_anqpo_peer_v1_t peer[BCM_FLEX_ARRAY];	/**< max ANQPO_MAX_PEER_LIST */
} wl_anqpo_peer_list_v1_t;

typedef struct {
	uint16  version;				/**<VERSION */
	uint16  length;					/**< length of entire structure */
	uint16 count;					/**< number of peers in list */
	wl_anqpo_peer_v2_t peer[BCM_FLEX_ARRAY];	/**< max ANQPO_MAX_PEER_LIST */
} wl_anqpo_peer_list_v2_t;

typedef struct {
	uint16 version;				/**< VERSION */
	uint16 length;				/**< length of entire structure */
	uint16 count;				/**< number of peers in list */
	uint16 PAD;
	wl_anqpo_peer_v3_t peer[];		/**< max ANQPO_MAX_PEER_LIST */
} wl_anqpo_peer_list_v3_t;


#define ANQPO_MAX_IGNORE_SSID		64
typedef struct {
	uint8 is_clear;				/**< set to clear list (not used on GET) */
	uint8 PAD;
	uint16 count;				/**< number of SSID in list */
	wlc_ssid_t ssid[BCM_FLEX_ARRAY];	/**< max ANQPO_MAX_IGNORE_SSID */
} wl_anqpo_ignore_ssid_list_t;

#define ANQPO_MAX_IGNORE_BSSID		64
typedef struct {
	uint8 is_clear;				/**< set to clear list (not used on GET) */
	uint8 PAD;
	uint16 count;				/**< number of addr in list */
	struct ether_addr bssid[];	/**< max ANQPO_MAX_IGNORE_BSSID */
} wl_anqpo_ignore_bssid_list_t;


struct toe_ol_stats_t {
	/** Num of tx packets that don't need to be checksummed */
	uint32 tx_summed;

	/* Num of tx packets where checksum is filled by offload engine */
	uint32 tx_iph_fill;
	uint32 tx_tcp_fill;
	uint32 tx_udp_fill;
	uint32 tx_icmp_fill;

	/*  Num of rx packets where toe finds out if checksum is good or bad */
	uint32 rx_iph_good;
	uint32 rx_iph_bad;
	uint32 rx_tcp_good;
	uint32 rx_tcp_bad;
	uint32 rx_udp_good;
	uint32 rx_udp_bad;
	uint32 rx_icmp_good;
	uint32 rx_icmp_bad;

	/* Num of tx packets in which csum error is injected */
	uint32 tx_tcp_errinj;
	uint32 tx_udp_errinj;
	uint32 tx_icmp_errinj;

	/* Num of rx packets in which csum error is injected */
	uint32 rx_tcp_errinj;
	uint32 rx_udp_errinj;
	uint32 rx_icmp_errinj;
};

/** Arp offload statistic counts */
struct arp_ol_stats_t {
	uint32  host_ip_entries; /**< Host IP table addresses (more than one if multihomed) */
	uint32  host_ip_overflow;	/**< Host IP table additions skipped due to overflow */

	uint32  arp_table_entries;	/**< ARP table entries */
	uint32  arp_table_overflow;	/**< ARP table additions skipped due to overflow */

	uint32  host_request;		/**< ARP requests from host */
	uint32  host_reply;		/**< ARP replies from host */
	uint32  host_service;		/**< ARP requests from host serviced by ARP Agent */

	uint32  peer_request;		/**< ARP requests received from network */
	uint32  peer_request_drop;	/**< ARP requests from network that were dropped */
	uint32  peer_reply;		/**< ARP replies received from network */
	uint32  peer_reply_drop;	/**< ARP replies from network that were dropped */
	uint32  peer_service;		/**< ARP request from host serviced by ARP Agent */
};

/** NS offload statistic counts */
struct nd_ol_stats_t {
	uint32  host_ip_entries;    /**< Host IP table addresses (more than one if multihomed) */
	uint32  host_ip_overflow;   /**< Host IP table additions skipped due to overflow */
	uint32  peer_request;       /**< NS requests received from network */
	uint32  peer_request_drop;  /**< NS requests from network that were dropped */
	uint32  peer_reply_drop;    /**< NA replies from network that were dropped */
	uint32  peer_service;       /**< NS request from host serviced by firmware */
};

/*
 * Neighbor Discovery Offloading
 */
enum {
	WL_ND_IPV6_ADDR_TYPE_UNICAST = 0,
	WL_ND_IPV6_ADDR_TYPE_ANYCAST
};

typedef struct wl_nd_host_ip_addr {
	struct ipv6_addr ip_addr;	/* host ip address */
	uint8 type;			/* type of address */
	uint8 PAD[3];
} wl_nd_host_ip_addr_t;

typedef struct wl_nd_host_ip_list {
	uint32 count;
	wl_nd_host_ip_addr_t host_ip[BCM_FLEX_ARRAY];
} wl_nd_host_ip_list_t;

#define WL_ND_HOSTIP_IOV_VER    1

enum {
	WL_ND_HOSTIP_OP_VER = 0,	/* get version */
	WL_ND_HOSTIP_OP_ADD,		/* add address */
	WL_ND_HOSTIP_OP_DEL,		/* delete specified address */
	WL_ND_HOSTIP_OP_DEL_UC,		/* delete all unicast address */
	WL_ND_HOSTIP_OP_DEL_AC,		/* delete all anycast address */
	WL_ND_HOSTIP_OP_DEL_ALL,	/* delete all addresses */
	WL_ND_HOSTIP_OP_LIST,		/* get list of host ip address */
	WL_ND_HOSTIP_OP_MAX
};

typedef struct wl_nd_hostip {
	uint16 version;				/* version of iovar buf */
	uint16 op_type;				/* operation type */
	uint32 length;				/* length of entire structure */
	union {
		wl_nd_host_ip_addr_t host_ip;	/* set param for add */
		uint16 version;			/* get return for ver */
	} u;
} wl_nd_hostip_t;

#define WL_ND_HOSTIP_FIXED_LEN		OFFSETOF(wl_nd_hostip_t, u)
#define WL_ND_HOSTIP_WITH_ADDR_LEN	(WL_ND_HOSTIP_FIXED_LEN + sizeof(wl_nd_host_ip_addr_t))

/*
 * Keep-alive packet offloading.
 */

/**
 * NAT keep-alive packets format: specifies the re-transmission period, the packet
 * length, and packet contents.
 */
typedef struct wl_keep_alive_pkt {
	uint32	period_msec;	/** Retransmission period (0 to disable packet re-transmits) */
	uint16	len_bytes;	/* Size of packet to transmit (0 to disable packet re-transmits) */
	uint8	data[BCM_FLEX_ARRAY];
				/** Variable len pkt to transmit.  Contents should include
				 * entire ethernet packet (enet header, IP header, UDP header,
				 * and UDP payload) in network byte order.
				 */
} wl_keep_alive_pkt_t;

#define WL_KEEP_ALIVE_FIXED_LEN		OFFSETOF(wl_keep_alive_pkt_t, data)

#define MAX_RSSI_COUNT			8
typedef struct rssi_struct {
	int8	val[MAX_RSSI_COUNT];	/**< rssi values in AFs */
	int16	sum;			/**< total rssi sum */
	uint8	cnt;			/**< number rssi samples */
	uint8	idx;			/**< next rssi location */
} rssi_struct_t;



/*
 * ptk_start: iovar to start 4-way handshake for secured ranging
*/

/* ptk negotiation security type - determines negotiation parameters */
typedef enum {
	WL_PTK_START_SEC_TYPE_PMK = 1
} wl_ptk_start_sec_type_t;

/* ptk negotiation role */
typedef enum {
	ROLE_NONE	= 0x0,
	ROLE_AUTH	= 0x1,
	ROLE_SUP	= 0x2,
	ROLE_STATIC	= 0x3,
	ROLE_INVALID	= 0xff,
	WL_PTK_START_ROLE_NONE = ROLE_NONE,
	WL_PTK_START_ROLE_AUTH = ROLE_AUTH,
	WL_PTK_START_ROLE_SUP = ROLE_SUP,
	WL_PTK_START_ROLE_STATIC = ROLE_STATIC,
	WL_PTK_START_ROLE_INVALID = ROLE_INVALID
} wl_ptk_start_role_t;

typedef struct wl_ptk_start_tlv {
	uint16 id;
	uint16 len;
	uint8 data[1];
} wl_ptk_start_tlv_t;

typedef enum {
	WL_PTK_START_TLV_PMK	= 1	/* uint8[] */
} wl_ptk_start_tlv_type;

typedef enum {
	WL_PTK_START_FLAG_NO_DATA_PROT	= 1,	/* data frame protection disabled */
	WL_PTK_START_FLAG_GEN_FTM_TPK	= 2	/* Generate FTM Toast/Seq Protection Key */
} wl_ptk_start_flags_t;

typedef struct wl_ptk_start_iov {
	uint16 version;
	uint16 len;				/* length of entire iov from version */
	wl_ptk_start_flags_t flags;
	wl_ptk_start_sec_type_t sec_type;
	wl_ptk_start_role_t role;
	struct ether_addr peer_addr;
	uint16 PAD;				/* reserved/32 bit alignment */
	wl_ptk_start_tlv_t tlvs[BCM_FLEX_ARRAY];
} wl_ptk_start_iov_t;

/*
 * Dongle pattern matching filter.
 */

#define MAX_WAKE_PACKET_CACHE_BYTES 128 /**< Maximum cached wake packet */

#define MAX_WAKE_PACKET_BYTES	    (DOT11_A3_HDR_LEN +			    \
				     DOT11_QOS_LEN +			    \
				     sizeof(struct dot11_llc_snap_header) + \
				     ETHER_MAX_DATA)

typedef struct pm_wake_packet {
	uint32	status;		/**< Is the wake reason a packet (if all the other field's valid) */
	uint32	pattern_id;	/**< Pattern ID that matched */
	uint32	original_packet_size;
	uint32	saved_packet_size;
	uint8	packet[MAX_WAKE_PACKET_CACHE_BYTES];
} pm_wake_packet_t;

/* Packet filter types. Currently, only pattern matching is supported. */
typedef enum wl_pkt_filter_type {
	WL_PKT_FILTER_TYPE_PATTERN_MATCH=0,       /**< Pattern matching filter */
	WL_PKT_FILTER_TYPE_MAGIC_PATTERN_MATCH=1, /**< Magic packet match */
	WL_PKT_FILTER_TYPE_PATTERN_LIST_MATCH=2,  /**< A pattern list (match all to match filter) */
	WL_PKT_FILTER_TYPE_ENCRYPTED_PATTERN_MATCH=3, /**< SECURE WOWL magic / net pattern match */
	WL_PKT_FILTER_TYPE_APF_MATCH=4, /* Android packet filter match */
	WL_PKT_FILTER_TYPE_PATTERN_MATCH_TIMEOUT=5, /* Pattern matching filter with timeout event */
	WL_PKT_FILTER_TYPE_IMMEDIATE_PATTERN_MATCH=6, /* Immediately pattern matching filter */
	WL_PKT_FILTYER_TYPE_MAX = 7,	/* Pkt filter type MAX */
} wl_pkt_filter_type_t;

#define WL_PKT_FILTER_TYPE wl_pkt_filter_type_t	/* backward compatibility; remove */

/* String mapping for types that may be used by applications or debug */
#define WL_PKT_FILTER_TYPE_NAMES \
	{ "PATTERN", WL_PKT_FILTER_TYPE_PATTERN_MATCH },	\
	{ "MAGIC",   WL_PKT_FILTER_TYPE_MAGIC_PATTERN_MATCH },	\
	{ "PATLIST", WL_PKT_FILTER_TYPE_PATTERN_LIST_MATCH },	\
	{ "SECURE WOWL", WL_PKT_FILTER_TYPE_ENCRYPTED_PATTERN_MATCH },	\
	{ "APF", WL_PKT_FILTER_TYPE_APF_MATCH }, \
	{ "PATTERN TIMEOUT", WL_PKT_FILTER_TYPE_PATTERN_MATCH_TIMEOUT }, \
	{ "IMMEDIATE", WL_PKT_FILTER_TYPE_IMMEDIATE_PATTERN_MATCH }

/** Secured WOWL packet was encrypted, need decrypted before check filter match */
typedef struct wl_pkt_decrypter {
	uint8* (*dec_cb)(void* dec_ctx, const void *sdu, int sending);
	void*  dec_ctx;
} wl_pkt_decrypter_t;

/**
 * Pattern matching filter. Specifies an offset within received packets to
 * start matching, the pattern to match, the size of the pattern, and a bitmask
 * that indicates which bits within the pattern should be matched.
 */
typedef struct wl_pkt_filter_pattern {
	uint32	offset;		/**< Offset within received packet to start pattern matching.
				 * Offset '0' is the first byte of the ethernet header.
				 */
	uint32	size_bytes;	/**< Size of the pattern.  Bitmask must be the same size. */
	uint8   mask_and_pattern[]; /**< Variable length mask and pattern data.  mask starts
				      * at offset 0.  Pattern immediately follows mask. for
				      * secured pattern, put the descrypter pointer to the
				      * beginning, mask and pattern postponed correspondingly
				      */
} wl_pkt_filter_pattern_t;

/** A pattern list is a numerically specified list of modified pattern structures. */
typedef struct wl_pkt_filter_pattern_listel {
	uint16 rel_offs;	/**< Offset to begin match (relative to 'base' below) */
	uint16 base_offs;	/**< Base for offset (defined below) */
	uint16 size_bytes;	/**< Size of mask/pattern */
	uint16 match_flags;	/**< Addition flags controlling the match */
	uint8  mask_and_data[]; /**< Variable length mask followed by data, each size_bytes */
} wl_pkt_filter_pattern_listel_t;

typedef struct wl_pkt_filter_pattern_list {
	uint8 list_cnt;		/**< Number of elements in the list */
	uint8 PAD1[1];		/**< Reserved (possible version: reserved) */
	uint16 totsize;		/**< Total size of this pattern list (includes this struct) */
	uint8 patterns[];   /**< Variable number of wl_pkt_filter_pattern_listel_t elements */
} wl_pkt_filter_pattern_list_t;

typedef struct wl_apf_program {
	uint16 version;
	uint16 instr_len;	/* number of instruction blocks */
	uint32 inst_ts;		/* program installation timestamp */
	uint8 instrs[];	/* variable length instructions */
} wl_apf_program_t;

typedef struct wl_pkt_filter_pattern_timeout {
	uint32	offset;	/* Offset within received packet to start pattern matching.
					 * Offset '0' is the first byte of the ethernet header.
					 */
	uint32	size_bytes;	/* Size of the pattern. Bitmask must be the same size. */
	uint32	timeout;	/* Timeout(seconds) */
	uint8	mask_and_pattern[]; /* Variable length mask and pattern data.
								 * mask starts at offset 0. Pattern
								 * immediately follows mask.
								*/
} wl_pkt_filter_pattern_timeout_t;

/** IOVAR "pkt_filter_add" parameter. Used to install packet filters. */
typedef struct wl_pkt_filter {
	uint32	id;		/**< Unique filter id, specified by app. */
	uint32	type;		/**< Filter type (WL_PKT_FILTER_TYPE_xxx). */
	uint32	negate_match;	/**< Negate the result of filter matches */
	union {			/* Filter definitions */
		wl_pkt_filter_pattern_t pattern;	/**< Pattern matching filter */
		wl_pkt_filter_pattern_list_t patlist; /**< List of patterns to match */
		wl_apf_program_t apf_program; /* apf program */
		wl_pkt_filter_pattern_timeout_t pattern_timeout; /* Pattern timeout event filter */
	} u;
	/* Do NOT add structure members after the filter definitions, since they
	 * may include variable length arrays.
	 */
} wl_pkt_filter_t;

/** IOVAR "tcp_keep_set" parameter. Used to install tcp keep_alive stuff. */
typedef struct wl_tcp_keep_set {
	uint32	val1;
	uint32	val2;
} wl_tcp_keep_set_t;

#define WL_PKT_FILTER_FIXED_LEN		  OFFSETOF(wl_pkt_filter_t, u)
#define WL_PKT_FILTER_PATTERN_FIXED_LEN	  OFFSETOF(wl_pkt_filter_pattern_t, mask_and_pattern)
#define WL_PKT_FILTER_PATTERN_LIST_FIXED_LEN OFFSETOF(wl_pkt_filter_pattern_list_t, patterns)
#define WL_PKT_FILTER_PATTERN_LISTEL_FIXED_LEN	\
			OFFSETOF(wl_pkt_filter_pattern_listel_t, mask_and_data)
#define WL_PKT_FILTER_PATTERN_TIMEOUT_FIXED_LEN	\
			OFFSETOF(wl_pkt_filter_pattern_timeout_t, mask_and_pattern)

#define WL_APF_INTERNAL_VERSION	1
#define WL_APF_PROGRAM_FIXED_LEN	OFFSETOF(wl_apf_program_t, instrs)
#define WL_APF_PROGRAM_LEN(apf_program)	\
	((apf_program)->instr_len * sizeof((apf_program)->instrs[0]))
#define WL_APF_PROGRAM_TOTAL_LEN(apf_program)	\
	(WL_APF_PROGRAM_FIXED_LEN + WL_APF_PROGRAM_LEN(apf_program))
#ifndef WL_APF_PROGRAM_MAX_SIZE
#define WL_APF_PROGRAM_MAX_SIZE (4u * 1024u)
#endif /* WL_APF_PROGRAM_MAX_SIZE */

/** IOVAR "pkt_filter_enable" parameter. */
typedef struct wl_pkt_filter_enable {
	uint32	id;		/**< Unique filter id */
	uint32	enable;		/**< Enable/disable bool */
} wl_pkt_filter_enable_t;

/** IOVAR "pkt_filter_list" parameter. Used to retrieve a list of installed filters. */
typedef struct wl_pkt_filter_list {
	uint32	num;		/**< Number of installed packet filters */
	uint8	filter[];	/**< Variable array of packet filters. */
} wl_pkt_filter_list_t;

#define WL_PKT_FILTER_LIST_FIXED_LEN	  OFFSETOF(wl_pkt_filter_list_t, filter)

/** IOVAR "pkt_filter_stats" parameter. Used to retrieve debug statistics. */
typedef struct wl_pkt_filter_stats {
	uint32	num_pkts_matched;	/**< # filter matches for specified filter id */
	uint32	num_pkts_forwarded;	/**< # packets fwded from dongle to host for all filters */
	uint32	num_pkts_discarded;	/**< # packets discarded by dongle for all filters */
} wl_pkt_filter_stats_t;

/** IOVAR "pkt_filter_ports" parameter.  Configure TCP/UDP port filters. */
typedef struct wl_pkt_filter_ports {
	uint8 version;				/**< Be proper */
	uint8 reserved;				/**< Be really proper */
	uint16 count;				/**< Number of ports following */
	/* End of fixed data */
	uint16 ports[BCM_FLEX_ARRAY];	/**< Placeholder for ports[<count>] */
} wl_pkt_filter_ports_t;

#define WL_PKT_FILTER_PORTS_FIXED_LEN	OFFSETOF(wl_pkt_filter_ports_t, ports)

#define WL_PKT_FILTER_PORTS_VERSION	0
#if defined(WL_PKT_FLTR_EXT) && !defined(WL_PKT_FLTR_EXT_DISABLED)
#define WL_PKT_FILTER_PORTS_MAX	256
#else
#define WL_PKT_FILTER_PORTS_MAX	128
#endif /* WL_PKT_FLTR_EXT && !WL_PKT_FLTR_EXT_DISABLED */

#define RSN_REPLAY_LEN 8
typedef struct _gtkrefresh {
	uint8	KCK[RSN_KCK_LENGTH];
	uint8	KEK[RSN_KEK_LENGTH];
	uint8	ReplayCounter[RSN_REPLAY_LEN];
} gtk_keyinfo_t;

/** Sequential Commands ioctl */
typedef struct wl_seq_cmd_ioctl {
	uint32 cmd;		/**< common ioctl definition */
	uint32 len;		/**< length of user buffer */
} wl_seq_cmd_ioctl_t;

#define WL_SEQ_CMD_ALIGN_BYTES	4

/**
 * These are the set of get IOCTLs that should be allowed when using
 * IOCTL sequence commands. These are issued implicitly by wl.exe each time
 * it is invoked. We never want to buffer these, or else wl.exe will stop working.
 */
#define WL_SEQ_CMDS_GET_IOCTL_FILTER(cmd) \
	(((cmd) == WLC_GET_MAGIC)		|| \
	 ((cmd) == WLC_GET_VERSION)		|| \
	 ((cmd) == WLC_GET_AP)			|| \
	 ((cmd) == WLC_GET_INSTANCE))

#define MAX_PKTENG_SWEEP_STEPS 40
typedef struct wl_pkteng {
	uint32 flags;
	uint32 delay;			/**< Inter-packet delay */
	uint32 nframes;			/**< Number of frames */
	uint32 length;			/**< Packet length */
	uint8  seqno;			/**< Enable/disable sequence no. */
	struct ether_addr dest;		/**< Destination address */
	struct ether_addr src;		/**< Source address */
	uint8  sweep_steps;		/**< Number of sweep power */
	uint8  est_pwr_cnt;             /**< Number of packets to collect est. pwr */
	uint8  PAD[1];
} wl_pkteng_t;

typedef struct wl_pkteng_est_pwr {
	uint32 ratespec; /* Ratespec of the packets */
	uint8 est_pwr_required; /* Requested Number of packets to collect est power */
	uint8 est_pwr_collected; /* Actual collected packets */
	uint8 core_num; /* Total number of Tx cores */
	uint8 PAD;
	int8  est_pwr[]; /* The est power buffer */
} wl_pkteng_est_pwr_t;

/* The wl_pkteng_est_pwr_t is encapsulated in a xtlv buffer with the following ID */
enum wl_pkgeng_estpwr_id {
	wl_pkteng_estpwr_data = 0u,
	wl_pkteng_estpwr_clear = 1u
};


/* IOVAR pkteng_sweep_counters response structure */
#define WL_PKTENG_SWEEP_COUNTERS_VERSION_1    1
typedef struct wl_pkteng_sweep_ctrs {
	uint16 version;			/**< Version - 1 */
	uint16 size;			/**< Complete Size including sweep_counters */
	uint16 sweep_steps;		/**< Number of steps */
	uint16 PAD;
	uint16 sweep_counter[];		/**< Array of frame counters */
} wl_pkteng_sweep_ctrs_t;

/* IOVAR pkteng_rx_pkt response structure */
#define WL_PKTENG_RX_PKT_VERSION_1	    1
typedef struct wl_pkteng_rx_pkt {
	uint16 version;		/**< Version - 1 */
	uint16 size;		/**< Complete Size including the packet */
	uint8 payload[];	/**< Packet payload */
} wl_pkteng_rx_pkt_t;

#define WL_PKTENG_RU_FILL_VER_1		1u
#define WL_PKTENG_RU_FILL_VER_2		2u
#define WL_PKTENG_RU_FILL_VER_3		3u
#define WL_PKTENG_RU_FILL_VER_4		4u

// struct for ru packet engine
typedef struct wl_pkteng_ru_v1 {
	uint16 version;			/* ver is 1 */
	uint16 length;			/* size of complete structure */
	uint8 bw;			/* bandwidth info */
	uint8 ru_alloc_val;		/* ru allocation index number */
	uint8 mcs_val;			/* mcs allocated value */
	uint8 nss_val;			/* num of spatial streams */
	uint32 num_bytes;		/* approx num of bytes to calculate other required params */
	uint8 cp_ltf_val ;		/* GI and LTF symbol size */
	uint8 he_ltf_symb ;		/* num of HE-LTF symbols */
	uint8 stbc;			/* STBC support */
	uint8 coding_val;		/* BCC/LDPC coding support */
	uint8 pe_category;		/* PE duration 0/8/16usecs  */
	uint8 dcm;			/* dual carrier modulation */
	uint8 mumimo_ltfmode;		/* ltf mode */
	uint8 trig_tx;			/* form and transmit the trigger frame */
	uint8 trig_type;		/* type of trigger frame */
	uint8 trig_period;		/* trigger tx periodicity TBD */
	struct ether_addr dest;		/* destination address for un-associated mode */
} wl_pkteng_ru_v1_t;

typedef struct wl_pkteng_ru_v2 {
	uint16 version;			/* ver is 1 */
	uint16 length;			/* size of complete structure */
	uint8 bw;			/* bandwidth info */
	uint8 ru_alloc_val;		/* ru allocation index number */
	uint8 mcs_val;			/* mcs allocated value */
	uint8 nss_val;			/* num of spatial streams */
	uint32 num_bytes;		/* approx num of bytes to calculate other required params */
	struct ether_addr dest;		/* destination address for un-associated mode */
	uint8 cp_ltf_val ;		/* GI and LTF symbol size */
	uint8 he_ltf_symb ;		/* num of HE-LTF symbols */
	uint8 stbc;			/* STBC support */
	uint8 coding_val;		/* BCC/LDPC coding support */
	uint8 pe_category;		/* PE duration 0/8/16usecs  */
	uint8 dcm;			/* dual carrier modulation */
	uint8 mumimo_ltfmode;		/* ltf mode */
	uint8 trig_tx;			/* form and transmit the trigger frame */
	uint8 trig_type;		/* type of trigger frame */
	uint8 trig_period;		/* trigger tx periodicity TBD */
	uint8 tgt_rssi; /* target rssi value in encoded format */
	uint8 PAD[3];		/* 2 byte padding to make structure size a multiple of 32bits */
} wl_pkteng_ru_v2_t;

typedef struct wl_pkteng_ru_v3 {
	uint16 version;			/* ver is 3 */
	uint16 length;			/* size of complete structure */
	uint8 bw;			/* bandwidth info */
	uint8 ru_alloc_val;		/* ru allocation index number */
	uint8 mcs_val;			/* mcs allocated value */
	uint8 nss_val;			/* num of spatial streams */
	uint32 num_bytes;		/* approx num of bytes to calculate other required params */
	struct ether_addr dest;		/* destination address for un-associated mode */
	uint8 cp_ltf_val ;		/* GI and LTF symbol size */
	uint8 he_ltf_symb ;		/* num of HE-LTF symbols */
	uint8 stbc;			/* STBC support */
	uint8 coding_val;		/* BCC/LDPC coding support */
	uint8 pe_category;		/* PE duration 0/8/16usecs  */
	uint8 dcm;			/* dual carrier modulation */
	uint8 mumimo_ltfmode;		/* ltf mode */
	uint8 trig_tx;			/* form and transmit the trigger frame */
	uint8 trig_type;		/* type of trigger frame */
	uint8 trig_period;		/* trigger tx periodicity TBD */
	uint8 tgt_rssi;			/* target rssi value in encoded format */
	uint8 sub_band;			/* in 160MHz case, 80L, 80U */
	uint8 betrig;			/* 11BE (EHT) trigger format */
	uint8 ps160;			/* PS160 indicator, 160L, 160U */
} wl_pkteng_ru_v3_t;

/* To be deleted. Keep V4 until TRUNK/RHINO SRC updated to use V3 */
typedef struct wl_pkteng_ru_v4 {
	uint16 version;			/* ver is 4 */
	uint16 length;			/* size of complete structure */
	uint32 num_bytes;		/* approx num of bytes to calculate other required params */
	struct ether_addr dest;		/* destination address for un-associated mode */
	uint16 ru_alloc_val;		/* ru allocation index number */
	uint8 bw;			/* bandwidth info */
	uint8 mcs_val;			/* mcs allocated value */
	uint8 nss_val;			/* num of spatial streams */
	uint8 cp_ltf_val ;		/* GI and LTF symbol size */
	uint8 he_ltf_symb ;		/* num of HE-LTF symbols */
	uint8 stbc;			/* STBC support */
	uint8 coding_val;		/* BCC/LDPC coding support */
	uint8 pe_category;		/* PE duration 0/8/16usecs  */
	uint8 dcm;			/* dual carrier modulation */
	uint8 mumimo_ltfmode;		/* ltf mode */
	uint8 trig_tx;			/* form and transmit the trigger frame */
	uint8 trig_type;		/* type of trigger frame */
	uint8 trig_period;		/* trigger tx periodicity TBD */
	uint8 tgt_rssi;			/* target rssi value in encoded format */
	uint8 sub_band;			/* in 160MHz case, 80L, 80U */
	uint8 betrig;			/* 11BE (EHT) trigger format */
	uint8 ps160;			/* PS160 indicator, 160L, 160U */
	uint8 PAD[3];			/* 3B padding to make structure size a multiple of 32b */
} wl_pkteng_ru_v4_t;

#ifndef WL_PKTENG_RU_VER
/* App uses the latest version - source picks it up from wlc_types.h */
typedef wl_pkteng_ru_v3_t wl_pkteng_ru_fill_t;
#endif

typedef struct wl_trig_frame_info_v1 {
	/* Structure versioning and structure length params */
	uint16 version;
	uint16 length;
	/* Below params are the fields related to trigger frame contents */
	/* Common Info Params Figure 9-52d - 11ax Draft 1.1 */
	uint16 lsig_len;
	uint16 trigger_type;
	uint16 cascade_indication;
	uint16 cs_req;
	uint16 bw;
	uint16 cp_ltf_type;
	uint16 mu_mimo_ltf_mode;
	uint16 num_he_ltf_syms;
	uint16 stbc;
	uint16 ldpc_extra_symb;
	uint16 ap_tx_pwr;
	uint16 afactor;
	uint16 pe_disambiguity;
	uint16 spatial_resuse;
	uint16 doppler;
	uint16 he_siga_rsvd;
	uint16 cmn_info_rsvd;
	/* User Info Params Figure 9-52e - 11ax Draft 1.1 */
	uint16 aid12;
	uint16 ru_alloc;
	uint16 coding_type;
	uint16 mcs;
	uint16 dcm;
	uint16 ss_alloc;
	uint16 tgt_rssi;
	uint16 usr_info_rsvd;
} wl_trig_frame_info_v1_t;

typedef struct wl_trig_frame_info_v2 {
	/* Structure versioning and structure length params */
	uint16 version;
	uint16 length;
	/* Below params are the fields related to trigger frame contents */
	/* Common Info Params Figure 9-52d - 11ax Draft 1.1 */
	uint16 lsig_len;
	uint16 trigger_type;
	uint16 cascade_indication;
	uint16 cs_req;
	uint16 bw;
	uint16 cp_ltf_type;
	uint16 mu_mimo_ltf_mode;
	uint16 num_he_ltf_syms;
	uint16 stbc;
	uint16 ldpc_extra_symb;
	uint16 ap_tx_pwr;
	uint16 afactor;
	uint16 pe_disambiguity;
	uint16 spatial_resuse;
	uint16 doppler;
	uint16 he_siga_rsvd;
	uint16 cmn_info_rsvd;
	/* User Info Params Figure 9-52e - 11ax Draft 1.1 */
	uint16 aid12;
	uint16 ru_alloc;
	uint16 coding_type;
	uint16 mcs;
	uint16 dcm;
	uint16 ss_alloc;
	uint16 tgt_rssi;
	uint16 usr_info_rsvd;
	uint16 trigger_spec; // 0: AX trigger, 1: BE trigger
	/* 11BE additional Common Info fields */
	uint16 p160;
	uint16 susrinfoflg;
	/* 11BE additional User Info fields */
	uint16 ps160;
	/* 11BE Special User Info fields */
	uint16 bwext;
	uint16 eht_spatial_reuse1;
	uint16 eht_spatial_reuse2;
	uint16 PAD;
} wl_trig_frame_info_v2_t;

#ifdef WL11BE
typedef wl_trig_frame_info_v2_t wl_trig_frame_info_t;
#else
typedef wl_trig_frame_info_v1_t wl_trig_frame_info_t;
#endif

/* wl pkteng_stats related definitions */
#define WL_PKTENG_STATS_V1 (1)
#define WL_PKTENG_STATS_V2 (2)

typedef struct wl_pkteng_stats_v1 {
	uint32 lostfrmcnt;		/**< RX PER test: no of frames lost (skip seqno) */
	int32 rssi;			/**< RSSI */
	int32 snr;			/**< signal to noise ratio */
	uint16 rxpktcnt[NUM_80211_RATES+1];
	uint8 rssi_qdb;			/**< qdB portion of the computed rssi */
	uint8  version;
} wl_pkteng_stats_v1_t;

typedef struct wl_pkteng_stats_v2 {
	uint32 lostfrmcnt;		/**< RX PER test: no of frames lost (skip seqno) */
	int32 rssi;			/**< RSSI */
	int32 snr;			/**< signal to noise ratio */
	uint16 rxpktcnt[NUM_80211_RATES+1];
	uint8 rssi_qdb;			/**< qdB portion of the computed rssi */
	uint8  version;
	uint16 length;
	uint16 PAD;
	int32 rssi_per_core[WL_RSSI_ANT_MAX];
	int32 rssi_per_core_qdb[WL_RSSI_ANT_MAX];
} wl_pkteng_stats_v2_t;


typedef struct wl_txcal_params {
	wl_pkteng_t pkteng;
	uint8 gidx_start;
	int8 gidx_step;
	uint8 gidx_stop;
	uint8  PAD;
} wl_txcal_params_t;

typedef struct wl_txcal_gainidx {
	uint8 num_actv_cores;
	uint8 gidx_start_percore[WL_STA_ANT_MAX];
	uint8 gidx_stop_percore[WL_STA_ANT_MAX];
	uint8 PAD[3];
} wl_txcal_gainidx_t;

typedef struct wl_txcal_params_v2 {
	wl_pkteng_t pkteng;
	int8 gidx_step;
	uint8 pwr_start[WL_STA_ANT_MAX];
	uint8 pwr_stop[WL_STA_ANT_MAX];
	uint8 init_start_idx;
	uint8 gidx_start_percore[WL_STA_ANT_MAX];
	uint8 gidx_stop_percore[WL_STA_ANT_MAX];
	uint16 version;
} wl_txcal_params_v2_t;

typedef struct wl_txtone_idxsweep_params {
	int8 gidx_step;
	uint8 gidx_start_percore[WL_STA_ANT_MAX];
	uint8 gidx_stop_percore[WL_STA_ANT_MAX];
	uint8 PAD[3];
	uint32 dwell_time;
} wl_txtone_idxsweep_params;

typedef wl_txcal_params_t wl_txcal_params_v1_t;

typedef struct wl_rssilog_params {
	uint8 enable;
	uint8 rssi_threshold;
	uint8 time_threshold;
	uint8 PAD;
} wl_rssilog_params_t;


typedef struct wl_sslpnphy_papd_debug_data {
	uint8 psat_pwr;
	uint8 psat_indx;
	uint8 final_idx;
	uint8 start_idx;
	int32 min_phase;
	int32 voltage;
	int8 temperature;
	uint8  PAD[3];
} wl_sslpnphy_papd_debug_data_t;
typedef struct wl_sslpnphy_debug_data {
	int16 papdcompRe [64];
	int16 papdcompIm [64];
} wl_sslpnphy_debug_data_t;
typedef struct wl_sslpnphy_spbdump_data {
	uint16 tbl_length;
	int16 spbreal[256];
	int16 spbimg[256];
} wl_sslpnphy_spbdump_data_t;
typedef struct wl_sslpnphy_percal_debug_data {
	uint32 cur_idx;
	uint32 tx_drift;
	uint8 prev_cal_idx;
	uint8  PAD[3];
	uint32 percal_ctr;
	int32 nxt_cal_idx;
	uint32 force_1idxcal;
	uint32 onedxacl_req;
	int32 last_cal_volt;
	int8 last_cal_temp;
	uint8  PAD[3];
	uint32 vbat_ripple;
	uint32 exit_route;
	int32 volt_winner;
} wl_sslpnphy_percal_debug_data_t;

typedef enum {
	wowl_pattern_type_bitmap = 0,
	wowl_pattern_type_arp,
	wowl_pattern_type_na
} wowl_pattern_type_t;

typedef struct wl_wowl_pattern {
	uint32		    masksize;		/**< Size of the mask in #of bytes */
	uint32		    offset;		/**< Pattern byte offset in packet */
	uint32		    patternoffset;	/**< Offset of start of pattern in the structure */
	uint32		    patternsize;	/**< Size of the pattern itself in #of bytes */
	uint32		    id;			/**< id */
	uint32		    reasonsize;		/**< Size of the wakeup reason code */
	wowl_pattern_type_t type;		/**< Type of pattern */
	/* Mask follows the structure above */
	/* Pattern follows the mask is at 'patternoffset' from the start */
} wl_wowl_pattern_t;

typedef struct wl_wowl_pattern_list {
	uint32			count;
	wl_wowl_pattern_t	pattern[BCM_FLEX_ARRAY];
} wl_wowl_pattern_list_t;

typedef struct wl_wowl_wakeind {
	uint8	pci_wakeind;	/**< Whether PCI PMECSR PMEStatus bit was set */
	uint32	ucode_wakeind;	/**< What wakeup-event indication was set by ucode */
} wl_wowl_wakeind_t;

/** per AC rate control related data structure */
typedef struct wl_txrate_class {
	uint8		init_rate;
	uint8		min_rate;
	uint8		max_rate;
} wl_txrate_class_t;

/** structure for Overlap BSS scan arguments */
typedef struct wl_obss_scan_arg {
	int16	passive_dwell;
	int16	active_dwell;
	int16	bss_widthscan_interval;
	int16	passive_total;
	int16	active_total;
	int16	chanwidth_transition_delay;
	int16	activity_threshold;
} wl_obss_scan_arg_t;

#define WL_OBSS_SCAN_PARAM_LEN	sizeof(wl_obss_scan_arg_t)

/** RSSI event notification configuration. */
typedef struct wl_rssi_event {
	uint32 rate_limit_msec;		/**< # of events posted to application will be limited to
					 * one per specified period (0 to disable rate limit).
					 */
	uint8 num_rssi_levels;		/**< Number of entries in rssi_levels[] below */
	int8 rssi_levels[MAX_RSSI_LEVELS];	/**< Variable number of RSSI levels. An event
						 * will be posted each time the RSSI of received
						 * beacons/packets crosses a level.
						 */
	int8 PAD[3];
} wl_rssi_event_t;

#define RSSI_MONITOR_VERSION_1    1
#define RSSI_MONITOR_STOP       (1 << 0)
typedef struct wl_rssi_monitor_cfg {
	uint8 version;
	uint8 flags;
	int8 max_rssi;
	int8 min_rssi;
} wl_rssi_monitor_cfg_t;

typedef struct wl_rssi_monitor_evt {
	uint8 version;
	int8 cur_rssi;
	uint16 pad;
} wl_rssi_monitor_evt_t;

/* CCA based channel quality event configuration (ID values for both config and report) */
#define WL_CHAN_QUAL_CCA	0
#define WL_CHAN_QUAL_NF		1
#define WL_CHAN_QUAL_NF_LTE	2
#define WL_CHAN_QUAL_TOTAL	3	/* The total IDs supported in both config and report */
/* Additional channel quality event support in report only (>= 0x100)
 * Notice that uint8 is used in configuration struct wl_chan_qual_metric_t, but uint16 is
 * used for report in struct cca_chan_qual_event_t. So the ID values beyond 8-bit are used
 * for reporting purpose only.
 * (The MSBs are not used as bit-mask. These defines are values)
 */
#define WL_CHAN_QUAL_FULL_CCA	(0x100u | WL_CHAN_QUAL_CCA)	/* CCA: ibss vs. obss */
#define WL_CHAN_QUAL_FULLPM_CCA	(0x200u | WL_CHAN_QUAL_CCA)	/* CCA: me vs. notme, PM vs. !PM */
#define WL_CHAN_QUAL_FULLPM_CCA_UNUSED		(0x300u | WL_CHAN_QUAL_CCA)
#define WL_CHAN_QUAL_FULLPM_CCA_OFDM_DESENSE	(0x400u | WL_CHAN_QUAL_CCA)
#define WL_CHAN_QUAL_FULLPM_CCA_ALL_DESENSE	(0x500u | WL_CHAN_QUAL_CCA)
/* CCA: me vs. notme, PM vs. !PM with OFDM Desense */

#define MAX_CHAN_QUAL_LEVELS	8

typedef struct wl_chan_qual_metric {
	uint8 id;				/**< metric ID */
	uint8 num_levels;			/**< Number of entries in rssi_levels[] below */
	uint16 flags;
	int16 htol[MAX_CHAN_QUAL_LEVELS];	/**< threshold level array: hi-to-lo */
	int16 ltoh[MAX_CHAN_QUAL_LEVELS];	/**< threshold level array: lo-to-hi */
} wl_chan_qual_metric_t;

typedef struct wl_chan_qual_event {
	uint32 rate_limit_msec;		/**< # of events posted to application will be limited to
					 * one per specified period (0 to disable rate limit).
					 */
	uint16 flags;
	uint16 num_metrics;
	wl_chan_qual_metric_t metric[WL_CHAN_QUAL_TOTAL];	/**< metric array */
} wl_chan_qual_event_t;
typedef struct wl_action_obss_coex_req {
	uint8 info;
	uint8 num;
	uint8 ch_list[BCM_FLEX_ARRAY];
} wl_action_obss_coex_req_t;


/** IOVar parameter block for small MAC address array with type indicator */
#define WL_IOV_MAC_PARAM_LEN  4

/** This value is hardcoded to be 16 and MUST match PKTQ_MAX_PREC value defined elsewhere */
#define WL_IOV_PKTQ_LOG_PRECS 16

#include <packed_section_start.h>
typedef BWL_PRE_PACKED_STRUCT struct {
	uint32 num_addrs;
	uint8   addr_type[WL_IOV_MAC_PARAM_LEN];
	struct ether_addr ea[WL_IOV_MAC_PARAM_LEN];
} BWL_POST_PACKED_STRUCT wl_iov_mac_params_t;
#include <packed_section_end.h>

/** This is extra info that follows wl_iov_mac_params_t */
typedef struct {
	uint32 addr_info[WL_IOV_MAC_PARAM_LEN];
} wl_iov_mac_extra_params_t;

/** Combined structure */
typedef struct {
	wl_iov_mac_params_t params;
	wl_iov_mac_extra_params_t extra_params;
} wl_iov_mac_full_params_t;

/** Parameter block for PKTQ_LOG statistics */
/* NOTE: this structure cannot change! It is exported to wlu as a binary format
 * A new format revision number must be created if the interface changes
 * The latest is v05; previous v01...v03 are no longer supported, v04 has
 * common base with v05
*/
#define PKTQ_LOG_COUNTERS_V4 \
	/* packets requested to be stored */ \
	uint32 requested; \
	/* packets stored */ \
	uint32 stored; \
	/* packets saved, because a lowest priority queue has given away one packet */ \
	uint32 saved; \
	/* packets saved, because an older packet from the same queue has been dropped */ \
	uint32 selfsaved; \
	/* packets dropped, because pktq is full with higher precedence packets */ \
	uint32 full_dropped; \
	 /* packets dropped because pktq per that precedence is full */ \
	uint32 dropped; \
	/* packets dropped, in order to save one from a queue of a highest priority */ \
	uint32 sacrificed; \
	/* packets droped because of hardware/transmission error */ \
	uint32 busy; \
	/* packets re-sent because they were not received */ \
	uint32 retry; \
	/* packets retried again (ps pretend) prior to moving power save mode */ \
	uint32 ps_retry; \
	 /* suppressed packet count */ \
	uint32 suppress; \
	/* packets finally dropped after retry limit */ \
	uint32 retry_drop; \
	/* the high-water mark of the queue capacity for packets - goes to zero as queue fills */ \
	uint32 max_avail; \
	/* the high-water mark of the queue utilisation for packets - ('inverse' of max_avail) */ \
	uint32 max_used; \
	 /* the maximum capacity of the queue */ \
	uint32 queue_capacity; \
	/* count of rts attempts that failed to receive cts */ \
	uint32 rtsfail; \
	/* count of packets sent (acked) successfully */ \
	uint32 acked; \
	/* running total of phy rate of packets sent successfully */ \
	uint32 txrate_succ; \
	/* running total of phy 'main' rate */ \
	uint32 txrate_main; \
	/* actual data transferred successfully */ \
	uint32 throughput; \
	/* time difference since last pktq_stats */ \
	uint32 time_delta;

typedef struct {
	PKTQ_LOG_COUNTERS_V4
} pktq_log_counters_v04_t;

/** v5 is the same as V4 with extra parameter */
typedef struct {
	PKTQ_LOG_COUNTERS_V4
	/** cumulative time to transmit */
	uint32 airtime;
} pktq_log_counters_v05_t;

typedef struct {
	uint8                num_prec[WL_IOV_MAC_PARAM_LEN];
	pktq_log_counters_v04_t  counters[WL_IOV_MAC_PARAM_LEN][WL_IOV_PKTQ_LOG_PRECS];
	uint32               counter_info[WL_IOV_MAC_PARAM_LEN];
	uint32               pspretend_time_delta[WL_IOV_MAC_PARAM_LEN];
	char                 headings[];
} pktq_log_format_v04_t;

typedef struct {
	uint8                num_prec[WL_IOV_MAC_PARAM_LEN];
	pktq_log_counters_v05_t  counters[WL_IOV_MAC_PARAM_LEN][WL_IOV_PKTQ_LOG_PRECS];
	uint32               counter_info[WL_IOV_MAC_PARAM_LEN];
	uint32               pspretend_time_delta[WL_IOV_MAC_PARAM_LEN];
	char                 headings[];
} pktq_log_format_v05_t;


typedef struct {
	uint32               version;
	wl_iov_mac_params_t  params;
	union {
		pktq_log_format_v04_t v04;
		pktq_log_format_v05_t v05;
	} pktq_log;
} wl_iov_pktq_log_t;

/* PKTQ_LOG_AUTO, PKTQ_LOG_DEF_PREC flags introduced in v05, they are ignored by v04 */
#define PKTQ_LOG_AUTO     (1 << 31)
#define PKTQ_LOG_DEF_PREC (1 << 30)

typedef struct wl_pfn_macaddr_cfg_0 {
	uint8 version;
	uint8 reserved;
	struct ether_addr macaddr;
} wl_pfn_macaddr_cfg_0_t;
#define LEGACY1_WL_PFN_MACADDR_CFG_VER 0
#define WL_PFN_MAC_OUI_ONLY_MASK      1
#define WL_PFN_SET_MAC_UNASSOC_MASK   2
#define WL_PFN_RESTRICT_LA_MAC_MASK   4
#define WL_PFN_MACADDR_FLAG_MASK     0x7
/** To configure pfn_macaddr */
typedef struct wl_pfn_macaddr_cfg {
	uint8 version;
	uint8 flags;
	struct ether_addr macaddr;
} wl_pfn_macaddr_cfg_t;
#define WL_PFN_MACADDR_CFG_VER 1

/*
 * SCB_BS_DATA iovar definitions start.
 */
#define SCB_BS_DATA_STRUCT_VERSION	1

/** The actual counters maintained for each station */
typedef struct {
	/* The following counters are a subset of what pktq_stats provides per precedence. */
	uint32 retry;          /**< packets re-sent because they were not received */
	uint32 retry_drop;     /**< packets finally dropped after retry limit */
	uint32 rtsfail;        /**< count of rts attempts that failed to receive cts */
	uint32 acked;          /**< count of packets sent (acked) successfully */
	uint32 txrate_succ;    /**< running total of phy rate of packets sent successfully */
	uint32 txrate_main;    /**< running total of phy 'main' rate */
	uint32 throughput;     /**< actual data transferred successfully */
	uint32 time_delta;     /**< time difference since last pktq_stats */
	uint32 airtime;        /**< cumulative total medium access delay in useconds */
} iov_bs_data_counters_t;

/** The structure for individual station information. */
#include <packed_section_start.h>
typedef BWL_PRE_PACKED_STRUCT struct {
	struct ether_addr	station_address;	/**< The station MAC address */
	uint16			station_flags;		/**< Bit mask of flags, for future use. */
	iov_bs_data_counters_t	station_counters;	/**< The actual counter values */
} BWL_POST_PACKED_STRUCT iov_bs_data_record_t;
#include <packed_section_end.h>

#include <packed_section_start.h>
typedef BWL_PRE_PACKED_STRUCT struct {
	uint16	structure_version;	/**< Structure version number (for wl/wlu matching) */
	uint16	structure_count;	/**< Number of iov_bs_data_record_t records following */
	iov_bs_data_record_t structure_record[BCM_FLEX_ARRAY];	/**< 0 - structure_count records */
} BWL_POST_PACKED_STRUCT iov_bs_data_struct_t;
#include <packed_section_end.h>

/* Bitmask of options that can be passed in to the iovar. */
enum {
	SCB_BS_DATA_FLAG_NO_RESET = (1<<0)	/**< Do not clear the counters after reading */
};
/*
 * SCB_BS_DATA iovar definitions end.
 */

typedef struct wlc_extlog_cfg {
	int32 max_number;
	uint16 module;	/**< bitmap */
	uint8 level;
	uint8 flag;
	uint16 version;
	uint16 PAD;
} wlc_extlog_cfg_t;

typedef struct log_record {
	uint32 time;
	uint16 module;
	uint16 id;
	uint8 level;
	uint8 sub_unit;
	uint8 seq_num;
	uint8 PAD;
	int32 arg;
	char  str[MAX_ARGSTR_LEN];
	char  PAD[4-MAX_ARGSTR_LEN%4];
} log_record_t;

typedef struct wlc_extlog_req {
	uint32 from_last;
	uint32 num;
} wlc_extlog_req_t;

typedef struct wlc_extlog_results {
	uint16 version;
	uint16 record_len;
	uint32 num;
	log_record_t logs[BCM_FLEX_ARRAY];
} wlc_extlog_results_t;

typedef struct log_idstr {
	uint16	id;
	uint16	flag;
	uint8	arg_type;
	const char	*fmt_str;
} log_idstr_t;

#define FMTSTRF_USER		1

/* flat ID definitions
 * New definitions HAVE TO BE ADDED at the end of the table. Otherwise, it will
 * affect backward compatibility with pre-existing apps
 */
typedef enum {
	FMTSTR_DRIVER_UP_ID = 0,
	FMTSTR_DRIVER_DOWN_ID = 1,
	FMTSTR_SUSPEND_MAC_FAIL_ID = 2,
	FMTSTR_NO_PROGRESS_ID = 3,
	FMTSTR_RFDISABLE_ID = 4,
	FMTSTR_REG_PRINT_ID = 5,
	FMTSTR_EXPTIME_ID = 6,
	FMTSTR_JOIN_START_ID = 7,
	FMTSTR_JOIN_COMPLETE_ID = 8,
	FMTSTR_NO_NETWORKS_ID = 9,
	FMTSTR_SECURITY_MISMATCH_ID = 10,
	FMTSTR_RATE_MISMATCH_ID = 11,
	FMTSTR_AP_PRUNED_ID = 12,
	FMTSTR_KEY_INSERTED_ID = 13,
	FMTSTR_DEAUTH_ID = 14,
	FMTSTR_DISASSOC_ID = 15,
	FMTSTR_LINK_UP_ID = 16,
	FMTSTR_LINK_DOWN_ID = 17,
	FMTSTR_RADIO_HW_OFF_ID = 18,
	FMTSTR_RADIO_HW_ON_ID = 19,
	FMTSTR_EVENT_DESC_ID = 20,
	FMTSTR_PNP_SET_POWER_ID = 21,
	FMTSTR_RADIO_SW_OFF_ID = 22,
	FMTSTR_RADIO_SW_ON_ID = 23,
	FMTSTR_PWD_MISMATCH_ID = 24,
	FMTSTR_FATAL_ERROR_ID = 25,
	FMTSTR_AUTH_FAIL_ID = 26,
	FMTSTR_ASSOC_FAIL_ID = 27,
	FMTSTR_IBSS_FAIL_ID = 28,
	FMTSTR_EXTAP_FAIL_ID = 29,
	FMTSTR_MAX_ID
} log_fmtstr_id_t;

/** 11k Neighbor Report element (unversioned, deprecated) */
typedef dot11_neighbor_rep_ie_t nbr_element_t;

#define WL_RRM_NBR_RPT_VER		1

#define WL_NBR_RPT_FLAG_BSS_PREF_FROM_AP  0x01
/** 11k Neighbor Report element */
typedef struct nbr_rpt_elem {
	uint8 version;
	uint8 id;
	uint8 len;
	uint8 PAD;
	struct ether_addr bssid;
	uint8 pad_1[2];
	uint32 bssid_info;
	uint8 reg;
	uint8 channel;
	uint8 phytype;
	uint8 addtype; /* static for manual add or dynamic if auto-learning of neighbors */
	wlc_ssid_t ssid;
	chanspec_t chanspec;
	uint8 bss_trans_preference;
	uint8 flags;
} nbr_rpt_elem_t;

typedef enum event_msgs_ext_command {
	EVENTMSGS_NONE		=	0,
	EVENTMSGS_SET_BIT	=	1,
	EVENTMSGS_RESET_BIT	=	2,
	EVENTMSGS_SET_MASK	=	3
} event_msgs_ext_command_t;

#define EVENTMSGS_VER 1
/* Do not use below definition in newer code as
 * BCM_FLEX_ARRAY may not be defined to 1
 */
#define EVENTMSGS_EXT_STRUCT_SIZE	((uint)(sizeof(eventmsgs_ext_t) - 1))

/* len-	for SET it would be mask size from the application to the firmware */
/*		for GET it would be actual firmware mask size */
/* maxgetsize -	is only used for GET. indicate max mask size that the */
/*				application can read from the firmware */
typedef struct eventmsgs_ext
{
	uint8	ver;
	uint8	command;
	uint8	len;
	uint8	maxgetsize;
	uint8	mask[BCM_FLEX_ARRAY];
} eventmsgs_ext_t;

#include <packed_section_start.h>
typedef BWL_PRE_PACKED_STRUCT struct pcie_bus_tput_params {
	/** no of host dma descriptors programmed by the firmware before a commit */
	uint16		max_dma_descriptors;

	uint16		host_buf_len; /**< length of host buffer */
	dmaaddr_t	host_buf_addr; /**< physical address for bus_throughput_buf */
} BWL_POST_PACKED_STRUCT pcie_bus_tput_params_t;
#include <packed_section_end.h>

typedef struct pcie_bus_tput_stats {
	uint16		time_taken; /**< no of secs the test is run */
	uint16		nbytes_per_descriptor; /**< no of bytes of data dma ed per descriptor */

	/** no of desciptors for which dma is sucessfully completed within the test time */
	uint32		count;
} pcie_bus_tput_stats_t;

/* limits of dma length */
/* dma max size is 65527 but to keep it 4byte aligned, rounding off to FLOOR 65524 */
#define DMA_LEN_MAX	(65524u)
#define DMA_LEN_MIN	(8u)

/* direction of DMA */
#define BUS_TPUT_PARAMS_FLAG_DIR_H2D		0x0u
#define BUS_TPUT_PARAMS_FLAG_DIR_D2H		0x1u

#define BUS_TPUT_PARAMS_VERSION_1	(1u)
#include <packed_section_start.h>
BWL_PRE_PACKED_STRUCT struct bus_tput_params_v1 {
	uint16  ver; /**<version */
	uint16  len; /**<length */
	uint32  flags; /**< param flags */
	uint16  dma_descriptors; /**<total # of dma descs to be programmed by f/w */
	uint16  host_buf_len; /**< length of host buffer */
	uint32	host_buf_addr_lo; /**< physical address for bus_throughput_buf_lo */
	uint32	host_buf_addr_hi; /**< physical address for bus_throughput_buf_hi */
} BWL_POST_PACKED_STRUCT;
#include <packed_section_end.h>

/* direction of DMA */
#define BUS_TPUT_STATS_FLAG_DIR_H2D		0x0u
#define BUS_TPUT_STATS_FLAG_DIR_D2H		0x1u

/* after stats, if buf is available with string to print */
#define BUS_TPUT_STATS_FLAG_BUF_PRINT		0x2u

#define BUS_TPUT_STATS_VERSION_1	(1u)
#include <packed_section_start.h>
BWL_PRE_PACKED_STRUCT struct bus_tput_stats_v1 {
	uint16	ver; /**<version */
	uint16	len; /**<length */
	uint32  time_taken; /**< no of usecs the test is run */
	uint16  count;  /**< no of dma desc transferred */
	uint16  nbytes_per_descriptor; /**< no of bytes of data dma ed per descriptor */
	uint32  flags; /**< stats flags */
} BWL_POST_PACKED_STRUCT;
#include <packed_section_end.h>

#define HOST_WAKEUP_DATA_VER 1
#include <packed_section_start.h>
/* Bus interface host wakeup data */
typedef BWL_PRE_PACKED_STRUCT struct wl_host_wakeup_data {
	uint16 ver;
	uint16 len;
	uchar data[BCM_FLEX_ARRAY];	/* wakeup data */
} BWL_POST_PACKED_STRUCT wl_host_wakeup_data_t;
#include <packed_section_end.h>

#define HOST_WAKEUP_DATA_VER_2 2
#include <packed_section_start.h>
/* Bus interface host wakeup data */
typedef BWL_PRE_PACKED_STRUCT struct wl_host_wakeup_data_v2 {
	uint16 ver;
	uint16 len;
	uint32 gpio_toggle_time; /* gpio toggle time in ms */
	uchar data[BCM_FLEX_ARRAY];	/* wakeup data */
} BWL_POST_PACKED_STRUCT wl_host_wakeup_data_v2_t;
#include <packed_section_end.h>

typedef struct keepalives_max_idle {
	uint16  keepalive_count;        /**< nmbr of keepalives per bss_max_idle period */
	uint8   mkeepalive_index;       /**< mkeepalive_index for keepalive frame to be used */
	uint8   PAD;			/**< to align next field */
	uint16  max_interval;           /**< seconds */
} keepalives_max_idle_t;

#define PM_IGNORE_BCMC_PROXY_ARP (1 << 0)
#define PM_IGNORE_BCMC_ALL_DMS_ACCEPTED (1 << 1)

/* ##### HMAP section ##### */
#define PCIE_MAX_HMAP_WINDOWS 8u
#define PCIE_HMAPTEST_VERSION_2 2u
#define HMAPTEST_INVALID_OFFSET 0xFFFFFFFFu
#define HMAPTEST_DEFAULT_WRITE_PATTERN 0xBABECAFEu
#define HMAPTEST_ACCESS_ARM 0u
#define HMAPTEST_ACCESS_M2M 1u
#define HMAPTEST_ACCESS_D11 2u
#define HMAPTEST_ACCESS_NONE 3u

typedef struct pcie_hmaptest {
	uint16	version;		/* Version */
	uint16	length;		/* Length of entire structure */
	uint32	xfer_len;
	uint32	accesstype;
	uint32	is_write;
	uint32	is_invalid;
	uint32	host_addr_hi;
	uint32	host_addr_lo;
	uint32	host_offset;
	uint32  value; /* 4 byte value to be filled in case of write access test */
	uint32	delay; /* wait time  in seconds before initiating access from dongle */
} pcie_hmaptest_t;

/* HMAP window register set */
typedef struct hmapwindow {
	uint32 baseaddr_lo; /* BaseAddrLower */
	uint32 baseaddr_hi; /* BaseAddrUpper */
	uint32 windowlength; /* Window Length */
} hmapwindow_t;

#define PCIE_HMAP_VERSION_1	1u
typedef struct pcie_hmap {
	uint16	version;		/**< Version */
	uint16	length;			/**< Length of entire structure */
	uint32	enable;			/**< status of HMAP enabled/disabled */
	uint32	nwindows;		/* no. of HMAP windows enabled */
	uint32	window_config;		/* HMAP window_config register */
	uint32	hmap_violationaddr_lo;	/* violating address lo */
	uint32	hmap_violationaddr_hi;	/* violating addr hi */
	uint32	hmap_violation_info;	/* violation info */
	hmapwindow_t hwindows[];	/* Multiple hwindows */
} pcie_hmap_t;

/** IPV4 Arp offloads for ndis context */
#include <packed_section_start.h>
BWL_PRE_PACKED_STRUCT struct hostip_id {
	struct ipv4_addr ipa;
	uint8 id;
} BWL_POST_PACKED_STRUCT;
#include <packed_section_end.h>

/* Return values */
#define ND_REPLY_PEER		0x1	/**< Reply was sent to service NS request from peer */
#define ND_REQ_SINK		0x2	/**< Input packet should be discarded */
#define ND_FORCE_FORWARD	0X3	/**< For the dongle to forward req to HOST */

/** Neighbor Solicitation Response Offload IOVAR param */
#include <packed_section_start.h>
typedef BWL_PRE_PACKED_STRUCT struct nd_param {
	struct ipv6_addr	host_ip[2];
	struct ipv6_addr	solicit_ip;
	struct ipv6_addr	remote_ip;
	uint8	host_mac[ETHER_ADDR_LEN];
	uint32	offload_id;
} BWL_POST_PACKED_STRUCT nd_param_t;
#include <packed_section_end.h>

/* restricted_roam IOVAR */
#define WLC_RESTRICTED_ROAM_CONFIG_VER_1 (1u)
typedef struct wlc_restricted_roam_cfg_v1 {
	uint16	ver;			/* Version */
	uint16	len;			/* Length of the structure */
	uint32	lowrssi_roam_thresh;	/* time thresh for restricting low rssi roam */
} wlc_restricted_roam_cfg_v1_t;

enum wlc_restricted_roam_cmd_id {
	WLC_RESTRICTED_ROAM_XTLV_LOWRSSI_ROAM_THRESH_ID		= 1u,
	WLC_RESTRICTED_ROAM_XTLV_LAST_ID
};

typedef struct wl_pfn_roam_thresh {
	uint32 pfn_alert_thresh; /**< time in ms */
	uint32 roam_alert_thresh; /**< time in ms */
} wl_pfn_roam_thresh_t;


/* Reasons for wl_pmalert_t */
#define PM_DUR_EXCEEDED			(1<<0)
#define MPC_DUR_EXCEEDED		(1<<1)
#define ROAM_ALERT_THRESH_EXCEEDED	(1<<2)
#define PFN_ALERT_THRESH_EXCEEDED	(1<<3)
#define CONST_AWAKE_DUR_ALERT		(1<<4)
#define CONST_AWAKE_DUR_RECOVERY	(1<<5)
#define RESTRICTED_LOWRSSI_ROAM_THRESH_EXCEEDED	(1<<6)

#define MIN_PM_ALERT_LEN 9

/** Data sent in EXCESS_PM_WAKE event */
#define WL_PM_ALERT_VERSION_3 3

/** This structure is for version 3; version 2 will be deprecated in by FW */
#include <packed_section_start.h>
typedef BWL_PRE_PACKED_STRUCT struct wl_pmalert {
	uint16 version;		/**< Version = 3 is TLV format */
	uint16 length;		/**< Length of entire structure */
	uint32 reasons;		/**< reason(s) for pm_alert */
	uint8 data[BCM_FLEX_ARRAY];
				/**< TLV data, a series of structures,
				 * each starting with type and length.
				 *
				 * Padded as necessary so each section
				 * starts on a 4-byte boundary.
				 *
				 * Both type and len are uint16, but the
				 * upper nibble of length is reserved so
				 * valid len values are 0-4095.
				*/
} BWL_POST_PACKED_STRUCT wl_pmalert_t;
#include <packed_section_end.h>

#define NUM_P2P_BSS_UCODE_DBG          3
#define IDX_P2P_BSS_UCODE_DBG          12
#define NUM_PSM_PHY_DBG                20

/* Type values for the data section */
#define WL_PMALERT_FIXED	0	/**< struct wl_pmalert_fixed_t, fixed fields */
#define WL_PMALERT_PMSTATE	1	/**< struct wl_pmalert_pmstate_t, variable */
#define WL_PMALERT_EVENT_DUR	2	/**< struct wl_pmalert_event_dur_t, variable */
#define WL_PMALERT_UCODE_DBG	3	/**< struct wl_pmalert_ucode_dbg_v1, variable */
#define WL_PMALERT_PS_ALLOWED_HIST	4 /**< struct wl_pmalert_ps_allowed_history, variable */
#define WL_PMALERT_EXT_UCODE_DBG	5 /**< struct wl_pmalert_ext_ucode_dbg_t, variable */
#define WL_PMALERT_EPM_START_EVENT_DUR	6 /**< struct wl_pmalert_event_dur_t, variable */
#define WL_PMALERT_UCODE_DBG_V2		7 /**< struct wl_pmalert_ucode_dbg_v2, variable */
#define WL_PMALERT_FIXED_SC		8 /**< struct wl_pmalert_fixed_sc_t, fixed fields */
#define WL_PMALERT_UCODE_DBG_SC		9 /**< struct wl_pmalert_ucode_dbg_v2, variable */

typedef struct wl_pmalert_fixed {
	uint16 type;		/**< WL_PMALERT_FIXED */
	uint16 len;		/**< Up to 4K-1, top 4 bits are reserved */
	uint32 prev_stats_time;	/**< msecs */
	uint32 curr_time;	/**< ms */
	uint32 prev_pm_dur;	/**< msecs */
	uint32 pm_dur;		/**< Total sleep time in PM, msecs */
	uint32 prev_mpc_dur;	/**< msecs */
	uint32 mpc_dur;		/**< Total sleep time in MPC, msecs */
	uint32 hw_macc;		/**< HW maccontrol */
	uint32 sw_macc;		/**< SW maccontrol */

	/* int32 drifts = remote - local; +ve drift -> local-clk slow */
	int32 last_drift;	/**< Most recent TSF drift from beacon */
	int32 min_drift;	/**< Min TSF drift from beacon in magnitude */
	int32 max_drift;	/**< Max TSF drift from beacon in magnitude */

	uint32 avg_drift;	/**< Avg TSF drift from beacon */
	uint32 drift_cnt;	/**< Count of drift readings over which avg_drift was computed */
	uint32 frts_time;	/**< Cumulative ms spent in data frts since driver load */
	uint32 frts_end_cnt;	/**< No of times frts ended since driver load */
	uint32 prev_frts_dur;	/**< Data frts duration at start of pm-period */
	uint32 cal_dur;		/**< Cumulative ms spent in calibration */
	uint32 prev_cal_dur;	/**< cal duration at start of pm-period */
} wl_pmalert_fixed_t;

#define WL_PMALERT_SC_FLAG_SC_DOWN           (1u << 0u)
#define WL_PMALERT_SC_FLAG_SC_HIBERNATE      (1u << 1u)

typedef struct wl_pmalert_fixed_sc {
	uint16 type;            /* WL_PMALERT_FIXED_SC */
	uint16 len;             /* Up to 4K-1, top 4 bits are reserved */
	uint32 flags;
	uint32 prev_stats_time; /* msecs */
	uint32 curr_time;       /*  msecs */
	uint32 prev_pm_dur;     /* msecs */
	uint32 pm_dur;          /* Total sleep time in PM, msecs */
	uint32 win_down_time;   /* Total down time in the window in ms */
	uint32 hw_macc;         /* HW maccontrol */
	uint32 sw_macc;         /* SW maccontrol */

	/* int32 drifts = remote - local; +ve drift -> local-clk slow */
	int32 last_drift;   /* Most recent TSF drift from beacon */
	int32 min_drift;    /* Min TSF drift from beacon in magnitude */
	int32 max_drift;    /* Max TSF drift from beacon in magnitude */
	uint32 avg_drift;   /* Avg TSF drift from beacon */
	uint32 drift_cnt;   /* Count of drift readings over which avg_drift was computed */

	uint32 cal_dur;      /* Cumulative ms spent in calibration */
	uint32 prev_cal_dur; /* cal duration at start of pm-period */
} wl_pmalert_fixed_sc_t;

typedef struct wl_pmalert_pmstate {
	uint16 type;	     /**< WL_PMALERT_PMSTATE */
	uint16 len;	     /**< Up to 4K-1, top 4 bits are reserved */

	uint8 pmwake_idx;   /**< for stepping through pm_state */
	uint8 PAD[3];
	/* Array of pmstate; len of array is based on tlv len */
	wlc_pm_debug_t pmstate[BCM_FLEX_ARRAY];
} wl_pmalert_pmstate_t;

typedef struct wl_pmalert_event_dur {
	uint16 type;	     /**< WL_PMALERT_EVENT_DUR */
	uint16 len;	     /**< Up to 4K-1, top 4 bits are reserved */

	/* Array of event_dur, len of array is based on tlv len */
	uint32 event_dur[BCM_FLEX_ARRAY];
} wl_pmalert_event_dur_t;

#include <packed_section_start.h>
BWL_PRE_PACKED_STRUCT struct wl_pmalert_ucode_dbg_v1 {
	uint16 type;         /* WL_PMALERT_UCODE_DBG */
	uint16 len;      /* Up to 4K-1, top 4 bits are reserved */
	uint32 macctrl;
	uint16 m_p2p_hps;
	uint32 psm_brc;
	uint32 ifsstat;
	uint16 m_p2p_bss_dtim_prd[MAX_P2P_BSS_DTIM_PRD];
	uint32 psmdebug[NUM_PSM_PHY_DBG];
	uint32 phydebug[NUM_PSM_PHY_DBG];
	uint16 M_P2P_BSS[NUM_P2P_BSS_UCODE_DBG][IDX_P2P_BSS_UCODE_DBG];
	uint16 M_P2P_PRE_TBTT[NUM_P2P_BSS_UCODE_DBG];

	/* Following is valid only for corerevs<40 */
	uint16 xmtfifordy;

	/* Following 3 are valid only for 11ac corerevs (>=40) */
	uint16 psm_maccommand;
	uint16 txe_status1;
	uint16 AQMFifoReady;
} BWL_POST_PACKED_STRUCT;
#include <packed_section_end.h>

#include <packed_section_start.h>
BWL_PRE_PACKED_STRUCT struct wl_pmalert_ucode_dbg_v2 {
	uint16 type;	     /**< WL_PMALERT_UCODE_DBG_V2 */
	uint16 len;	     /**< Up to 4K-1, top 4 bits are reserved */
	uint32 macctrl;
	uint16 m_p2p_hps;
	uint32 psm_brc;
	uint32 ifsstat;
	uint16 m_p2p_bss_dtim_prd[MAX_P2P_BSS_DTIM_PRD];
	uint32 psmdebug[NUM_PSM_PHY_DBG];
	uint32 phydebug[NUM_PSM_PHY_DBG];
	uint16 M_P2P_BSS[NUM_P2P_BSS_UCODE_DBG][IDX_P2P_BSS_UCODE_DBG];
	uint16 M_P2P_PRE_TBTT[NUM_P2P_BSS_UCODE_DBG];

	/* Following is valid only for corerevs<40 */
	uint16 xmtfifordy;

	/* Following 3 are valid only for 11ac corerevs (>=40) */
	uint16 psm_maccommand;
	uint16 txe_status1;
	uint32 AQMFifoReady;
} BWL_POST_PACKED_STRUCT;
#include <packed_section_end.h>

#include <packed_section_start.h>
BWL_PRE_PACKED_STRUCT struct wl_pmalert_ucode_dbg_sc_v1 {
	uint16 type;	     /**< WL_PMALERT_UCODE_DBG_SC */
	uint16 len;	     /**< Up to 4K-1, top 4 bits are reserved */
	/* ucode SHM and registers */
	uint32 macctrl;
	uint16 m_p2p_hps;
	uint32 psm_brc;
	uint32 ifsstat;
	uint16 m_p2p_bss_dtim_prd[MAX_P2P_BSS_DTIM_PRD];
	uint32 psmdebug[NUM_PSM_PHY_DBG];
	uint32 phydebug[NUM_PSM_PHY_DBG];
	uint16 M_P2P_BSS[NUM_P2P_BSS_UCODE_DBG][IDX_P2P_BSS_UCODE_DBG];
	uint16 M_P2P_PRE_TBTT[NUM_P2P_BSS_UCODE_DBG];
	uint16 psm_maccommand;
} BWL_POST_PACKED_STRUCT;
#include <packed_section_end.h>

typedef struct wlc_ps_debug {
	uint32 timestamp;	     /**< timestamp in millisecond */
	uint32 ps_mask;		     /**< reason(s) for disallowing ps */
} wlc_ps_debug_t;

typedef struct wl_pmalert_ps_allowed_hist {
	uint16 type;	     /**< WL_PMALERT_PS_ALLOWED_HIST */
	uint16 len;	     /**< Up to 4K-1, top 4 bits are reserved */
	uint32 ps_allowed_start_idx;
	/* Array of ps_debug, len of array is based on tlv len */
	wlc_ps_debug_t ps_debug[BCM_FLEX_ARRAY];
} wl_pmalert_ps_allowed_hist_t;

/* Structures and constants used for "vndr_ie" IOVar interface */
#define VNDR_IE_CMD_LEN		4	/**< length of the set command string:
					 * "add", "del" (+ NUL)
					 */

#define VNDR_IE_INFO_HDR_LEN	(sizeof(uint32))

#include <packed_section_start.h>
typedef BWL_PRE_PACKED_STRUCT struct {
	uint32 pktflag;			/**< bitmask indicating which packet(s) contain this IE */
	vndr_ie_t vndr_ie_data;		/**< vendor IE data */
} BWL_POST_PACKED_STRUCT vndr_ie_info_t;
#include <packed_section_end.h>

#include <packed_section_start.h>
typedef BWL_PRE_PACKED_STRUCT struct {
	int32 iecount;					/**< entries in the vndr_ie_list[] */
	vndr_ie_info_t vndr_ie_list[BCM_FLEX_ARRAY];	/**< var len list of vndr_ie_info_t */
} BWL_POST_PACKED_STRUCT vndr_ie_buf_t;
#include <packed_section_end.h>

#include <packed_section_start.h>
typedef BWL_PRE_PACKED_STRUCT struct {
	char cmd[VNDR_IE_CMD_LEN];	/**< vndr_ie IOVar set command : "add", "del" + NUL */
	vndr_ie_buf_t vndr_ie_buffer;	/**< buffer containing Vendor IE list information */
} BWL_POST_PACKED_STRUCT vndr_ie_setbuf_t;
#include <packed_section_end.h>

/** tag_ID/length/value_buffer tuple */
#include <packed_section_start.h>
typedef BWL_PRE_PACKED_STRUCT struct {
	uint8	id;
	uint8	len;
	uint8	data[1];
} BWL_POST_PACKED_STRUCT tlv_t;
#include <packed_section_end.h>

#include <packed_section_start.h>
typedef BWL_PRE_PACKED_STRUCT struct {
	uint32 pktflag;		/**< bitmask indicating which packet(s) contain this IE */
	tlv_t ie_data;		/**< IE data */
} BWL_POST_PACKED_STRUCT ie_info_t;
#include <packed_section_end.h>

#include <packed_section_start.h>
typedef BWL_PRE_PACKED_STRUCT struct {
	int32 iecount;				/**< number of entries in the ie_list[] array */
	ie_info_t ie_list[BCM_FLEX_ARRAY];	/**< variable size list of ie_info_t structs */
} BWL_POST_PACKED_STRUCT ie_buf_t;
#include <packed_section_end.h>

#include <packed_section_start.h>
typedef BWL_PRE_PACKED_STRUCT struct {
	char cmd[VNDR_IE_CMD_LEN];	/**< ie IOVar set command : "add" + NUL */
	ie_buf_t ie_buffer;	/**< buffer containing IE list information */
} BWL_POST_PACKED_STRUCT ie_setbuf_t;
#include <packed_section_end.h>

#include <packed_section_start.h>
typedef BWL_PRE_PACKED_STRUCT struct {
	uint32 pktflag;		/**< bitmask indicating which packet(s) contain this IE */
	uint8 id;		/**< IE type */
} BWL_POST_PACKED_STRUCT ie_getbuf_t;
#include <packed_section_end.h>

/* structures used to define format of wps ie data from probe requests */
/* passed up to applications via iovar "prbreq_wpsie" */
typedef struct sta_prbreq_wps_ie_hdr {
	struct ether_addr staAddr;
	uint16 ieLen;
} sta_prbreq_wps_ie_hdr_t;

#include <packed_section_start.h>
typedef BWL_PRE_PACKED_STRUCT struct sta_prbreq_wps_ie_data {
	sta_prbreq_wps_ie_hdr_t hdr;
	uint8 ieData[BCM_FLEX_ARRAY];
} BWL_POST_PACKED_STRUCT sta_prbreq_wps_ie_data_t;
#include <packed_section_end.h>

#include <packed_section_start.h>
typedef BWL_PRE_PACKED_STRUCT struct sta_prbreq_wps_ie_list {
	uint32 totLen;
	uint8 ieDataList[BCM_FLEX_ARRAY];
} BWL_POST_PACKED_STRUCT sta_prbreq_wps_ie_list_t;
#include <packed_section_end.h>

/*
 * Structure tx_pwr_rpt_t is variable-length, new fields can be endcoded in the buffer.
 * Please see "Curpower TLV Buffer Guide" before changing the structure
 */

#include <packed_section_start.h>
typedef BWL_PRE_PACKED_STRUCT struct {
	uint32 flags;
	chanspec_t chanspec;		/**< txpwr report for this channel */
	chanspec_t local_chanspec;	/**< channel on which we are associated */
	uint8 local_max;		/**< local max according to the AP */
	uint8 local_constraint;		/**< local constraint according to the AP */
	int8  antgain[2];		/**< Ant gain for each band - from SROM */
	uint8 rf_cores;			/**< count of RF Cores being reported */
	uint8 est_Pout[4];		/**< Latest tx power out estimate per RF chain */
	uint8 est_Pout_act[4];	/**< Latest tx power out estimate per RF chain w/o adjustment */
	uint8 est_Pout_cck;		/**< Latest CCK tx power out estimate */
	uint8 tx_power_max[4];		/**< Maximum target power among all rates */
	uint32 tx_power_max_rate_ind[4];  /**< Index of the rate with the max target power */
	int8 sar;			/**< SAR limit for display by wl executable */
	int8 channel_bandwidth;		/**< 20, 40 or 80 MHz bandwidth? */
	uint8 version;			/**< Version of the data format wlu <--> driver */
	uint8 display_core;		/**< Displayed curpower core */
	int8 target_offsets[4];		/**< Target power offsets for current rate per core */
	uint32 last_tx_ratespec;	/**< Ratespec for last transmition */
	uint32 user_target;		/**< user limit */
	uint32 ppr_len;			/**< length of each ppr serialization buffer */
	int8 SARLIMIT[MAX_STREAMS_SUPPORTED];
	/* Please see the document before changing this structure */
	uint8  pprdata[BCM_FLEX_ARRAY];	/**< TLV buffer for ppr and other data */
} BWL_POST_PACKED_STRUCT tx_pwr_rpt_t;

typedef BWL_PRE_PACKED_STRUCT struct {
	uint32 flags;
	chanspec_t chanspec;			/**< txpwr report for this channel */
	chanspec_t local_chanspec;		/**< channel on which we are associated */
	uint8 local_max;			/**< local max according to the AP */
	uint8 local_constraint;			/**< local constraint according to the AP */
	int8  PAD[2];				/**< unused */
	uint8 rf_cores;				/**< count of RF Cores being reported */
	uint8 est_Pout[4];			/**< Latest tx power out estimate per RF chain */
	uint8 est_Pout_act[4]; /**< Latest tx power out estimate per RF chain w/o adjustment */
	uint8 est_Pout_cck;			/**< Latest CCK tx power out estimate */
	uint8 tx_power_max[4];			/**< Maximum target power among all rates */
	uint32 tx_power_max_rate_ind[4];  /**< Index of the rate with the max target power */
	int8 sar;				/**< SAR limit for display by wl executable */
	int8 channel_bandwidth;		/**< 20, 40 or 80 MHz bandwidth? */
	uint8 version;				/**< Version of the data format wlu <--> driver */
	uint8 display_core;			/**< Displayed curpower core */
	int8 target_offsets[4];		/**< Target power offsets for current rate per core */
	uint32 last_tx_ratespec;	/**< Ratespec for last transmition */
	uint32 user_target;		/**< user limit */
	uint32 ppr_len;		/**< length of each ppr serialization buffer */
	int8 SARLIMIT[MAX_STREAMS_SUPPORTED];
	int8  antgain[3];			/**< Ant gain for each band - from SROM */
	/* Please see the document before changing this structure */
	uint8  pprdata[BCM_FLEX_ARRAY];		/**< TLV buffer for ppr and other data */
} BWL_POST_PACKED_STRUCT tx_pwr_rpt_v2_t;

#define WL_TPC_MAX_BW_TX_PWR_LEN  8u
#define WL_TPC_MAX_RU_TX_PWR_LEN  8u
#define WL_TPE_MAX_TX_PWRS_LEN    16u

typedef BWL_PRE_PACKED_STRUCT struct wl_tpc_bw_txpwr {
	wl_tx_bw_t bw;
	int8 txpwr;
} BWL_POST_PACKED_STRUCT wl_tpc_bw_txpwr_t;

typedef BWL_PRE_PACKED_STRUCT struct wl_tpc_ru_txpwr {
	int ru;       /* RU of wl_he_rate_type_t type */
	int8 txpwr;
} BWL_POST_PACKED_STRUCT wl_tpc_ru_txpwr_t;

typedef BWL_PRE_PACKED_STRUCT struct {
	uint32 flags;
	chanspec_t chanspec;       /**< txpwr report for this channel */
	chanspec_t local_chanspec; /**< channel on which we are associated */
	uint8 local_max;           /**< local max according to the AP */
	uint8 local_constraint;    /**< local constraint according to the AP */
	int8  PAD[2];              /**< unused */
	uint8 rf_cores;            /**< count of RF Cores being reported */
	uint8 est_Pout[4];         /**< Latest tx power out estimate per RF chain */
	uint8 est_Pout_act[4];     /**< Latest tx power out estimate per RF chain w/o adjustment */
	uint8 est_Pout_cck;        /**< Latest CCK tx power out estimate */
	uint8 tx_power_max[4];     /**< Maximum target power among all rates */
	uint32 tx_power_max_rate_ind[4];  /**< Index of the rate with the max target power */
	int8 sar;                  /**< SAR limit for display by wl executable */
	int8 channel_bandwidth;    /**< 20, 40 or 80 MHz bandwidth? */
	uint8 version;             /**< Version of the data format wlu <--> driver */
	uint8 display_core;        /**< Displayed curpower core */
	int8 target_offsets[4];    /**< Target power offsets for current rate per core */
	uint32 last_tx_ratespec;   /**< Ratespec for last transmition */
	uint32 user_target;        /**< user limit */
	uint32 ppr_len;            /**< length of each ppr serialization buffer */
	int8 SARLIMIT[MAX_STREAMS_SUPPORTED];
	int8  antgain[3];          /**< Ant gain for each band - from SROM */
	uint8 tpe_txpwrs_len;      /**< # max txpwrs rcvd in TPE elem */
	int8 tpe_txpwrs[WL_TPE_MAX_TX_PWRS_LEN];  /**< TPE parsed max txpwrs */
	uint8 sb_txpwrs_len;       /**< No of TPE sb_txpwrs present */
	wl_tpc_bw_txpwr_t sb_txpwrs[WL_TPC_MAX_BW_TX_PWR_LEN];  /**< TPE Subband max txpwrs */
	uint8 ru_txpwrs_len;       /**< No of TPE RU txpwrs present */
	wl_tpc_ru_txpwr_t ru_txpwrs[WL_TPC_MAX_RU_TX_PWR_LEN];  /**< TPE RU max txpwrs */
	/* Please see the document before changing this structure */
	uint8  pprdata[BCM_FLEX_ARRAY];         /**< TLV buffer for ppr and other data */
} BWL_POST_PACKED_STRUCT tx_pwr_rpt_v3_t;
#include <packed_section_end.h>

typedef struct tx_pwr_ru_rate_info {
	uint16 version;
	uint16 ru_alloc;
	uint16 mcs;
	uint16 nss;
	uint16 num_he_ltf_syms;
	uint16 ldpc;
	uint16 gi;
	uint16 txmode;
	uint16 dcm;
	uint16 tx_chain;
} tx_pwr_ru_rate_info_t;

#define TX_PWR_RU_RATE_INFO_VER		1

/* TLV ID for curpower report, ID <= 63 is reserved for ppr module */
typedef enum tx_pwr_tlv_id {
	TX_PWR_RPT_RU_RATE_INFO_ID = 64u,
	TX_PWR_RPT_RUPDOFFSET_ID   = 65u,
	TX_PWR_RPT_DEV_CAT_ID      = 66u,
	TX_PWR_RPT_PAMODE_ID	   = 67u,
	TX_PWR_RPT_LPC_OFFSET_ID   = 68u,
	TX_PWR_RPT_6G_DIRGAIN_ID   = 69u
} tx_pwr_tlv_id_t;

#include <packed_section_start.h>
typedef BWL_PRE_PACKED_STRUCT struct {
	struct ipv4_addr	ipv4_addr;
	struct ether_addr nexthop;
} BWL_POST_PACKED_STRUCT ibss_route_entry_t;
#include <packed_section_end.h>

#include <packed_section_start.h>
typedef BWL_PRE_PACKED_STRUCT struct {
	uint32 num_entry;
	ibss_route_entry_t route_entry[BCM_FLEX_ARRAY];
} BWL_POST_PACKED_STRUCT ibss_route_tbl_t;
#include <packed_section_end.h>

#define MAX_IBSS_ROUTE_TBL_ENTRY	64

#define TXPWR_TARGET_VERSION_0	  0
#include <packed_section_start.h>
typedef BWL_PRE_PACKED_STRUCT struct {
	int32 version;		/**< version number */
	chanspec_t chanspec;	/**< txpwr report for this channel */
	int8 txpwr[WL_STA_ANT_MAX]; /**< Max tx target power, in qdb */
	uint8 rf_cores;		/**< count of RF Cores being reported */
} BWL_POST_PACKED_STRUCT txpwr_target_max_t;
#include <packed_section_end.h>

#define BSS_PEER_INFO_PARAM_VER_0	0u
/** Input structure for IOV_BSS_PEER_INFO */
#include <packed_section_start.h>
typedef BWL_PRE_PACKED_STRUCT	struct {
	uint16			version;
	struct	ether_addr ea;	/**< peer MAC address */
} BWL_POST_PACKED_STRUCT bss_peer_info_param_t;
#include <packed_section_end.h>

#define BSS_PEER_INFO_VER_0	0u

#include <packed_section_start.h>
typedef BWL_PRE_PACKED_STRUCT struct {
	uint16			version;
	struct ether_addr	ea;
	int32			rssi;
	uint32			tx_rate;	/**< current tx rate */
	uint32			rx_rate;	/**< current rx rate */
	wl_rateset_t		rateset;	/**< rateset in use */
	uint32			age;		/**< age in seconds */
} BWL_POST_PACKED_STRUCT bss_peer_info_t;
#include <packed_section_end.h>

#define BSS_PEER_LIST_INFO_VER_0	0u

#include <packed_section_start.h>
typedef BWL_PRE_PACKED_STRUCT struct {
	uint16			version;
	uint16			bss_peer_info_len;	/**< length of bss_peer_info_t */
	uint32			count;			/**< number of peer info */
	bss_peer_info_t		peer_info[BCM_FLEX_ARRAY];		/**< peer info */
} BWL_POST_PACKED_STRUCT bss_peer_list_info_t;
#include <packed_section_end.h>

#define BSS_PEER_LIST_INFO_FIXED_LEN OFFSETOF(bss_peer_list_info_t, peer_info)

#define AIBSS_BCN_FORCE_CONFIG_VER_0	0u

/** structure used to configure AIBSS beacon force xmit */
#include <packed_section_start.h>
typedef BWL_PRE_PACKED_STRUCT struct {
	uint16  version;
	uint16	len;
	uint32 initial_min_bcn_dur;	/**< dur in ms to check a bcn in bcn_flood period */
	uint32 min_bcn_dur;	/**< dur in ms to check a bcn after bcn_flood period */
	uint32 bcn_flood_dur; /**< Initial bcn xmit period in ms */
} BWL_POST_PACKED_STRUCT aibss_bcn_force_config_t;
#include <packed_section_end.h>

#define AIBSS_TXFAIL_CONFIG_VER_0    0u
#define AIBSS_TXFAIL_CONFIG_VER_1    1u

/** structure used to configure aibss tx fail event */
#include <packed_section_start.h>
typedef BWL_PRE_PACKED_STRUCT struct {
	uint16  version;
	uint16  len;
	uint32 bcn_timeout;     /**< dur in seconds to receive 1 bcn */
	uint32 max_tx_retry;     /**< no of consecutive no acks to send txfail event */
	uint32 max_atim_failure; /**< no of consecutive atim failure */
} BWL_POST_PACKED_STRUCT aibss_txfail_config_t;
#include <packed_section_end.h>

#include <packed_section_start.h>
typedef BWL_PRE_PACKED_STRUCT struct wl_aibss_if {
	uint16 version;
	uint16 len;
	uint32 flags;
	struct ether_addr addr;
	chanspec_t chspec;
} BWL_POST_PACKED_STRUCT wl_aibss_if_t;
#include <packed_section_end.h>

#include <packed_section_start.h>
typedef BWL_PRE_PACKED_STRUCT struct wlc_ipfo_route_entry {
	struct ipv4_addr ip_addr;
	struct ether_addr nexthop;
} BWL_POST_PACKED_STRUCT wlc_ipfo_route_entry_t;
#include <packed_section_end.h>

#include <packed_section_start.h>
typedef BWL_PRE_PACKED_STRUCT struct wlc_ipfo_route_tbl {
	uint32 num_entry;
	wlc_ipfo_route_entry_t route_entry[BCM_FLEX_ARRAY];
} BWL_POST_PACKED_STRUCT wlc_ipfo_route_tbl_t;
#include <packed_section_end.h>

#define WL_IPFO_ROUTE_TBL_FIXED_LEN 4
#define WL_MAX_IPFO_ROUTE_TBL_ENTRY	64

/* Global ASSERT Logging */
#define ASSERTLOG_VERSION_0x0100	0x0100u
#define MAX_ASSRTSTR_LEN	64

typedef struct assert_record {
	uint32 time;
	uint8 seq_num;
	int8 str[MAX_ASSRTSTR_LEN];
	uint8 PAD[3];
} assert_record_t;

typedef struct assertlog_results {
	uint16 version;
	uint16 record_len;
	uint32 num;
	assert_record_t logs[BCM_FLEX_ARRAY];
} assertlog_results_t;

#define LOGRRC_FIX_LEN	8
#define IOBUF_ALLOWED_NUM_OF_LOGREC(type, len) ((len - LOGRRC_FIX_LEN)/sizeof(type))
/* BCMWAPI_WAI */
#define IV_LEN 16 /* same as SMS4_WPI_PN_LEN */
struct wapi_sta_msg_t {
	uint16	msg_type;
	uint16	datalen;
	uint8	vap_mac[6];
	uint8	reserve_data1[2];
	uint8	sta_mac[6];
	uint8	reserve_data2[2];
	uint8	gsn[IV_LEN];
	uint8	wie[TLV_BODY_LEN_MAX + TLV_HDR_LEN]; /* 257 */
	uint8	PAD[3]; /* padding for alignment */
};
/* #endif BCMWAPI_WAI */
/* chanim acs record */
typedef struct {
	uint8 valid;
	uint8 trigger;
	chanspec_t selected_chspc;
	int8 bgnoise;
	uint8 PAD[3];
	uint32 glitch_cnt;
	uint8 ccastats;
	uint8 chan_idle;
	uint8 PAD[2];
	uint32 timestamp;
} chanim_acs_record_t;

typedef struct {
	chanim_acs_record_t acs_record[CHANIM_ACS_RECORD];
	uint8 count;
	uint8 PAD[3];
	uint32 timestamp;
} wl_acs_record_t;

#define WL_CHANIM_STATS_V2 2
#define CCASTATS_V2_MAX 9
typedef struct chanim_stats_v2 {
	uint32 glitchcnt;               /**< normalized as per second count */
	uint32 badplcp;                 /**< normalized as per second count */
	uint8 ccastats[CCASTATS_V2_MAX];   /**< normalized as 0-255 */
	int8 bgnoise;                   /**< background noise level (in dBm) */
	chanspec_t chanspec;            /**< ctrl chanspec of the interface */
	uint32 timestamp;               /**< time stamp at which the stats are collected */
	uint32 bphy_glitchcnt;          /**< normalized as per second count */
	uint32 bphy_badplcp;            /**< normalized as per second count */
	uint8 chan_idle;                /**< normalized as 0~255 */
	uint8 PAD[3];
} chanim_stats_v2_t;

typedef struct chanim_stats {
	uint32 glitchcnt;               /**< normalized as per second count */
	uint32 badplcp;                 /**< normalized as per second count */
	uint8 ccastats[CCASTATS_MAX];   /**< normalized as 0-255 */
	int8 bgnoise;                   /**< background noise level (in dBm) */
	uint8 pad_1[11 - CCASTATS_MAX];
	chanspec_t chanspec;            /**< ctrl chanspec of the interface */
	uint8 pad_2[2];
	uint32 timestamp;               /**< time stamp at which the stats are collected */
	uint32 bphy_glitchcnt;          /**< normalized as per second count */
	uint32 bphy_badplcp;            /**< normalized as per second count */
	uint8 chan_idle;                /**< normalized as 0~255 */
	uint8 PAD[3];
} chanim_stats_t;

#define WL_CHANIM_STATS_VERSION_3	 3
typedef struct {
	uint32 buflen;
	uint32 version;
	uint32 count;
	chanim_stats_t stats[BCM_FLEX_ARRAY];
} wl_chanim_stats_t;

#define WL_SC_CHANIM_STATS_V1	 1u
/* sc chanim interface stats */
typedef struct sc_chanim_stats_v1 {
	uint32 stats_ms;		/* duraion for which stats are collected, in ms */
	chanspec_t chanspec;
	uint16 PAD;
	uint32 sc_only_rx_dur;		/* rx only on sc, in ms */
	uint32 sc_rx_mc_rx_dur;		/* Rx on SC when MC is active, in ms */
	uint32 sc_rx_ac_rx_dur;		/* Rx on SC when AC is active, in ms */
	uint32 sc_rx_mc_tx_dur;		/* sc rx with MC tx, in ms */
	uint32 sc_rx_ac_bt_tx_dur;	/* sc rx with AC-BT tx, in ms */
	uint32 sc_rx_bt_rx_dur;		/* sc rx when BT Main is active, in ms */
	uint32 sc_rx_btsc_rx_dur;	/* BT sc and wlan SC rx is active, in ms */
} sc_chanim_stats_v1_t;

typedef struct {
	uint32 version;
	uint32 length;
	uint8 flags;	/* flags: to print the stats,
			 * WL_CHANIM_COUNT_ONE ==> Query stats for Home channel,
			 * WL_CHANIM_COUNT_ALL ==> Query stats for all channels
			 */
	uint8 id;	/* Module id, to know which module has sent the stats
			 * SC_CHANIM_ID_SCAN ==> For SCAN
			 * SC_CHANIM_ID_STA ==> For STA
			 */
	uint8 count;	/* o/p: Count of channels for which stats needs to be displayed.
			 * This value is number of channels supported in particular locale when
			 * flags is WL_CHANIM_COUNT_ALL, one when flag is
			 * WL_CHANIM_COUNT_ONE
			 */
	uint8 PAD;
	sc_chanim_stats_v1_t sc_stats[BCM_FLEX_ARRAY];
} wl_chanim_sc_stats_v1_t;

/* WL_CHANIM_SC_STATS_FIXED_LEN: when we change size of any field above sc_stats, this macro
 * needs versioning. At present it uses offset of v1, which is same for all versions so far
 */
#define WL_CHANIM_SC_STATS_FIXED_LEN OFFSETOF(wl_chanim_sc_stats_v1_t, sc_stats)
#define WL_CHANIM_STATS_FIXED_LEN OFFSETOF(wl_chanim_stats_t, stats)

#define WL_SC_CHANIM_STATS_V2	 2u
/* sc chanim interface stats */
typedef struct sc_chanim_stats_v2 {
	uint32 stats_ms;		/* duraion for which stats are collected, in ms */
	chanspec_t chanspec;
	uint16 PAD;
	uint32 sc_only_rx_dur;		/* rx only on sc, in ms */
	uint32 sc_rx_mc_rx_dur;		/* Rx on SC when MC is active, in ms */
	uint32 sc_rx_ac_rx_dur;		/* Rx on SC when AC is active, in ms */
	uint32 sc_rx_mc_tx_dur;		/* sc rx with MC tx, in ms */
	uint32 sc_rx_ac_bt_tx_dur;	/* sc rx with AC-BT tx, in ms */
	uint32 sc_rx_bt_rx_dur;		/* sc rx when BT Main is active, in ms */
	uint32 sc_btle_overlap_dur;	/* wlsc was awake and btsc le scan overlapped, in ms */
	uint32 sc_btpage_overlap_dur;	/* wlsc was awake and btsc page scan overlapped, in ms */
	uint32 sc_btle_blnk_dur;	/* wlauxtx blanked btsc le scan, in ms */
	uint32 sc_btpage_blnk_dur;	/* wlauxtx blanked btsc page scan, in ms */
} sc_chanim_stats_v2_t;

typedef struct {
	uint32 version;
	uint32 length;
	uint8 flags;	/* flags: to print the stats,
			 * WL_CHANIM_COUNT_ONE ==> Query stats for Home channel,
			 * WL_CHANIM_COUNT_ALL ==> Query stats for all channels
			 */
	uint8 id;	/* Module id, to know which module has sent the stats
			 * SC_CHANIM_ID_SCAN ==> For SCAN
			 * SC_CHANIM_ID_STA ==> For STA
			 */
	uint8 count;	/* o/p: Count of channels for which stats needs to be displayed.
			 * This value is number of channels supported in particular locale when
			 * flags is WL_CHANIM_COUNT_ALL, one when flag is
			 * WL_CHANIM_COUNT_ONE
			 */
	uint8 PAD;
	sc_chanim_stats_v2_t sc_stats[BCM_FLEX_ARRAY];
} wl_chanim_sc_stats_v2_t;

#define WL_SC_CHANIM_STATS_V3	 3u
/* sc chanim interface stats */
typedef struct sc_chanim_stats_v3 {
	uint32 stats_ms;		/* duraion for which stats are collected, in ms */
	chanspec_t chanspec;
	uint16 PAD;
	uint32 sc_only_rx_dur;		/* rx only on sc, in ms */
	uint32 sc_rx_mc_rx_dur;		/* Rx on SC when MC is active, in ms */
	uint32 sc_rx_ac_rx_dur;		/* Rx on SC when AC is active, in ms */
	uint32 sc_rx_mc_tx_dur;		/* sc rx with MC tx, in ms */
	uint32 sc_rx_ac_bt_tx_dur;	/* sc rx with AC-BT tx, in ms */
	uint32 sc_rx_bt_rx_dur;		/* sc rx when BT Main is active, in ms */
	uint32 sc_btle_overlap_dur;	/* wlsc was awake and btsc le scan overlapped, in ms */
	uint32 sc_btpage_overlap_dur;	/* wlsc was awake and btsc page scan overlapped, in ms */
	uint32 sc_btle_blnk_dur;	/* wlauxtx blanked btsc le scan, in ms */
	uint32 sc_btpage_blnk_dur;	/* wlauxtx blanked btsc page scan, in ms */
	uint32 ac_btle_overlap_dur;	/* wlaux was awake and btsc le scan overlapped, in ms */
	uint32 ac_btpage_overlap_dur;	/* wlaux was awake and btsc page scan overlapped, in ms */
} sc_chanim_stats_v3_t;

typedef struct {
	uint32 version;
	uint32 length;
	uint8 flags;	/* flags: to print the stats,
			 * WL_CHANIM_COUNT_ONE ==> Query stats for Home channel,
			 * WL_CHANIM_COUNT_ALL ==> Query stats for all channels
			 */
	uint8 id;	/* Module id, to know which module has sent the stats
			 * SC_CHANIM_ID_SCAN ==> For SCAN
			 * SC_CHANIM_ID_STA ==> For STA
			 */
	uint8 count;	/* o/p: Count of channels for which stats needs to be displayed.
			 * This value is number of channels supported in particular locale when
			 * flags is WL_CHANIM_COUNT_ALL, one when flag is
			 * WL_CHANIM_COUNT_ONE
			 */
	uint8 PAD;
	sc_chanim_stats_v3_t sc_stats[BCM_FLEX_ARRAY];
} wl_chanim_sc_stats_v3_t;

#define WL_SC_CHANIM_STATS_V4	 4u
/* sc chanim interface stats */
typedef struct sc_chanim_stats_v4 {
	uint32 stats_ms;		/* duraion for which stats are collected, in ms */
	chanspec_t chanspec;
	uint16 PAD;
	uint32 sc_only_rx_dur;		/* rx only on sc, in ms */
	uint32 sc_rx_mc_rx_dur;		/* Rx on SC when MC is active, in ms */
	uint32 sc_rx_ac_rx_dur;		/* Rx on SC when AC is active, in ms */
	uint32 sc_rx_mc_tx_dur;		/* sc rx with MC tx, in ms */
	uint32 sc_rx_ac_bt_tx_dur;	/* sc rx with AC-BT tx, in ms */
	uint32 sc_rx_bt_rx_dur;		/* sc rx when BT Main is active, in ms */
	uint32 sc_btle_overlap_dur;	/* wlsc was awake and btsc le scan overlapped, in ms */
	uint32 sc_btpage_overlap_dur;	/* wlsc was awake and btsc page scan overlapped, in ms */
	uint32 ac_btle_blnk_dur;	/* wlauxtx blanked btsc le scan, in ms */
	uint32 ac_btpage_blnk_dur;	/* wlauxtx blanked btsc page scan, in ms */
	uint32 ac_btle_overlap_dur;	/* wlaux was awake and btsc le scan overlapped, in ms */
	uint32 ac_btpage_overlap_dur;	/* wlaux was awake and btsc page scan overlapped, in ms */
	uint32 timestamp;		/* Time when stats last updated */
} sc_chanim_stats_v4_t;

typedef struct {
	uint32 version;
	uint32 length;
	uint8 flags;	/* flags: to print the stats,
			 * WL_CHANIM_COUNT_ONE ==> Query stats for Home channel,
			 * WL_CHANIM_COUNT_ALL ==> Query stats for all channels
			 */
	uint8 id;	/* Module id, to know which module has sent the stats
			 * SC_CHANIM_ID_SCAN ==> For SCAN
			 * SC_CHANIM_ID_STA ==> For STA
			 */
	uint8 count;	/* o/p: Count of channels for which stats needs to be displayed.
			 * This value is number of channels supported in particular locale when
			 * flags is WL_CHANIM_COUNT_ALL, one when flag is
			 * WL_CHANIM_COUNT_ONE
			 */
	uint8 PAD;
	sc_chanim_stats_v4_t sc_stats[BCM_FLEX_ARRAY];
} wl_chanim_sc_stats_v4_t;

#define WL_SC_CHANIM_STATS_V5	 5u
/* sc chanim interface stats */
typedef struct sc_chanim_stats_v5 {
	uint32 stats_ms;		/* duraion for which stats are collected, in ms */
	chanspec_t chanspec;
	uint16 sc_btrx_trans_cnt;	/* BT RX transitions */
	uint32 sc_only_rx_dur;		/* rx only on sc, in ms */
	uint32 sc_rx_mc_rx_dur;		/* Rx on SC when MC is active, in ms */
	uint32 sc_rx_ac_rx_dur;		/* Rx on SC when AC is active, in ms */
	uint32 sc_rx_mc_tx_dur;		/* sc rx with MC tx, in ms */
	uint32 sc_rx_ac_bt_tx_dur;	/* sc rx with AC-BT tx, in ms */
	uint32 sc_rx_bt_rx_dur;		/* sc rx when BT Main is active, in ms */
	uint32 sc_btle_overlap_dur;	/* wlsc was awake and btsc le scan overlapped, in ms */
	uint32 sc_btpage_overlap_dur;	/* wlsc was awake and btsc page scan overlapped, in ms */
	uint32 ac_btle_blnk_dur;	/* wlauxtx blanked btsc le scan, in ms */
	uint32 ac_btpage_blnk_dur;	/* wlauxtx blanked btsc page scan, in ms */
	uint32 ac_btle_overlap_dur;	/* wlaux was awake and btsc le scan overlapped, in ms */
	uint32 ac_btpage_overlap_dur;	/* wlaux was awake and btsc page scan overlapped, in ms */
	uint32 timestamp;		/* Time when stats last updated */
} sc_chanim_stats_v5_t;

typedef struct {
	uint32 version;
	uint32 length;
	uint8 flags;	/* flags: to print the stats,
			 * WL_CHANIM_COUNT_ONE ==> Query stats for Home channel,
			 * WL_CHANIM_COUNT_ALL ==> Query stats for all channels
			 */
	uint8 id;	/* Module id, to know which module has sent the stats
			 * SC_CHANIM_ID_SCAN ==> For SCAN
			 * SC_CHANIM_ID_STA ==> For STA
			 */
	uint8 count;	/* o/p: Count of channels for which stats needs to be displayed.
			 * This value is number of channels supported in particular locale when
			 * flags is WL_CHANIM_COUNT_ALL, one when flag is
			 * WL_CHANIM_COUNT_ONE
			 */
	uint8 PAD;
	sc_chanim_stats_v5_t sc_stats[BCM_FLEX_ARRAY];
} wl_chanim_sc_stats_v5_t;

#define WL_SC_CHANIM_STATS_V6	 6u
/* sc chanim interface stats */
typedef struct sc_chanim_stats_v6 {
	uint32 stats_ms;		/* duraion for which stats are collected, in ms */
	chanspec_t chanspec;
	uint16 sc_btrx_trans_cnt;	/* BT RX transitions */
	uint32 sc_only_rx_dur;		/* rx only on sc, in ms */
	uint32 sc_rx_mc_rx_dur;		/* Rx on SC when MC is active, in ms */
	uint32 sc_rx_ac_rx_dur;		/* Rx on SC when AC is active, in ms */
	uint32 sc_rx_mc_tx_dur;		/* sc rx with MC tx, in ms */
	uint32 sc_rx_ac_bt_tx_dur;	/* sc rx with AC-BT tx, in ms */
	uint32 sc_rx_bt_rx_dur;		/* sc rx when BT Main is active, in ms */
	uint32 sc_btle_overlap_dur;	/* wlsc was awake and btsc le scan overlapped, in ms */
	uint32 sc_btpage_overlap_dur;	/* wlsc was awake and btsc page scan overlapped, in ms */
	uint32 ac_btle_blnk_dur;	/* wlauxtx blanked btsc le scan, in ms */
	uint32 ac_btpage_blnk_dur;	/* wlauxtx blanked btsc page scan, in ms */
	uint32 ac_btle_overlap_dur;	/* wlaux was awake and btsc le scan overlapped, in ms */
	uint32 ac_btpage_overlap_dur;	/* wlaux was awake and btsc page scan overlapped, in ms */
	uint32 timestamp;		/* Time when stats last updated */
	uint32 sc_rx_5g_bt_rx_dur;	/* sc rx when 5G BT Main is active, in ms */
	uint16 sc_5g_btrx_trans_cnt;	/* 5G BT RX transitions */
	uint16 pad;
} sc_chanim_stats_v6_t;

typedef struct {
	uint32 version;
	uint32 length;
	uint8 flags;	/* flags: to print the stats,
			 * WL_CHANIM_COUNT_ONE ==> Query stats for Home channel,
			 * WL_CHANIM_COUNT_ALL ==> Query stats for all channels
			 */
	uint8 id;	/* Module id, to know which module has sent the stats
			 * SC_CHANIM_ID_SCAN ==> For SCAN
			 * SC_CHANIM_ID_STA ==> For STA
			 */
	uint8 count;	/* o/p: Count of channels for which stats needs to be displayed.
			 * This value is number of channels supported in particular locale when
			 * flags is WL_CHANIM_COUNT_ALL, one when flag is
			 * WL_CHANIM_COUNT_ONE
			 */
	uint8 PAD;
	sc_chanim_stats_v6_t sc_stats[];
} wl_chanim_sc_stats_v6_t;

#define WL_SC_CHANIM_STATS_V7	 7u

typedef struct sc_chanim_stats_v7 {
	uint32 stats_ms;                /* duration for which stats are collected, in ms */
	chanspec_t chanspec;
	uint16 pad;
	uint32 sc_only_rx_dur;          /* rx only on sc, in ms */
	uint32 sc_rx_mc_rx_dur;         /* Rx on SC when MC is active, in ms */
	uint32 sc_rx_ac_rx_dur;         /* Rx on SC when AC is active, in ms */
	uint32 sc_rx_mc_tx_dur;         /* sc rx with MC tx, in ms */
	uint32 sc_rx_ac_bt_tx_dur;      /* sc rx with AC-BT tx, in ms */
	uint32 sc_rx_bt_rx_dur;         /* sc rx when BT Main is active, in ms */
	uint32 sc_btle_overlap_dur;     /* wlsc was awake and btsc le scan overlapped, in ms */
	uint32 sc_btpage_overlap_dur;   /* wlsc was awake and btsc page scan overlapped, in ms */
	uint32 ac_btle_blnk_dur;        /* wlauxtx blanked btsc le scan, in ms */
	uint32 ac_btpage_blnk_dur;      /* wlauxtx blanked btsc page scan, in ms */
	uint32 ac_btle_overlap_dur;     /* wlaux was awake and btsc le scan overlapped, in ms */
	uint32 ac_btpage_overlap_dur;   /* wlaux was awake and btsc page scan overlapped, in ms */
	uint32 timestamp;               /* Time when stats last updated */
	uint32 sc_rx_5g_bt_rx_dur;      /* sc rx when 5G BT Main is active, in ms */
	uint16 sc_5g_btrx_trans_cnt;    /* 5G BT RX transitions */
	uint16 sc_fbc_trans_cnt;         /* Shared radio FBC transition count */
	uint32 sc_rx_ac_bt_ded_mode_dur; /* Dedicated mode duration */
} sc_chanim_stats_v7_t;

typedef struct {
	uint32 version;
	uint32 length;
	uint8 flags;	/* flags: to print the stats,
			 * WL_CHANIM_COUNT_ONE ==> Query stats for Home channel,
			 * WL_CHANIM_COUNT_ALL ==> Query stats for all channels
			 */
	uint8 id;	/* Module id, to know which module has sent the stats
			 * SC_CHANIM_ID_SCAN ==> For SCAN
			 * SC_CHANIM_ID_STA ==> For STA
			 */
	uint8 count;	/* o/p: Count of channels for which stats needs to be displayed.
			 * This value is number of channels supported in particular locale when
			 * flags is WL_CHANIM_COUNT_ALL, one when flag is
			 * WL_CHANIM_COUNT_ONE
			 */
	uint8 PAD;
	sc_chanim_stats_v7_t sc_stats[];
} wl_chanim_sc_stats_v7_t;

/* sc_chanim periodic ecounters structs for WL_IFSTATS_XTLV_SC_CHANIM_PERIODIC_STATS
 *  [similar to wl_chanim_sc_stats_vX_t, but constrained in size due to its known periodicity
 *  of reporting]
 */
#define WLC_SC_CHANIM_PERIODIC_STATS_V2	(2u)
#define WLC_SC_STATS_MAX_BANDS_V1	2u

typedef struct sc_chanim_stats_perband_periodic_v2 {
	uint8	PAD[3];
	uint8	band_id;		/* band for which stats reported; 0:5G,1:2G */
	uint16	sc_only_rx_dur;		/* rx only on sc, in ms */
	uint16	sc_rx_mc_rx_dur;	/* Rx on SC when MC is active, in ms */
	uint16	sc_rx_ac_rx_dur;	/* Rx on SC when AC is active, in ms */
	uint16	sc_rx_mc_tx_dur;	/* sc rx with MC tx, in ms */
	uint16	sc_rx_ac_bt_tx_dur;	/* sc rx with AC-BT tx, in ms */
	uint16	sc_rx_bt_rx_dur;	/* sc rx when BT Main is active, in ms */
	uint16	sc_btle_overlap_dur;	/* wlsc was awake and btsc le scan overlapped, in ms */
	uint16	sc_btpage_overlap_dur;	/* wlsc was awake and btsc page scan overlapped, in ms */
	uint16	sc_btle_blnk_dur;	/* wlauxtx blanked btsc le scan, in ms */
	uint16	sc_btpage_blnk_dur;	/* wlauxtx blanked btsc page scan, in ms */
} sc_chanim_stats_perband_periodic_v2_t;

typedef struct wl_chanim_sc_periodic_stats_v2 {
	uint16	ver;
	uint16	len;
	uint8	PAD[3];
	uint8	count;	/* number of sc_stats below */
	/* per band accum stats */
	sc_chanim_stats_perband_periodic_v2_t sc_stats[WLC_SC_STATS_MAX_BANDS_V1];
} wl_chanim_sc_periodic_stats_v2_t;

#define WLC_SC_CHANIM_PERIODIC_STATS_V3	(3u)

typedef struct sc_chanim_stats_perband_periodic_v3 {
	uint8	PAD[3];
	uint8	band_id;		/* band for which stats reported; 0:5G,1:2G */
	uint16	sc_only_rx_dur;		/* rx only on sc, in ms */
	uint16	sc_rx_mc_rx_dur;	/* Rx on SC when MC is active, in ms */
	uint16	sc_rx_ac_rx_dur;	/* Rx on SC when AC is active, in ms */
	uint16	sc_rx_mc_tx_dur;	/* sc rx with MC tx, in ms */
	uint16	sc_rx_ac_bt_tx_dur;	/* sc rx with AC-BT tx, in ms */
	uint16	sc_rx_bt_rx_dur;	/* sc rx when BT Main is active, in ms */
	uint16	sc_btle_overlap_dur;	/* wlsc was awake and btsc le scan overlapped, in ms */
	uint16	sc_btpage_overlap_dur;	/* wlsc was awake and btsc page scan overlapped, in ms */
	uint16	sc_btle_blnk_dur;	/* wlauxtx blanked btsc le scan, in ms */
	uint16	sc_btpage_blnk_dur;	/* wlauxtx blanked btsc page scan, in ms */
	uint16	ac_btle_overlap_dur;	/* wlaux was awake and btsc le scan overlapped, in ms */
	uint16	ac_btpage_overlap_dur;	/* wlaux was awake and btsc page scan overlapped, in ms */
} sc_chanim_stats_perband_periodic_v3_t;

typedef struct wl_chanim_sc_periodic_stats_v3 {
	uint16	ver;
	uint16	len;
	uint8	PAD[3];
	uint8	count;	/* number of sc_stats below */
	/* per band accum stats */
	sc_chanim_stats_perband_periodic_v3_t sc_stats[WLC_SC_STATS_MAX_BANDS_V1];
} wl_chanim_sc_periodic_stats_v3_t;

/** Noise measurement metrics. */
#define NOISE_MEASURE_KNOISE	0x1

/** scb probe parameter */
typedef struct {
	uint32 scb_timeout;
	uint32 scb_activity_time;
	uint32 scb_max_probe;
} wl_scb_probe_t;

/* structure/defines for selective mgmt frame (smf) stats support */

#define SMFS_VERSION_1	 1
/** selected mgmt frame (smf) stats element */
typedef struct wl_smfs_elem {
	uint32 count;
	uint16 code;  /**< SC or RC code */
	uint8 PAD[2];
} wl_smfs_elem_t;

typedef struct wl_smf_stats {
	uint32 version;
	uint16 length;	/**< reserved for future usage */
	uint8 type;
	uint8 codetype;
	uint32 ignored_cnt;
	uint32 malformed_cnt;
	uint32 count_total; /**< count included the interested group */
	wl_smfs_elem_t elem[BCM_FLEX_ARRAY];
} wl_smf_stats_t;

#define WL_SMFSTATS_FIXED_LEN OFFSETOF(wl_smf_stats_t, elem);

enum {
	SMFS_CODETYPE_SC,
	SMFS_CODETYPE_RC
};

typedef enum smfs_type {
	SMFS_TYPE_AUTH,
	SMFS_TYPE_ASSOC,
	SMFS_TYPE_REASSOC,
	SMFS_TYPE_DISASSOC_TX,
	SMFS_TYPE_DISASSOC_RX,
	SMFS_TYPE_DEAUTH_TX,
	SMFS_TYPE_DEAUTH_RX,
	SMFS_TYPE_MAX
} smfs_type_t;

/* #ifdef PHYMON */

#define PHYMON_VERSION_1 1

typedef struct wl_phycal_core_state {
	/* Tx IQ/LO calibration coeffs */
	int16 tx_iqlocal_a;
	int16 tx_iqlocal_b;
	int8 tx_iqlocal_ci;
	int8 tx_iqlocal_cq;
	int8 tx_iqlocal_di;
	int8 tx_iqlocal_dq;
	int8 tx_iqlocal_ei;
	int8 tx_iqlocal_eq;
	int8 tx_iqlocal_fi;
	int8 tx_iqlocal_fq;

	/** Rx IQ calibration coeffs */
	int16 rx_iqcal_a;
	int16 rx_iqcal_b;

	uint8 tx_iqlocal_pwridx; /**< Tx Power Index for Tx IQ/LO calibration */
	uint8 PAD[3];
	uint32 papd_epsilon_table[64]; /**< PAPD epsilon table */
	int16 papd_epsilon_offset; /**< PAPD epsilon offset */
	uint8 curr_tx_pwrindex; /**< Tx power index */
	int8 idle_tssi; /**< Idle TSSI */
	int8 est_tx_pwr; /**< Estimated Tx Power (dB) */
	int8 est_rx_pwr; /**< Estimated Rx Power (dB) from RSSI */
	uint16 rx_gaininfo; /**< Rx gain applied on last Rx pkt */
	uint16 init_gaincode; /**< initgain required for ACI */
	int8 estirr_tx;
	int8 estirr_rx;
} wl_phycal_core_state_t;

typedef struct wl_phycal_state {
	int32 version;
	int8 num_phy_cores; /**< number of cores */
	int8 curr_temperature; /**< on-chip temperature sensor reading */
	chanspec_t chspec; /**< channspec for this state */
	uint8 aci_state; /**< ACI state: ON/OFF */
	uint8 PAD;
	uint16 crsminpower; /**< crsminpower required for ACI */
	uint16 crsminpowerl; /**< crsminpowerl required for ACI */
	uint16 crsminpoweru; /**< crsminpoweru required for ACI */
	wl_phycal_core_state_t phycal_core[BCM_FLEX_ARRAY];
} wl_phycal_state_t;

#define WL_PHYCAL_STAT_FIXED_LEN OFFSETOF(wl_phycal_state_t, phycal_core)
/* endif PHYMON */

/** discovery state */
typedef struct wl_p2p_disc_st {
	uint8 state;	/**< see state */
	uint8 PAD;
	chanspec_t chspec;	/**< valid in listen state */
	uint16 dwell;	/**< valid in listen state, in ms */
} wl_p2p_disc_st_t;

/** scan request */
typedef struct wl_p2p_scan {
	uint8 type;		/**< 'S' for WLC_SCAN, 'E' for "escan" */
	uint8 reserved[3];
	/* scan or escan parms... */
} wl_p2p_scan_t;

/** i/f request */
typedef struct wl_p2p_if {
	struct ether_addr addr;
	uint8 type;	/**< see i/f type */
	uint8 PAD;
	chanspec_t chspec;	/**< for p2p_ifadd GO */
} wl_p2p_if_t;

/** i/f query */
typedef struct wl_p2p_ifq {
	uint32 bsscfgidx;
	char ifname[BCM_MSG_IFNAME_MAX];
} wl_p2p_ifq_t;

/** OppPS & CTWindow */
typedef struct wl_p2p_ops {
	uint8 ops;	/**< 0: disable 1: enable */
	uint8 ctw;	/**< >= 10 */
} wl_p2p_ops_t;

/** absence and presence request */
typedef struct wl_p2p_sched_desc {
	uint32 start;
	uint32 interval;
	uint32 duration;
	uint32 count;	/**< see count */
} wl_p2p_sched_desc_t;

typedef struct wl_p2p_sched {
	uint8 type;	/**< see schedule type */
	uint8 action;	/**< see schedule action */
	uint8 option;	/**< see schedule option */
	uint8 PAD;
	wl_p2p_sched_desc_t desc[BCM_FLEX_ARRAY];
} wl_p2p_sched_t;

typedef struct wl_p2p_wfds_hash {
	uint32	advt_id;
	uint16	nw_cfg_method;
	uint8	wfds_hash[6];
	uint8	name_len;
	uint8	service_name[MAX_WFDS_SVC_NAME_LEN];
	uint8	PAD[3];
} wl_p2p_wfds_hash_t;

typedef struct wl_p2p_config_params {
	uint16		enable;			/**< 0: disable 1: enable */
	uint16		chanspec;		/* GO chanspec */
	wlc_ssid_t	ssid;			/* SSID */
} wl_p2p_config_params_t;

typedef struct wl_bcmdcs_data {
	uint32 reason;
	chanspec_t chspec;
	uint8	PAD[2];
} wl_bcmdcs_data_t;
/* ifdef EXT_STA */
/**
 * Format of IHV data passed to OID_DOT11_NIC_SPECIFIC_EXTENSION.
 */
typedef struct _IHV_NIC_SPECIFIC_EXTENSION {
	uint8			oui[4];     /**< vendor specific OUI value */
	uint32			event;      /**< event code */
	uint8			ihvData[BCM_FLEX_ARRAY];    /**< ihv data */
} IHV_NIC_SPECIFIC_EXTENSION, *PIHV_NIC_SPECIFIC_EXTENSION;
#define IHV_NIC_SPECIFIC_EXTENTION_HEADER	OFFSETOF(IHV_NIC_SPECIFIC_EXTENSION, ihvData[0])
/* EXT_STA */
/** NAT configuration */
typedef struct {
	uint32 ipaddr;		/**< interface ip address */
	uint32 ipaddr_mask;	/**< interface ip address mask */
	uint32 ipaddr_gateway;	/**< gateway ip address */
	uint8 mac_gateway[6];	/**< gateway mac address */
	uint8	PAD[2];
	uint32 ipaddr_dns;	/**< DNS server ip address, valid only for public if */
	uint8 mac_dns[6];	/**< DNS server mac address,  valid only for public if */
	uint8 GUID[38];		/**< interface GUID */
} nat_if_info_t;

typedef struct {
	uint32 op;		/**< operation code */
	uint8 pub_if;		/**< set for public if, clear for private if */
	uint8	PAD[3];
	nat_if_info_t if_info;	/**< interface info */
} nat_cfg_t;

typedef struct {
	int32 state;	/**< NAT state returned */
} nat_state_t;

typedef struct flush_txfifo {
	uint32 txfifobmp;
	uint32 hwtxfifoflush;
	struct ether_addr ea;
	uint8	PAD[2];
} flush_txfifo_t;

enum {
	SPATIAL_MODE_2G_IDX = 0,
	SPATIAL_MODE_5G_LOW_IDX,
	SPATIAL_MODE_5G_MID_IDX,
	SPATIAL_MODE_5G_HIGH_IDX,
	SPATIAL_MODE_5G_UPPER_IDX,
	SPATIAL_MODE_MAX_IDX
};

#define WLC_TXCORE_MAX			4	/**< max number of txcore supports */
#define WLC_TXCORE_MAX_OLD		2	/**< backward compatibilty for TXCAL */
#define WLC_TXCORE_MAX_GAINSWEEP_MEAS	2	/* 2 cores max for macos */
#define WLC_SUBBAND_MAX			4	/**< max number of sub-band supports */
typedef struct {
	uint8	band2g[WLC_TXCORE_MAX];
	uint8	band5g[WLC_SUBBAND_MAX][WLC_TXCORE_MAX];
} sar_limit_t;

#define MAX_NUM_TXCAL_MEAS		128u
#define MIN_NUM_PWR_STEP		2u

#define MAX_NUM_PWR_STEP		40	/* leave it as signed for backward compatibility */

#define TXCAL_IOVAR_VERSION		0x1	/* need to leave it as signed one
						 * for backward compatibility
						 */
#define TXCAL_IOVAR_VERSION_0		0x0u
#define TXCAL_IOVAR_VERSION_1		0x1u
#define TXCAL_IOVAR_VERSION_2		0x2u
#define TXCAL_IOVAR_VERSION_3		0x3u
#define TXCAL_IOVAR_VERSION_4		0x4u  /* TXCAL_GAINSWEEP_MEAS_VER_V2 */
#define TXCAL_IOVAR_VERSION_5		0x5u  /* TXCAL_GAINSWEEP_MEAS_VER_V2 no pa_modes */

#define TXCAL_GAINSWEEP_VER		(TXCAL_GAINSWEEP_VERSION_V2)
#define TXCAL_GAINSWEEP_VERSION_V2	2

#define TXCAL_GAINSWEEP_MEAS_VER	0x1u
#define TXCAL_GAINSWEEP_MEAS_VER_V2	0x2u

/* Below macro will no longer be used.
 * Please specifiy the current supporting version in the 'txcal_ver' iovar.
 */
#define TXCAL_IOVAR_LATEST		TXCAL_GAINSWEEP_VER

/* below are used for bphy/ofdm separated LSC */
#define TXCAL_PWR_BPHY    0
#define TXCAL_PWR_OFDM    1

typedef struct wl_txcal_meas_percore {
	uint16 tssi[MAX_NUM_TXCAL_MEAS];
	int16 pwr[MAX_NUM_TXCAL_MEAS];
} wl_txcal_meas_percore_t;

typedef struct wl_txcal_meas_ncore {
	uint16 version;
	uint8 valid_cnt;
	uint8 num_core;
	wl_txcal_meas_percore_t txcal_percore[BCM_FLEX_ARRAY];
} wl_txcal_meas_ncore_t;

typedef struct wl_txcal_power_tssi_percore {
	int16 tempsense;
	int16 pwr_start;
	uint8 pwr_start_idx;
	uint8 num_entries;
	uint16 PAD;
	uint8 tssi[MAX_NUM_PWR_STEP];
} wl_txcal_power_tssi_percore_t;

typedef struct wl_txcal_power_tssi_ncore {
	uint16 version;
	uint8 set_core;
	uint8 channel;
	uint8 num_core;
	uint8 gen_tbl;
	uint8 ofdm;
	uint8 PAD;
	wl_txcal_power_tssi_percore_t tssi_percore[WLC_TXCORE_MAX];
} wl_txcal_power_tssi_ncore_t;

typedef struct wl_txcal_power_tssi_ncore_v2 {
	uint16		version;	/* structure version */
	uint16		length;		/* structure length */
	uint8		set_core;	/* set core */
	uint8		num_core;	/* number of core */
	uint8		gen_tbl;	/* generate table */
	uint8		ofdm;		/* ofdm:1 or bphy:0 */
	chanspec_t	chanspec;	/* chanspec */
	uint8		pa_mode;	/* TX PA mode */
	uint8		PAD;		/* explict padding */
	wl_txcal_power_tssi_percore_t	tssi_percore[WLC_TXCORE_MAX];	/* tssi value entries */
} wl_txcal_power_tssi_ncore_v2_t;

typedef struct wl_txcal_meas {
	uint16 tssi[WLC_TXCORE_MAX][MAX_NUM_TXCAL_MEAS];
	int16 pwr[WLC_TXCORE_MAX][MAX_NUM_TXCAL_MEAS];
	uint8 valid_cnt;
	uint8 PAD;
} wl_txcal_meas_t;

typedef struct wl_txcal_meas_old {
	uint16 tssi[WLC_TXCORE_MAX_OLD][MAX_NUM_TXCAL_MEAS];
	int16 pwr[WLC_TXCORE_MAX_OLD][MAX_NUM_TXCAL_MEAS];
	uint8 valid_cnt;
	uint8 PAD;
} wl_txcal_meas_old_t;

typedef struct wl_txcal_power_tssi {
	uint8 set_core;
	uint8 channel;
	int16 tempsense[WLC_TXCORE_MAX];
	int16 pwr_start[WLC_TXCORE_MAX];
	uint8 pwr_start_idx[WLC_TXCORE_MAX];
	uint8 num_entries[WLC_TXCORE_MAX];
	uint8 tssi[WLC_TXCORE_MAX][MAX_NUM_PWR_STEP];
	uint8 gen_tbl;
	uint8 ofdm;
} wl_txcal_power_tssi_t;

typedef struct wl_txcal_power_tssi_v2 {
	uint16		version;			/* structure version */
	uint16		length;				/* structure length */
	chanspec_t	chanspec;			/* chanspec */
	uint8		set_core;			/* set core */
	uint8		gen_tbl;			/* generate table */
	uint8		ofdm;				/* ofdm:1 or bphy:0 */
	uint8		pa_mode;			/* TX PA mode */
	uint8		PAD[2];				/* explicit padding */
	wl_txcal_power_tssi_percore_t	per_core[];	/* info per core */
} wl_txcal_power_tssi_v2_t;

typedef struct wl_txcal_power_tssi_old {
	uint8 set_core;
	uint8 channel;
	int16 tempsense[WLC_TXCORE_MAX_OLD];
	int16 pwr_start[WLC_TXCORE_MAX_OLD];
	uint8 pwr_start_idx[WLC_TXCORE_MAX_OLD];
	uint8 num_entries[WLC_TXCORE_MAX_OLD];
	uint8 tssi[WLC_TXCORE_MAX_OLD][MAX_NUM_PWR_STEP];
	uint8 gen_tbl;
	uint8 ofdm;
} wl_txcal_power_tssi_old_t;

typedef struct wl_olpc_pwr {
	uint16 version;
	uint8 core;
	uint8 channel;
	int16 tempsense;
	uint8 olpc_idx;
	uint8 ofdm;
} wl_olpc_pwr_t;

typedef struct wl_olpc_pwr_v2 {
	uint16		version;	/* structure version */
	uint16		length;		/* structure length */
	chanspec_t	chanspec;	/* chanspec */
	int16		tempsense;	/* tempsense */
	uint8		core;		/* core selection */
	uint8		olpc_idx;	/* tx power index */
	uint8		ofdm;		/* ofdm:1 or bphy:0 */
	uint8		pa_mode;	/* TX PA mode */
} wl_olpc_pwr_v2_t;

/* mode select for olpc_anchor iovar */
enum olpc_anchor_band {
	ANCHOR_MODE_2G = 0,		/* anchor mode for 2G */
	ANCHOR_MODE_5G = 1,		/* anchor mode for 5G */
	ANCHOR_MODE_6G = 2		/* anchor mode for 6G */
};

typedef struct wl_olpc_anchor {
	uint16		version;	/* structure version */
	uint16		length;		/* structure length */
	uint8		band;		/* 2G/5G/6G/XX */
	uint8		anchor;		/* anchor point power level */
	uint8		PAD[2];		/* explicit padding */
} wl_olpc_anchor_t;

/** IOVAR "mempool" parameter. Used to retrieve a list of memory pool statistics. */
typedef struct wl_mempool_stats {
	int32 num;		/**< Number of memory pools */
	bcm_mp_stats_t s[BCM_FLEX_ARRAY];	/**< Variable array of memory pool stats. */
} wl_mempool_stats_t;

/* Both powersel_params and lpc_params are used by IOVAR lpc_params.
 * The powersel_params is replaced by lpc_params in later WLC versions.
 */
typedef struct powersel_params {
	/* LPC Params exposed via IOVAR */
	int32		tp_ratio_thresh;  /**< Throughput ratio threshold */
	uint8		rate_stab_thresh; /**< Thresh for rate stability based on nupd */
	uint8		pwr_stab_thresh; /**< Number of successes before power step down */
	uint8		pwr_sel_exp_time; /**< Time lapse for expiry of database */
	uint8		PAD;
} powersel_params_t;

#define WL_LPC_PARAMS_VER_2	2u

typedef struct lpc_params {
	uint16		version;
	uint16		length;
	/* LPC Params exposed via IOVAR */
	uint8		rate_stab_thresh; /**< Lowest power reached by algorithm */
	uint8		pwr_stab_thresh; /**< Number of successes before power step down */
	uint8		lpc_exp_time; /**< Time lapse for expiry of database */
	uint8		pwrup_slow_step; /**< Step size for slow step up */
	uint8		pwrup_fast_step; /**< Step size for fast step up */
	uint8		pwrdn_slow_step; /**< Step size for slow step down */
} lpc_params_t;

#define WL_LPC_EPABYPASS_PARAMS_VER	1u
typedef struct lpc_epabypass_params_v1 {
	/* LPC EPABYPASS Params dumped via IOVAR */
	uint16		bypass_pkt_cnts; /* no. of packets with epabypass mode */
	uint16		pkt_cnts; /* no. of packets cnts  */
	uint8		enable; /* epabypass enable */
	int8		epabypass_pwr; /* pwr at which switch to epabypass mode */
	uint8		PAD[2];
} lpc_epabypass_params_v1_t;

typedef struct wl_lpc_epabypass_ioc_v1 {
	uint16 version; /* structure version */
	uint16 len;     /* structure length */
	uint16 id;      /* ID of the sub-command */
	uint8  data;
	uint8  PAD;
	lpc_epabypass_params_v1_t params;
} wl_lpc_epabypass_ioc_t;

/* EPABYPASS sub command IDs */
enum {
	IOV_LPC_EPABYPASS_CMD_ENABLE  = 0u,	/* enable/disable */
	IOV_LPC_EPABYPASS_CMD_FORCED  = 1u,	/* forced epabypass */
	IOV_LPC_EPABYPASS_CMD_MAXPWR  = 2u,	/* maxpwr */
	IOV_LPC_EPABYPASS_CMD_PARAMS  = 3u,	/* params */
	IOV_LPC_EPABYPASS_CMD_RESET_BYPCNTS  = 4u /* reset byp_pkt_cnts */
};

/* tx pkt delay statistics */
#define	SCB_RETRY_SHORT_DEF	7	/**< Default Short retry Limit */
#define WLPKTDLY_HIST_NBINS	16	/**< number of bins used in the Delay histogram */

/** structure to store per-AC delay statistics */
typedef struct scb_delay_stats {
	uint32 txmpdu_lost;	/**< number of MPDUs lost */
	uint32 txmpdu_cnt[SCB_RETRY_SHORT_DEF]; /**< retry times histogram */
	uint32 delay_sum[SCB_RETRY_SHORT_DEF]; /**< cumulative packet latency */
	uint32 delay_min;	/**< minimum packet latency observed */
	uint32 delay_max;	/**< maximum packet latency observed */
	uint32 delay_avg;	/**< packet latency average */
	uint32 delay_hist[WLPKTDLY_HIST_NBINS];	/**< delay histogram */
	uint32 delay_count;	/**< minimum number of time period units before
				consequent packet delay events can be generated
				*/
	uint32 prev_txmpdu_cnt;	/**< Previous value of txmpdu_cnt[] during last iteration */
	uint32 prev_delay_sum;	/**< Previous value of delay_sum[] during last iteration */
} scb_delay_stats_t;

/** structure for txdelay event */
typedef struct txdelay_event {
	uint8				status;
	uint8				PAD[3];
	int32				rssi;
	chanim_stats_t		chanim_stats;
	scb_delay_stats_t	delay_stats[AC_COUNT];
} txdelay_event_t;

/** structure for txdelay parameters */
typedef struct txdelay_params {
	uint16	ratio;	/**< Avg Txdelay Delta */
	uint8	cnt;	/**< Sample cnt */
	uint8	period;	/**< Sample period */
	uint8	tune;	/**< Debug */
	uint8	PAD;
} txdelay_params_t;
#define MAX_TXDELAY_STATS_SCBS 6
#define TXDELAY_STATS_VERSION_1	1

enum {
	TXDELAY_STATS_PARTIAL_RESULT = 0,
	TXDELAY_STATS_FULL_RESULT = 1
};

typedef struct scb_total_delay_stats {
	struct  ether_addr ea;
	uint8   PAD[2];
	scb_delay_stats_t dlystats[AC_COUNT];
} scb_total_delay_stats_t;

typedef struct txdelay_stats {
	uint32  version;
	uint32  full_result;    /* 0:Partial, 1:full */
	uint32  scb_cnt;        /* in:requested, out:returned */
	scb_total_delay_stats_t scb_delay_stats[BCM_FLEX_ARRAY];
} txdelay_stats_t;

#define WL_TXDELAY_STATS_FIXED_SIZE \
	(sizeof(txdelay_stats_t)+(MAX_TXDELAY_STATS_SCBS-1)*sizeof(scb_total_delay_stats_t))
enum {
	WNM_SERVICE_DMS = 1,
	WNM_SERVICE_FMS = 2,
	WNM_SERVICE_TFS = 3
};

/** Definitions for WNM/NPS TCLAS */
typedef struct wl_tclas {
	uint8 user_priority;
	uint8 fc_len;
	dot11_tclas_fc_t fc;
} wl_tclas_t;

#define WL_TCLAS_FIXED_SIZE	OFFSETOF(wl_tclas_t, fc)

typedef struct wl_tclas_list {
	uint32 num;
	wl_tclas_t tclas[];
} wl_tclas_list_t;

/** Definitions for WNM/NPS Traffic Filter Service */
typedef struct wl_tfs_req {
	uint8 tfs_id;
	uint8 tfs_actcode;
	uint8 tfs_subelem_id;
	uint8 send;
} wl_tfs_req_t;

typedef struct wl_tfs_filter {
	uint8 status;			/**< Status returned by the AP */
	uint8 tclas_proc;		/**< TCLAS processing value (0:and, 1:or)  */
	uint8 tclas_cnt;		/**< count of all wl_tclas_t in tclas array */
	uint8 tclas[BCM_FLEX_ARRAY];			/**< VLA of wl_tclas_t */
} wl_tfs_filter_t;
#define WL_TFS_FILTER_FIXED_SIZE	OFFSETOF(wl_tfs_filter_t, tclas)

typedef struct wl_tfs_fset {
	struct ether_addr ea;		/**< Address of AP/STA involved with this filter set */
	uint8 tfs_id;			/**< TFS ID field chosen by STA host */
	uint8 status;			/**< Internal status TFS_STATUS_xxx */
	uint8 actcode;			/**< Action code DOT11_TFS_ACTCODE_xxx */
	uint8 token;			/**< Token used in last request frame */
	uint8 notify;			/**< Notify frame sent/received because of this set */
	uint8 filter_cnt;		/**< count of all wl_tfs_filter_t in filter array */
	uint8 filter[1];		/**< VLA of wl_tfs_filter_t */
} wl_tfs_fset_t;
#define WL_TFS_FSET_FIXED_SIZE		OFFSETOF(wl_tfs_fset_t, filter)

enum {
	TFS_STATUS_DISABLED = 0,	/**< TFS filter set disabled by user */
	TFS_STATUS_DISABLING = 1,	/**< Empty request just sent to AP */
	TFS_STATUS_VALIDATED = 2,	/**< Filter set validated by AP (but maybe not enabled!) */
	TFS_STATUS_VALIDATING = 3,	/**< Filter set just sent to AP */
	TFS_STATUS_NOT_ASSOC = 4,	/**< STA not associated */
	TFS_STATUS_NOT_SUPPORT = 5,	/**< TFS not supported by AP */
	TFS_STATUS_DENIED = 6,		/**< Filter set refused by AP (=> all sets are disabled!) */
};

typedef struct wl_tfs_status {
	uint8 fset_cnt;				/**< count of all wl_tfs_fset_t in fset array */
	wl_tfs_fset_t fset[BCM_FLEX_ARRAY];	/**< VLA of wl_tfs_fset_t */
} wl_tfs_status_t;

typedef struct wl_tfs_set {
	uint8 send;		/**< Immediatly register registered sets on AP side */
	uint8 tfs_id;		/**< ID of a specific set (existing or new), or nul for all */
	uint8 actcode;		/**< Action code for this filter set */
	uint8 tclas_proc;	/**< TCLAS processing operator for this filter set */
} wl_tfs_set_t;

typedef struct wl_tfs_term {
	uint8 del;			/**< Delete internal set once confirmation received */
	uint8 tfs_id;			/**< ID of a specific set (existing), or nul for all */
} wl_tfs_term_t;


#define DMS_DEP_PROXY_ARP (1 << 0)

/* Definitions for WNM/NPS Directed Multicast Service */
enum {
	DMS_STATUS_DISABLED     = 0,	/**< DMS desc disabled by user */
	DMS_STATUS_ACCEPTED     = 1,	/**< Request accepted by AP */
	DMS_STATUS_NOT_ASSOC    = 2,	/**< STA not associated */
	DMS_STATUS_NOT_SUPPORT  = 3,	/**< DMS not supported by AP */
	DMS_STATUS_DENIED       = 4,	/**< Request denied by AP */
	DMS_STATUS_TERM         = 5,	/**< Request terminated by AP */
	DMS_STATUS_REMOVING     = 6,	/**< Remove request just sent */
	DMS_STATUS_ADDING       = 7,	/**< Add request just sent */
	DMS_STATUS_ERROR        = 8,	/**< Non compliant AP behvior */
	DMS_STATUS_IN_PROGRESS  = 9,	/**< Request just sent */
	DMS_STATUS_REQ_MISMATCH = 10,	/**< Conditions for sending DMS req not met */
	DMS_STATUS_TIMEOUT      = 11	/**< Request Time out */
};

typedef struct wl_dms_desc {
	uint8 user_id;
	uint8 status;
	uint8 token;
	uint8 dms_id;
	uint8 tclas_proc;
	uint8 mac_len;		/**< length of all ether_addr in data array, 0 if STA */
	uint8 tclas_len;	/**< length of all wl_tclas_t in data array */
	uint8 data[1];		/**< VLA of ether_addr and wl_tclas_t (in this order ) */
} wl_dms_desc_t;

#define WL_DMS_DESC_FIXED_SIZE	OFFSETOF(wl_dms_desc_t, data)

typedef struct wl_dms_status {
	uint32 cnt;
	wl_dms_desc_t desc[BCM_FLEX_ARRAY];
} wl_dms_status_t;

typedef struct wl_dms_set {
	uint8 send;
	uint8 user_id;
	uint8 tclas_proc;
} wl_dms_set_t;

typedef struct wl_dms_term {
	uint8 del;
	uint8 user_id;
} wl_dms_term_t;

typedef struct wl_service_term {
	uint8 service;
	union {
		wl_dms_term_t dms;
	} u;
} wl_service_term_t;

/** Definitions for WNM/NPS BSS Transistion */
#define WL_BSSTRANS_QUERY_VERSION_1 1
typedef struct wl_bsstrans_query {
	uint16 version;   /* structure  version */
	uint16 pad0;  /* padding for 4-byte allignment */
	wlc_ssid_t ssid; /* SSID of NBR elem to be queried for */
	uint8 reason; /* Reason code of the BTQ */
	uint8 PAD[3];  /* padding for 4-byte allignment */
} wl_bsstrans_query_t;

#define BTM_QUERY_NBR_COUNT_MAX 16

#define WL_BTQ_NBR_LIST_VERSION_1 1
typedef struct wl_btq_nbr_list {
	uint16 version;   /* structure  version */
	uint8 count; /* No. of BTQ NBRs returned */
	uint8 PAD;  /* padding for 4-byte allignment */
	nbr_rpt_elem_t btq_nbt_elem[]; /* BTQ NBR elem in a BTQ NBR list */
} wl_btq_nbr_list_t;

typedef struct wl_bsstrans_req {
	uint16 tbtt;			/**< time of BSS to end of life, in unit of TBTT */
	uint16 dur;			/**< time of BSS to keep off, in unit of minute */
	uint8 reqmode;			/**< request mode of BSS transition request */
	uint8 unicast;			/**< request by unicast or by broadcast */
} wl_bsstrans_req_t;

enum {
	BSSTRANS_RESP_AUTO = 0,		/**< Currently equivalent to ENABLE */
	BSSTRANS_RESP_DISABLE = 1,	/**< Never answer BSS Trans Req frames */
	BSSTRANS_RESP_ENABLE = 2,	/**< Always answer Req frames with preset data */
	BSSTRANS_RESP_WAIT = 3,		/**< Send ind, wait and/or send preset data (NOT IMPL) */
	BSSTRANS_RESP_IMMEDIATE = 4	/**< After an ind, set data and send resp (NOT IMPL) */
};

typedef struct wl_bsstrans_resp {
	uint8 policy;
	uint8 status;
	uint8 delay;
	struct ether_addr target;
} wl_bsstrans_resp_t;

/* "wnm_bsstrans_policy" argument programs behavior after BSSTRANS Req reception.
 * BSS-Transition feature is used by multiple programs such as NPS-PF, VE-PF,
 * Band-steering, Hotspot 2.0 and customer requirements. Each PF and its test plan
 * mandates different behavior on receiving BSS-transition request. To accomodate
 * such divergent behaviors these policies have been created.
 */
typedef enum {
	WL_BSSTRANS_POLICY_ROAM_ALWAYS = 0,	/**< Roam (or disassociate) in all cases */
	WL_BSSTRANS_POLICY_ROAM_IF_MODE = 1,	/**< Roam only if requested by Request Mode field */
	WL_BSSTRANS_POLICY_ROAM_IF_PREF = 2,	/**< Roam only if Preferred BSS provided */
	WL_BSSTRANS_POLICY_WAIT = 3,		/**< Wait for deauth and send Accepted status */
	WL_BSSTRANS_POLICY_PRODUCT = 4,		/**< Policy for real product use cases */
	WL_BSSTRANS_POLICY_PRODUCT_WBTEXT = 5,	/**< Policy for real product use cases */
	WL_BSSTRANS_POLICY_MBO = 6,		/**< Policy for MBO certification */
	WL_BSSTRANS_POLICY_T2LM = 7,		/**< Policy for TID-to-link mapping use-case */
	WL_BSSTRANS_POLICY_MAX = 8
} wnm_bsstrans_policy_type_t;

/** Definitions for WNM/NPS TIM Broadcast */
typedef struct wl_timbc_offset {
	int16 offset;		/**< offset in us */
	uint16 fix_intv;	/**< override interval sent from STA */
	uint16 rate_override;	/**< use rate override to send high rate TIM broadcast frame */
	uint8 tsf_present;	/**< show timestamp in TIM broadcast frame */
	uint8 PAD;
} wl_timbc_offset_t;

typedef struct wl_timbc_set {
	uint8 interval;		/**< Interval in DTIM wished or required. */
	uint8 flags;		/**< Bitfield described below */
	uint16 rate_min;	/**< Minimum rate required for High/Low TIM frames. Optionnal */
	uint16 rate_max;	/**< Maximum rate required for High/Low TIM frames. Optionnal */
} wl_timbc_set_t;

enum {
	WL_TIMBC_SET_TSF_REQUIRED = 1,	/**< Enable TIMBC only if TSF in TIM frames */
	WL_TIMBC_SET_NO_OVERRIDE = 2,	/**< ... if AP does not override interval */
	WL_TIMBC_SET_PROXY_ARP = 4,	/**< ... if AP support Proxy ARP */
	WL_TIMBC_SET_DMS_ACCEPTED = 8	/**< ... if all DMS desc have been accepted */
};

typedef struct wl_timbc_status {
	uint8 status_sta;		/**< Status from internal state machine (check below) */
	uint8 status_ap;		/**< From AP response frame (check 8.4.2.86 from 802.11) */
	uint8 interval;
	uint8 PAD;
	int32 offset;
	uint16 rate_high;
	uint16 rate_low;
} wl_timbc_status_t;

enum {
	WL_TIMBC_STATUS_DISABLE = 0,		/**< TIMBC disabled by user */
	WL_TIMBC_STATUS_REQ_MISMATCH = 1,	/**< AP settings do no match user requirements */
	WL_TIMBC_STATUS_NOT_ASSOC = 2,		/**< STA not associated */
	WL_TIMBC_STATUS_NOT_SUPPORT = 3,	/**< TIMBC not supported by AP */
	WL_TIMBC_STATUS_DENIED = 4,		/**< Req to disable TIMBC sent to AP */
	WL_TIMBC_STATUS_ENABLE = 5		/**< TIMBC enabled */
};

/** Definitions for PM2 Dynamic Fast Return To Sleep */
typedef struct wl_pm2_sleep_ret_ext {
	uint8  logic;			/**< DFRTS logic: see WL_DFRTS_LOGIC_* below */
	uint8  trctl;			/**< DFRTS transition/revert control */
	uint16 low_ms;			/**< Low FRTS timeout */
	uint16 high_ms;			/**< High FRTS timeout */
	uint16 rx_pkts_threshold;	/**< switching threshold: # rx pkts */
	uint16 tx_pkts_threshold;	/**< switching threshold: # tx pkts */
	uint16 txrx_pkts_threshold;	/**< switching threshold: # (tx+rx) pkts */
	uint32 rx_bytes_threshold;	/**< switching threshold: # rx bytes */
	uint32 tx_bytes_threshold;	/**< switching threshold: # tx bytes */
	uint32 txrx_bytes_threshold;	/**< switching threshold: # (tx+rx) bytes */
} wl_pm2_sleep_ret_ext_t;

/* Values for the logic field */
#define WL_DFRTS_LOGIC_OFF	0	/**< Feature is disabled */
#define WL_DFRTS_LOGIC_OR	1	/**< OR all non-zero threshold conditions */
#define WL_DFRTS_LOGIC_AND	2	/**< AND all non-zero threshold conditions */

/* Values for the trctl field */
#define WL_DFRTS_REVERT_CNT	0x07u	/**< Revert after N periods < threshold */

/* Values for the passive_on_restricted_mode iovar.  When set to non-zero, this iovar
 * disables automatic conversions of a channel from passively scanned to
 * actively scanned.  These values only have an effect for country codes such
 * as XZ where some 5 GHz channels are defined to be passively scanned.
 */
#define WL_PASSACTCONV_DISABLE_NONE	0	/**< Enable permanent and temporary conversions */
#define WL_PASSACTCONV_DISABLE_ALL	1	/**< Disable permanent and temporary conversions */
#define WL_PASSACTCONV_DISABLE_PERM	2	/**< Disable only permanent conversions */

/* Definitions for Reliable Multicast */
#define WL_RMC_CNT_VERSION	   1
#define WL_RMC_TR_VERSION	   1
#define WL_RMC_MAX_CLIENT	   32
#define WL_RMC_FLAG_INBLACKLIST	   1
#define WL_RMC_FLAG_ACTIVEACKER	   2
#define WL_RMC_FLAG_RELMCAST	   4
#define WL_RMC_MAX_TABLE_ENTRY     4

#define WL_RMC_VER		   1
#define WL_RMC_INDEX_ACK_ALL       255
#define WL_RMC_NUM_OF_MC_STREAMS   4
#define WL_RMC_MAX_TRS_PER_GROUP   1
#define WL_RMC_MAX_TRS_IN_ACKALL   1
#define WL_RMC_ACK_MCAST0          0x02
#define WL_RMC_ACK_MCAST_ALL       0x01
#define WL_RMC_ACTF_TIME_MIN       300		/**< time in ms */
#define WL_RMC_ACTF_TIME_MAX       20000	/**< time in ms */
#define WL_RMC_MAX_NUM_TRS	   32		/**< maximun transmitters allowed */
#define WL_RMC_ARTMO_MIN           350		/**< time in ms */
#define WL_RMC_ARTMO_MAX           40000	/**< time in ms */

/* RMC events in action frames */
enum rmc_opcodes {
	RELMCAST_ENTRY_OP_DISABLE = 0,   /**< Disable multi-cast group */
	RELMCAST_ENTRY_OP_DELETE  = 1,   /**< Delete multi-cast group */
	RELMCAST_ENTRY_OP_ENABLE  = 2,   /**< Enable multi-cast group */
	RELMCAST_ENTRY_OP_ACK_ALL = 3    /**< Enable ACK ALL bit in AMT */
};

/* RMC operational modes */
enum rmc_modes {
	WL_RMC_MODE_RECEIVER    = 0,	 /**< Receiver mode by default */
	WL_RMC_MODE_TRANSMITTER = 1,	 /**< Transmitter mode using wl ackreq */
	WL_RMC_MODE_INITIATOR   = 2	 /**< Initiator mode using wl ackreq */
};

/** Each RMC mcast client info */
typedef struct wl_relmcast_client {
	uint8 flag;			/**< status of client such as AR, R, or blacklisted */
	uint8 PAD;
	int16 rssi;			/**< rssi value of RMC client */
	struct ether_addr addr;		/**< mac address of RMC client */
} wl_relmcast_client_t;

/** RMC Counters */
typedef struct wl_rmc_cnts {
	uint16  version;		/**< see definition of WL_CNT_T_VERSION */
	uint16  length;			/**< length of entire structure */
	uint16	dupcnt;			/**< counter for duplicate rmc MPDU */
	uint16	ackreq_err;		/**< counter for wl ackreq error    */
	uint16	af_tx_err;		/**< error count for action frame transmit   */
	uint16	null_tx_err;		/**< error count for rmc null frame transmit */
	uint16	af_unicast_tx_err;	/**< error count for rmc unicast frame transmit */
	uint16	mc_no_amt_slot;		/**< No mcast AMT entry available */
	/* Unused. Keep for rom compatibility */
	uint16	mc_no_glb_slot;		/**< No mcast entry available in global table */
	uint16	mc_not_mirrored;	/**< mcast group is not mirrored */
	uint16	mc_existing_tr;		/**< mcast group is already taken by transmitter */
	uint16	mc_exist_in_amt;	/**< mcast group is already programmed in amt */
	/* Unused. Keep for rom compatibility */
	uint16	mc_not_exist_in_gbl;	/**< mcast group is not in global table */
	uint16	mc_not_exist_in_amt;	/**< mcast group is not in AMT table */
	uint16	mc_utilized;		/**< mcast addressed is already taken */
	uint16	mc_taken_other_tr;	/**< multi-cast addressed is already taken */
	uint32	rmc_rx_frames_mac;      /**< no of mc frames received from mac */
	uint32	rmc_tx_frames_mac;      /**< no of mc frames transmitted to mac */
	uint32	mc_null_ar_cnt;         /**< no. of times NULL AR is received */
	uint32	mc_ar_role_selected;	/**< no. of times took AR role */
	uint32	mc_ar_role_deleted;	/**< no. of times AR role cancelled */
	uint32	mc_noacktimer_expired;  /**< no. of times noack timer expired */
	uint16  mc_no_wl_clk;           /**< no wl clk detected when trying to access amt */
	uint16  mc_tr_cnt_exceeded;     /**< No of transmitters in the network exceeded */
} wl_rmc_cnts_t;

/** RMC Status */
typedef struct wl_relmcast_st {
	uint8         ver;		/**< version of RMC */
	uint8         num;		/**< number of clients detected by transmitter */
	wl_relmcast_client_t clients[WL_RMC_MAX_CLIENT];
	uint16        err;		/**< error status (used in infra) */
	uint16        actf_time;	/**< action frame time period */
} wl_relmcast_status_t;

/** Entry for each STA/node */
typedef struct wl_rmc_entry {
	/* operation on multi-cast entry such add,
	 * delete, ack-all
	 */
	int8    flag;
	struct ether_addr addr;		/**< multi-cast group mac address */
} wl_rmc_entry_t;

/** RMC table */
typedef struct wl_rmc_entry_table {
	uint8   index;			/**< index to a particular mac entry in table */
	uint8   opcode;			/**< opcodes or operation on entry */
	wl_rmc_entry_t entry[WL_RMC_MAX_TABLE_ENTRY];
} wl_rmc_entry_table_t;

typedef struct wl_rmc_trans_elem {
	struct ether_addr tr_mac;	/**< transmitter mac */
	struct ether_addr ar_mac;	/**< ar mac */
	uint16 artmo;			/**< AR timeout */
	uint8 amt_idx;			/**< amt table entry */
	uint8 PAD;
	uint16 flag;			/**< entry will be acked, not acked, programmed, full etc */
} wl_rmc_trans_elem_t;

/** RMC transmitters */
typedef struct wl_rmc_trans_in_network {
	uint8         ver;		/**< version of RMC */
	uint8         num_tr;		/**< number of transmitters in the network */
	wl_rmc_trans_elem_t trs[WL_RMC_MAX_NUM_TRS];
} wl_rmc_trans_in_network_t;

/** To update vendor specific ie for RMC */
typedef struct wl_rmc_vsie {
	uint8	oui[DOT11_OUI_LEN];
	uint8	PAD;
	uint16	payload;	/**< IE Data Payload */
} wl_rmc_vsie_t;


/* structures  & defines for proximity detection  */
enum proxd_method {
	PROXD_UNDEFINED_METHOD = 0,
	PROXD_RSSI_METHOD = 1,
	PROXD_TOF_METHOD = 2
};

/* structures for proximity detection device role */
#define WL_PROXD_MODE_DISABLE	0
#define WL_PROXD_MODE_NEUTRAL	1
#define WL_PROXD_MODE_INITIATOR	2
#define WL_PROXD_MODE_TARGET	3

#define WL_PROXD_ACTION_STOP		0
#define WL_PROXD_ACTION_START		1

#define WL_PROXD_FLAG_TARGET_REPORT		0x1
#define WL_PROXD_FLAG_REPORT_FAILURE		0x2
#define WL_PROXD_FLAG_INITIATOR_REPORT		0x4
#define WL_PROXD_FLAG_NOCHANSWT			0x8
#define WL_PROXD_FLAG_NETRUAL			0x10
#define WL_PROXD_FLAG_INITIATOR_RPTRTT		0x20
#define WL_PROXD_FLAG_ONEWAY			0x40
#define WL_PROXD_FLAG_SEQ_EN			0x80
#define WL_PROXD_FLAG_DUMP_CHANNEL_ESTIMATES	0x100

#define WL_PROXD_SETFLAG_K		0x1	/**< Delay K factor for initiator/target */
#define WL_PROXD_SETFLAG_N		0x2	/**< Thresh crossing params for 5g/2g sequence */
#define WL_PROXD_SETFLAG_S		0x4	/**< Simple threshold crossing values for 5g/2g */
#define WL_PROXD_SETFLAG_C		0x8	/**< 2G GD offsets for core 0/1 */
#define WL_PROXD_SETFLAG_D		0x10	/**< 5G GD offsets for core 0/1 */

#define WL_PROXD_RANDOM_WAKEUP	0x8000
#define WL_PROXD_MAXREPORT	8

typedef struct wl_proxd_iovar {
	uint16	method;		/**< Proximity Detection method */
	uint16	mode;		/**< Mode (neutral, initiator, target) */
} wl_proxd_iovar_t;

/*
 * structures for proximity detection parameters
 * consists of two parts, common and method specific params
 * common params should be placed at the beginning
 */

typedef struct wl_proxd_params_common	{
	chanspec_t	chanspec;	/**< channel spec */
	int16		tx_power;	/**< tx power of Proximity Detection(PD) frames (in dBm) */
	uint16		tx_rate;	/**< tx rate of PD rames  (in 500kbps units) */
	uint16		timeout;	/**< timeout value */
	uint16		interval;	/**< interval between neighbor finding attempts (in TU) */
	uint16		duration;	/**< duration of neighbor finding attempts (in ms) */
} wl_proxd_params_common_t;

typedef struct wl_proxd_params_rssi_method {
	chanspec_t	chanspec;	/**< chanspec for home channel */
	int16		tx_power;	/**< tx power of Proximity Detection frames (in dBm) */
	uint16		tx_rate;	/**< tx rate of PD frames, 500kbps units */
	uint16		timeout;	/**< state machine wait timeout of the frames (in ms) */
	uint16		interval;	/**< interval between neighbor finding attempts (in TU) */
	uint16		duration;	/**< duration of neighbor finding attempts (in ms) */
					/* method specific ones go after this line */
	int16		rssi_thresh;	/**< RSSI threshold (in dBm) */
	uint16		maxconvergtmo;	/**< max wait converge timeout (in ms) */
} wl_proxd_params_rssi_method_t;

#define Q1_NS			25	/**< Q1 time units */

/* Number of bandwidth that the TOF can support */
#define TOF_BW_NUM		3
/* Number of total index including seq tx/rx idx */
#define TOF_BW_SEQ_NUM          5

#define TOF_SEQ_KVAL_2G_CHANOFFS_LEN	4u	/* Channel offets for 2G, 1, 2-7, 8-10, 11-13 */
#define TOF_SEQ_KVAL_5G_CHANOFFS_LEN	4u	/* Channel offets for 5G, 42, 58, 106-139, 155 */

enum tof_seq_kval_2g_chan_offset {
	TOF_SEQ_KVAL_2G_CHANOFF_COREID = 0,	/* core id */
	TOF_SEQ_KVAL_2G_CHANOFF_GROUP1 = 1,	/* channels: 1 */
	TOF_SEQ_KVAL_2G_CHANOFF_GROUP2 = 2,	/* channels: 2-7 */
	TOF_SEQ_KVAL_2G_CHANOFF_GROUP3 = 3,	/* channels: 8-10 */
	TOF_SEQ_KVAL_2G_CHANOFF_GROUP4 = 4	/* channels: 11-13 */
};

enum tof_seq_kval_5g_chan_offset {
	TOF_SEQ_KVAL_5G_CHANOFF_COREID = 0,	/* core id */
	TOF_SEQ_KVAL_5G_CHANOFF_GROUP1 = 1,	/* channels: 42 */
	TOF_SEQ_KVAL_5G_CHANOFF_GROUP2 = 2,	/* channels: 58 */
	TOF_SEQ_KVAL_5G_CHANOFF_GROUP3 = 3,	/* channels: 106-139 */
	TOF_SEQ_KVAL_5G_CHANOFF_GROUP4 = 4	/* channels: 155 */
};

enum tof_bw_index {
	TOF_BW_20MHZ_INDEX = 0,
	TOF_BW_40MHZ_INDEX = 1,
	TOF_BW_80MHZ_INDEX = 2,
	TOF_BW_SEQTX_INDEX = 3,
	TOF_BW_SEQRX_INDEX = 4
};

/*
 * Version 2 of above bw defines
 * and enum tof_bw_index,
 * with advent of 160 MHz
 */
#define TOF_BW_NUM_V2		4u
#define TOF_BW_SEQ_NUM_V2	6u

enum tof_bw_index_v2 {
	TOF_BW_20MHZ_INDEX_V2 = 0,
	TOF_BW_40MHZ_INDEX_V2 = 1,
	TOF_BW_80MHZ_INDEX_V2 = 2,
	TOF_BW_160MHZ_INDEX_V2 = 3,
	TOF_BW_SEQTX_INDEX_V2 = 4,
	TOF_BW_SEQRX_INDEX_V2 = 5
};

#define BANDWIDTH_BASE	20	/**< base value of bandwidth */
#define TOF_BW_20MHZ    (BANDWIDTH_BASE << TOF_BW_20MHZ_INDEX_V2)
#define TOF_BW_40MHZ    (BANDWIDTH_BASE << TOF_BW_40MHZ_INDEX_V2)
#define TOF_BW_80MHZ    (BANDWIDTH_BASE << TOF_BW_80MHZ_INDEX_V2)
#define TOF_BW_160MHZ    (BANDWIDTH_BASE << TOF_BW_160MHZ_INDEX_V2)

#define TOF_BW_10MHZ    10

#define NFFT_BASE		64	/**< base size of fft */
#define TOF_NFFT_20MHZ  (NFFT_BASE << TOF_BW_20MHZ_INDEX_V2)
#define TOF_NFFT_40MHZ  (NFFT_BASE << TOF_BW_40MHZ_INDEX_V2)
#define TOF_NFFT_80MHZ  (NFFT_BASE << TOF_BW_80MHZ_INDEX_V2)
#define TOF_NFFT_160MHZ  (NFFT_BASE << TOF_BW_160MHZ_INDEX_V2)

typedef struct wl_proxd_params_tof_method {
	chanspec_t	chanspec;	/**< chanspec for home channel */
	int16		tx_power;	/**< tx power of Proximity Detection(PD) frames (in dBm) */
	uint16		tx_rate;	/**< tx rate of PD rames  (in 500kbps units) */
	uint16		timeout;	/**< state machine wait timeout of the frames (in ms) */
	uint16		interval;	/**< interval between neighbor finding attempts (in TU) */
	uint16		duration;	/**< duration of neighbor finding attempts (in ms) */
	/* specific for the method go after this line */
	struct ether_addr tgt_mac;	/**< target mac addr for TOF method */
	uint16		ftm_cnt;	/**< number of the frames txed by initiator */
	uint16		retry_cnt;	/**< number of retransmit attampts for ftm frames */
	int16		vht_rate;	/**< ht or vht rate */
	/* add more params required for other methods can be added here  */
} wl_proxd_params_tof_method_t;

typedef struct wl_proxd_seq_config
{
	int16 N_tx_log2;
	int16 N_rx_log2;
	int16 N_tx_scale;
	int16 N_rx_scale;
	int16 w_len;
	int16 w_offset;
} wl_proxd_seq_config_t;

#define WL_PROXD_TUNE_VERSION_1		1
#define WL_PROXD_TUNE_VERSION_2		2
#include <packed_section_start.h>
/* For legacy ranging target (e.g. 43430, 43342) */
typedef BWL_PRE_PACKED_STRUCT struct wl_proxd_params_tof_tune_v1 {
	uint32		version;
	uint32		Ki;			/**< h/w delay K factor for initiator */
	uint32		Kt;			/**< h/w delay K factor for target */
	int16		vhtack;			/**< enable/disable VHT ACK */
	int16		N_log2[TOF_BW_SEQ_NUM]; /**< simple threshold crossing */
	int16		w_offset[TOF_BW_NUM];	/**< offset of threshold crossing window(per BW) */
	int16		w_len[TOF_BW_NUM];	/**< length of threshold crossing window(per BW) */
	int32		maxDT;			/**< max time difference of T4/T1 or T3/T2 */
	int32		minDT;			/**< min time difference of T4/T1 or T3/T2 */
	uint8		totalfrmcnt;	/**< total count of transfered measurement frames */
	uint16		rsv_media;		/**< reserve media value for TOF */
	uint32		flags;			/**< flags */
	uint8		core;			/**< core to use for tx */
	uint8		setflags;		/* set flags of K, N. S values  */
	int16		N_scale[TOF_BW_SEQ_NUM]; /**< simple threshold crossing */
	uint8		sw_adj;			/**< enable sw assisted timestamp adjustment */
	uint8		hw_adj;			/**< enable hw assisted timestamp adjustment */
	uint8		seq_en;			/**< enable ranging sequence */
	uint8		ftm_cnt[TOF_BW_SEQ_NUM]; /**< number of ftm frames based on bandwidth */
	int16		N_log2_2g;		/**< simple threshold crossing for 2g channel */
	int16		N_scale_2g;		/**< simple threshold crossing for 2g channel */
	wl_proxd_seq_config_t seq_5g20;
	wl_proxd_seq_config_t seq_2g20;		/* Thresh crossing params for 2G Sequence */
	uint16          bitflip_thresh;		/* bitflip threshold */
	uint16          snr_thresh;		/* SNR threshold */
	int8            recv_2g_thresh;		/* 2g recieve sensitivity threshold */
	uint32          acs_gdv_thresh;
	int8            acs_rssi_thresh;
	uint8           smooth_win_en;
	int32		emu_delay;
} BWL_POST_PACKED_STRUCT wl_proxd_params_tof_tune_v1_t;
#include <packed_section_end.h>

#include <packed_section_start.h>
/* For legacy ranging initiator (including 4364) */
typedef BWL_PRE_PACKED_STRUCT struct wl_proxd_params_tof_tune_v2 {
	uint32		version;
	uint32		Ki;			/**< h/w delay K factor for initiator */
	uint32		Kt;			/**< h/w delay K factor for target */
	int16		vhtack;			/**< enable/disable VHT ACK */
	int16		N_log2[TOF_BW_SEQ_NUM]; /**< simple threshold crossing */
	int16		w_offset[TOF_BW_NUM];	/**< offset of threshold crossing window(per BW) */
	int16		w_len[TOF_BW_NUM];	/**< length of threshold crossing window(per BW) */
	int32		maxDT;			/**< max time difference of T4/T1 or T3/T2 */
	int32		minDT;			/**< min time difference of T4/T1 or T3/T2 */
	uint8		totalfrmcnt;		/**< total count of transfered measurement frames */
	uint16		rsv_media;		/**< reserve media value for TOF */
	uint32		flags;			/**< flags */
	uint8		core;			/**< core to use for tx */
	uint8		setflags;		/* set flags of K, N. S values  */
	int16		N_scale[TOF_BW_SEQ_NUM]; /**< simple threshold crossing */
	uint8		sw_adj;			/**< enable sw assisted timestamp adjustment */
	uint8		hw_adj;			/**< enable hw assisted timestamp adjustment */
	uint8		seq_en;			/**< enable ranging sequence */
	uint8		ftm_cnt[TOF_BW_SEQ_NUM]; /**< number of ftm frames based on bandwidth */
	int16		N_log2_2g;		/**< simple threshold crossing for 2g channel */
	int16		N_scale_2g;		/**< simple threshold crossing for 2g channel */
	wl_proxd_seq_config_t seq_5g20;
	wl_proxd_seq_config_t seq_2g20;		/* Thresh crossing params for 2G Sequence */
	uint16          bitflip_thresh;		/* bitflip threshold */
	uint16          snr_thresh;		/* SNR threshold */
	int8            recv_2g_thresh;		/* 2g recieve sensitivity threshold */
	uint32          acs_gdv_thresh;
	int8            acs_rssi_thresh;
	uint8           smooth_win_en;
	int32		acs_gdmm_thresh;
	int8		acs_delta_rssi_thresh;
	int32		emu_delay;
	uint8		core_mask;		/* core mask selection */
} BWL_POST_PACKED_STRUCT wl_proxd_params_tof_tune_v2_t;
#include <packed_section_end.h>

#define WL_PROXD_TUNE_VERSION_3		3
/* Future ranging support */
typedef struct wl_proxd_params_tof_tune_v3 {
	uint16		version;
	uint16		len;
	uint32		Ki;			/**< h/w delay K factor for initiator */
	uint32		Kt;			/**< h/w delay K factor for target */
	int16		vhtack;			/**< enable/disable VHT ACK */
	int16		ch_offset_2gcore_val1; /**< ch 1 offset for core 0/1 */
	int16		N_log2[TOF_BW_SEQ_NUM]; /**< simple threshold crossing */
	int16		ch_offset_2gcore_val2; /**< ch 2-7 offset for core 0/1 */
	int16		w_offset[TOF_BW_NUM];	/**< offset of threshold crossing window(per BW) */
	int16		ch_offset_2gcore_val3; /**< ch 8-10 offset for core 0/1 */
	int16		w_len[TOF_BW_NUM];	/**< length of threshold crossing window(per BW) */
	int16		ch_offset_2gcore_val4; /**< ch 11-13 offset for core 0/1 */
	int32		maxDT;			/**< max time difference of T4/T1 or T3/T2 */
	int32		minDT;			/**< min time difference of T4/T1 or T3/T2 */
	uint8		totalfrmcnt;	/**< total count of transfered measurement frames */
	uint8		PAD[3];
	uint16		rsv_media;		/**< reserve media value for TOF */
	uint16		ch_offset_2gcore;   /**< 2g core id */
	uint32		flags;			/**< flags */
	uint8		core;			/**< core to use for tx */
	uint8		setflags;		/* set flags of K, N, S, C values  */
	uint16		PAD;
	int16		N_scale[TOF_BW_SEQ_NUM]; /**< simple threshold crossing */
	uint8		sw_adj;			/**< enable sw assisted timestamp adjustment */
	uint8		hw_adj;			/**< enable hw assisted timestamp adjustment */
	uint8		seq_en;			/**< enable ranging sequence */
	uint8		PAD[3];
	uint8		ftm_cnt[TOF_BW_SEQ_NUM]; /**< number of ftm frames based on bandwidth */
	uint8		PAD[3];
	int16		N_log2_2g;		/**< simple threshold crossing for 2g channel */
	int16		N_scale_2g;		/**< simple threshold crossing for 2g channel */
	wl_proxd_seq_config_t seq_5g20;
	wl_proxd_seq_config_t seq_2g20;		/* Thresh crossing params for 2G Sequence */
	uint16          bitflip_thresh;		/* bitflip threshold */
	uint16          snr_thresh;		/* SNR threshold */
	int8            recv_2g_thresh;		/* 2g recieve sensitivity threshold */
	uint8		PAD[3];
	uint32          acs_gdv_thresh;
	int8            acs_rssi_thresh;
	uint8           smooth_win_en;
	uint16		PAD;
	int32		acs_gdmm_thresh;
	int8		acs_delta_rssi_thresh;
	uint8		PAD[3];
	int32		emu_delay;
	uint8		core_mask;		/* core mask selection */
	uint8		PAD[3];
} wl_proxd_params_tof_tune_v3_t;

/*
 * tof tune with 160 MHz support
 */
#define WL_PROXD_TUNE_VERSION_4		4u
typedef struct wl_proxd_params_tof_tune_v4 {
	uint16	version;
	uint16	len;
	uint8	core;		/**< core to use for tx */
	uint8	setflags;	/* set flags of K, N. S values  */
	uint8	totalfrmcnt;	/**< total count of transfered measurement frames */
	uint8	sw_adj;		/**< enable sw assisted timestamp adjustment */
	uint8	hw_adj;		/**< enable hw assisted timestamp adjustment */
	uint8	seq_en;		/**< enable ranging sequence */
	uint8	smooth_win_en;
	uint8	core_mask;	/* core mask selection */
	int8	recv_2g_thresh;	/* 2g recieve sensitivity threshold */
	int8	acs_rssi_thresh;
	int8	acs_delta_rssi_thresh;
	uint8	ftm_cnt[TOF_BW_SEQ_NUM_V2];	/**< no. of ftm frames based on bw */
	uint8	PAD[3];		/* Use this for any int8/16 uint8/16 ext in future */
	uint16	rsv_media;	/**< reserve media value for TOF */
	uint16	bitflip_thresh;	/* bitflip threshold */
	uint16	snr_thresh;	/* SNR threshold */
	int16	vhtack;		/**< enable/disable VHT ACK */
	int16	N_log2_2g;	/**< simple threshold crossing for 2g channel */
	int16	N_scale_2g;	/**< simple threshold crossing for 2g channel */
	int16	N_log2[TOF_BW_SEQ_NUM_V2];	/**< simple threshold crossing */
	int16	w_offset[TOF_BW_NUM_V2];	/**< offset of thresh crossing window(per BW) */
	int16	w_len[TOF_BW_NUM_V2];		/**< length of thresh crossing window(per BW) */
	int16	N_scale[TOF_BW_SEQ_NUM_V2];	/**< simple threshold crossing */
	uint32	Ki;		/**< h/w delay K factor for initiator */
	uint32	Kt;		/**< h/w delay K factor for target */
	uint32	flags;		/**< flags */
	uint32	acs_gdv_thresh;
	int32	maxDT;		/**< max time difference of T4/T1 or T3/T2 */
	int32	minDT;		/**< min time difference of T4/T1 or T3/T2 */
	int32	acs_gdmm_thresh;
	int32	emu_delay;
	wl_proxd_seq_config_t seq_5g20;		/* Thresh crossing params for 2G Sequence */
	wl_proxd_seq_config_t seq_2g20;		/* Thresh crossing params for 2G Sequence */
} wl_proxd_params_tof_tune_v4_t;

/*
 * tof tune v5
 */
#define WL_PROXD_TUNE_VERSION_5		5u
typedef struct wl_proxd_params_tof_tune_v5 {
	uint16	version;
	uint16	len;
	uint8	core;			/**< core to use for tx */
	uint8	setflags;		/* set flags of K, N. S values  */
	uint8	totalfrmcnt;		/**< total count of transfered measurement frames */
	uint8	sw_adj;			/**< enable sw assisted timestamp adjustment */
	uint8	hw_adj;			/**< enable hw assisted timestamp adjustment */
	uint8	seq_en;			/**< enable ranging sequence */
	uint8	smooth_win_en;		/**< enable smooth win with ifft from matched filter */
	uint8	core_mask;		/* core mask selection */
	int8	recv_2g_thresh;		/* 2g recieve sensitivity threshold */
	int8	acs_rssi_thresh;	/* RSSI threshold below which core will not be used
					 * for auto core selection.
					 */
	int8	acs_delta_rssi_thresh;	/* threshold for RSSI delta from max. RSSI among cores */
	uint8	ftm_cnt[TOF_BW_SEQ_NUM_V2];	/**< no. of ftm frames based on bw */
	uint8	PAD;			/* Use this for any int8/uint8 ext in future */
	uint16	ch_offset_2gcore;	/**< 2g core (0/1) to apply ch_offset_2g_core_valN values */
	uint16	rsv_media;		/**< reserve media value for TOF */
	uint16	bitflip_thresh;		/* bitflip threshold */
	uint16	snr_thresh;		/* SNR threshold */
	int16	vhtack;			/**< enable/disable VHT ACK */
	int16	N_log2_2g;		/**< simple threshold crossing for 2g channel */
	int16	N_scale_2g;		/**< simple threshold crossing for 2g channel */
	int16	N_log2[TOF_BW_SEQ_NUM_V2];	/**< simple threshold crossing */
	int16	w_offset[TOF_BW_NUM_V2];	/**< offset of thresh crossing window(per BW) */
	int16	w_len[TOF_BW_NUM_V2];		/**< length of thresh crossing window(per BW) */
	int16	N_scale[TOF_BW_SEQ_NUM_V2];	/**< simple threshold crossing */
	uint32	Ki;			/**< h/w delay K factor for initiator */
	uint32	Kt;			/**< h/w delay K factor for target */
	uint32	flags;			/**< flags */
	uint32	acs_gdv_thresh;		/* Group delay variance threshold below which
					 * core not used for auto core selection.
					 */
	int32	maxDT;			/**< max time difference of T4/T1 or T3/T2 */
	int32	minDT;			/**< min time difference of T4/T1 or T3/T2 */
	int32	acs_gdmm_thresh;	/**< Threshold for group delay max - min */
	int32	emu_delay;		/**< Inserted delay by the chan emulator 10th of nano-sec */
	wl_proxd_seq_config_t seq_5g20;	/* Thresh crossing params for 5G Sequence */
	wl_proxd_seq_config_t seq_2g20;	/* Thresh crossing params for 2G Sequence */
	/* ch_offset_2gcore_valN: Group Delay offset values used to correct distance calculation.
	 * units are in tenth of nano-sec.
	 */
	int16	ch_offset_2gcore_val1;	/* ch 1 offset for core specified by ch_offset_2gcore */
	int16	ch_offset_2gcore_val2;	/* ch 2-7 offset for core specified by ch_offset_2gcore */
	int16	ch_offset_2gcore_val3;	/* ch 8-10 offset for core specified by ch_offset_2gcore */
	int16	ch_offset_2gcore_val4;	/* ch 11-13 offset for core specified by ch_offset_2gcore */
	uint8	PAD[2];
	uint16	ch_offset_5gcore;	/**< 5g core (0/1) to apply ch_offset_5g_core_valN values */
	/* ch_offset_5gcore_valN: Group Delay offset values used to correct distance calculation.
	 * units are in tenth of nano-sec.
	 */
	int16	ch_offset_5gcore_val1;	/* ch 42 offset for core specified by ch_offset_5gcore */
	int16	ch_offset_5gcore_val2;	/* ch 58 offset for core specified by ch_offset_5gcore */
	int16	ch_offset_5gcore_val3;	/* ch 106-138 ofst for core specified by ch_offset_5gcore */
	int16	ch_offset_5gcore_val4;	/* ch 155 offset for core specified by ch_offset_5gcore */
} wl_proxd_params_tof_tune_v5_t;

/*
 * tof tune v4 with negative kvalue support
 */
#define WL_PROXD_TUNE_VERSION_6		6u
typedef struct wl_proxd_params_tof_tune_v6 {
	uint16	version;
	uint16	len;
	uint8	core;		/**< core to use for tx */
	uint8	setflags;	/* set flags of K, N. S values  */
	uint8	totalfrmcnt;	/**< total count of transfered measurement frames */
	uint8	sw_adj;		/**< enable sw assisted timestamp adjustment */
	uint8	hw_adj;		/**< enable hw assisted timestamp adjustment */
	uint8	seq_en;		/**< enable ranging sequence */
	uint8	smooth_win_en;
	uint8	core_mask;	/* core mask selection */
	int8	recv_2g_thresh;	/* 2g recieve sensitivity threshold */
	int8	acs_rssi_thresh;
	int8	acs_delta_rssi_thresh;
	uint8	ftm_cnt[TOF_BW_SEQ_NUM_V2];	/**< no. of ftm frames based on bw */
	uint8	PAD[3];		/* Use this for any int8/16 uint8/16 ext in future */
	uint16	rsv_media;	/**< reserve media value for TOF */
	uint16	bitflip_thresh;	/* bitflip threshold */
	uint16	snr_thresh;	/* SNR threshold */
	int16	vhtack;		/**< enable/disable VHT ACK */
	int16	N_log2_2g;	/**< simple threshold crossing for 2g channel */
	int16	N_scale_2g;	/**< simple threshold crossing for 2g channel */
	int16	N_log2[TOF_BW_SEQ_NUM_V2];	/**< simple threshold crossing */
	int16	w_offset[TOF_BW_NUM_V2];	/**< offset of thresh crossing window(per BW) */
	int16	w_len[TOF_BW_NUM_V2];		/**< length of thresh crossing window(per BW) */
	int16	N_scale[TOF_BW_SEQ_NUM_V2];	/**< simple threshold crossing */
	int32	Ki;		/**< h/w delay K factor for initiator */
	int32	Kt;		/**< h/w delay K factor for target */
	uint32	flags;		/**< flags */
	uint32	acs_gdv_thresh;
	int32	maxDT;		/**< max time difference of T4/T1 or T3/T2 */
	int32	minDT;		/**< min time difference of T4/T1 or T3/T2 */
	int32	acs_gdmm_thresh;
	int32	emu_delay;
	wl_proxd_seq_config_t seq_5g20;		/* Thresh crossing params for 2G Sequence */
	wl_proxd_seq_config_t seq_2g20;		/* Thresh crossing params for 2G Sequence */
} wl_proxd_params_tof_tune_v6_t;

typedef struct wl_proxd_params_iovar {
	uint16	method;			/**< Proximity Detection method */
	uint8	PAD[2];
	union {
		/* common params for pdsvc */
		wl_proxd_params_common_t	cmn_params;	/**< common parameters */
		/*  method specific */
		wl_proxd_params_rssi_method_t	rssi_params;	/**< RSSI method parameters */
		wl_proxd_params_tof_method_t	tof_params;	/**< TOF method parameters */
		/* tune parameters */
		wl_proxd_params_tof_tune_v3_t	tof_tune;	/**< TOF tune parameters */
	} u;				/**< Method specific optional parameters */
} wl_proxd_params_iovar_t;

/*
 * proxd param iov with 160 MHz support
 */
#define WL_PROXD_IOVAR_VERSION_2	2u
typedef struct wl_proxd_params_iovar_v2 {
	uint16	version;
	uint16	len;
	uint16  method;                 /**< Proximity Detection method */
	uint16  PAD;
	union {
		/* common params for pdsvc */
		wl_proxd_params_common_t        cmn_params;     /**< common parameters */
		/*  method specific */
		wl_proxd_params_rssi_method_t   rssi_params;    /**< RSSI method parameters */
		wl_proxd_params_tof_method_t    tof_params;     /**< TOF method parameters */
		/* tune parameters */
		wl_proxd_params_tof_tune_v4_t   tof_tune;       /**< TOF tune parameters */
	} u;                            /**< Method specific optional parameters */
	uint8 tlv_params[];     /* xtlvs for variable ext params */
} wl_proxd_params_iovar_v2_t;

#define WL_PROXD_IOVAR_VERSION_3	3u
typedef struct wl_proxd_params_iovar_v3 {
	uint16	version;
	uint16	len;
	uint16  method;                 /**< Proximity Detection method */
	uint16  PAD;
	union {
		/* common params for pdsvc */
		wl_proxd_params_common_t        cmn_params;     /**< common parameters */
		/*  method specific */
		wl_proxd_params_rssi_method_t   rssi_params;    /**< RSSI method parameters */
		wl_proxd_params_tof_method_t    tof_params;     /**< TOF method parameters */
		/* tune parameters */
		wl_proxd_params_tof_tune_v5_t   tof_tune;       /**< TOF tune parameters */
	} u;                            /**< Method specific optional parameters */
	uint8 tlv_params[];		/* xtlvs for variable ext params */
} wl_proxd_params_iovar_v3_t;

/*
 * proxd param iov with negative kvalue support
 */
#define WL_PROXD_IOVAR_VERSION_4	4u
typedef struct wl_proxd_params_iovar_v4 {
	uint16	version;
	uint16	len;
	uint16  method;                 /**< Proximity Detection method */
	uint16  PAD;
	union {
		/* common params for pdsvc */
		wl_proxd_params_common_t        cmn_params;     /**< common parameters */
		/*  method specific */
		wl_proxd_params_rssi_method_t   rssi_params;    /**< RSSI method parameters */
		wl_proxd_params_tof_method_t    tof_params;     /**< TOF method parameters */
		/* tune parameters */
		wl_proxd_params_tof_tune_v6_t   tof_tune;       /**< TOF tune parameters */
	} u;                            /**< Method specific optional parameters */
	uint8 tlv_params[];     /* xtlvs for variable ext params */
} wl_proxd_params_iovar_v4_t;

#define PROXD_COLLECT_GET_STATUS	0
#define PROXD_COLLECT_SET_STATUS	1
#define PROXD_COLLECT_QUERY_HEADER	2
#define PROXD_COLLECT_QUERY_DATA	3
#define PROXD_COLLECT_QUERY_DEBUG	4
#define PROXD_COLLECT_REMOTE_REQUEST	5
#define PROXD_COLLECT_DONE		6

typedef enum {
	WL_PROXD_COLLECT_METHOD_TYPE_DISABLE		= 0x0,
	WL_PROXD_COLLECT_METHOD_TYPE_IOVAR		= 0x1,
	WL_PROXD_COLLECT_METHOD_TYPE_EVENT		= 0x2,
	WL_PROXD_COLLECT_METHOD_TYPE_EVENT_LOG		= 0x4
} wl_proxd_collect_method_type_t;

typedef uint16 wl_proxd_collect_method_t;	/* query status: method to send proxd collect */

#include <packed_section_start.h>
typedef BWL_PRE_PACKED_STRUCT struct wl_proxd_collect_query {
	uint32		method;		/**< method */
	uint8		request;	/**< Query request. */
	uint8		status;		/**< bitmask 0 -- disable, 0x1 -- enable collection, */
					/* 0x2 -- Use generic event, 0x4 -- use event log */
	uint16		index;		/**< The current frame index [0 to total_frames - 1]. */
	uint16		mode;		/**< Initiator or Target */
	uint8		busy;		/**< tof sm is busy */
	uint8		remote;		/**< Remote collect data */
} BWL_POST_PACKED_STRUCT wl_proxd_collect_query_t;
#include <packed_section_end.h>

#include <packed_section_start.h>
typedef BWL_PRE_PACKED_STRUCT struct wl_proxd_collect_header {
	uint16		total_frames;			/**< The total frames for this collect. */
	uint16		nfft;				/**< nfft value */
	uint16		bandwidth;			/**< bandwidth */
	uint16		channel;			/**< channel number */
	uint32		chanspec;			/**< channel spec */
	uint32		fpfactor;			/**< avb timer value factor */
	uint16		fpfactor_shift;			/**< avb timer value shift bits */
	int32		distance;			/**< distance calculated by fw */
	uint32		meanrtt;			/**< mean of RTTs */
	uint32		modertt;			/**< mode of RTTs */
	uint32		medianrtt;			/**< median of RTTs */
	uint32		sdrtt;				/**< standard deviation of RTTs */
	uint32		clkdivisor;			/**< clock divisor */
	uint16		chipnum;			/**< chip type */
	uint8		chiprev;			/**< chip revision */
	uint8		phyver;				/**< phy version */
	struct ether_addr	localMacAddr;		/**< local mac address */
	struct ether_addr	remoteMacAddr;		/**< remote mac address */
	wl_proxd_params_tof_tune_v3_t params;
} BWL_POST_PACKED_STRUCT wl_proxd_collect_header_t;
#include <packed_section_end.h>

#define WL_PROXD_COLLECT_HEADER_VERSION_2	2u
typedef struct wl_proxd_collect_header_v2 {
	uint16	version;
	uint16	len;
	uint8	chiprev;	/**< chip revision */
	uint8	phyver;		/**< phy version */
	uint8	PAD[2];		/* Use this for any int8/16 uint8/16 ext in future */
	uint16	total_frames;	/**< The total frames for this collect. */
	uint16	nfft;		/**< nfft value */
	uint16	bandwidth;	/**< bandwidth */
	uint16	channel;	/**< channel number */
	uint16	fpfactor_shift;	/**< avb timer value shift bits */
	uint16	chipnum;	/**< chip type */
	uint32	chanspec;	/**< channel spec */
	uint32	fpfactor;	/**< avb timer value factor */
	uint32	meanrtt;	/**< mean of RTTs */
	uint32	modertt;	/**< mode of RTTs */
	uint32	medianrtt;	/**< median of RTTs */
	uint32	sdrtt;		/**< standard deviation of RTTs */
	uint32	clkdivisor;	/**< clock divisor */
	int32	distance;	/**< distance calculated by fw */
	struct ether_addr localMacAddr;		/**< local mac address */
	uint16	PAD;		/* Use this for any int8/16 uint8/16 ext in future */
	struct ether_addr remoteMacAddr;	/**< remote mac address */
	uint16	PAD;		/* Use this for any int8/16 uint8/16 ext in future */
	wl_proxd_params_tof_tune_v4_t params;	/* TOF tune params */
	uint8 tlv_params[];     /* xtlvs for variable ext params */
} wl_proxd_collect_header_v2_t;

/*
 * proxd collect header with 160 MHz support
 */
#define WL_PROXD_COLLECT_HEADER_VERSION_3	3u
typedef struct wl_proxd_collect_header_v3 {
	uint16	version;
	uint16	len;
	uint8	chiprev;	/**< chip revision */
	uint8	phyver;		/**< phy version */
	uint8	PAD[2];		/* Use this for any int8/16 uint8/16 ext in future */
	uint16	total_frames;	/**< The total frames for this collect. */
	uint16	nfft;		/**< nfft value */
	uint16	bandwidth;	/**< bandwidth */
	uint16	channel;	/**< channel number */
	uint16	fpfactor_shift;	/**< avb timer value shift bits */
	uint16	chipnum;	/**< chip type */
	uint32	chanspec;	/**< channel spec */
	uint32	fpfactor;	/**< avb timer value factor */
	uint32	meanrtt;	/**< mean of RTTs */
	uint32	modertt;	/**< mode of RTTs */
	uint32	medianrtt;	/**< median of RTTs */
	uint32	sdrtt;		/**< standard deviation of RTTs */
	uint32	clkdivisor;	/**< clock divisor */
	int32	distance;	/**< distance calculated by fw */
	struct ether_addr localMacAddr;		/**< local mac address */
	uint16	PAD;		/* Use this for any int8/16 uint8/16 ext in future */
	struct ether_addr remoteMacAddr;	/**< remote mac address */
	uint16	PAD;		/* Use this for any int8/16 uint8/16 ext in future */
	wl_proxd_params_tof_tune_v5_t params;	/* TOF tune params */
	uint8 tlv_params[];     /* xtlvs for variable ext params */
} wl_proxd_collect_header_v3_t;

/*
 * proxd collect header with negative kvalue support
 */
#define WL_PROXD_COLLECT_HEADER_VERSION_4	4u
typedef struct wl_proxd_collect_header_v4 {
	uint16	version;
	uint16	len;
	uint8	chiprev;	/**< chip revision */
	uint8	phyver;		/**< phy version */
	uint8	PAD[2];		/* Use this for any int8/16 uint8/16 ext in future */
	uint16	total_frames;	/**< The total frames for this collect. */
	uint16	nfft;		/**< nfft value */
	uint16	bandwidth;	/**< bandwidth */
	uint16	channel;	/**< channel number */
	uint16	fpfactor_shift;	/**< avb timer value shift bits */
	uint16	chipnum;	/**< chip type */
	uint32	chanspec;	/**< channel spec */
	uint32	fpfactor;	/**< avb timer value factor */
	uint32	meanrtt;	/**< mean of RTTs */
	uint32	modertt;	/**< mode of RTTs */
	uint32	medianrtt;	/**< median of RTTs */
	uint32	sdrtt;		/**< standard deviation of RTTs */
	uint32	clkdivisor;	/**< clock divisor */
	int32	distance;	/**< distance calculated by fw */
	struct ether_addr localMacAddr;		/**< local mac address */
	uint16	PAD;		/* Use this for any int8/16 uint8/16 ext in future */
	struct ether_addr remoteMacAddr;	/**< remote mac address */
	uint16	PAD;		/* Use this for any int8/16 uint8/16 ext in future */
	wl_proxd_params_tof_tune_v6_t params;	/* TOF tune params */
	uint8 tlv_params[];     /* xtlvs for variable ext params */
} wl_proxd_collect_header_v4_t;

/* ifdef WL_NAN */
/*  ********************** NAN wl interface struct types and defs ******************** */
/*
 * Uses new common IOVAR batch processing mechanism
 */

/*
 * NAN config control
 * Bits 0 - 23 can be set by host
 * Bits 24 - 31 - Internal use for firmware, host cannot set it
 */

/*
 * Bit 0 : If set to 1, means event uses nan bsscfg,
 * otherwise uses infra bsscfg. Default is using infra bsscfg
 */
#define WL_NAN_CTRL_ROUTE_EVENT_VIA_NAN_BSSCFG	0x000001
/* If set, discovery beacons are transmitted on 2G band */
#define WL_NAN_CTRL_DISC_BEACON_TX_2G		0x000002
/* If set, sync beacons are transmitted on 2G band */
#define WL_NAN_CTRL_SYNC_BEACON_TX_2G		0x000004
/* If set, discovery beacons are transmitted on 5G band */
#define WL_NAN_CTRL_DISC_BEACON_TX_5G		0x000008
/* If set, sync beacons are transmitted on 5G band */
#define WL_NAN_CTRL_SYNC_BEACON_TX_5G		0x000010
/* If set, auto datapath responses will be sent by FW */
#define WL_NAN_CTRL_AUTO_DPRESP			0x000020
/* If set, auto datapath confirms will be sent by FW */
#define WL_NAN_CTRL_AUTO_DPCONF			0x000040
/* If set, auto schedule responses will be sent by FW */
#define WL_NAN_CTRL_AUTO_SCHEDRESP		0x000080
/* If set, auto schedule confirms will be sent by FW */
#define WL_NAN_CTRL_AUTO_SCHEDCONF		0x000100
/* If set, proprietary rates are supported by FW */
#define WL_NAN_CTRL_PROP_RATE			0x000200
/* If set, service awake_dw overrides global dev awake_dw */
#define WL_NAN_CTRL_SVC_OVERRIDE_DEV_AWAKE_DW	0x000400
/* If set, merge scan will be disabled */
#define WL_NAN_CTRL_SCAN_DISABLE		0x000800
/* If set, power save will be disabled */
#define WL_NAN_CTRL_POWER_SAVE_DISABLE		0x001000
/* If set, device will merge to configured CID only */
#define WL_NAN_CTRL_MERGE_CONF_CID_ONLY		0x002000
/* If set, 5g core will be brought down in single band NAN */
#define WL_NAN_CTRL_5G_SLICE_POWER_OPT		0x004000
#define WL_NAN_CTRL_DUMP_HEAP			0x008000
/* If set, host generates and assign ndp id for ndp sessions */
#define WL_NAN_CTRL_HOST_GEN_NDPID		0x010000
/* If set, nan ndp inactivity watchdog will be activated */
#define WL_NAN_CTRL_DELETE_INACTIVE_PEERS	0x020000
/* If set, nan assoc coex will be activated */
#define WL_NAN_CTRL_INFRA_ASSOC_COEX		0x040000
/* If set, dam will accept all NDP/RNG request from the peer including counter */
#define WL_NAN_CTRL_DAM_ACCEPT_ALL		0x080000
/* If set, nan mac ignores role for tx discovery beacon for periodic config */
#define WL_NAN_CTRL_FASTDISC_IGNO_ROLE		0x100000
/* If set, include NA in NAN beacons (disc beacons for now) */
#define WL_NAN_CTRL_INCL_NA_IN_BCNS		0x200000
/* If set, host assist will be enabled */
#define WL_NAN_CTRL_HOST_ASSIST			0x400000
/* If set, host configures NDI associated with the service */
#define WL_NAN_CTRL_HOST_CFG_SVC_NDI		0x800000

/* Value when all host-configurable bits set */
#define WL_NAN_CTRL_MAX_MASK			0xFFFFFF
#define WL_NAN_CFG_CTRL_FW_BITS			8

/* Last 8-bits are firmware controlled bits.
 * Bit 31:
 * If set - indicates that NAN initialization is successful
 * Bit 30:
 * If set - indicates that NAN MAC cfg creation is successful
 *
 * NOTE: These are only ready-only bits for host.
 * All sets to these bits from host are masked off
 */
#define WL_NAN_PROTO_INIT_DONE		(1u << 31u)
#define WL_NAN_CFG_CREATE_DONE		(1u << 30u)

#define WL_NAN_GET_PROTO_INIT_STATUS(x) \
		(((x) & WL_NAN_PROTO_INIT_DONE) ? TRUE:FALSE)
#define WL_NAN_CLEAR_PROTO_INIT_STATUS(x) \
		((x) &= ~WL_NAN_PROTO_INIT_DONE)
#define WL_NAN_SET_PROTO_INIT_STATUS(x) \
		((x) |= (WL_NAN_PROTO_INIT_DONE))

#define WL_NAN_GET_CFG_CREATE_STATUS(x) \
		(((x) & WL_NAN_CFG_CREATE_DONE) ? TRUE:FALSE)
#define WL_NAN_CLEAR_CFG_CREATE_STATUS(x) \
		((x) &= ~WL_NAN_CFG_CREATE_DONE)
#define WL_NAN_SET_CFG_CREATE_STATUS(x) \
		((x) |= (WL_NAN_CFG_CREATE_DONE))

#define WL_NAN_IOCTL_VERSION			0x2
/* < some sufficient ioc buff size for our module */
#define WL_NAN_IOC_BUFSZ			256
/* some sufficient ioc buff size for dump commands */
#define WL_NAN_IOC_BUFSZ_EXT			1024
#define WL_NAN_MAX_SIDS_IN_BEACONS		127 /* Max allowed SIDs */
#define WL_NAN_MASTER_RANK_LEN			8
#define WL_NAN_RANGE_LIMITED			0x0040 /* Publish/Subscribe flags */

/** The service hash (service id) is exactly this many bytes. */
#define WL_NAN_SVC_HASH_LEN			6
#define WL_NAN_HASHES_PER_BLOOM			4 /** Number of hash functions per bloom filter */

/* no. of max last disc results */
#define WL_NAN_MAX_DISC_RESULTS			3

/* Max len of Rx and Tx filters */
#define WL_NAN_MAX_SVC_MATCH_FILTER_LEN	255

/* Max service name len */
#define WL_NAN_MAX_SVC_NAME_LEN	32

/* Type of Data path connection */
#define WL_NAN_DP_TYPE_UNICAST			0
#define WL_NAN_DP_TYPE_MULTICAST		1

/* MAX security params length PMK field */
#define WL_NAN_NCS_SK_PMK_LEN 32

/* NAN pairing max password length */
#define WL_NAN_PAIRING_PASSWORD_LEN		255u

/* NAN pairing max NPBA attr length */
#define WL_NAN_NPBA_INFO_MAX_LEN		255u

/* NAN pairing max NIRA attr length */
#define WL_NAN_NIRA_INFO_MAX_LEN		255u

/* NAN pairing key max length */
#define WL_NAN_NPK_MAX_LEN			255u

/* NAN identity key max length */
#define WL_NAN_NIK_MAX_LEN			255u

/* NAN pairing XTLV list max length; x2 for local NIK and peer NIK */
#define WL_NAN_XTLV_LIST_MAX_LEN (WL_NAN_NPK_MAX_LEN + (2 * WL_NAN_NIK_MAX_LEN))

/* NAN S3 ALL maps */
#define WL_NAN_AVAIL_S3_ALLMAPS			0xFF

/* Post disc attr ID type */
typedef uint8 wl_nan_post_disc_attr_id_t;

/*
 * Component IDs
 */
typedef enum {
	WL_NAN_COMPID_CONFIG = 1,
	WL_NAN_COMPID_ELECTION = 2,
	WL_NAN_COMPID_SD = 3,
	WL_NAN_COMPID_TIMESYNC = 4,
	WL_NAN_COMPID_DATA_PATH = 5,
	WL_NAN_COMPID_DEBUG = 15 /* Keep this at the end */
} wl_nan_comp_id_t;

#define WL_NAN_COMP_SHIFT	8
#define WL_NAN_COMP_MASK(_c)	(0x0F & ((uint8)(_c)))
#define WL_NAN_COMP_ID(_c)	(WL_NAN_COMP_MASK(_c) << WL_NAN_COMP_SHIFT)

/* NAN Events */

/** Instance ID type (unique identifier) */
typedef uint8 wl_nan_instance_id_t;

/* Publish sent for a subscribe */
/* WL_NAN_EVENT_REPLIED */

typedef struct wl_nan_ev_replied {
	struct ether_addr	sub_mac; /* Subscriber MAC */
	wl_nan_instance_id_t	pub_id; /* Publisher Instance ID */
	uint8			sub_id; /* Subscriber ID */
	int8			sub_rssi; /* Subscriber RSSI */
	uint8			PAD[3];
} wl_nan_ev_replied_t;

typedef struct wl_nan_event_replied {
	struct ether_addr	sub_mac; /* Subscriber MAC */
	wl_nan_instance_id_t	pub_id; /* Publisher Instance ID */
	uint8			sub_id; /* Subscriber ID */
	int8			sub_rssi; /* Subscriber RSSI */
	uint8		attr_num;
	uint16		attr_list_len;  /* sizeof attributes attached to payload */
	uint8		attr_list[0];   /* attributes payload */
} wl_nan_event_replied_t;

/* NAN Tx status of transmitted frames */
#define WL_NAN_TXS_FAILURE	0
#define WL_NAN_TXS_SUCCESS	1

/* NAN Reason codes for Suspension Start and Resume Request status */
typedef enum wl_nan_suspension_status {
	WL_NAN_STATUS_BUSY_AWAKE    = 0,  /* chip has to remain awake due to other active session */
	WL_NAN_STATUS_AWAKE	    = 1,  /* chip wakes from sleep on Resume Request */
	WL_NAN_STATUS_SUSPENDED	    = 2	  /* chip goes to sleep on Suspension Request */
} wl_nan_suspension_status_t;

/* NAN frame types */
enum wl_nan_frame_type {
	/* discovery frame types */
	WL_NAN_FRM_TYPE_PUBLISH = 1,
	WL_NAN_FRM_TYPE_SUBSCRIBE = 2,
	WL_NAN_FRM_TYPE_FOLLOWUP = 3,

	/* datapath frame types */
	WL_NAN_FRM_TYPE_DP_REQ = 4,
	WL_NAN_FRM_TYPE_DP_RESP = 5,
	WL_NAN_FRM_TYPE_DP_CONF = 6,
	WL_NAN_FRM_TYPE_DP_INSTALL = 7,
	WL_NAN_FRM_TYPE_DP_END = 8,

	/* schedule frame types */
	WL_NAN_FRM_TYPE_SCHED_REQ = 9,
	WL_NAN_FRM_TYPE_SCHED_RESP = 10,
	WL_NAN_FRM_TYPE_SCHED_CONF = 11,
	WL_NAN_FRM_TYPE_SCHED_UPD = 12,

	/* ranging frame types */
	WL_NAN_FRM_TYPE_RNG_REQ = 13,
	WL_NAN_FRM_TYPE_RNG_RESP = 14,
	WL_NAN_FRM_TYPE_RNG_TERM = 15,
	WL_NAN_FRM_TYPE_RNG_REPORT = 16,

	WL_NAN_FRM_TYPE_UNSOLICIT_SDF = 17,

	WL_NAN_FRM_REKEY_TRIGGER = 18,
	WL_NAN_FRM_GRP_REKEY_REQ = 19,
	WL_NAN_FRM_GRP_REKEY_RESP = 20,
	WL_NAN_FRM_TYPE_INVALID
};
typedef uint8 wl_nan_frame_type_t;

/* NAN Reason codes for tx status */
enum wl_nan_txs_reason_codes {
	WL_NAN_REASON_SUCCESS =		  1,    /* NAN status success */
	WL_NAN_REASON_TIME_OUT =	  2,	/* timeout reached */
	WL_NAN_REASON_DROPPED =		  3,    /* pkt dropped due to internal failure */
	WL_NAN_REASON_MAX_RETRIES_DONE =  4	/* Max retries exceeded */
};

/* For NAN TX status */
typedef struct wl_nan_event_txs {
	uint8 status;		/* For TX status, success or failure */
	uint8 reason_code;	/* to identify reason when status is failure */
	uint16 host_seq;	/* seq num to keep track of pkts sent by host */
	uint8 type;		/* wl_nan_frame_type_t */
	uint8 PAD;
	uint16 opt_tlvs_len;
	uint8 opt_tlvs[];
} wl_nan_event_txs_t;

/* SD transmit pkt's event status is sent as optional tlv in wl_nan_event_txs_t */
typedef struct wl_nan_event_sd_txs {
	uint8 inst_id;	    /* Publish or subscribe instance id */
	uint8 req_id;	    /* Requestor instance id */
} wl_nan_event_sd_txs_t;

/* nanho fsm tlv WL_NAN_XTLV_NANHO_OOB_TXS(0x0b0a) */
typedef struct wl_nan_event_nanho_txs {
	uint32 fsm_id;	    /* nho fsm id */
	uint16 seq_id;	    /* nho seq id */
	uint16 PAD;
} wl_nan_event_nanho_txs_t;

/* Subscribe or Publish instance Terminated */

/* WL_NAN_EVENT_TERMINATED */

#define	NAN_SD_TERM_REASON_TIMEOUT	1
#define	NAN_SD_TERM_REASON_HOSTREQ	2
#define	NAN_SD_TERM_REASON_FWTERM	3
#define	NAN_SD_TERM_REASON_FAIL		4

typedef struct wl_nan_ev_terminated {
	uint8 instance_id;	/* publish / subscribe instance id */
	uint8 reason;		/* 1=timeout, 2=Host/IOVAR, 3=FW Terminated 4=Failure */
	uint8 svctype;		/* 0 - Publish, 0x1 - Subscribe */
	uint8 PAD;              /* Align */
	uint32 tx_cnt;		/* Number of SDFs sent */
} wl_nan_ev_terminated_t;

/* Follow up received against a pub / subscr */
/* WL_NAN_EVENT_RECEIVE */

typedef struct wl_nan_ev_receive {
	struct ether_addr remote_addr;	/* Peer NAN device MAC */
	uint8	local_id;		/* Local subscribe or publish ID */
	uint8	remote_id;		/* Remote subscribe or publish ID */
	int8	fup_rssi;
	uint8	attr_num;
	uint16	attr_list_len;  /* sizeof attributes attached to payload */
	uint8	attr_list[0];   /* attributes payload */
} wl_nan_ev_receive_t;

/* WL_NAN_EVENT_DISC_CACHE_TIMEOUT */
#define WL_NAN_DISC_CACHE_EXPIRY_ENTRIES_MAX 8

typedef struct wl_nan_disc_expired_cache_entry {
	uint8 l_sub_id;			/* local sub instance_id */
	uint8 r_pub_id;			/* remote-matched pub instance_id */
	struct ether_addr r_nmi_addr;	/* remote-matched pub nmi addr */
} wl_nan_disc_expired_cache_entry_t;

typedef struct wl_nan_ev_disc_cache_timeout {
	uint16 count;      /* no. of expired cache entries */
	uint16 PAD;
	wl_nan_disc_expired_cache_entry_t cache_exp_list[];
} wl_nan_ev_disc_cache_timeout_t;

/* For NAN event mask extention */
#define WL_NAN_EVMASK_EXTN_VER	1
#define WL_NAN_EVMASK_EXTN_LEN	16	/* 16*8 = 128 masks supported */

typedef struct wl_nan_event_extn {
	uint8	ver;
	uint8	pad;
	uint16	len;
	uint8	evmask[];
} wl_nan_evmask_extn_t;

/* WL_NAN_XTLV_DATA_DP_TXS */

typedef struct wl_nan_data_dp_txs {
	uint8 ndp_id;
	uint8 PAD;
	struct ether_addr indi; /* initiator ndi */
} wl_nan_data_dp_txs_t;

/* WL_NAN_XTLV_RNG_TXS */

typedef struct wl_nan_range_txs {
	uint8	range_id;
	uint8	PAD[3];
} wl_nan_range_txs_t;

#define NAN_MAX_BANDS 2

enum wl_nan_oob_af_txs_reason_code {
	WL_NAN_OOB_AF_TX_REASON_BMAP	= 0x01, /* Bitmap schedule */
	WL_NAN_OOB_AF_TX_REASON_TIMEOUT	= 0x02, /* OOB AF session timeout */
	WL_NAN_OOB_AF_TX_REASON_DISABLED= 0x03, /* OOB disabled while it was scheduled */
};

/* WL_NAN_EVENT_OOB_AF_TXS event sent to host after NAN OOB AF transmit attempted */
typedef struct wl_nan_event_oob_af_txs {
	uint8 result; /* For TX status, success or failure */
	uint8 reason; /* wl_nan_oob_af_txs_reason_code */
	uint16 token; /* Token from the host */
} wl_nan_event_oob_af_txs_t;

/* WL_NAN_EVENT_OOB_AF_TXS status */
#define NAN_OOB_AF_TXS_ACK		0x1
#define NAN_OOB_AF_TXS_NO_ACK		0x2
#define NAN_OOB_AF_TXS_EXPIRED		0x3
#define NAN_OOB_AF_TXS_DISABLED		0x4

/* WL_NAN_EVENT_OOB_AF_RX event sent to host upon receiving a NAN OOB AF frame */
typedef struct wl_nan_event_oob_af_rx {
	struct ether_addr sa;
	struct ether_addr da;
	struct ether_addr bssid;
	uint16 payload_len;
	uint8  payload[];	/* AF hdr + NAN attrbutes in TLV format */
} wl_nan_event_oob_af_rx_t;

#define WL_NAN_CFG_OOB_AF_BODY_MAX_SIZE	1024u /* Max size of AF payload */
#define WL_NAN_CFG_OOB_AF_MAX_TIMEOUT	8000u /* 8 Sec. Max timeout(in ms) for OOB AF session */

/* Max size of the NAN OOB AF information in the IOVAR */
#define WL_NAN_CFG_OOB_AF_MAX_INFO_SIZE \
	(sizeof(bcm_xtlv_t) + sizeof(wl_nan_oob_af_t) + \
	  WL_NAN_CFG_OOB_AF_BODY_MAX_SIZE)
/*
 * TLVs - Below XTLV definitions will be deprecated
 * in due course (soon as all other branches update
 * to the comp ID based XTLVs listed below).
 */
enum wl_nan_cmd_xtlv_id {
	WL_NAN_XTLV_MAC_ADDR	      =	0x120,
	WL_NAN_XTLV_MATCH_RX	      = 0x121,
	WL_NAN_XTLV_MATCH_TX	      =	0x122,
	WL_NAN_XTLV_SVC_INFO	      = 0x123,
	WL_NAN_XTLV_SVC_NAME	      = 0x124,
	WL_NAN_XTLV_SR_FILTER	      = 0x125,
	WL_NAN_XTLV_FOLLOWUP	      = 0x126,
	WL_NAN_XTLV_SVC_LIFE_COUNT    = 0x127,
	WL_NAN_XTLV_AVAIL	      = 0x128,
	WL_NAN_XTLV_SDF_RX	      = 0x129,
	WL_NAN_XTLV_SDE_CONTROL	      = 0x12a,
	WL_NAN_XTLV_SDE_RANGE_LIMIT   = 0x12b,
	WL_NAN_XTLV_NAN_AF	      = 0x12c,
	WL_NAN_XTLV_SD_TERMINATE      = 0x12d,
	WL_NAN_XTLV_CLUSTER_ID	      = 0x12e,
	WL_NAN_XTLV_PEER_RSSI	      = 0x12f,
	WL_NAN_XTLV_BCN_RX	      = 0x130,
	WL_NAN_XTLV_REPLIED	      = 0x131, /* Publish sent for a subscribe */
	WL_NAN_XTLV_RECEIVED	      = 0x132, /* FUP Received */
	WL_NAN_XTLV_DISC_RESULTS      = 0x133, /* Discovery results */
	WL_NAN_XTLV_TXS		      = 0x134, /* TX status */
	WL_NAN_XTLV_SUSPEND_STATUS    = 0x135  /* Suspend/Resume status */
};

#define WL_NAN_CMD_GLOBAL		0x00
#define WL_NAN_CMD_CFG_COMP_ID		0x01
#define WL_NAN_CMD_ELECTION_COMP_ID	0x02
#define WL_NAN_CMD_SD_COMP_ID		0x03
#define WL_NAN_CMD_SYNC_COMP_ID		0x04
#define WL_NAN_CMD_DATA_COMP_ID		0x05
#define WL_NAN_CMD_DAM_COMP_ID		0x06
#define WL_NAN_CMD_RANGE_COMP_ID	0x07
#define WL_NAN_CMD_GENERIC_COMP_ID	0x08
#define WL_NAN_CMD_SCHED_COMP_ID	0x09
#define WL_NAN_CMD_NSR_COMP_ID		0x0a /* NAN Save Restore */
#define WL_NAN_CMD_NANHO_COMP_ID	0x0b /* NAN Host offload */
#define WL_NAN_CMD_DBG_COMP_ID		0x0f
#define WL_NAN_CMD_PAIRING_COMP_ID	0x10
#define WL_NAN_CMD_PAIRING_PEER_COMP_ID	0x11
#define WL_NAN_CMD_TETHER_COMP_ID	0x12
#define WL_NAN_CMD_SNAP_LLC_CFG_COMP_ID	0x13


#define WL_NAN_CMD_COMP_SHIFT		8
#define NAN_CMD(x, y)  (((x) << WL_NAN_CMD_COMP_SHIFT) | (y))

/*
 * Module based NAN TLV IDs
 */
typedef enum wl_nan_tlv {

	WL_NAN_XTLV_CFG_MATCH_RX	= NAN_CMD(WL_NAN_CMD_CFG_COMP_ID, 0x01),
	WL_NAN_XTLV_CFG_MATCH_TX	= NAN_CMD(WL_NAN_CMD_CFG_COMP_ID, 0x02),
	WL_NAN_XTLV_CFG_SR_FILTER	= NAN_CMD(WL_NAN_CMD_CFG_COMP_ID, 0x03),
	WL_NAN_XTLV_CFG_SVC_NAME	= NAN_CMD(WL_NAN_CMD_CFG_COMP_ID, 0x04),
	WL_NAN_XTLV_CFG_NAN_STATUS	= NAN_CMD(WL_NAN_CMD_CFG_COMP_ID, 0x05),
	WL_NAN_XTLV_CFG_SVC_LIFE_COUNT	= NAN_CMD(WL_NAN_CMD_CFG_COMP_ID, 0x06),
	WL_NAN_XTLV_CFG_SVC_HASH	= NAN_CMD(WL_NAN_CMD_CFG_COMP_ID, 0x07),
	WL_NAN_XTLV_CFG_SEC_CSID	= NAN_CMD(WL_NAN_CMD_CFG_COMP_ID, 0x08), /* Security CSID */
	WL_NAN_XTLV_CFG_SEC_PMK		= NAN_CMD(WL_NAN_CMD_CFG_COMP_ID, 0x09), /* Security PMK */
	WL_NAN_XTLV_CFG_SEC_PMKID	= NAN_CMD(WL_NAN_CMD_CFG_COMP_ID, 0x0A),
	WL_NAN_XTLV_CFG_SEC_SCID	= NAN_CMD(WL_NAN_CMD_CFG_COMP_ID, 0x0B),
	WL_NAN_XTLV_CFG_VNDR_PAYLOAD	= NAN_CMD(WL_NAN_CMD_CFG_COMP_ID, 0x0C),
	WL_NAN_XTLV_CFG_HOST_INDPID	= NAN_CMD(WL_NAN_CMD_CFG_COMP_ID, 0x0D),
	/* when host ndpid is used */
	WL_NAN_XTLV_CFG_MAC_ADDR	= NAN_CMD(WL_NAN_CMD_CFG_COMP_ID, 0x0E),
	/* fast disc time bitmap config */
	WL_NAN_XTLV_CFG_FDISC_TBMP	= NAN_CMD(WL_NAN_CMD_CFG_COMP_ID, 0x0F),
	WL_NAN_XTLV_CFG_SEC_GTK_CSID    = NAN_CMD(WL_NAN_CMD_CFG_COMP_ID, 0x10),
	WL_NAN_XTLV_CFG_NO_PEER_CACHE	= NAN_CMD(WL_NAN_CMD_CFG_COMP_ID, 0x20),

	/* NAN R4, host constucted NPBA (NAN Pairing Bootstrapping Attribute), the entire
	 * NPBA attibute is in bcm_xlv_t:
	 * len:  contains the NABA attribute length
	 * data: contains the NABA attrbute data, including id.
	 * The Host exchanges this attribute during the
	 * NAN pairing and bootstrapping in SDF Follow-ups and SDF Publish.
	 */
	WL_NAN_XTLV_CFG_NPBA_INFO	= NAN_CMD(WL_NAN_CMD_CFG_COMP_ID, 0x11),
	/* multicast data slot bitmap config */
	WL_NAN_XTLV_CFG_MCAST_AVAIL_BMP	= NAN_CMD(WL_NAN_CMD_CFG_COMP_ID, 0x12),

	WL_NAN_XTLV_SD_SVC_INFO		= NAN_CMD(WL_NAN_CMD_SD_COMP_ID, 0x01),
	WL_NAN_XTLV_SD_FOLLOWUP		= NAN_CMD(WL_NAN_CMD_SD_COMP_ID, 0x02),
	WL_NAN_XTLV_SD_SDF_RX		= NAN_CMD(WL_NAN_CMD_SD_COMP_ID, 0x03),
	WL_NAN_XTLV_SD_SDE_CONTROL	= NAN_CMD(WL_NAN_CMD_SD_COMP_ID, 0x04),
	WL_NAN_XTLV_SD_SDE_RANGE_LIMIT	= NAN_CMD(WL_NAN_CMD_SD_COMP_ID, 0x05),
	WL_NAN_XTLV_SD_NAN_AF		= NAN_CMD(WL_NAN_CMD_SD_COMP_ID, 0x06),
	WL_NAN_XTLV_SD_TERM		= NAN_CMD(WL_NAN_CMD_SD_COMP_ID, 0x07),
	WL_NAN_XTLV_SD_REPLIED		= NAN_CMD(WL_NAN_CMD_SD_COMP_ID, 0x08), /* Pub sent */
	WL_NAN_XTLV_SD_FUP_RECEIVED	= NAN_CMD(WL_NAN_CMD_SD_COMP_ID, 0x09), /* FUP Received */
	WL_NAN_XTLV_SD_DISC_RESULTS	= NAN_CMD(WL_NAN_CMD_SD_COMP_ID, 0x0A), /* Pub RX */
	WL_NAN_XTLV_SD_TXS		= NAN_CMD(WL_NAN_CMD_SD_COMP_ID, 0x0B), /* Tx status */
	WL_NAN_XTLV_SD_SDE_SVC_INFO	= NAN_CMD(WL_NAN_CMD_SD_COMP_ID, 0x0C),
	WL_NAN_XTLV_SD_SDE_SVC_UPD_IND	= NAN_CMD(WL_NAN_CMD_SD_COMP_ID, 0x0D),
	WL_NAN_XTLV_SD_SVC_NDI		= NAN_CMD(WL_NAN_CMD_SD_COMP_ID, 0x0E),
	WL_NAN_XTLV_SD_NDP_SPEC_INFO	= NAN_CMD(WL_NAN_CMD_SD_COMP_ID, 0x0F),
	WL_NAN_XTLV_SD_NDPE_TLV_LIST	= NAN_CMD(WL_NAN_CMD_SD_COMP_ID, 0x10),
	WL_NAN_XTLV_SD_NDL_QOS_UPD	= NAN_CMD(WL_NAN_CMD_SD_COMP_ID, 0x11),
	WL_NAN_XTLV_SD_DISC_CACHE_TIMEOUT	= NAN_CMD(WL_NAN_CMD_SD_COMP_ID, 0x12),
	WL_NAN_XTLV_SD_PEER_NMI		= NAN_CMD(WL_NAN_CMD_SD_COMP_ID, 0x13),
	WL_NAN_XTLV_SD_FUP_UNSYNC_CHANSPEC = NAN_CMD(WL_NAN_CMD_SD_COMP_ID, 0x14),
	WL_NAN_XTLV_SD_CHANSPEC		= NAN_CMD(WL_NAN_CMD_SD_COMP_ID, 0x15),
	WL_NAN_XTLV_SD_INSTANCE_ID	= NAN_CMD(WL_NAN_CMD_SD_COMP_ID, 0x16),
	WL_NAN_XTLV_SD_SVC_SUSPEND_STATUS	= NAN_CMD(WL_NAN_CMD_SD_COMP_ID, 0x17),

	WL_NAN_XTLV_SYNC_BCN_RX		= NAN_CMD(WL_NAN_CMD_SYNC_COMP_ID, 0x01),
	WL_NAN_XTLV_EV_MR_CHANGED	= NAN_CMD(WL_NAN_CMD_SYNC_COMP_ID, 0x02),

	WL_NAN_XTLV_DATA_DP_END		= NAN_CMD(WL_NAN_CMD_DATA_COMP_ID, 0x01),
	WL_NAN_XTLV_DATA_DP_INFO	= NAN_CMD(WL_NAN_CMD_DATA_COMP_ID, 0x02),
	WL_NAN_XTLV_DATA_DP_SEC_INST	= NAN_CMD(WL_NAN_CMD_DATA_COMP_ID, 0x03),
	WL_NAN_XTLV_DATA_DP_TXS		= NAN_CMD(WL_NAN_CMD_DATA_COMP_ID, 0x04), /* txs for dp */
	WL_NAN_XTLV_DATA_DP_OPAQUE_INFO	 = NAN_CMD(WL_NAN_CMD_DATA_COMP_ID, 0x05),
	WL_NAN_XTLV_RANGE_INFO		= NAN_CMD(WL_NAN_CMD_RANGE_COMP_ID, 0x01),
	WL_NAN_XTLV_RNG_TXS		= NAN_CMD(WL_NAN_CMD_RANGE_COMP_ID, 0x02),

	WL_NAN_XTLV_EV_SLOT_INFO	= NAN_CMD(WL_NAN_CMD_DBG_COMP_ID, 0x01),
	WL_NAN_XTLV_EV_GEN_INFO         = NAN_CMD(WL_NAN_CMD_DBG_COMP_ID, 0x02),
	WL_NAN_XTLV_CCA_STATS		= NAN_CMD(WL_NAN_CMD_DBG_COMP_ID, 0x03),
	WL_NAN_XTLV_PER_STATS		= NAN_CMD(WL_NAN_CMD_DBG_COMP_ID, 0x04),
	WL_NAN_XTLV_CHBOUND_INFO	= NAN_CMD(WL_NAN_CMD_DBG_COMP_ID, 0x05),
	WL_NAN_XTLV_SLOT_STATS		= NAN_CMD(WL_NAN_CMD_DBG_COMP_ID, 0x06),

	WL_NAN_XTLV_DAM_NA_ATTR		= NAN_CMD(WL_NAN_CMD_DAM_COMP_ID, 0x01), /* na attr */
	WL_NAN_XTLV_HOST_ASSIST_REQ	= NAN_CMD(WL_NAN_CMD_DAM_COMP_ID, 0x02), /* host assist */

	/* wl_nan_fw_cap_t */
	WL_NAN_XTLV_GEN_FW_CAP		= NAN_CMD(WL_NAN_CMD_GENERIC_COMP_ID, 0x01),
	/* wl_nan_fw_cap_v2_t */
	WL_NAN_XTLV_GEN_FW_CAP_V2	= NAN_CMD(WL_NAN_CMD_GENERIC_COMP_ID, 0x02),

	/* NAN OOB AF tlv */
	WL_NAN_XTLV_OOB_AF		= NAN_CMD(WL_NAN_CMD_GENERIC_COMP_ID, 0x03),
	/* Unencrypted AF Rx xtlv */
	WL_NAN_XTLV_UNENC_AF_RX		= NAN_CMD(WL_NAN_CMD_GENERIC_COMP_ID, 0x04),

	WL_NAN_XTLV_SCHED_INFO		= NAN_CMD(WL_NAN_CMD_SCHED_COMP_ID, 0x01),
	WL_NAN_XTLV_NDL_SCHED_INFO	= NAN_CMD(WL_NAN_CMD_SCHED_COMP_ID, 0x02),

	/* Nan Save-Restore XTLVs */
	WL_NAN_XTLV_NSR2_PEER		= NAN_CMD(WL_NAN_CMD_NSR_COMP_ID, 0x21),
	WL_NAN_XTLV_NSR2_NDP		= NAN_CMD(WL_NAN_CMD_NSR_COMP_ID, 0x22),

	/* Host offload XTLVs */
	WL_NAN_XTLV_NANHO_PEER_ENTRY	= NAN_CMD(WL_NAN_CMD_NANHO_COMP_ID, 0x01),
	WL_NAN_XTLV_NANHO_DCAPLIST	= NAN_CMD(WL_NAN_CMD_NANHO_COMP_ID, 0x02),
	WL_NAN_XTLV_NANHO_DCSLIST	= NAN_CMD(WL_NAN_CMD_NANHO_COMP_ID, 0x03),
	WL_NAN_XTLV_NANHO_BLOB		= NAN_CMD(WL_NAN_CMD_NANHO_COMP_ID, 0x04),
	WL_NAN_XTLV_NANHO_NDP_STATE	= NAN_CMD(WL_NAN_CMD_NANHO_COMP_ID, 0x05),
	WL_NAN_XTLV_NANHO_FRM_TPLT	= NAN_CMD(WL_NAN_CMD_NANHO_COMP_ID, 0x06),
	WL_NAN_XTLV_NANHO_OOB_NAF	= NAN_CMD(WL_NAN_CMD_NANHO_COMP_ID, 0x07),
	WL_NAN_XTLV_NANHO_LOG_ERR_CTRL	= NAN_CMD(WL_NAN_CMD_NANHO_COMP_ID, 0x08),
	WL_NAN_XTLV_NANHO_LOG_DBG_CTRL	= NAN_CMD(WL_NAN_CMD_NANHO_COMP_ID, 0x09),
	WL_NAN_XTLV_NANHO_OOB_TXS	= NAN_CMD(WL_NAN_CMD_NANHO_COMP_ID, 0x0A),
	WL_NAN_XTLV_NANHO_DCAP_ATTR	= NAN_CMD(WL_NAN_CMD_NANHO_COMP_ID, 0x0B),
	WL_NAN_XTLV_NANHO_ELEM_ATTR	= NAN_CMD(WL_NAN_CMD_NANHO_COMP_ID, 0x0C),
	WL_NAN_XTLV_NANHO_SEC_SA	= NAN_CMD(WL_NAN_CMD_NANHO_COMP_ID, 0x0D),

	/* NAN pairing XTLVs */
	WL_NAN_XTLV_PAIRING_PASSWORD	= NAN_CMD(WL_NAN_CMD_PAIRING_COMP_ID, 0x01),

	/* Caching NPK */
	WL_NAN_XTLV_PAIRING_CACHING_NPK	= NAN_CMD(WL_NAN_CMD_PAIRING_COMP_ID, 0x02),

	/* Local NIK */
	WL_NAN_XTLV_PAIRING_LOCAL_NIK	= NAN_CMD(WL_NAN_CMD_PAIRING_COMP_ID, 0x03),

	/* Local NIRA */
	WL_NAN_XTLV_PAIRING_LOCAL_NIRA	= NAN_CMD(WL_NAN_CMD_PAIRING_COMP_ID, 0x04),

	/* Peer NIK */
	WL_NAN_XTLV_PAIRING_PEER_NIK	= NAN_CMD(WL_NAN_CMD_PAIRING_COMP_ID, 0x05),

	/* XTLV list to include NPK, local NIK and peer NIK */
	WL_NAN_XTLV_PAIRING_XTLV_LIST	= NAN_CMD(WL_NAN_CMD_PAIRING_COMP_ID, 0x06),

	/* Pairing session ID */
	WL_NAN_XTLV_PAIRING_SID = NAN_CMD(WL_NAN_CMD_PAIRING_COMP_ID, 0x07),

	/* Pairing flags */
	WL_NAN_XTLV_PAIRING_FLAGS = NAN_CMD(WL_NAN_CMD_PAIRING_COMP_ID, 0x08),

	/* Pairing peer nonce */
	WL_NAN_XTLV_PAIRING_PEER_NONCE = NAN_CMD(WL_NAN_CMD_PAIRING_COMP_ID, 0x09),

	/* Pairing peer tag */
	WL_NAN_XTLV_PAIRING_PEER_TAG = NAN_CMD(WL_NAN_CMD_PAIRING_COMP_ID, 0x0A),

	/* Pairing PASN policy */
	WL_NAN_XTLV_PAIRING_PASN_POLICY = NAN_CMD(WL_NAN_CMD_PAIRING_COMP_ID, 0x0B),

	/* Pubisher instance ID for pairing */
	WL_NAN_XTLV_PAIRING_PUB_ID = NAN_CMD(WL_NAN_CMD_PAIRING_COMP_ID, 0x0C),

	/* Pairing_cfg to share pairing setup, verification and cache cfg from Host to FW */
	WL_NAN_XTLV_PAIRING_CFG = NAN_CMD(WL_NAN_CMD_PAIRING_COMP_ID, 0x0D),

	/* Tether xtlv */
	WL_NAN_XTLV_TETHER		= NAN_CMD(WL_NAN_CMD_TETHER_COMP_ID, 0x01),

	/* NAN snap xtlv sub commands - xtlv_list, rx_data, tx_data */
	WL_NAN_XTLV_SNAP_LLC_XTLV_LIST	= NAN_CMD(WL_NAN_CMD_SNAP_LLC_CFG_COMP_ID, 0x01),
	WL_NAN_XTLV_SNAP_RX_DATA	= NAN_CMD(WL_NAN_CMD_SNAP_LLC_CFG_COMP_ID, 0x02),
	WL_NAN_XTLV_SNAP_TX_DATA	= NAN_CMD(WL_NAN_CMD_SNAP_LLC_CFG_COMP_ID, 0x03)

} wl_nan_tlv_t;

/* Sub Module ID's for NAN */
enum {
	NAN_MAC =		0,	/* nan mac */
	NAN_DISC =		1,	/* nan discovery */
	NAN_DBG =		2,	/* nan debug */
	NAN_SCHED =		3,	/* nan sched */
	NAN_PEER_ENTRY =	4,	/* nan peer entry */
	NAN_AVAIL =		5,	/* nan avail */
	NAN_DAM =		6,	/* nan dam */
	NAN_FSM =		7,	/* nan fsm registry */
	NAN_NDP =		8,	/* nan ndp */
	NAN_NDL =		9,	/* nan ndl */
	NAN_DP =		10,	/* nan dp core */
	NAN_RNG =		11,	/* nan ranging */
	NAN_SEC =		12,	/* nan sec */
/*
 * Need to make sure NAN branches update their nan_init tables before
 * removing this check. Otherwise it causes odt/smoketest failures
 */
#ifdef WL_NAN_PAIRING
	NAN_PAIRING =	13,	/* nan pairing */
	NAN_LAST =	14
#else
	NAN_LAST =	13
#endif /* WL_NAN_PAIRING */
};

enum wl_nan_sub_cmd_xtlv_id {

	/* Special command - Tag zero */
	WL_NAN_CMD_GLB_NAN_VER = NAN_CMD(WL_NAN_CMD_GLOBAL, 0x00),

	 /* nan cfg sub-commands */

	WL_NAN_CMD_CFG_NAN_INIT = NAN_CMD(WL_NAN_CMD_CFG_COMP_ID, 0x01),
	WL_NAN_CMD_CFG_ROLE = NAN_CMD(WL_NAN_CMD_CFG_COMP_ID, 0x02),
	WL_NAN_CMD_CFG_HOP_CNT = NAN_CMD(WL_NAN_CMD_CFG_COMP_ID, 0x03),
	WL_NAN_CMD_CFG_HOP_LIMIT = NAN_CMD(WL_NAN_CMD_CFG_COMP_ID, 0x04),
	WL_NAN_CMD_CFG_WARMUP_TIME = NAN_CMD(WL_NAN_CMD_CFG_COMP_ID, 0x05),
	WL_NAN_CMD_CFG_STATUS = NAN_CMD(WL_NAN_CMD_CFG_COMP_ID, 0x06),
	WL_NAN_CMD_CFG_OUI = NAN_CMD(WL_NAN_CMD_CFG_COMP_ID, 0x07),
	WL_NAN_CMD_CFG_COUNT = NAN_CMD(WL_NAN_CMD_CFG_COMP_ID, 0x08),
	WL_NAN_CMD_CFG_CLEARCOUNT = NAN_CMD(WL_NAN_CMD_CFG_COMP_ID, 0x09),
	WL_NAN_CMD_CFG_CHANNEL = NAN_CMD(WL_NAN_CMD_CFG_COMP_ID, 0x0A),
	WL_NAN_CMD_CFG_BAND = NAN_CMD(WL_NAN_CMD_CFG_COMP_ID, 0x0B),
	WL_NAN_CMD_CFG_CID = NAN_CMD(WL_NAN_CMD_CFG_COMP_ID, 0x0C),
	WL_NAN_CMD_CFG_IF_ADDR = NAN_CMD(WL_NAN_CMD_CFG_COMP_ID, 0x0D),
	WL_NAN_CMD_CFG_BCN_INTERVAL = NAN_CMD(WL_NAN_CMD_CFG_COMP_ID, 0x0E),
	WL_NAN_CMD_CFG_SDF_TXTIME = NAN_CMD(WL_NAN_CMD_CFG_COMP_ID, 0x0F),
	WL_NAN_CMD_CFG_SID_BEACON = NAN_CMD(WL_NAN_CMD_CFG_COMP_ID, 0x10),
	WL_NAN_CMD_CFG_DW_LEN = NAN_CMD(WL_NAN_CMD_CFG_COMP_ID, 0x11),
	WL_NAN_CMD_CFG_AVAIL = NAN_CMD(WL_NAN_CMD_CFG_COMP_ID, 0x12),
	WL_NAN_CMD_CFG_WFA_TM = NAN_CMD(WL_NAN_CMD_CFG_COMP_ID, 0x13),
	WL_NAN_CMD_CFG_EVENT_MASK =  NAN_CMD(WL_NAN_CMD_CFG_COMP_ID, 0x14),
	WL_NAN_CMD_CFG_NAN_CONFIG = NAN_CMD(WL_NAN_CMD_CFG_COMP_ID, 0x15), /* ctrl */
	WL_NAN_CMD_CFG_NAN_ENAB = NAN_CMD(WL_NAN_CMD_CFG_COMP_ID, 0x16),
	WL_NAN_CMD_CFG_ULW = NAN_CMD(WL_NAN_CMD_CFG_COMP_ID, 0x17),
	WL_NAN_CMD_CFG_NAN_CONFIG2 = NAN_CMD(WL_NAN_CMD_CFG_COMP_ID, 0x18), /* ctrl2 */
	WL_NAN_CMD_CFG_DEV_CAP = NAN_CMD(WL_NAN_CMD_CFG_COMP_ID, 0x19),
	WL_NAN_CMD_CFG_SCAN_PARAMS = NAN_CMD(WL_NAN_CMD_CFG_COMP_ID, 0x1A),
	WL_NAN_CMD_CFG_VNDR_PAYLOAD = NAN_CMD(WL_NAN_CMD_CFG_COMP_ID, 0x1B),
	WL_NAN_CMD_CFG_FASTDISC = NAN_CMD(WL_NAN_CMD_CFG_COMP_ID, 0x1C),
	WL_NAN_CMD_CFG_MIN_TX_RATE = NAN_CMD(WL_NAN_CMD_CFG_COMP_ID, 0x1D),
	WL_NAN_CMD_CFG_FSM_TIMEOUT = NAN_CMD(WL_NAN_CMD_CFG_COMP_ID, 0x1E),
	WL_NAN_CMD_CFG_REKEY = NAN_CMD(WL_NAN_CMD_CFG_COMP_ID, 0x1F), /* REKEY */
	WL_NAN_CMD_CFG_INSTANT_CHAN = NAN_CMD(WL_NAN_CMD_CFG_COMP_ID, 0x20),
	WL_NAN_CMD_CFG_TSF = NAN_CMD(WL_NAN_CMD_CFG_COMP_ID, 0x21),
	WL_NAN_CMD_CFG_MCAST_AVAIL = NAN_CMD(WL_NAN_CMD_CFG_COMP_ID, 0x22),
	WL_NAN_CMD_CFG_DW_DWELL = NAN_CMD(WL_NAN_CMD_CFG_COMP_ID, 0x23),
	WL_NAN_CMD_CFG_SNAP_HDR = NAN_CMD(WL_NAN_CMD_CFG_COMP_ID, 0x24),
	WL_NAN_CMD_CFG_MAX = WL_NAN_CMD_CFG_SNAP_HDR,

	/* Add new commands before and update */

	/* nan election sub-commands */
	WL_NAN_CMD_ELECTION_HOST_ENABLE = NAN_CMD(WL_NAN_CMD_ELECTION_COMP_ID, 0x01),
	WL_NAN_CMD_ELECTION_METRICS_CONFIG = NAN_CMD(WL_NAN_CMD_ELECTION_COMP_ID, 0x02),
	WL_NAN_CMD_ELECTION_METRICS_STATE = NAN_CMD(WL_NAN_CMD_ELECTION_COMP_ID, 0x03),
	WL_NAN_CMD_ELECTION_LEAVE = NAN_CMD(WL_NAN_CMD_ELECTION_COMP_ID, 0x03),
	WL_NAN_CMD_ELECTION_MERGE = NAN_CMD(WL_NAN_CMD_ELECTION_COMP_ID, 0x04),
	WL_NAN_CMD_ELECTION_ADVERTISERS = NAN_CMD(WL_NAN_CMD_ELECTION_COMP_ID, 0x05),
	WL_NAN_CMD_ELECTION_RSSI_THRESHOLD = NAN_CMD(WL_NAN_CMD_ELECTION_COMP_ID, 0x06),
	WL_NAN_CMD_ELECTION_MAX = WL_NAN_CMD_ELECTION_RSSI_THRESHOLD,
	/* New commands go before and update */

	/* nan SD sub-commands */
	WL_NAN_CMD_SD_PARAMS = NAN_CMD(WL_NAN_CMD_SD_COMP_ID, 0x01),
	WL_NAN_CMD_SD_PUBLISH = NAN_CMD(WL_NAN_CMD_SD_COMP_ID, 0x02),
	WL_NAN_CMD_SD_PUBLISH_LIST = NAN_CMD(WL_NAN_CMD_SD_COMP_ID, 0x03),
	WL_NAN_CMD_SD_CANCEL_PUBLISH = NAN_CMD(WL_NAN_CMD_SD_COMP_ID, 0x04),
	WL_NAN_CMD_SD_SUBSCRIBE = NAN_CMD(WL_NAN_CMD_SD_COMP_ID, 0x05),
	WL_NAN_CMD_SD_SUBSCRIBE_LIST = NAN_CMD(WL_NAN_CMD_SD_COMP_ID, 0x06),
	WL_NAN_CMD_SD_CANCEL_SUBSCRIBE = NAN_CMD(WL_NAN_CMD_SD_COMP_ID, 0x07),
	WL_NAN_CMD_SD_VND_INFO = NAN_CMD(WL_NAN_CMD_SD_COMP_ID, 0x08),
	WL_NAN_CMD_SD_STATS = NAN_CMD(WL_NAN_CMD_SD_COMP_ID, 0x09),
	WL_NAN_CMD_SD_TRANSMIT = NAN_CMD(WL_NAN_CMD_SD_COMP_ID, 0x0A),
	WL_NAN_CMD_SD_FUP_TRANSMIT = NAN_CMD(WL_NAN_CMD_SD_COMP_ID, 0x0B),
	WL_NAN_CMD_SD_CONNECTION = NAN_CMD(WL_NAN_CMD_SD_COMP_ID, 0x0C),
	WL_NAN_CMD_SD_SHOW = NAN_CMD(WL_NAN_CMD_SD_COMP_ID, 0x0D),
	WL_NAN_CMD_SD_DISC_CACHE_TIMEOUT = NAN_CMD(WL_NAN_CMD_SD_COMP_ID, 0x0E),
	WL_NAN_CMD_SD_DISC_CACHE_CLEAR = NAN_CMD(WL_NAN_CMD_SD_COMP_ID, 0x0F),
	WL_NAN_CMD_SD_SUSPEND_RESUME = NAN_CMD(WL_NAN_CMD_SD_COMP_ID, 0x10),
	WL_NAN_CMD_SD_MAX = WL_NAN_CMD_SD_SUSPEND_RESUME,

	/* nan time sync sub-commands */

	WL_NAN_CMD_SYNC_SOCIAL_CHAN = NAN_CMD(WL_NAN_CMD_SYNC_COMP_ID, 0x01),
	WL_NAN_CMD_SYNC_AWAKE_DWS = NAN_CMD(WL_NAN_CMD_SYNC_COMP_ID, 0x02),
	WL_NAN_CMD_SYNC_BCN_RSSI_NOTIF_THRESHOLD = NAN_CMD(WL_NAN_CMD_SYNC_COMP_ID, 0x03),
	WL_NAN_CMD_SYNC_MAX = WL_NAN_CMD_SYNC_BCN_RSSI_NOTIF_THRESHOLD,

	/* nan2 commands */
	WL_NAN_CMD_DATA_CONFIG = NAN_CMD(WL_NAN_CMD_DATA_COMP_ID, 0x01),
	WL_NAN_CMD_DATA_RSVD02 = NAN_CMD(WL_NAN_CMD_DATA_COMP_ID, 0x02),
	WL_NAN_CMD_DATA_RSVD03 = NAN_CMD(WL_NAN_CMD_DATA_COMP_ID, 0x03),
	WL_NAN_CMD_DATA_DATAREQ = NAN_CMD(WL_NAN_CMD_DATA_COMP_ID, 0x04),
	WL_NAN_CMD_DATA_DATARESP = NAN_CMD(WL_NAN_CMD_DATA_COMP_ID, 0x05),
	WL_NAN_CMD_DATA_DATAEND = NAN_CMD(WL_NAN_CMD_DATA_COMP_ID, 0x06),
	WL_NAN_CMD_DATA_SCHEDUPD = NAN_CMD(WL_NAN_CMD_DATA_COMP_ID, 0x07),
	WL_NAN_CMD_DATA_RSVD08 = NAN_CMD(WL_NAN_CMD_DATA_COMP_ID, 0x08),
	WL_NAN_CMD_DATA_CAP = NAN_CMD(WL_NAN_CMD_DATA_COMP_ID, 0x9),
	WL_NAN_CMD_DATA_STATUS = NAN_CMD(WL_NAN_CMD_DATA_COMP_ID, 0x0A),
	WL_NAN_CMD_DATA_STATS = NAN_CMD(WL_NAN_CMD_DATA_COMP_ID, 0x0B),
	WL_NAN_CMD_DATA_RSVD0C = NAN_CMD(WL_NAN_CMD_DATA_COMP_ID, 0x0C),
	WL_NAN_CMD_DATA_NDP_SHOW = NAN_CMD(WL_NAN_CMD_DATA_COMP_ID, 0x0D),
	WL_NAN_CMD_DATA_DATACONF = NAN_CMD(WL_NAN_CMD_DATA_COMP_ID, 0x0E),
	WL_NAN_CMD_DATA_MIN_TX_RATE = NAN_CMD(WL_NAN_CMD_DATA_COMP_ID, 0x0F),
	WL_NAN_CMD_DATA_MAX_PEERS = NAN_CMD(WL_NAN_CMD_DATA_COMP_ID, 0x10),
	WL_NAN_CMD_DATA_DP_IDLE_PERIOD = NAN_CMD(WL_NAN_CMD_DATA_COMP_ID, 0x11),
	WL_NAN_CMD_DATA_DP_OPAQUE_INFO = NAN_CMD(WL_NAN_CMD_DATA_COMP_ID, 0x12),
	WL_NAN_CMD_DATA_DP_HB_DURATION = NAN_CMD(WL_NAN_CMD_DATA_COMP_ID, 0x13),
	WL_NAN_CMD_DATA_DATAEND_V2 = NAN_CMD(WL_NAN_CMD_DATA_COMP_ID, 0x14),
	WL_NAN_CMD_DATA_PATH_MAX = WL_NAN_CMD_DATA_DATAEND_V2, /* New ones before and update */

	/* nan dam sub-commands */
	WL_NAN_CMD_DAM_CFG = NAN_CMD(WL_NAN_CMD_DAM_COMP_ID, 0x01),
	WL_NAN_CMD_DAM_MAX = WL_NAN_CMD_DAM_CFG,  /* New ones before and update */

	/* nan2.0 ranging commands */
	WL_NAN_CMD_RANGE_REQUEST = NAN_CMD(WL_NAN_CMD_RANGE_COMP_ID, 0x01),
	WL_NAN_CMD_RANGE_AUTO = NAN_CMD(WL_NAN_CMD_RANGE_COMP_ID, 0x02),
	WL_NAN_CMD_RANGE_RESPONSE = NAN_CMD(WL_NAN_CMD_RANGE_COMP_ID, 0x03),
	WL_NAN_CMD_RANGE_CANCEL = NAN_CMD(WL_NAN_CMD_RANGE_COMP_ID, 0x04),
	WL_NAN_CMD_RANGE_IDLE_COUNT = NAN_CMD(WL_NAN_CMD_RANGE_COMP_ID, 0x05),
	WL_NAN_CMD_RANGE_CANCEL_EXT = NAN_CMD(WL_NAN_CMD_RANGE_COMP_ID, 0x06),

	/*  nan debug sub-commands  */
	WL_NAN_CMD_DBG_SCAN_PARAMS = NAN_CMD(WL_NAN_CMD_DBG_COMP_ID, 0x01),
	WL_NAN_CMD_DBG_SCAN = NAN_CMD(WL_NAN_CMD_DBG_COMP_ID, 0x02),
	WL_NAN_CMD_DBG_SCAN_RESULTS =  NAN_CMD(WL_NAN_CMD_DBG_COMP_ID, 0x03),
	/* This is now moved under CFG */
	WL_NAN_CMD_DBG_EVENT_MASK =  NAN_CMD(WL_NAN_CMD_DBG_COMP_ID, 0x04),
	WL_NAN_CMD_DBG_EVENT_CHECK =  NAN_CMD(WL_NAN_CMD_DBG_COMP_ID, 0x05),
	WL_NAN_CMD_DBG_DUMP =  NAN_CMD(WL_NAN_CMD_DBG_COMP_ID, 0x06),
	WL_NAN_CMD_DBG_CLEAR =  NAN_CMD(WL_NAN_CMD_DBG_COMP_ID, 0x07),
	WL_NAN_CMD_DBG_RSSI =  NAN_CMD(WL_NAN_CMD_DBG_COMP_ID, 0x08),
	WL_NAN_CMD_DBG_DEBUG =  NAN_CMD(WL_NAN_CMD_DBG_COMP_ID, 0x09),
	WL_NAN_CMD_DBG_TEST1 =  NAN_CMD(WL_NAN_CMD_DBG_COMP_ID, 0x0A),
	WL_NAN_CMD_DBG_TEST2 =  NAN_CMD(WL_NAN_CMD_DBG_COMP_ID, 0x0B),
	WL_NAN_CMD_DBG_TEST3 =  NAN_CMD(WL_NAN_CMD_DBG_COMP_ID, 0x0C),
	WL_NAN_CMD_DBG_DISC_RESULTS =  NAN_CMD(WL_NAN_CMD_DBG_COMP_ID, 0x0D),
	WL_NAN_CMD_DBG_STATS =  NAN_CMD(WL_NAN_CMD_DBG_COMP_ID, 0x0E),
	WL_NAN_CMD_DBG_LEVEL =  NAN_CMD(WL_NAN_CMD_DBG_COMP_ID, 0x0F),
	WL_NAN_CMD_DBG_MAX = WL_NAN_CMD_DBG_LEVEL, /* New ones before and update */

	 /* Generic componenet */
	WL_NAN_CMD_GEN_STATS = NAN_CMD(WL_NAN_CMD_GENERIC_COMP_ID, 0x01),
	WL_NAN_CMD_GEN_FW_CAP = NAN_CMD(WL_NAN_CMD_GENERIC_COMP_ID, 0x02),
	WL_NAN_CMD_CFG_OOB_AF = NAN_CMD(WL_NAN_CMD_GENERIC_COMP_ID, 0x03),
	WL_NAN_CMD_GEN_MAX = WL_NAN_CMD_CFG_OOB_AF,

	/* NAN Save-Restore */
	WL_NAN_CMD_NSR2 = NAN_CMD(WL_NAN_CMD_NSR_COMP_ID, 0x20),
	WL_NAN_CMD_NSR2_MAX = WL_NAN_CMD_NSR2,

	/* Host offload sub-commands */
	WL_NAN_CMD_NANHO_UPDATE = NAN_CMD(WL_NAN_CMD_NANHO_COMP_ID, 0x01), /* WILL BE REMOVED */
	WL_NAN_CMD_NANHO_INFO = NAN_CMD(WL_NAN_CMD_NANHO_COMP_ID, 0x01),
	WL_NAN_CMD_NANHO_FRM_TPLT = NAN_CMD(WL_NAN_CMD_NANHO_COMP_ID, 0x02),	/* unused */
	WL_NAN_CMD_NANHO_OOB_NAF = NAN_CMD(WL_NAN_CMD_NANHO_COMP_ID, 0x03),	/* unused */
	WL_NAN_CMD_NANHO_LOG_CTRL = NAN_CMD(WL_NAN_CMD_NANHO_COMP_ID, 0x04),
	WL_NAN_CMD_NANHO_VER = NAN_CMD(WL_NAN_CMD_NANHO_COMP_ID, 0x05),
	WL_NAN_CMD_NANHO_MAX = WL_NAN_CMD_NANHO_VER,

	/* NAN pairing sub-commands */
	WL_NAN_CMD_PAIRING = NAN_CMD(WL_NAN_CMD_PAIRING_COMP_ID, 0x01),
	WL_NAN_CMD_PAIRING_KEY_INFO = NAN_CMD(WL_NAN_CMD_PAIRING_COMP_ID, 0x02),
	WL_NAN_CMD_PAIRING_PEER_INFO = NAN_CMD(WL_NAN_CMD_PAIRING_COMP_ID, 0x03),
	WL_NAN_CMD_PAIRING_END = NAN_CMD(WL_NAN_CMD_PAIRING_COMP_ID, 0x04),
	WL_NAN_CMD_PAIRING_MAX = WL_NAN_CMD_PAIRING_END,

	/* Add submodules below, and update WL_NAN_CMD_MAX */

	/* ROML check for this enum should use WL_NAN_CMD_MAX */
	WL_NAN_CMD_MAX = WL_NAN_CMD_PAIRING_MAX
};


/*
 * Component/Module based NAN TLV IDs for NAN stats
 */
typedef enum wl_nan_stats_tlv {
	WL_NAN_XTLV_SYNC_MAC_STATS		= NAN_CMD(WL_NAN_CMD_SYNC_COMP_ID, 0x01),

	WL_NAN_XTLV_SD_DISC_STATS		= NAN_CMD(WL_NAN_CMD_SD_COMP_ID, 0x01),

	WL_NAN_XTLV_DATA_NDP_STATS		= NAN_CMD(WL_NAN_CMD_DATA_COMP_ID, 0x01),
	WL_NAN_XTLV_DATA_NDL_STATS		= NAN_CMD(WL_NAN_CMD_DATA_COMP_ID, 0x02),
	WL_NAN_XTLV_DATA_SEC_STATS		= NAN_CMD(WL_NAN_CMD_DATA_COMP_ID, 0x03),

	WL_NAN_XTLV_GEN_SCHED_STATS		= NAN_CMD(WL_NAN_CMD_GENERIC_COMP_ID, 0x01),
	WL_NAN_XTLV_GEN_PEER_STATS		= NAN_CMD(WL_NAN_CMD_GENERIC_COMP_ID, 0x02),
	WL_NAN_XTLV_GEN_PEER_STATS_DEVCAP	= NAN_CMD(WL_NAN_CMD_GENERIC_COMP_ID, 0x03),
	WL_NAN_XTLV_GEN_PEER_STATS_NDP		= NAN_CMD(WL_NAN_CMD_GENERIC_COMP_ID, 0x04),
	WL_NAN_XTLV_GEN_PEER_STATS_SCHED	= NAN_CMD(WL_NAN_CMD_GENERIC_COMP_ID, 0x05),
	WL_NAN_XTLV_GEN_AVAIL_STATS_SCHED	= NAN_CMD(WL_NAN_CMD_GENERIC_COMP_ID, 0x06),
	WL_NAN_XTLV_GEN_NDP_STATS               = NAN_CMD(WL_NAN_CMD_GENERIC_COMP_ID, 0x07),
	WL_NAN_XTLV_GEN_PARSE_STATS		= NAN_CMD(WL_NAN_CMD_GENERIC_COMP_ID, 0x08),
	WL_NAN_XTLV_GEN_BIP_STATS		= NAN_CMD(WL_NAN_CMD_GENERIC_COMP_ID, 0x09),
	WL_NAN_XTLV_GEN_PEER_BIP_STATS		= NAN_CMD(WL_NAN_CMD_GENERIC_COMP_ID, 0x0A),
	WL_NAN_XTLV_GEN_GTK_STATS		= NAN_CMD(WL_NAN_CMD_GENERIC_COMP_ID, 0x0B),
	WL_NAN_XTLV_GEN_PEER_REKEY_STATS	= NAN_CMD(WL_NAN_CMD_GENERIC_COMP_ID, 0x0C),
	WL_NAN_XTLV_GEN_PEER_GTK_STATS		= NAN_CMD(WL_NAN_CMD_GENERIC_COMP_ID, 0x0D),
	WL_NAN_XTLV_GEN_PEER_PAIRING_STATS	= NAN_CMD(WL_NAN_CMD_GENERIC_COMP_ID, 0x0E),
	WL_NAN_XTLV_GEN_PEER_KA_OOB_AF_STAT	= NAN_CMD(WL_NAN_CMD_GENERIC_COMP_ID, 0x0F),
	WL_NAN_XTLV_GEN_DW_DWELL_STATS		= NAN_CMD(WL_NAN_CMD_GENERIC_COMP_ID, 0x10),

	WL_NAN_XTLV_DAM_STATS			= NAN_CMD(WL_NAN_CMD_DAM_COMP_ID, 0x01),
	WL_NAN_XTLV_DAM_AVAIL_STATS		= NAN_CMD(WL_NAN_CMD_DAM_COMP_ID, 0x02),

	WL_NAN_XTLV_RANGE_STATS			= NAN_CMD(WL_NAN_CMD_RANGE_COMP_ID, 0x01)
} wl_nan_stats_tlv_t;

/* NAN stats WL_NAN_CMD_GEN_STATS command */
/* Input data */
typedef struct wl_nan_cmn_get_stat {
	uint32	modules_btmap;	/* Bitmap to indicate module stats are needed:
				 * See NAN Sub Module ID's above
				 */
	uint8	operation;	/* Get, Get and Clear */
	uint8	arg1;		/* Submodule control variable1 */
	uint8	arg2;		/* Submodule control variable2 */
	uint8	PAD;		/* May not be needed as TLV's are aligned,add to pass compile chk */
} wl_nan_cmn_get_stat_t;

/* Output for Stats container */
typedef struct wl_nan_cmn_stat {
	uint32	n_stats;	/* Number of different sub TLV stats present in the container */
	uint32	totlen;		/* Total Length of stats data in container */
	uint8   stats_tlvs [];	/* Stat TLV's container */
} wl_nan_cmn_stat_t;

/* Defines for operation */
#define WLA_NAN_STATS_GET	0
#define WLA_NAN_STATS_GET_CLEAR	1

#define WL_NAN_STAT_ALL 0xFFFFFFFF

/* NAN Mac stats */

typedef struct wl_nan_mac_band_stats {
	uint32 bcn_tx;	/* 2g/5g disc/sync beacon tx count */
	uint32 bcn_rx;	/* 2g/5g disc/sync beacon rx count */
	uint32 dws;	/* Number of 2g/5g DW's */
} wl_nan_mac_band_stats_t;

/* Note: if this struct is changing update wl_nan_slot_ecounters_vX_t version,
 * as this struct is sent as payload in wl_nan_slot_ecounter_vX_ts
 */
typedef struct wl_nan_mac_stats {
	wl_nan_mac_band_stats_t	band[NAN_MAX_BANDS];	/* MAC sync band specific stats */
	uint32 naf_tx;			/* NAN AF tx */
	uint32 naf_rx;			/* NAN AF rx */
	uint32 sdf_tx;			/* SDF tx */
	uint32 sdf_rx;			/* SDF rx */
	uint32 cnt_sync_bcn_rx_tu[3];	/* delta bw */
	uint32 cnt_bcn_tx_out_dw;	/* TX sync beacon outside dw */
	uint32 cnt_role_am_dw;		/* anchor master role due to dw */
	uint32 cnt_am_hop_err;		/* wrong hopcount set for AM */
} wl_nan_mac_stats_t;

typedef struct wl_nan_mac_stats_v1 {
	wl_nan_mac_band_stats_t	band[NAN_MAX_BANDS];	/* MAC sync band specific stats */
	uint32 naf_tx;			/* NAN AF tx */
	uint32 naf_rx;			/* NAN AF rx */
	uint32 sdf_tx;			/* SDF tx */
	uint32 sdf_rx;			/* SDF rx */
	uint32 cnt_sync_bcn_rx_tu[3];	/* delta bw */
	uint32 cnt_bcn_tx_out_dw;	/* TX sync beacon outside dw */
	uint32 cnt_role_am_dw;		/* anchor master role due to dw */
	uint32 cnt_am_hop_err;		/* wrong hopcount set for AM */
	uint32 merge_scan_cnt_2g;	/* 2G band merge scan cnt */
	uint32 merge_scan_cnt_5g;	/* 5G band merge scan cnt */
} wl_nan_mac_stats_v1_t;

typedef struct wl_nan_mac_stats_v2 {
	wl_nan_mac_band_stats_t	band[NAN_MAX_BANDS];	/* MAC sync band specific stats */
	uint32 naf_tx;			/* NAN AF tx */
	uint32 naf_rx;			/* NAN AF rx */
	uint32 sdf_tx;			/* SDF tx */
	uint32 sdf_rx;			/* SDF rx */
	uint32 cnt_sync_bcn_rx_tu[3];	/* delta bw */
	uint32 cnt_bcn_tx_out_dw;	/* TX sync beacon outside dw */
	uint32 cnt_role_am_dw;		/* anchor master role due to dw */
	uint32 cnt_am_hop_err;		/* wrong hopcount set for AM */
	uint32 merge_scan_cnt_2g;	/* 2G band merge scan cnt */
	uint32 merge_scan_cnt_5g;	/* 5G band merge scan cnt */
	uint16 ucast_sdf_oth_bssid;	/* ucast sdf sent by peer that is in other custer */
	uint16 ucast_naf_oth_bssid;	/* ucast naf sent by peer that is in other cluster */
} wl_nan_mac_stats_v2_t;

/* NAN Sched stats */
/* Per core Sched stats */
typedef struct nan_sched_stats_core {
	uint32	slotstart;		/* slot_start */
	uint32	slotend;		/* slot_end */
	uint32	slotskip;		/* slot_skip */
	uint32	slotstart_partial;	/* slot resume */
	uint32	slotend_partial;	/* slot pre-empt */
	uint8   avail_upd_cnt;		/* count to track num of times avail has been updated */
	uint8	PAD[3];
} nan_sched_stats_core_t;
/* Common Sched stats */
typedef struct nan_sched_stats_cmn {
	uint32	slot_adj_dw;	/* Slot adjusts due to DW changes */
	uint32	slot_dur;	/* Total slot duration in TU's */
} nan_sched_stats_cmn_t;

/* Note: if this struct is changing update wl_nan_slot_ecounters_vX_t version,
 * as this struct is sent as payload in wl_nan_slot_ecounters_vX_t
 */
typedef struct nan_sched_stats {
	nan_sched_stats_cmn_t cmn;
	nan_sched_stats_core_t slice[MAX_NUM_D11CORES];
} nan_sched_stats_t;
/* End NAN Sched stats */

/* NAN Discovery stats */
typedef struct nan_disc_stats {
	uint32 pub_tx;		/* Publish tx */
	uint32 pub_rx;		/* Publish rx */
	uint32 sub_tx;		/* Subscribe tx */
	uint32 sub_rx;		/* Subscribe rx */
	uint32 fup_tx;		/* Followup tx */
	uint32 fup_rx;		/* Followup rx */
	uint32 pub_resp_ignored;	/* response to incoming publish ignored */
	uint32 sub_resp_ignored;	/* response to incoming subscribe ignored */
	uint32 auto_resp_ignored;	/* response to incoming disc bcn ignored */
	uint32 disc_cache_ovfl;		/* Number of seen once disc cache overflows in system */
} nan_disc_stats_t;
/* NAN Discovery stats end */

/* statistics for nan sec */
typedef struct nan_sec_stats_s {
	uint32 mic_fail;		/* rx mic fail */
	uint32 replay_fail;		/* replay counter */
	uint32 tx_fail;			/* tx fail (from txstatus) */
	uint32 key_info_err;		/* key info field err */
	uint32 ok_sessions;		/* successful mx negotiations */
	uint32 fail_sessions;		/* failed sessions */
	uint32 keydesc_err;		/* key desc error */
	uint32 invalid_cipher;		/* cipher suite not valid */
	uint32 pmk_not_found;		/* no pmk found for given service or for any reason */
	uint32 no_pmk_for_pmkid;	/* no pmk found for give pmkid */
	uint32 key_install_err;		/* failed to install keys */
	uint32 no_keydesc_attr;		/* key desc attr missing */
	uint32 nonce_mismatch;		/* nonce mismatch */
} nan_sec_stats_t;

typedef struct nan_peer_rekey_stats {
	uint32 ptk_rekey_start_i;	/* number of ptk rekeys as initiator */
	uint32 ptk_rekey_start_r;	/*  number of ptk rekeys as responder */
	uint32 ptk_rekey_success_i;	/* ptk rekey failures as initiator */
	uint32 ptk_rekey_success_r;	/* ptk rekey failures as responder */
	uint32 grp_rekey_start;		/* Number of group rekey starts */
	uint32 grp_rekey_success;	/* Number of group rekey success */
} nan_peer_rekey_stats_t;

/* statistics for nan BIP security: WL_NAN_XTLV_GEN_BIP_STATS */
typedef struct nan_sec_bip_stats_s {
	uint8	bip_algo;		/* CSID/Algo used */
	uint8	bigtk_tx_id;		/* BIGTK txid used */
	uint8	igtk_tx_id;		/* IGTK txid used */
	uint8	PAD[1];
	uint32	igtk_sessions;		/* Number of igtk exchanges */
	uint32	bigtk_sessions;		/* Number of bigtk exchanges */
	uint32	igtk_tx;		/* Number of igtk pks tx */
	uint32	bigtk_tx;		/* Number of bigtk pks tx */
	uint32	igtk_rx;		/* Number of igtk pks tx */
	uint32	bigtk_rx;		/* Number of bigtk pks tx */
	uint32  igtk_rx_err;		/* Number of igtk rx error */
	uint32  bigtk_rx_err;		/* Number of bigtk rx error */
	uint32	igtk_reject;		/* Number of mgmt frames rejected */
	uint32	bigtk_reject;		/* Number of bcns rejected */
} nan_sec_bip_stats_t;

/* statistics for nan Peer BIP security : WL_NAN_XTLV_GEN_PEER_BIP_STATS */
typedef struct nan_sec_peer_bip_stats_s {
	uint8	bip_algo;		/* CSID/Algo used */
	uint8	bigtk_rx_id;		/* BIGTK rxid used for peer */
	uint8	igtk_rx_id;		/* IGTK rxid used  for peer */
	uint8	PAD[1];
	uint32	igtk_tx;		/* Number of igtk pks tx */
	uint32	bigtk_tx;		/* Number of bigtk pks tx */
	uint32	igtk_rx;		/* Number of igtk pks tx */
	uint32	bigtk_rx;		/* Number of bigtk pks tx */
	uint32  igtk_rx_err;		/* Number of igtk rx error */
	uint32  bigtk_rx_err;		/* Number of bigtk rx error */
	uint32	igtk_reject;		/* Number of mgmt frames rejected */
	uint32	bigtk_reject;		/* Number of bcns rejected */
} nan_sec_peer_bip_stats_t;
#define NAN_SEC_STAT_MAX_NDI 2
/* WL_NAN_XTLV_GEN_GTK_STATS */
typedef struct nan_sec_gtk_stats_s {
	uint32	gtk_sessions;	/* Number of gtk exchanges */
	uint32	gtk_tx;		/* Number of gtk pks tx */
	uint32	gtk_rx;		/* Number of gtk pks rx */
	uint8	gtk_tx_csid;	/* CSID tx */
	uint8	PAD[3];
	uint32	gtk_tx_err;	/* Number of gtk tx error */
	uint32  gtk_rx_err;	/* Number of gtk rx error */
	struct	ether_addr ndi_addr[NAN_SEC_STAT_MAX_NDI]; /* NDI addr to dump for gtk txid */
	uint8	gtk_tx_id[NAN_SEC_STAT_MAX_NDI]; /* gtkid stat per NDI */
	uint8	PAD[2];
} nan_sec_gtk_stats_t;

/* WL_NAN_XTLV_GEN_PEER_GTK_STATS */
typedef struct nan_sec_peer_gtk_stats_s {
	uint8	gtk_csid;
	uint8	gtk_rx_id;	/* GTK rx id used for peer */
	uint8	PAD[2];
	uint32	gtk_rx;		/* Number of gtk pks rx */
	uint32  gtk_rx_err;	/* Number of gtk rx error */
} nan_sec_peer_gtk_stats_t;

/* WL_NAN_XTLV_GEN_PEER_STATS */
typedef struct wl_nan_peer_stats {
	struct ether_addr nmi;
	uint8 PAD[2];
	uint32 pkt_enq; /* counter for queued pkt of peer */

	/* NDL */
	bool ndl_exist;
	uint8 ndl_state;
	bool counter_proposed;
	uint8 PAD;

	/* NDL QoS */
	uint16 local_max_latency;
	uint16 peer_max_latency;
	uint8 local_min_slots;
	uint8 peer_min_slots;

	/* security association */
	struct ether_addr sec_laddr;	/* local mac addr */
	struct ether_addr sec_raddr;	/* remote mac addr */
	uint8 sec_csid;
	uint8 PAD;
} wl_nan_peer_stats_t;

/* WL_NAN_XTLV_GEN_PEER_STATS_DEVCAP */
typedef struct wl_nan_peer_stats_dev_cap {
	uint8 mapid;
	uint8 awake_dw_2g;
	uint8 awake_dw_5g;
	uint8 bands_supported;
	uint8 op_mode;
	uint8 num_antennas;
	uint16 chan_switch_time;
	uint8 capabilities;
	uint8 PAD[3];
} wl_nan_peer_stats_dev_cap_t;

/* WL_NAN_XTLV_GEN_PEER_STATS_NDP */
typedef struct wl_nan_peer_stats_ndp {
	uint8 peer_role;
	uint8 ndp_state;
	uint8 indp_id;	/* initiator ndp id */
	uint8 ndp_ctrl;	/* ndp control field */
	struct ether_addr peer_nmi;
	struct ether_addr peer_ndi;
	struct ether_addr local_ndi;

	/* peer scb info */
	bool scb_allocated;
	bool scb_found;
	uint32 scb_flags;
	uint32 scb_flags2;
	uint32 scb_flags3;
} wl_nan_peer_stats_ndp_t;

enum {
	WL_NAN_SCHED_STAT_SLOT_COMM	= 0x01,	/* Committed slot */
	WL_NAN_SCHED_STAT_SLOT_COND	= 0x02,	/* Conditional slot(proposal/counter) */
	WL_NAN_SCHED_STAT_SLOT_NDC	= 0x04,	/* NDC slot */
	WL_NAN_SCHED_STAT_SLOT_IMMUT	= 0x08,	/* Immutable slot */
	WL_NAN_SCHED_STAT_SLOT_RANGE	= 0x10, /* Ranging slot */
};
typedef uint16 wl_nan_stats_sched_slot_info_t;

typedef struct wl_nan_stats_sched_slot {
	wl_nan_stats_sched_slot_info_t info;	/* capture slot type and more info */
	chanspec_t chanspec;
} wl_nan_stats_sched_slot_t;

/* WL_NAN_XTLV_GEN_PEER_STATS_SCHED, WL_NAN_XTLV_GEN_AVAIL_STATS_SCHED */
typedef struct wl_nan_stats_sched {
	uint8 map_id;
	uint8 seq_id;	/* seq id from NA attr */
	uint8 slot_dur;
	uint8 PAD;
	uint16 period;
	uint16 num_slot;
	wl_nan_stats_sched_slot_t slot[];
} wl_nan_stats_sched_t;

/* WL_NAN_XTLV_GEN_PEER_STATS_SCHED */
typedef struct wl_nan_peer_stats_sched {
	uint8 map_id;
	uint8 seq_id;	/* seq id from NA attr */
	uint8 slot_dur;
	uint8 PAD;
	uint16 period;
	uint16 num_slot;
	wl_nan_stats_sched_slot_t slot[];
} wl_nan_peer_stats_sched_t;

/* WL_NAN_XTLV_GEN_PEER_KA_OOB_AF_STAT */
typedef struct nan_peer_oob_af_stat {
	uint32 tx_ok;
	uint32 rx_ok;
	uint32 tx_fail;
	uint32 rx_timeout;
} nan_peer_oob_af_stat_t;

/* WL_NAN_XTLV_RANGE_STATS  */
typedef struct wl_nan_range_stats {
	uint16 rng_ssn_estb;
	uint16 rng_ssn_fail;
	uint16 rng_sched_start;
	uint16 rng_sched_end;
	uint16 ftm_ssn_success; /* number of succesfull ftm sessions */
	uint16 ftm_ssn_fail;
	uint16 num_meas; /* number of ftm frames */
	uint16 num_valid_meas; /* number of ftm frames with valid timestamp */
} wl_nan_range_stats_t;

/* defines for ndp stats flag */

#define NAN_NDP_STATS_FLAG_ROLE_MASK		0x01u
#define NAN_NDP_STATS_FLAG_ROLE_INIT		0x00u
#define NAN_NDP_STATS_FLAG_ROLE_RESP		0x01u

#define NAN_NDP_STATS_STATE_BIT_SHIFT		1u
#define NAN_NDP_STATS_FLAG_STATE_MASK		0x07u
#define NAN_NDP_STATS_FLAG_STATE_IN_PROG	0x00u
#define NAN_NDP_STATS_FLAG_STATE_ESTB		0x01u
#define NAN_NDP_STATS_FLAG_STATE_TEARDOWN_WAIT	0x02u

#define NAN_NDP_STATS_FLAG_MESH			0x10u

/* More states can be added here, when needed */

/* WL_NAN_XTLV_GEN_NDP_STATS */
typedef struct wl_nan_ndp_stats_s {
	uint8 ndp_id;
	uint8 indp_id;
	uint8 flags;
	uint8 nan_sec_csid;
	struct ether_addr lndi_addr;
	struct ether_addr pnmi_addr;
	struct ether_addr pndi_addr;
	uint8 PAD[2];
} wl_nan_ndp_stats_t;

/* WL_NAN_XTLV_EV_SLOT_INFO */
typedef struct wl_nan_slot_info_s {
	/* dw slot start expected */
	uint32	dwst_h;
	uint32	dwst_l;
	/* dw slot start actual */
	uint32	act_dwst_h;
	uint32	act_dwst_l;
	uint16	cur_chan[MAX_NUM_D11CORES];	/* sdb channels */
	uint16	dw_chan;	/* dw channel */
	uint8	dw_no;		/* dw number */
	uint8	slot_seq_no;	/* slot seq no. */
} wl_nan_slot_info_t;

/* WL_NAN_XTLV_DAM_STATS */
typedef struct wl_nan_dam_stats {
	uint32 cnt_rej_crb_inuse;		/* DAM rejections because of crb in use already */
} wl_nan_dam_stats_t;

/* WL_NAN_EVENT_MR_CHANGED */
typedef uint8 wl_nan_mr_changed_t;
#define WL_NAN_AMR_CHANGED	1
#define WL_NAN_IMR_CHANGED	2

/*
 * The macro BCMUTILS_ERR_CODES is defined only
 * when using the common header file(a new approach) bcmutils/include/bcmerror.h.
 * Otherwise, use the error codes from this file.
 */
#ifndef BCMUTILS_ERR_CODES

/** status - TBD BCME_ vs NAN status - range reserved for BCME_ */
enum {
	/* add new status here... */
	WL_NAN_E_NOT_SUPPORTED		= -2141,
	WL_NAN_E_NOT_ASSOCIATED		= -2140,
	WL_NAN_E_BUSY			= -2139,
	WL_NAN_E_REDUNDANT		= -2138,
	WL_NAN_E_GRP_REKEY_FAIL		= -2137,	/* Group rekey failure */
	WL_NAN_E_NO_ACTION		= -2136,	/* status for no action */
	WL_NAN_E_INVALID_TOKEN		= -2135,	/* invalid token or mismatch */
	WL_NAN_E_INVALID_ATTR		= -2134,	/* generic invalid attr error */
	WL_NAN_E_INVALID_NDL_ATTR	= -2133,	/* invalid NDL attribute */
	WL_NAN_E_SCB_NORESOURCE		= -2132,	/* no more peer scb available */
	WL_NAN_E_PEER_NOTAVAIL		= -2131,
	WL_NAN_E_SCB_EXISTS		= -2130,
	WL_NAN_E_INVALID_PEER_NDI	= -2129,
	WL_NAN_E_INVALID_LOCAL_NDI	= -2128,
	WL_NAN_E_ALREADY_EXISTS		= -2127,	/* generic NAN error for duplication */
	WL_NAN_E_EXCEED_MAX_NUM_MAPS	= -2126,
	WL_NAN_E_INVALID_DEV_CHAN_SCHED	= -2125,
	WL_NAN_E_INVALID_PEER_BLOB_TYPE	= -2124,
	WL_NAN_E_INVALID_LCL_BLOB_TYPE	= -2123,
	WL_NAN_E_BCMC_PDPA		= -2122,	/* BCMC NAF PDPA */
	WL_NAN_E_TIMEOUT		= -2121,
	WL_NAN_E_HOST_CFG		= -2120,
	WL_NAN_E_NO_ACK			= -2119,
	WL_NAN_E_SECINST_FAIL		= -2118,
	WL_NAN_E_REJECT_NDL		= -2117,	/* generic NDL rejection error */
	WL_NAN_E_INVALID_NDP_ATTR	= -2116,
	WL_NAN_E_HOST_REJECTED		= -2115,
	WL_NAN_E_PCB_NORESOURCE		= -2114,
	WL_NAN_E_NDC_EXISTS		= -2113,
	WL_NAN_E_NO_NDC_ENTRY_AVAIL	= -2112,
	WL_NAN_E_INVALID_NDC_ENTRY      = -2111,
	WL_NAN_E_SD_TX_LIST_FULL        = -2110,
	WL_NAN_E_SVC_SUB_LIST_FULL      = -2109,
	WL_NAN_E_SVC_PUB_LIST_FULL      = -2108,
	WL_NAN_E_SDF_MAX_LEN_EXCEEDED   = -2107,
	WL_NAN_E_ZERO_CRB		= -2106,	/* no CRB between local and peer */
	WL_NAN_E_PEER_NDC_NOT_SELECTED	= -2105,	/* peer ndc not selected */
	WL_NAN_E_DAM_CHAN_CONFLICT	= -2104,	/* dam schedule channel conflict */
	WL_NAN_E_DAM_SCHED_PERIOD	= -2103,	/* dam schedule period mismatch */
	WL_NAN_E_LCL_NDC_NOT_SELECTED	= -2102,	/* local selected ndc not configured */
	WL_NAN_E_NDL_QOS_INVALID_NA	= -2101,	/* na doesn't comply with ndl qos */
	WL_NAN_E_CLEAR_NAF_WITH_SA_AS_RNDI = -2100,	/* rx clear naf with peer rndi */
	WL_NAN_E_SEC_CLEAR_PKT		= -2099,	/* rx clear pkt from a peer with sec_sa */
	WL_NAN_E_PROT_NON_PDPA_NAF	= -2098,	/* rx protected non PDPA frame */
	WL_NAN_E_DAM_DOUBLE_REMOVE	= -2097,	/* remove peer schedule already removed */
	WL_NAN_E_DAM_DOUBLE_MERGE	= -2096,	/* merge peer schedule already merged */
	WL_NAN_E_DAM_REJECT_INVALID	= -2095,	/* reject for invalid schedule */
	WL_NAN_E_DAM_REJECT_RANGE	= -2094,
	WL_NAN_E_DAM_REJECT_QOS		= -2093,
	WL_NAN_E_DAM_REJECT_NDC		= -2092,
	WL_NAN_E_DAM_REJECT_PEER_IMMUT	= -2091,
	WL_NAN_E_DAM_REJECT_LCL_IMMUT	= -2090,
	WL_NAN_E_DAM_EXCEED_NUM_SCHED	= -2089,
	WL_NAN_E_DAM_INVALID_SCHED_MAP	= -2088,	/* invalid schedule map list */
	WL_NAN_E_DAM_INVALID_LCL_SCHED	= -2087,
	WL_NAN_E_INVALID_MAP_ID		= -2086,
	WL_NAN_E_CHAN_OVERLAP_ACROSS_MAP = -2085,
	WL_NAN_E_INVALID_CHAN_LIST	= -2084,
	WL_NAN_E_INVALID_RANGE_TBMP	= -2083,
	WL_NAN_E_INVALID_IMMUT_SCHED	= -2082,
	WL_NAN_E_INVALID_NDC_ATTR	= -2081,
	WL_NAN_E_INVALID_TIME_BITMAP	= -2080,
	WL_NAN_E_INVALID_NA_ATTR	= -2079,
	WL_NAN_E_NO_NA_ATTR_IN_AVAIL_MAP = -2078,	/* no na attr saved in avail map */
	WL_NAN_E_INVALID_MAP_IDX	= -2077,
	WL_NAN_E_SEC_SA_NOTFOUND	= -2076,
	WL_NAN_E_BSSCFG_NOTFOUND	= -2075,
	WL_NAN_E_SCB_NOTFOUND		= -2074,
	WL_NAN_E_NCS_SK_KDESC_TYPE      = -2073,
	WL_NAN_E_NCS_SK_KEY_DESC_VER    = -2072,	/* key descr ver */
	WL_NAN_E_NCS_SK_KEY_TYPE        = -2071,	/* key descr type */
	WL_NAN_E_NCS_SK_KEYINFO_FAIL    = -2070,	/* key info (generic) */
	WL_NAN_E_NCS_SK_KEY_LEN         = -2069,	/* key len */
	WL_NAN_E_NCS_SK_KDESC_NOT_FOUND = -2068,	/* key desc not found */
	WL_NAN_E_NCS_SK_INVALID_PARAMS  = -2067,	/* invalid args */
	WL_NAN_E_NCS_SK_KDESC_INVALID   = -2066,	/* key descr is not valid */
	WL_NAN_E_NCS_SK_NONCE_MISMATCH  = -2065,
	WL_NAN_E_NCS_SK_KDATA_SAVE_FAIL = -2064,	/* not able to save key data */
	WL_NAN_E_NCS_SK_AUTH_TOKEN_CALC_FAIL = -2063,
	WL_NAN_E_NCS_SK_PTK_CALC_FAIL   = -2062,
	WL_NAN_E_INVALID_STARTOFFSET	= -2061,
	WL_NAN_E_BAD_NA_ENTRY_TYPE	= -2060,
	WL_NAN_E_INVALID_CHANBMP	= -2059,
	WL_NAN_E_INVALID_OP_CLASS	= -2058,
	WL_NAN_E_NO_IES			= -2057,
	WL_NAN_E_NO_PEER_ENTRY_AVAIL	= -2056,
	WL_NAN_E_INVALID_PEER		= -2055,
	WL_NAN_E_PEER_EXISTS		= -2054,
	WL_NAN_E_PEER_NOTFOUND		= -2053,
	WL_NAN_E_NO_MEM			= -2052,
	WL_NAN_E_INVALID_OPTION		= -2051,
	WL_NAN_E_INVALID_BAND		= -2050,
	WL_NAN_E_INVALID_MAC		= -2049,
	WL_NAN_E_BAD_INSTANCE		= -2048,
	WL_NAN_E_INVALID_S3_ATTR	= -2047,
	/* NAN status code reserved from -2046 to -3071 */
	/* Do NOT add new status below -2046 */
	WL_NAN_E_ERROR			= -1,
	WL_NAN_E_OK			= 0
};

#endif	/* BCMUTILS_ERR_CODES */

/* Error codes used in vendor specific attribute in Data Path Termination frames */
enum {
	WL_NAN_DPEND_E_OK		= 0,
	WL_NAN_DPEND_E_ERROR		= 1,
	WL_NAN_DPEND_E_HOST_CMD		= 2,
	WL_NAN_DPEND_E_HOST_REJECTED	= 3,	/* host rejected rx frame  */
	WL_NAN_DPEND_E_RESOURCE_LIMIT	= 4,
	WL_NAN_DPEND_E_NO_ACK_RCV	= 5,
	WL_NAN_DPEND_E_TIMEOUT		= 6,
	WL_NAN_DPEND_E_NO_ELT		= 7,	/* rx frame missing element container */
	WL_NAN_DPEND_E_NO_NDP_ATTR	= 8,
	WL_NAN_DPEND_E_NO_AVAIL_ATTR	= 9,
	WL_NAN_DPEND_E_NO_NDC_ATTR	= 10,
	WL_NAN_DPEND_E_NO_RANGE_BM	= 11,
	WL_NAN_DPEND_E_INVALID_NDP_ATTR	= 12,
	WL_NAN_DPEND_E_INVALID_NDC_ATTR	= 13,
	WL_NAN_DPEND_E_INVALID_IMMUT	= 14,
	WL_NAN_DPEND_E_INVALID_NDL_QOS	= 15,
	WL_NAN_DPEND_E_INVALID_SEC_PARAMS = 16,
	WL_NAN_DPEND_E_REJECT_AVAIL	= 17,
	WL_NAN_DPEND_E_REJECT_NDL	= 18,
	WL_NAN_DPEND_E_SCB_NORESOURCE	= 19
};

typedef int32 wl_nan_status_t;

/** nan cmd list entry  */
enum wl_nan_sub_cmd_input_flags {
	WL_NAN_SUB_CMD_FLAG_NONE = 0,
	WL_NAN_SUB_CMD_FLAG_SKIP = 1, /* Skip to next sub-command on error */
	WL_NAN_SUB_CMD_FLAG_TERMINATE = 2, /* Terminate processing and return */
	WL_NAN_SUB_CMD_FLAG_LAST /* Keep this at the end */
};

/** container for nan events */
typedef struct wl_nan_ioc {
	uint16	version;	/**< interface command or event version */
	uint16	id;			/**< nan ioctl cmd  ID  */
	uint16	len;		/**< total length of all tlv records in data[]  */
	uint16	PAD;		/**< pad to be 32 bit aligment */
	uint8	data [];	/**< var len payload of bcm_xtlv_t type */
} wl_nan_ioc_t;

/*
 * NAN sub-command data structures
 */

/*
 * Config component WL_NAN_CMD_CFG_XXXX sub-commands
 * WL_NAN_CMD_CFG_ENABLE
 */
enum wl_nan_config_state {
	WL_NAN_CONFIG_STATE_DISABLE = 0,
	WL_NAN_CONFIG_STATE_ENABLE = 1
};

typedef int8 wl_nan_config_state_t;

/* WL_NAN_CMD_CFG_NAN_INIT */

typedef uint8 wl_nan_init_t;

/* WL_NAN_CMD_CFG_NAN_VERSION */
typedef uint16 wl_nan_ver_t;

/* WL_NAN_CMD_CFG_NAN_CONFIG  */
typedef uint32 wl_nan_cfg_ctrl_t;

/* WL_NAN_CMD_CFG_NAN_CONFIG2 */
typedef struct wl_nan_cfg_ctrl2 {
	uint32 flags1; /* wl_nan_cfg_ctrl2_flags1 */
	uint32 flags2; /* wl_nan_cfg_ctrl2_flags2 */
} wl_nan_cfg_ctrl2_t;

enum wl_nan_cfg_ctrl2_flags1 {
	/* Allows unicast SDF TX while local device is under NDP/NDL negotiation,
	 * but Not with the peer SDF destined to.
	 */
	WL_NAN_CTRL2_FLAG1_ALLOW_SDF_TX_UCAST_IN_PROG		= 0x00000001,
	/* Allows broadcast SDF TX while local device is under NDP/NDL negotiation */
	WL_NAN_CTRL2_FLAG1_ALLOW_SDF_TX_BCAST_IN_PROG		= 0x00000002,
	/* Allows the device to send schedule update automatically on local schedule change */
	WL_NAN_CTRL2_FLAG1_AUTO_SCHEDUPD			= 0x00000004,
	/* Allows the device to handle slot pre_close operations */
	WL_NAN_CTRL2_FLAG1_SLOT_PRE_CLOSE			= 0x00000008,
	/* Control flag to enable/disable NDPE capability */
	WL_NAN_CTRL2_FLAG1_NDPE_CAP				= 0x00000010,
	/* Control flag to enable/disable AUTO DAM LWT mode */
	WL_NAN_CTRL2_FLAG1_AUTODAM_LWT_MODE			= 0x00000020,
	/* Control flag to enable/disable PMK per NDP */
	WL_NAN_CTRL2_FLAG1_PMK_PER_NDP				= 0x00000040,
	/* Control flag to enable/disable allowing clear Schedule Update on Secured connection */
	WL_NAN_CTRL2_FLAG1_SEC_ALLOW_CLEAR_SCHED_UPD_PKT	= 0x00000080,
	/* Control flag to disable/enable 5G FAW */
	WL_NAN_CTRL2_FLAG1_DISABLE_5G_FAW			= 0x00000100,
	/* Control flag to disable/enable AUTO DAM 6G CAP */
	WL_NAN_CTRL2_FLAG1_DISABLE_AUTODAM_6G_CAP		= 0x00000200,
	/* Control flag to disable/enable allowing of unsecured OOB AF in a secured connection */
	WL_NAN_CTRL2_FLAG1_ALLOW_UNSECURED_OOB_AF		= 0x00000400,
	/* Control flag to enable/disable 6G FULL avail */
	WL_NAN_CTRL2_FLAG1_6G_FULL_AVAIL			= 0x00000800,
	/* Control flag to enable/disable PTK REKEY */
	WL_NAN_CTRL2_FLAG1_PTK_REKEY				= 0x00001000,
	/* Control flag to enable/disable GRP REKEY */
	WL_NAN_CTRL2_FLAG1_GTK_REKEY				= 0x00002000,
	/* Control flag to enable/disable GTK */
	WL_NAN_CTRL2_FLAG1_GTK					= 0x00004000,
	/* Control flag to enable/disable IGTK */
	WL_NAN_CTRL2_FLAG1_IGTK					= 0x00008000,
	/* Control flag to enable/disable BIGTK */
	WL_NAN_CTRL2_FLAG1_BIGTK				= 0x00010000,
	/* Control flag to enable/disable BIP_CMAC_128 */
	WL_NAN_CTRL2_FLAG1_BIP_CMAC_128				= 0x00020000,
	/* Control flag to enable/disable BIP_GMAC_256 */
	WL_NAN_CTRL2_FLAG1_BIP_GMAC_256				= 0x00040000,
	/* Control flag to enable/disable Notifs to BTcoex */
	WL_NAN_CTRL2_FLAG1_BTCX_NOTIF				= 0x00080000,
	/* Control flag to enable/disable NAN 3.1 Instant mode */
	WL_NAN_CTRL2_FLAG1_INSTANT_MODE				= 0x00100000,
	/* Control flag to disable/enable AUTO DAM dynamic schedule upgrade/downgrade */
	WL_NAN_CTRL2_FLAG1_DISABLE_AUTODAM_DYN_UPDOWN		= 0x00200000,
	/* Control flag to reject group addressed AFs w/o IGTK */
	WL_NAN_CTRL2_FLAG1_IGTK_REJECT_UNPROT			= 0x00400000,
	/* Control flag to reject bcn w/o BIGTK */
	WL_NAN_CTRL2_FLAG1_BIGTK_REJECT_UNPROT			= 0x00800000,
	/* Control flags to set infra slot duration.
	 * Two Bits[MSB:LSB], 00 -128ms, 01 - 64ms, 10 - 32ms, 11 - Reserved
	 */
	WL_NAN_CTRL2_FLAG1_INFRA_SLOT_INTRVL_LSB		= 0x01000000,
	WL_NAN_CTRL2_FLAG1_INFRA_SLOT_INTRVL_MSB		= 0x02000000,
	/* Control flag to enable NAN EHT */
	WL_NAN_CTRL2_FLAG1_EHT_ENABLE				= 0x04000000,
	/* Control flag to enable NAN STRML */
	WL_NAN_CTRL2_FLAG1_STRML_ENABLE				= 0x08000000,
	/* Control flag to enable NAN EMLSR */
	WL_NAN_CTRL2_FLAG1_EMLSR_ENABLE				= 0x10000000,
	/* Allow election (role change) outside of DW */
	WL_NAN_CTRL2_FLAG1_ELECTION_OUTOF_DW			= 0x40000000
};
#define WL_NAN_CTRL2_FLAGS1_MASK	0xFFFFFFFF

enum wl_nan_cfg_ctrl2_flags2 {
	/* Control flag to enable/disable NAN Pairing */
	WL_NAN_CTRL2_FLAG2_PAIRING				=	(1u << 0u), /* bit 0 */
	/* Control flag to enable/disable NAN Pairing NPK/NIK caching */
	WL_NAN_CTRL2_FLAG2_PAIRING_NPK_CACHING			=	(1u << 1u), /* bit 1 */
	/* Control flag to enable/disable NAN Pairing verification */
	WL_NAN_CTRL2_FLAG2_PAIRING_VERIFICATION			=	(1u << 2u), /* bit 2 */
	/*
	 * When a NAN device acting as a WFA test bed, is forced to include certain
	 * attributes than the default ones. For example, disabling use of NDPE attribute.
	 *
	 * Use this flag along with the WFA test mode flags (WL_NAN_WFA_TM_*) to
	 * enable WFA test mode operations. NAN R4 onwards, this flag is not
	 * needed.
	 */
	WL_NAN_CTRL2_FLAG2_USE_WFA_MODE				=	(1u << 3u), /* bit 3 */
	WL_NAN_CTRL2_FLAG2_S3_CAPABLE				=	(1u << 4u) /* bit 4 */
};
#define WL_NAN_CTRL2_FLAGS2_MASK	0x0000001f

/*
 * WL_NAN_CMD_CFG_BAND, WL_NAN_CMD_CFG_RSSI_THRESHOLD(Get only)
 */
typedef uint8 wl_nan_band_t;

/*
 * WL_NAN_CMD_CFG_ROLE
 */
enum wl_nan_role {
	WL_NAN_ROLE_AUTO = 0,
	WL_NAN_ROLE_NON_MASTER_NON_SYNC = 1,
	WL_NAN_ROLE_NON_MASTER_SYNC = 2,
	WL_NAN_ROLE_MASTER = 3,
	WL_NAN_ROLE_ANCHOR_MASTER = 4
};

typedef uint8 wl_nan_role_t;

typedef struct wl_nan_device_state
{
	wl_nan_role_t role;		/* Sync Master, Non-Sync Master */
	uint8 state;	/* TBD  */
	uint8 hopcount;	/* Hops to the Anchor Master */
	struct ether_addr immediate_master; /* Master MAC */
	struct ether_addr anchor_master;	/* Anchor Master MAC */
	struct ether_addr cluster_id; /* Cluster ID to which this device belongs to */
	uint8 PAD[3];
	uint32 tsf_high;  /* NAN Cluster TSFs */
	uint32 tsf_low;
} wl_nan_device_state_t;

/* NAN DW dwell stats to keep cnt per slice */
typedef struct wl_nan_dw_dwell_stat {
	uint32 dwell_cnt[NAN_MAX_BANDS];
} wl_nan_dw_dwell_stat_t;

typedef struct wl_nan_cfg_dw_dwell {
	/* dw dwell time (limited to a max of awake_dws * 512 TUs) */
	uint32		dwell_time[NAN_MAX_BANDS]; /* in  msec */
	/* to indicate slice on which dwell is allowed
	 * Bit 1: 2G only
	 * Bit 2: 5G only
	 */
	uint8		flags;
	uint8		pad[3];
} wl_nan_cfg_dw_dwell_t;

/* Definitions for snap header flags -
 * SNAP_HDR_MATCH - match the snap header
 * This is applicable for the rx side only
 */
#define NAN_SNAP_HDR_MATCH	(1u << 0u)

#define NAN_SNAP_HDR_MAX_LEN	32u
/* MAX SNAP HDR LEN for rx_hdr, tx_hdr, and xtlv_list */
#define WL_NAN_SNAP_CFG_XTLV_LIST_MAX_LEN (3u * NAN_SNAP_HDR_MAX_LEN)

typedef struct wl_nan_cfg_snap_hdr {
	/* flags for snap hdr config as defined above */
	uint32 flags;
	/* local mac addr to use */
	struct ether_addr local_addr;
	/* peer mac addr to use */
	struct ether_addr peer_addr;
	/* list of tlvs - rx_hdr, tx_hdr, and optional xtlv_list */
	uint8  tlv_params[];
} wl_nan_cfg_snap_hdr_t;

/*
 * WL_NAN_CMD_CFG_HOP_CNT, WL_NAN_CMD_CFG_HOP_LIMIT
 */
typedef uint8 wl_nan_hop_count_t;

/*
 * WL_NAN_CMD_CFG_WARMUP_TIME
 */
typedef uint32 wl_nan_warmup_time_ticks_t;

/*
 * WL_NAN_CMD_CFG_RSSI_THRESHOLD
 * rssi_close and rssi_mid are used to transition master to non-master
 * role by NAN state machine. rssi thresholds corresponding to the band
 * will be updated.
 */
/* To be deprecated */
typedef struct wl_nan_rssi_threshold {
	wl_nan_band_t band;
	int8 rssi_close;
	int8 rssi_mid;
	uint8 PAD;
} wl_nan_rssi_threshold_t;

/* WL_NAN_CMD_ELECTION_RSSI_THRESHOLD */

typedef struct wl_nan_rssi_thld {
	int8 rssi_close_2g;
	int8 rssi_mid_2g;
	int8 rssi_close_5g;
	int8 rssi_mid_5g;
} wl_nan_rssi_thld_t;

/* WL_NAN_CMD_DATA_MAX_PEERS */

typedef uint8 wl_nan_max_peers_t;

/*
 * WL_NAN_CMD_CFG_STATUS
 */

typedef enum wl_nan_election_mode {
	WL_NAN_ELECTION_RUN_BY_HOST = 1,
	WL_NAN_ELECTION_RUN_BY_FW = 2
} wl_nan_election_mode_t;

typedef struct wl_nan_conf_status {
	struct ether_addr	nmi;		/*  NAN mgmt interface address */
	uint8			enabled;	/* NAN is enabled */
	uint8			role;		/* Current nan sync role */
	struct ether_addr	cid;		/*  Current Cluster id */
	uint8			social_chans[2];	/* Social channels */
	uint8			mr[8];		/* Self Master Rank */
	uint8			amr[8];		/* Anchor Master Rank */
	uint32			ambtt;		/* Anchor master beacon target time */
	uint32			cluster_tsf_h;	/* Current Cluster TSF High */
	uint32			cluster_tsf_l;	/* Current Cluster TSF Low */
	uint8			election_mode; /* Election mode, host or firmware */
	uint8			hop_count;	/* Current Hop count */
	uint8			imr[8];		/* Immediate Master Rank */
	uint8			status;		/* Genenric status for nan events */
	uint8			PAD[3];		/* remove after precommit */
	uint16			opt_tlvs_len;
	uint8			opt_tlvs[];
} wl_nan_conf_status_t;

/*
 * WL_NAN_CMD_CFG_OUI
 */
typedef struct wl_nan_oui_type {
	uint8 nan_oui[DOT11_OUI_LEN];
	uint8 type;
} wl_nan_oui_type_t;

/*
 * WL_NAN_CMD_CFG_COUNT
 */
typedef struct wl_nan_count {
	uint32 cnt_bcn_tx;		/**< TX disc/sync beacon count */
	uint32 cnt_bcn_rx;		/**< RX disc/sync beacon count */
	uint32 cnt_svc_disc_tx;		/**< TX svc disc frame count */
	uint32 cnt_svc_disc_rx;		/**< RX svc disc frame count */
} wl_nan_count_t;
/*
 * Election component WL_NAN_CMD_ELECTION_XXXX sub-commands
 * WL_NAN_CMD_ELECTION_HOST_ENABLE
 */
enum wl_nan_enable_flags {
	WL_NAN_DISABLE_FLAG_HOST_ELECTION = 0,
	WL_NAN_ENABLE_FLAG_HOST_ELECTION = 1
};

/*
 * 0 - disable host based election
 * 1 - enable host based election
 */
typedef uint8 wl_nan_host_enable_t;

/*
 * WL_NAN_CMD_CFG_TSF
 */
typedef struct wl_nan_tsf_config {
	uint32 tsf_hi;
	uint32 tsf_lo;
} wl_nan_tsf_config_t;


/*
 * WL_NAN_CMD_ELECTION_METRICS_CONFIG
 */
/* Set only */
typedef struct wl_nan_election_metric_config {
	uint8 random_factor; /* Configured random factor */
	uint8 master_pref;	/* configured master preference */
	uint8 PAD[2];
} wl_nan_election_metric_config_t;

/*
 * WL_NAN_CMD_ELECTION_METRICS_STATE
 */
/* Get only */
typedef struct wl_nan_election_metric_state {
	uint8 random_factor; /* random factor used in MIs */
	uint8 master_pref;	 /* Master advertised in MIs */
	uint8 PAD[2];
} wl_nan_election_metric_state_t;

/*
 * WL_NAN_CMD_ELECTION_LEAVE
 * WL_NAN_CMD_ELECTION_STOP
 */
typedef struct ether_addr wl_nan_cluster_id_t;

#define NHO_SEC_NCS_SK_REPLAY_CNT_LEN    8u
/* kck */
#define NHO_SEC_NCS_SK_256_KCK_LEN         24u  /* refer nan2 r21 7.1.4.1 */
/* kek */
#define NHO_SEC_NCS_SK_256_KEK_LEN         32u  /* refer nan2 r21 7.1.4.1 */
/* tk */
#define NHO_SEC_NCS_SK_256_TK_LEN          32u  /* refer nan2 r21 section 7 */

#define NHO_SEC_NCS_SK_MAX_KEY_LEN      (NHO_SEC_NCS_SK_256_KCK_LEN \
					+ NHO_SEC_NCS_SK_256_KEK_LEN \
					+ NHO_SEC_NCS_SK_256_TK_LEN)

#define NHO_SEC_KEY_INSTALL_FLAG                 (1 << 0)
#define NHO_SEC_KEY_UNINSTALL_FLAG               (1 << 1)

/* WL_NAN_XTLV_NANHO_SEC_SA */
typedef struct nanho_sec_sa {
	int8 csid;	/* Cipher suite ID to identify the security type */
	uint8 kck_len; /* KCK len in key_buf */
	uint8 kek_len; /* KEK len in key_buf */
	uint8 tk_len; /* Transient key len in key_buf */
	uint16 flags;
	uint16 PAD;
	struct ether_addr laddr;	/* local mac addr */
	struct ether_addr raddr;	/* remote mac addr */
	uint8 key_krc[NHO_SEC_NCS_SK_REPLAY_CNT_LEN];   /* Key Replay ctr */
	uint8 key_buf[NHO_SEC_NCS_SK_MAX_KEY_LEN];		/* PTK = KCK + KEK + TK */
} nanho_sec_sa_t;

/*
 * WL_NAN_CMD_ELECTION_MERGE
 * 0 - disable cluster merge
 * 1 - enable cluster merge
 */
typedef uint8 wl_nan_merge_enable_t;

/*
 * WL_NAN_CMD_CFG_ROLE
 * role = 0 means configuration by firmware(obsolete); otherwise by host
 * when host configures role, also need target master address to sync to
 */
#define NAN_SYNC_MASTER_SELF			1
#define NAN_SYNC_MASTER_USE_TIMING		2	/* Use the tsf timing provided */
#define NAN_SYNC_MASTER_AMREC_UPD		4	/* provide AM record update */

/*
	struct ether_addr addr:
	when NAN_SYNC_MASTER_USE_TIMING is set, addr is the mac of Rx NAN beacon
	providing the timing info
	ltsf_h, ltsf_l:
	The local TSF timestamp filled in by FW in the WL_NAN_EVENT_BCN_RX event;
	rtsf_h, rtsf_l:
	The timestamp in the Rx beacon frame, filled in by host
	uint32 ambtt:
	the amtt in the cluster ID attribute in the Rx beacon frame
*/

typedef struct nan_sync_master {
	uint8 flag;	/*  1: self; 2: use TSF timing; 4: AMR update */
	uint8  hop_count;
	struct ether_addr addr;
	struct ether_addr cluster_id;
	chanspec_t channel; /* bcn reception channel */
	uint32 ltsf_h;
	uint32 ltsf_l;
	uint32 rtsf_h;
	uint32 rtsf_l;
	uint8 amr[WL_NAN_MASTER_RANK_LEN];
	uint32 ambtt;
} nan_sync_master_t;


/* NAN advertiser structure */
/* TODO RSDB: add chspec to indicates core corresponds correct core */
typedef struct nan_adv_entry {
	uint8 age;	/* used to remove stale entries */
	uint8 hop_count;	/* for NTLV support, use bit7 for virtual NAN peer */
	struct ether_addr addr;
	struct ether_addr cluster_id;
	chanspec_t channel; /* bcn reception channel */
	uint32 ltsf_h;
	uint32 ltsf_l;
	uint32 rtsf_h;
	uint32 rtsf_l;
	uint8 amr[WL_NAN_MASTER_RANK_LEN];
	uint32 ambtt;
	int8	rssi[NAN_MAX_BANDS];		/* rssi last af was received at */
	int8	last_rssi[NAN_MAX_BANDS];	/* rssi in the last AF */
} nan_adv_entry_t;
#define NAN_VIRTUAL_PEER_BIT	0x80

typedef enum {
	NAC_CNT_NTLV_AF_TX = 0,		/* count of AF containing NTLV tx */
	NAC_CNT_NTLV_AF_RX,		/* count of AF containing NTLV rx */
	NAC_CNT_NTLV_TMERR_TX,		/* count of NTLV tx timing error */
	NAC_CNT_NTLV_TMERR_RX,		/* count of NTLV rx timing error */
	NAC_CNT_NTLV_TM_MISMATCH,	/* count of TopMaster mismatch in Rx NTLV processing */
	NAC_CNT_NTLV_ADV_EXISTED,	/* count of NTLV ignored bc advertiser existed from bcn */
	NAC_CNT_NTLV_STALED_BCN,	/* count of staled bcn from NTLV info */
	NAC_CNT_NTLV_MERGE,		/* count of NTLV used for NAN cluster merge */
	NAC_CNT_NTLV_ELECTION_DROP,	/* count of NTLV dropped in NAN election */
	NAC_CNT_NTLV_TSF_ADOPT,		/* count of NTLV used for NAN TSF adoption */
	NAC_CNT_NTLV_LAST
} nac_cnt_enum_t;

#define NAC_MAX_CNT	(NAC_CNT_NTLV_LAST)

typedef struct nac_stats {
	uint32 nac_cnt[NAC_MAX_CNT];
} nac_stats_t;

typedef struct nan_adv_table {
	uint8  num_adv;
	uint8	adv_size;
	uint8	PAD[2];
	nan_adv_entry_t adv_nodes[0];
} nan_adv_table_t;

typedef struct wl_nan_role_cfg {
	wl_nan_role_t cfg_role;
	wl_nan_role_t cur_role;
	uint8 PAD[2];
	nan_sync_master_t target_master;
} wl_nan_role_cfg_t;

typedef struct wl_nan_role_config {
	wl_nan_role_t role;
	struct ether_addr target_master;
	uint8 PAD;
} wl_nan_role_config_t;

typedef int8 wl_nan_sd_optional_field_types_t;

/* Flag bits for Publish and Subscribe (wl_nan_sd_params_t flags) */

/* First 8 bits are blocked for mapping
 * against svc_control flag bits which goes out
 * as part of SDA attribute in air in SDF frames
 */
#define WL_NAN_RANGE_LIMITED		0x0040

/* Bits specific to Publish */

#define WL_NAN_PUB_UNSOLICIT	0x1000	/* Unsolicited Tx */
#define WL_NAN_PUB_SOLICIT		0x2000	/* Solicited Tx */
#define WL_NAN_PUB_BOTH			0x3000	/* Both the above */

#define WL_NAN_PUB_BCAST		0x4000	/* bcast solicited Tx only */
#define WL_NAN_PUB_EVENT		0x8000	/* Event on each solicited Tx */
#define WL_NAN_PUB_SOLICIT_PENDING	0x10000 /* Used for one-time solicited Publish */

#define WL_NAN_FOLLOWUP			0x20000 /* Follow-up frames */
#define WL_NAN_TX_FOLLOWUP             0x40000 /* host generated transmit Follow-up frames */
#define WL_NAN_TX_FOLLOWUP_UNSYNC      0x80000 /* host generated TX Follow-up in unsync disc */

/* Event generation indicator (default is continuous) */
#define WL_NAN_MATCH_ONCE		0x100000
#define WL_NAN_MATCH_NEVER		0x200000

#define WL_NAN_SVC_BCN_CARRY		0x400000 /* Include SVC in beacons */
#define WL_NAN_SVC_AUTO_RSP		0x800000 /* auto response SDF */

/* Nan Service Based control Flags */
/* If set, dev will take care of dp_resp */
#define WL_NAN_SVC_CTRL_AUTO_DPRESP               0x1000000
/* If set, host wont rec event "receive" */
#define WL_NAN_SVC_CTRL_SUPPRESS_EVT_RECEIVE      0x2000000
/* If set, host wont rec event "replied" */
#define WL_NAN_SVC_CTRL_SUPPRESS_EVT_REPLIED      0x4000000
/* If set, host wont rec event "terminated" */
#define WL_NAN_SVC_CTRL_SUPPRESS_EVT_TERMINATED   0x8000000

/* Pub/sub service supports suspension */
#define WL_NAN_SVC_CFG_SUSPENDABLE		  0x10000000

/* Bits specific to Subscribe */

#define WL_NAN_SUB_ACTIVE		0x1000 /* Active subscribe mode */
#define WL_NAN_SUB_MATCH_IF_SVC_INFO	0x2000 /* Service info in publish */

#define WL_NAN_TTL_UNTIL_CANCEL	0xFFFFFFFF /* Special values for time to live (ttl) parameter */

/*
 * Publish -  runs until first transmission
 * Subscribe - runs until first  DiscoveryResult event
 */
#define WL_NAN_TTL_FIRST	0

#define NAN_REKEY_PTK	0x01
#define NAN_REKEY_GTK	0x02
#define NAN_REKEY_MAX	(NAN_REKEY_PTK | NAN_REKEY_GTK)


/*
 * WL_NAN_CMD_SD_PARAMS
 */
typedef struct wl_nan_sd_params
{
	uint16	length; /* length including options */
	uint8	period; /* period of the unsolicited SDF xmission in DWs */
	uint8   awake_dw; /* interval between two DWs where SDF tx/rx are done */
	uint8	svc_hash[WL_NAN_SVC_HASH_LEN]; /* Hash for the service name */
	uint8	instance_id; /* Instance of the current service */
	int8	proximity_rssi; /* RSSI limit to Rx subscribe or pub SDF 0 no effect */
	uint32	flags; /* bitmap representing aforesaid optional flags */
	int32	ttl; /* TTL for this instance id, -1 will run till cancelled */
	tlv_t	optional[BCM_FLEX_ARRAY]; /* optional fields in the SDF  as appropriate */
} wl_nan_sd_params_t;

/*
 * WL_NAN_CMD_SD_PUBLISH_LIST
 * WL_NAN_CMD_SD_SUBSCRIBE_LIST
 */
typedef struct wl_nan_service_info
{
	uint8 instance_id;	/* Publish instance ID */
	uint8 service_hash[WL_NAN_SVC_HASH_LEN]; /* Hash for service name */
} wl_nan_service_info_t;

typedef struct wl_nan_service_list
{
	uint16 id_count; /* Number of registered publish/subscribe services */
	wl_nan_service_info_t list[BCM_FLEX_ARRAY]; /* service info defined by nan_service */
} wl_nan_service_list_t;

/*
 * WL_NAN_CMD_CFG_BCN_INTERVAL
 */
typedef uint16 wl_nan_disc_bcn_interval_t;

/*
 * WL_NAN_CMD_CFG_SDF_TXTIME
 */
typedef uint16 wl_nan_svc_disc_txtime_t;

/*
 * WL_NAN_CMD_CFG_STOP_BCN_TX
 */
typedef uint16 wl_nan_stop_bcn_tx_t;

/*
 * WL_NAN_CMD_CFG_FSM_TIMEOUT
 */
typedef uint32 wl_nan_fsm_timeout_t;


// nan rekey cfg iovar
typedef struct wl_nan_rekey {
	struct ether_addr ndi_addr;
	uint8 rekey_ctrl;
} wl_nan_rekey_t;


/*
 * WL_NAN_CMD_CFG_SID_BEACON
 */
typedef struct wl_nan_sid_beacon_control {
	uint8 sid_enable;	/* Flag to indicate the inclusion of Service IDs in Beacons */
	uint8 sid_count;	/* Limit for number of publish SIDs to be included in Beacons */
	uint8 sub_sid_count;	/* Limit for number of subscribe SIDs to be included in Beacons */
	uint8 PAD;
} wl_nan_sid_beacon_control_t;

/*
 * WL_NAN_CMD_CFG_DW_LEN
 */
typedef uint16 wl_nan_dw_len_t;

/*
 * WL_NAN_CMD_CFG_AWAKE_DW   Will be deprecated.
 */
typedef struct wl_nan_awake_dw {
	wl_nan_band_t band;	/* 0 - b mode 1- a mode */
	uint8 interval;		/* 1 or 2 or 4 or 8 or 16 */
	uint16 PAD;
} wl_nan_awake_dw_t;

/*
 * WL_NAN_CMD_CFG_AWAKE_DWS
 */
typedef struct wl_nan_awake_dws {
	uint8 dw_interval_2g;	/* 2G DW interval */
	uint8 dw_interval_5g;	/* 5G DW interval */
	uint16 PAD;
} wl_nan_awake_dws_t;

/* WL_NAN_CMD_SYNC_BCN_RSSI_NOTIF_THRESHOLD */

typedef struct wl_nan_rssi_notif_thld {
	int8 bcn_rssi_2g;
	int8 bcn_rssi_5g;
	int16 PAD;
} wl_nan_rssi_notif_thld_t;

/*
 * WL_NAN_CMD_CFG_SOCIAL_CHAN
 */
typedef struct wl_nan_social_channels {
	uint8 soc_chan_2g;	/* 2G social channel */
	uint8 soc_chan_5g;	/* 5G social channel */
	uint16 PAD;
} wl_nan_social_channels_t;

/*
 * WL_NAN_CMD_SD_CANCEL_PUBLISH
 * WL_NAN_CMD_SD_CANCEL_SUBSCRIBE
 */
typedef uint8 wl_nan_instance_id; /* Instance ID of an active publish instance */

/*
 * WL_NAN_CMD_SD_VND_INFO
 */
typedef struct wl_nan_sd_vendor_info
{
	uint16 length; /* Size in bytes of the payload following this field */
	uint8 data[];	/* Vendor Information */
} wl_nan_sd_vendor_info_t;

/*
 * WL_NAN_CMD_SD_STATS
 */
typedef struct wl_nan_sd_stats {
	uint32  sdftx;
	uint32  sdfrx;
	uint32  sdsrffail;
	uint32  sdrejrssi;
	uint32  sdfollowuprx;
	uint32  sdsubmatch;
	uint32  sdpubreplied;
	uint32  sdmftfail1;
	uint32  sdmftfail2;
	uint32  sdmftfail3;
	uint32  sdmftfail4;
}  wl_nan_sd_stats_t;

/* Flag bits for sd transmit message (wl_nan_sd_transmit_t flags) */

/* If set, host wont rec "tx status" event for tx-followup msg */
#define WL_NAN_FUP_SUPR_EVT_TXS      0x01
/* If set, insert shared key descr attribute in tx-followup msg */
#define WL_NAN_FUP_ADD_SKDA          0x02
/* If set, send tx-followup msg without peer availability check */
#define WL_NAN_FUP_UNSYNC_TX         0x04
/* more flags can be added here */

/*
 * WL_NAN_CMD_SD_TRANSMIT
 * WL_NAN_CMD_SD_FUP_TRANSMIT
 */
typedef struct wl_nan_sd_transmit {
	uint8	local_service_id;		/* Sender Service ID */
	uint8	requestor_service_id;		/* Destination Service ID */
	struct ether_addr destination_addr;	/* Destination MAC */
	uint16	token;				/* follow_up_token when a follow-up
						 * msg is queued successfully
						 */
	uint8	priority;			/* requested relative prio */
	uint8	flags;                          /* Flags for tx follow-up msg */
	uint16	opt_len;			/* total length of optional tlvs */
	uint8	opt_tlv[];			/* optional tlvs in bcm_xtlv_t type */
} wl_nan_sd_transmit_t;

/* disc cache timeout for a cache entry */
typedef uint16 wl_nan_disc_cache_timeout_t;

/*
 * WL_NAN_CMD_SYNC_TSRESERVE
 */
/** time slot */
#define NAN_MAX_TIMESLOT	32
typedef struct wl_nan_timeslot {
	uint32	abitmap; /**< available bitmap */
	uint32 chanlist[NAN_MAX_TIMESLOT];
} wl_nan_timeslot_t;

/*
 * Deprecated
 *
 * WL_NAN_CMD_SYNC_TSRELEASE
 */
typedef uint32 wl_nan_ts_bitmap_t;

/* nan passive scan params */
#define NAN_SCAN_MAX_CHCNT 8
/* nan merge scan params */
typedef struct wl_nan_scan_params {
	/* dwell time of discovery channel corresponds to band_idx.
	 * If set to 0 then fw default will be used.
	 */
	uint16 dwell_time;
	/* scan period of  discovery channel corresponds to band_idx.
	 * If set to 0 then fw default will be used.
	 */
	uint16 scan_period;
	/* band index of discovery channel */
	uint8 band_index;
	uint8 PAD;
} wl_nan_scan_params_t;

/*
 * WL_NAN_CMD_DBG_SCAN
 */
typedef struct wl_nan_dbg_scan {
	struct ether_addr cid;
	uint8 PAD[2];
} wl_nan_dbg_scan_t;

/* NAN_DBG_LEVEL */
typedef struct wl_nan_dbg_level {
	uint32 nan_err_level; /* for Error levels */
	uint32 nan_dbg_level; /* for bebug logs and trace */
	uint32 nan_info_level; /* for dumps like prhex */
} wl_nan_dbg_level_t;

/*
 * WL_NAN_CMD_DBG_EVENT_MASK
 */
typedef uint32 wl_nan_event_mask_t;

/*
 * WL_NAN_CMD_DBG_EVENT_CHECK
 */
typedef uint8 wl_nan_dbg_ifname[BCM_MSG_IFNAME_MAX];

/*
 * WL_NAN_CMD_DBG_DUMP
 * WL_NAN_CMD_DBG_CLEAR
 */
enum wl_nan_dbg_dump_type {
	WL_NAN_DBG_DT_RSSI_DATA = 1,
	WL_NAN_DBG_DT_STATS_DATA = 2,
	/*
	 * Additional enums before this line
	 */
	WL_NAN_DBG_DT_INVALID
};
typedef int8 wl_nan_dbg_dump_type_t;

/** various params and ctl swithce for nan_debug instance  */
/*
 * WL_NAN_CMD_DBG_DEBUG
 */
typedef struct wl_nan_debug_params {
	uint16	cmd;	/**< debug cmd to perform a debug action */
	uint16	status;
	uint32	msglevel; /**< msg level if enabled */
	uint8	enabled; /**< runtime debuging enabled */
	uint8 collect;
	uint8 PAD[2];
} wl_nan_debug_params_t;


typedef struct wl_nan_sched_svc_timeslot_s {
	uint32 abitmap; /* availability bitmap */
	uint32 chanlist[NAN_MAX_TIMESLOT];
	uint8  res; /* resolution: 0 = 16ms, 1 = 32ms, 2 = 64ms 3 = reserved. REfer NAN spec */
	uint8  mapid; /* mapid from NAN spec. Used to differentiate 2G Vs 5G band */
	uint8 PAD[2];
} wl_nan_sched_svc_timeslot_t;

/*
 * WL_NAN_CMD_DATA_DP_IDLE_PERIOD
 */
typedef uint16 wl_nan_ndp_idle_period_t;

/*
 * WL_NAN_CMD_DATA_DP_HB_DURATION
 */
typedef uint16 wl_nan_ndp_hb_duration_t;

/* nan cmd IDs */
enum wl_nan_cmds {
	 /* nan cfg /disc & dbg ioctls */
	WL_NAN_CMD_ENABLE = 1,
	WL_NAN_CMD_ATTR = 2,
	WL_NAN_CMD_NAN_JOIN = 3,
	WL_NAN_CMD_LEAVE = 4,
	WL_NAN_CMD_MERGE = 5,
	WL_NAN_CMD_STATUS = 6,
	WL_NAN_CMD_TSRESERVE = 7,
	WL_NAN_CMD_TSSCHEDULE = 8,
	WL_NAN_CMD_TSRELEASE = 9,
	WL_NAN_CMD_OUI = 10,
	WL_NAN_CMD_OOB_AF = 11,
	WL_NAN_CMD_SCAN_PARAMS = 12,

	WL_NAN_CMD_COUNT = 15,
	WL_NAN_CMD_CLEARCOUNT = 16,

	/*  discovery engine commands */
	WL_NAN_CMD_PUBLISH = 20,
	WL_NAN_CMD_SUBSCRIBE = 21,
	WL_NAN_CMD_CANCEL_PUBLISH = 22,
	WL_NAN_CMD_CANCEL_SUBSCRIBE = 23,
	WL_NAN_CMD_TRANSMIT = 24,
	WL_NAN_CMD_CONNECTION = 25,
	WL_NAN_CMD_SHOW = 26,
	WL_NAN_CMD_STOP = 27,	/* stop nan for a given cluster ID  */
	/*  nan debug iovars & cmds  */
	WL_NAN_CMD_SCAN = 47,
	WL_NAN_CMD_SCAN_RESULTS = 48,
	WL_NAN_CMD_EVENT_MASK = 49,
	WL_NAN_CMD_EVENT_CHECK = 50,
	WL_NAN_CMD_DUMP = 51,
	WL_NAN_CMD_CLEAR = 52,
	WL_NAN_CMD_RSSI = 53,

	WL_NAN_CMD_DEBUG = 60,
	WL_NAN_CMD_TEST1 = 61,
	WL_NAN_CMD_TEST2 = 62,
	WL_NAN_CMD_TEST3 = 63,
	WL_NAN_CMD_DISC_RESULTS = 64,
	/* nan 2.0 data path commands */
	WL_NAN_CMD_DATAPATH = 65
};

/*   NAN DP interface commands  */
enum wl_nan_dp_cmds {
	/* nan 2.0 ioctls */
	WL_NAN_CMD_DP_CAP = 1000,
	WL_NAN_CMD_DP_CONFIG = 1001,
	WL_NAN_CMD_DP_CREATE = 1002,
	WL_NAN_CMD_DP_AUTO_CONNECT = 1003,
	WL_NAN_CMD_DP_DATA_REQ = 1004,
	WL_NAN_CMD_DP_DATA_RESP = 1005,
	WL_NAN_CMD_DP_SCHED_UPD = 1006,
	WL_NAN_CMD_DP_END = 1007,
	WL_NAN_CMD_DP_CONNECT = 1008,
	WL_NAN_CMD_DP_STATUS = 1009
};

/* TODO Should remove this fixed length */
#define WL_NAN_DATA_SVC_SPEC_INFO_LEN 32 /* arbitrary */
#define WL_NAN_DP_MAX_SVC_INFO	      0xFF
#define WL_NAN_DATA_NDP_INST_SUPPORT 16

/* Nan flags (16 bits) */
#define WL_NAN_DP_FLAG_SVC_INFO			0x0001u
#define WL_NAN_DP_FLAG_CONFIRM			0x0002u
#define WL_NAN_DP_FLAG_EXPLICIT_CFM		0x0004u
#define WL_NAN_DP_FLAG_SECURITY			0x0008u
#define WL_NAN_DP_FLAG_HAST_NDL_COUNTER		0x0010u /* Host assisted NDL counter */
#define WL_NAN_DP_FLAG_IGNORE_NDC_BITMAP	0x0040u /* Ignore NDC bitmap as per host config */
/* If it is a rekey or new NDP setup 0 = new NDP, 1 = REKEY */
#define WL_NAN_DP_FLAG_REKEY		0x0020u
/* NAN Datapath host status */
#define WL_NAN_DP_STATUS_ACCEPTED     1
#define WL_NAN_DP_STATUS_REJECTED     0

/* to be done */
typedef struct wl_nan_dp_cap {
	uint8 tbd;
} wl_nan_dp_cap_t;


/** The service hash (service id) is exactly this many bytes. */
#define WL_NAN_SVC_HASH_LEN	6
/** Number of hash functions per bloom filter */
#define WL_NAN_HASHES_PER_BLOOM 4
/* no. of max last disc results */
#define WL_NAN_MAX_DISC_RESULTS	3

/* NAN security related defines */
/* NCS-SK related */
#define WL_NAN_NCS_SK_PMK_LEN	32
#define WL_NAN_NCS_SK_PMKID_LEN	16

/* recent discovery results */
typedef struct wl_nan_disc_result_s
{
	wl_nan_instance_id_t instance_id;	/* instance id of pub/sub req */
	wl_nan_instance_id_t peer_instance_id;	/* peer instance id of pub/sub req/resp */
	uint8 svc_hash[WL_NAN_SVC_HASH_LEN];	/* service descp string */
	struct ether_addr peer_mac;	/* peer mac address */
} wl_nan_disc_result_t;

/* list of recent discovery results */
typedef struct wl_nan_disc_results_s
{
	wl_nan_disc_result_t disc_result[WL_NAN_MAX_DISC_RESULTS];
} wl_nan_disc_results_list_t;

/* nan 1.0 events */
/* To be deprecated - will be replaced by event_disc_result */
typedef struct wl_nan_ev_disc_result {
	wl_nan_instance_id_t pub_id;
	wl_nan_instance_id_t sub_id;
	struct ether_addr pub_mac;
	uint8 opt_tlvs[0];
} wl_nan_ev_disc_result_t;

typedef struct wl_nan_event_disc_result {
	wl_nan_instance_id_t pub_id;
	wl_nan_instance_id_t sub_id;
	struct ether_addr pub_mac;
	int8		publish_rssi;		/* publisher RSSI */
	uint8		attr_num;
	uint16		attr_list_len;	/* length of the all the attributes in the SDF */
	uint8		attr_list[0];	/* list of NAN attributes */
} wl_nan_event_disc_result_t;

typedef struct wl_nan_ev_p2p_avail {
	struct ether_addr sender;
	struct ether_addr p2p_dev_addr;
	uint8 dev_role;
	uint8 resolution;
	uint8 repeat;
	uint8 PAD[3];
	chanspec_t chanspec;
	uint32 avail_bmap;
} wl_nan_ev_p2p_avail_t;

typedef struct wl_nan_ev_suspension_s {
	wl_nan_suspension_status_t status;
} wl_nan_ev_suspension_t;

typedef struct wl_nan_suspend_resume_req_s {
	wl_nan_instance_id_t service_id;
	uint8 suspend;
} wl_nan_suspend_resume_req_t;

/* Various nan suspend states */
typedef enum wl_nan_cmn_suspend_state_s {
	WL_NAN_CMN_SUSPEND_NONE		  = 0,	  /* Default state */
	WL_NAN_CMN_SUSPEND_PENDING	  = 1,	  /* Peding because of other active session */
	WL_NAN_CMN_SUSPEND_INPROG	  = 2,	  /* all sessions suspended and suspend in prog */
	WL_NAN_CMN_SUSPEND_ENTERED	  = 3,	  /* NAN in suspended state */
	WL_NAN_CMN_SUSPEND_EXIT_INPROG	  = 4	  /* Resume came and suspend exit started */
} wl_nan_cmn_suspend_state_t;

/*
* discovery interface event structures *
*/

enum wl_nan_oob_af_flags {
	WL_NAN_OOB_AF_FLAG_SEND_EVENT	= 0x0001, /* send tx status event */
	WL_NAN_OOB_AF_FLAG_FLUSH_PCB	= 0x0002, /* flush PCB */
	WL_NAN_OOB_AF_FLAG_ADD_DCAP	= 0x0004, /* add dev cap attr into NAF body */
	WL_NAN_OOB_AF_FLAG_ADD_ELMT	= 0x0008, /* add elmt container attr into NAF body */
	WL_NAN_OOB_AF_FLAG_MFP_REQUIRED	= 0x0010  /* MFP required */
};
typedef uint16 wl_nan_oob_af_flags_t;

/* mandatory parameters for OOB action frame */
typedef struct wl_nan_oob_af_params_s
{
	uint8 fup_lcl_id;	/* local instance ID of follow-up SDF */
	uint8 fup_peer_id;	/* peer instance ID of follow-up SDF */
	uint8 sdf_type;		/* represented by service control type NAN_SC_XXX */
	uint8 unused_uint8;
	uint32 unused_uint32;
	struct ether_addr bssid;
	struct ether_addr dest;
	uint32 pkt_lifetime;
	uint8 n2af_sub_type;	/* NAN2 AF sub type */
	uint8 retry_cnt;	/* NAF tx retry (not 802.11 re-tx) */
	uint16 token;		/* NAN host seq num */
	uint16 flags;		/* wl_nan_oob_af_flags_t */
	uint32 fsm_id;		/* unique fsm id */
	uint16 payload_len;
	uint8 payload[BCM_FLEX_ARRAY];
} wl_nan_oob_af_params_t;

/* NAN Ranging */

/* Bit defines for global flags */
#define WL_NAN_RANGING_ENABLE		1 /**< enable RTT */
#define WL_NAN_RANGING_RANGED		2 /**< Report to host if ranged as target */
typedef struct nan_ranging_config {
	uint32 chanspec;		/**< Ranging chanspec */
	uint16 timeslot;		/**< NAN RTT start time slot  1-511 */
	uint16 duration;		/**< NAN RTT duration in ms */
	struct ether_addr allow_mac;	/**< peer initiated ranging: the allowed peer mac
					 * address, a unicast (for one peer) or
					 * a broadcast for all. Setting it to all zeros
					 * means responding to none,same as not setting
					 * the flag bit NAN_RANGING_RESPOND
					 */
	uint16 flags;
} wl_nan_ranging_config_t;

/** list of peers for self initiated ranging */
/** Bit defines for per peer flags */
#define WL_NAN_RANGING_REPORT (1<<0)	/**< Enable reporting range to target */
typedef struct nan_ranging_peer {
	uint32 chanspec;		/**< desired chanspec for this peer */
	uint32 abitmap;			/**< available bitmap */
	struct ether_addr ea;		/**< peer MAC address */
	uint8 frmcnt;			/**< frame count */
	uint8 retrycnt;			/**< retry count */
	uint16 flags;			/**< per peer flags, report or not */
	uint16 PAD;
} wl_nan_ranging_peer_t;
typedef struct nan_ranging_list {
	uint8 count;			/**< number of MAC addresses */
	uint8 num_peers_done;		/**< host set to 0, when read, shows number of peers
					 * completed, success or fail
					 */
	uint8 num_dws;			/**< time period to do the ranging, specified in dws */
	uint8 reserve;			/**< reserved field */
	wl_nan_ranging_peer_t rp[BCM_FLEX_ARRAY];	/**< variable length array of peers */
} wl_nan_ranging_list_t;

/* ranging results, a list for self initiated ranging and one for peer initiated ranging */
/* There will be one structure for each peer */
#define WL_NAN_RANGING_STATUS_SUCCESS		1
#define WL_NAN_RANGING_STATUS_FAIL		2
#define WL_NAN_RANGING_STATUS_TIMEOUT		3
#define WL_NAN_RANGING_STATUS_ABORT		4 /**< with partial results if sounding count > 0 */
typedef struct nan_ranging_result {
	uint8 status;			/**< 1: Success, 2: Fail 3: Timeout 4: Aborted */
	uint8 sounding_count;		/**< number of measurements completed (0 = failure) */
	struct ether_addr ea;		/**< initiator MAC address */
	uint32 chanspec;		/**< Chanspec where the ranging was done */
	uint32 timestamp;		/**< 32bits of the TSF timestamp ranging was completed at */
	uint32 distance;		/**< mean distance in meters expressed as Q4 number.
					 * Only valid when sounding_count > 0. Examples:
					 * 0x08 = 0.5m
					 * 0x10 = 1m
					 * 0x18 = 1.5m
					 * set to 0xffffffff to indicate invalid number
					 */
	int32 rtt_var;			/**< standard deviation in 10th of ns of RTTs measured.
					 * Only valid when sounding_count > 0
					 */
	struct ether_addr tgtea;	/**< target MAC address */
	uint8 PAD[2];
} wl_nan_ranging_result_t;
typedef struct nan_ranging_event_data {
	uint8 mode;			/**< 1: Result of host initiated ranging */
					/* 2: Result of peer initiated ranging */
	uint8 reserved;
	uint8 success_count;		/**< number of peers completed successfully */
	uint8 count;			/**< number of peers in the list */
	wl_nan_ranging_result_t rr[BCM_FLEX_ARRAY];	/**< variable array of ranging peers */
} wl_nan_ranging_event_data_t;

enum {
	WL_NAN_STATS_RSSI = 1,
	WL_NAN_STATS_DATA = 2,
	WL_NAN_STATS_DP = 3,
/*
 * ***** ADD before this line ****
 */
	WL_NAN_STATS_INVALID
};
typedef struct wl_nan_dp_stats {
	uint32 tbd; /* TBD */
} wl_nan_dp_stats_t;

typedef struct wl_nan_stats {
	/* general */
	uint32 cnt_dw; /* DW slots */
	uint32 cnt_disc_bcn_sch; /* disc beacon slots */
	uint32 cnt_amr_exp; /* count of ambtt expiries resetting roles */
	uint32 cnt_bcn_upd; /* count of beacon template updates */
	uint32 cnt_bcn_tx; /* count of sync & disc bcn tx */
	uint32 cnt_bcn_rx; /* count of sync & disc bcn rx */
	uint32 cnt_sync_bcn_tx; /* count of sync bcn tx within DW */
	uint32 cnt_disc_bcn_tx; /* count of disc bcn tx */
	uint32 cnt_sdftx_bcmc; /* count of bcast/mcast sdf tx */
	uint32 cnt_sdftx_uc; /* count of unicast sdf tx */
	uint32 cnt_sdftx_fail; /* count of unicast sdf tx fails */
	uint32 cnt_sdf_rx; /* count of  sdf rx */
	/* NAN roles */
	uint32 cnt_am; /* anchor master */
	uint32 cnt_master; /* master */
	uint32 cnt_nms; /* non master sync */
	uint32 cnt_nmns; /* non master non sync */
	/* TX */
	uint32 cnt_err_txtime; /* txtime in sync bcn frame not a multiple of dw intv */
	uint32 cnt_err_unsch_tx; /* tx while not in DW/ disc bcn slot */
	uint32 cnt_err_bcn_tx; /*  beacon tx error */
	uint32 cnt_sync_bcn_tx_miss; /* no. of times time delta between 2 cosequetive
						* sync beacons is more than expected
						*/
	/* MSCH */
	uint32 cnt_err_msch_reg; /* error is Dw/disc reg with msch */
	uint32 cnt_err_wrong_ch_cb; /* count of msch calbacks in wrong channel */
	uint32 cnt_dw_skip;	/* count of DW rejected */
	uint32 cnt_disc_skip; /* count of disc bcn rejected */
	uint32 cnt_dw_start_early; /* msch cb not at registered time */
	uint32 cnt_dw_start_late; /* no. of delays in slot start */
	/* SCANS */
	uint32 cnt_mrg_scan; /* count of merge scans completed */
	uint32 cnt_err_ms_rej; /* number of merge scan failed */
	uint32 cnt_scan_results; /* no. of nan beacons scanned */
	uint32 cnt_join_scan_rej; /* no. of join scans rejected */
	uint32 cnt_nan_scan_abort; /* no. of join scans rejected */
	/* enable/disable */
	uint32 cnt_nan_enab; /* no. of times nan feature got enabled */
	uint32 cnt_nan_disab; /* no. of times nan feature got disabled */
	uint32 cnt_sync_bcn_rx; /* count of sync bcn rx within DW */
	uint32 cnt_sync_bcn_rx_tu[3]; /* Delta bw the tsf in bcn & remote */
	uint32 cnt_bcn_tx_out_dw;  /* TX sync beacon outside dw */
	uint32 cnt_role_am_dw; /* anchor master role due to dw */
	uint32 cnt_am_hop_err; /* wrong hopcount set for AM */
} wl_nan_stats_t;

#define WL_NAN_MAC_MAX_NAN_PEERS 6
#define WL_NAN_MAC_MAX_RSSI_DATA_PER_PEER  10

typedef struct wl_nan_nbr_rssi {
	uint8 rx_chan; /* channel number on which bcn rcvd */
	uint8 PAD[3];
	int32 rssi_raw;  /* received rssi value */
	int32 rssi_avg;  /* normalized rssi value */
} wl_nan_peer_rssi_t;

typedef struct wl_nan_peer_rssi_entry {
	struct ether_addr mac;  /* peer mac address */
	uint8 flags;   /* TODO:rssi data order: latest first, oldest first etc */
	uint8 rssi_cnt;   /* rssi data sample present */
	wl_nan_peer_rssi_t rssi[WL_NAN_MAC_MAX_RSSI_DATA_PER_PEER]; /* RSSI data frm peer */
} wl_nan_peer_rssi_entry_t;

#define WL_NAN_PEER_RSSI      0x1
#define WL_NAN_PEER_RSSI_LIST 0x2

typedef struct wl_nan_nbr_rssi_data {
	uint8 flags;   /* this is a list or single rssi data */
	uint8 peer_cnt; /* number of peers */
	uint16 PAD; /* padding */
	wl_nan_peer_rssi_entry_t peers[BCM_FLEX_ARRAY]; /* peers data list */
} wl_nan_peer_rssi_data_t;

/* WL_NAN_CMD_DBG_DUMP, GET Resp */
typedef struct wl_nan_dbg_dump_rsp {
	wl_nan_dbg_dump_type_t dump_type; /* dump data type */
	uint8 PAD[3];
	union {
		wl_nan_peer_rssi_data_t peer_rssi;
		wl_nan_stats_t		nan_stats;
	} u;
} wl_nan_dbg_dump_rsp_t;

enum nan_termination_status {
	NAN_TERM_REASON_INVALID = 1,
	NAN_TERM_REASON_TIMEOUT = 2,
	NAN_TERM_REASON_USER_REQ = 3,
	NAN_TERM_REASON_FAILURE = 4,
	NAN_TERM_REASON_COUNT_REACHED = 5,
	NAN_TERM_REASON_DE_SHUTDOWN = 6,
	NAN_TERM_REASON_DISABLE_IN_PROGRESS = 7
};

/* nan2 data iovar */
/* nan2 qos */
typedef struct wl_nan_dp_qos
{
	uint8 tid;
	uint8 PAD;
	uint16 pkt_size;
	uint16 mean_rate;
	uint16 svc_interval;
} wl_nan_dp_qos_t;

#define WL_NAN_NDL_QOS_MAX_LAT_NO_PREF 0xFFFF

/* nan2 qos */
typedef struct wl_nan_ndl_qos
{
	uint8 min_slots;	/* min slots per dw interval */
	uint8 PAD;
	uint16 max_latency;	/* max latency */
} wl_nan_ndl_qos_t;

/* ndp config */
typedef struct wl_nan_ndp_config
{
	uint8 ndp_id;
	uint8 pub_id;
	struct ether_addr pub_addr;
	struct ether_addr data_addr;	/* configure local data addr */
	struct ether_addr init_data_addr;	/* initiator data addr */
	uint8 svc_spec_info[WL_NAN_DATA_SVC_SPEC_INFO_LEN];
	wl_nan_dp_qos_t qos;
	uint16 avail_len;
	uint8 PAD[3];
	uint8 data[BCM_FLEX_ARRAY];
} wl_nan_ndp_config_t;

/* nan2 device capabilities */
typedef struct wl_nan_ndp_oper_cfg {
	uint8 awake_dw_2g;
	uint8 awake_dw_5g;
	uint8 bands_supported;
	uint8 op_mode;
} wl_nan_ndp_oper_cfg_t;

typedef uint8 wl_nan_ndp_ndpid_t;
typedef uint8 wl_nan_ndp_conn_t;

#define WL_NAN_INVALID_NDPID	0	/* reserved ndp id */

typedef struct wl_nan_dp_req {
	uint8 type;		    /* 0- unicast 1 - multicast */
	uint8 pub_id;		    /* Publisher ID */
	uint16 flags;
	struct ether_addr peer_mac; /* Peer's NMI addr */
	struct ether_addr mcast_mac; /* Multicast addr */
	struct ether_addr ndi;
	wl_nan_dp_qos_t qos;
	wl_nan_ndl_qos_t ndl_qos;	/* ndl qos */
	uint8 tlv_params[];	/* xtlv parameters for command */
} wl_nan_dp_req_t;

/* NAN pairing flags */
/* Support for npk caching */
#define WL_NAN_PAIRING_FLAGS_NPK_CACHING	(1u << 0u)
/* Support for pairing setup */
#define WL_NAN_PAIRING_FLAGS_SETUP		(1u << 1u)

enum wl_nan_pairing_role {
	WL_NAN_PAIRING_ROLE_INITIATOR = 0,
	WL_NAN_PAIRING_ROLE_RESPONDER = 1,
	WL_NAN_PAIRING_ROLE_MAX = WL_NAN_PAIRING_ROLE_RESPONDER
};

typedef uint8 wl_nan_pairing_role_t;

enum wl_nan_pairing_type {
	WL_NAN_PAIRING_TYPE_SETUP = 0,
	WL_NAN_PAIRING_TYPE_VERIFICATION = 1,
	WL_NAN_PAIRING_TYPE_MAX = WL_NAN_PAIRING_TYPE_VERIFICATION
};

typedef uint8 wl_nan_pairing_type_t;

typedef uint8 wl_pasn_policy_t;

typedef uint8 wl_nan_pairing_state_t;
enum wl_nan_pairing_state {
	WL_NAN_PAIRING_STATUS_IDLE	= 0,
	WL_NAN_PAIRING_STATUS_CREATED	= 1,
	WL_NAN_PAIRING_STATUS_PAIRED	= 2,
	WL_NAN_PAIRING_STATUS_FAILED	= 3,
	WL_NAN_PAIRING_STATUS_UNKNOWN	= 4,
	WL_NAN_PAIRING_STATUS_MAX	= 4
};

/* WL_NAN_XTLV_GEN_PEER_PAIRING_STATS */
typedef struct wl_nan_peer_stats_pairing {
	wl_nan_pairing_role_t	role;		/* Current Role - Initiator or Responder */
	wl_nan_pairing_state_t	state;		/* Current paired state */
	uint8			sid;		/* current session id */
	wl_nan_pairing_type_t	type;		/* Current type setup or verification */
	wl_pasn_policy_t	policy;		/* Current policy PASN Auth or non-Auth */
	uint8			csid;		/* Selected CSID */
	uint8			PAD[2];
	uint16			sessions_pass;	/* Number of successful sessions */
	uint16			sessions_fail;	/* Number of failed sessions */
} wl_nan_peer_stats_pairing_t;

typedef uint8 wl_nan_pairing_bootstr_meth_t;

typedef struct wl_nan_pairing_oper {
	uint32 key_lifetime_ms;		/* PTKSA timeout interval configured. */
	struct ether_addr peer_addr;	/* Peer's addr */
	uint16 flags;			/* bit 0: NPK/NIK caching enabled */
	wl_nan_pairing_role_t role;	/* operation - Initiator or responder */
	wl_nan_pairing_type_t type;	/* Setup or verification */
	uint8 pub_id;			/* Publisher ID */
	uint8 pasn_policy;		/* specify policy supported by the session */
	uint8 pasn_csid;		/* PASN Cipher suite ID */
	uint8 response_code;		/* Response code ACCCEPT-0/REJECT-1 in pairing resp cmd */
	uint16 pairing_id;		/* Pairing instance ID for pairing resp */
	uint8 tlv_params[];		/* xtlv parameters for command */
} wl_nan_pairing_oper_t;

/* Pairing end IOVAR */
typedef struct wl_nan_pairing_end {
	uint16 pairing_id;		/* Pairing id sent in pairing establish */
	uint8 PAD[2];
} wl_nan_pairing_end_t;

/* uint8 - nan svc pairing configuration related flags */
/* Configuration flag to indicate if service supports pairing setup */
#define WL_NAN_SVC_CFG_ENAB_PAIRING_SETUP          (1u << 0u)
/* Configuration flag to indicate if service supports pairing cache */
#define WL_NAN_SVC_CFG_ENAB_PAIRING_CACHE          (1u << 1u)
/* Configuration flag to indicate if service supports pairing verification */
#define WL_NAN_SVC_CFG_ENAB_PAIRING_VERIFICATION   (1u << 2u)

/* Service specific Pairing configurations wrapped in WL_NAN_XTLV_PAIRING_CFG */
typedef struct wl_nan_pairing_config {
	uint16	supported_bootstrapping_methods; /* As defined in proto/nan.h */
	uint8	flags;				 /* WL_NAN_SVC_CFG_* flags defined above */
	uint8	pad;
} wl_nan_pairing_config_t;

typedef uint8 wl_nan_pairing_peer_info_operation_t;
enum wl_nan_pairing_peer_info_operation {
	WL_NAN_PAIRING_PEER_INFO_ADD		= 0,
	WL_NAN_PAIRING_PEER_INFO_DELETE		= 1,
	WL_NAN_PAIRING_PEER_INFO_DELETE_ALL	= 2
};

/* WL_NAN_CMD_PAIRING_PEER_INFO */
typedef struct wl_nan_pairing_peer_info {
	struct ether_addr peer_addr;	/* Peer's addr */
	uint8 pub_id;			/* Publisher ID */
	uint8 operation;		/* add , delete */
	uint16 flags;			/* Flags for auto response setup/verify, csid/s */
	uint8 pad[2];
	uint8 tlv_params[];		/* xtlv parameters for command */
} wl_nan_pairing_peer_info_t;

/* Common event structure for Nan pairing
 * Used for sending pairing Indication, Response, Establish events
 */
typedef struct wl_nan_ev_pairing_cmn {
	uint8 type;
	uint8 security;
	struct ether_addr peer_nmi;
	uint8 status;
	uint8 role;
	uint16 opt_tlv_len;
	uint8 opt_tlvs[];
} wl_nan_ev_pairing_cmn_t;

typedef uint16 wl_nan_pairing_flags_t;
typedef uint16 wl_nan_pairing_sid_t;

/* TODO  Need to replace ndp_id with lndp_id */
/* Return structure to data req IOVAR */
typedef struct wl_nan_dp_req_ret {
	struct ether_addr indi;	    /* Initiators data mac addr */
	uint8 ndp_id;		    /* Initiators ndpid */
	uint8 PAD;
} wl_nan_dp_req_ret_t;

typedef struct wl_nan_dp_resp {
	uint8 type;			/* 0- unicast 1 - multicast */
	uint8 status;			/* Accepted or Rejected */
	uint8 reason_code;
	/* Local NDP ID for unicast, mc_id for multicast, 0 for implicit NMSG */
	uint8 ndp_id;			/* can be host indp id also */
	wl_nan_dp_qos_t qos;
	/* Initiator data address for unicast or multicast address for multicast */
	struct ether_addr mac_addr;
	struct ether_addr ndi;
	uint16 flags;
	wl_nan_ndl_qos_t ndl_qos;	/* ndl qos */
	uint8 tlv_params[];		/* xtlv parameters for command */
} wl_nan_dp_resp_t;

/* Return structure to data resp IOVAR */
typedef struct wl_nan_dp_resp_ret {
	uint8 nmsgid;		    /* NMSG ID or for multicast else 0 */
	uint8 PAD[3];
} wl_nan_dp_resp_ret_t;

typedef struct wl_nan_dp_conf {
	uint8 lndp_id;	/* can be host ndp id */
	uint8 status;	/* Accepted or Rejected */
	uint8 PAD[2];
} wl_nan_dp_conf_t;

/* WL_NAN_CMD_DATA_DATAEND */
typedef struct wl_nan_dp_end
{
	uint8 lndp_id;	/* can be host ndp id */
	uint8 status;
	struct ether_addr mac_addr;	/* initiator's ndi */
} wl_nan_dp_end_t;

/* wl_nan_dp_end_v2_t flags */
#define WL_NAN_DP_END_V2_FLAG_NO_TX		0x0001u
#define WL_NAN_DP_END_V2_FLAG_NO_RETRY		0x0002u
#define WL_NAN_DP_END_V2_FLAG_NO_PEER_CACHE	0x0004u

/* WL_NAN_CMD_DATA_DATAEND_V2 */
typedef struct wl_nan_dp_end_v2
{
	uint8 ndp_id;		/* initiator's NDP ID or local NDP ID */
	uint8 status;
	struct ether_addr indi;	/* initiator's ndi */
	uint16 flags;		/* flags to enable/disable retry, etc. */
	uint16 opt_len;		/* total length of optional tlvs */
	uint8 opt_tlv[];	/* optional tlvs in bcm_xtlv_t type */
} wl_nan_dp_end_v2_t;

typedef struct wl_nan_dp_schedupd {
	uint8 type;		/* 0: unicast, 1: multicast */
	uint8 flags;
	struct ether_addr addr;	/* peer NMI or multicast addr */
	wl_nan_dp_qos_t qos;
	wl_nan_ndl_qos_t ndl_qos;	/* ndl qos */
	uint8 map_id;
	uint8 PAD;
	uint16 hostseq;
} wl_nan_dp_schedupd_t;

/* set: update with notification, unset: NDL setup handshake */
#define WL_NAN_DP_SCHEDUPD_NOTIF (1 << 0)

/* list ndp ids */
typedef struct wl_nan_ndp_id_list {
	uint16 ndp_count;
	uint8 lndp_id[];
} wl_nan_ndp_id_list_t;

/* nan2 status */
typedef struct ndp_session {
	uint8 lndp_id;
	uint8 state;
	uint8 pub_id;
	uint8 PAD;
} ndp_session_t;

typedef struct wl_nan_ndp_status {
	struct ether_addr peer_nmi;
	struct ether_addr peer_ndi;
	ndp_session_t session;
	struct ether_addr lndi;
	uint8 PAD[2];
} wl_nan_ndp_status_t;

#define NAN_DP_OPAQUE_INFO_DP_RESP 0x01
#define NAN_DP_OPAQUE_INFO_DP_CONF 0x02

typedef struct wl_nan_dp_opaque_info {
	uint8 frm_mask;   /* dp_resp / dp_conf as defined above. */
	struct ether_addr initiator_ndi;  /* NDI to match in the dp_req. */
	uint8 pub_id;     /* publish id where the opaque data is included. */
	uint8 len;        /* len of opaque_info[]. */
	uint8 PAD[3];
	uint8 opaque_info[0];
} wl_nan_dp_opaque_info_t;

/* events */
#define NAN_DP_SESSION_UNICAST         0
#define NAN_DP_SESSION_MULTICAST       1
#define NAN_DP_SECURITY_NONE           0
#define NAN_DP_SECURITY_CSID           1
#define NAN_DP_SECURITY_MK             2
#define WL_NAN_DATA_NMSGID_LEN    8 /* 8 bytes as per nan spec */

/* DP TERM event causes */
#define WL_NAN_DP_TERM_WITH_INACTIVITY		1u
#define WL_NAN_DP_TERM_WITH_FSM_DESTROY		2u
#define WL_NAN_DP_TERM_WITH_PEER_DP_END		3u
#define WL_NAN_DP_TERM_WITH_STALE_NDP		4u
#define WL_NAN_DP_TERM_WITH_DISABLE		5u
#define WL_NAN_DP_TERM_WITH_NDI_DEL		6u
#define WL_NAN_DP_TERM_WITH_PEER_HB_FAIL	7u
#define WL_NAN_DP_TERM_WITH_HOST_IOVAR		8u
#define WL_NAN_DP_TERM_WITH_ESTB_FAIL		9u
#define WL_NAN_DP_TERM_WITH_SCHED_REJECT	10u

/* Common event structure for Nan Datapath
 * Used for sending NDP Indication, Response, Confirmation, Securty Install and Establish events
 */
typedef struct wl_nan_ev_datapath_cmn {
	uint8 type;
	/* ndp_id is valid only if type is unicast */
	uint8 ndp_id;
	uint8 pub_id;
	uint8 security;
	/* Following two fields are valid only if type is unicast */
	struct ether_addr initiator_ndi;
	struct ether_addr responder_ndi;
	struct ether_addr peer_nmi;
	uint8 status;
	uint8 role;
	/* Following two fields are valid only if type is multicast */
	uint8 nmsg_id[WL_NAN_DATA_NMSGID_LEN];
	uint8 mc_id;
	uint8 event_cause;
	uint16 opt_tlv_len;
	uint8 opt_tlvs[];
} wl_nan_ev_datapath_cmn_t;

/* this is obsolete - DON'T USE */
typedef struct wl_nan_ev_datapath_end {
	uint8 ndp_id;
	uint8 status;
	uint8 PAD[2];
	struct ether_addr peer_nmi;
	struct ether_addr peer_ndi;
} wl_nan_ev_datapath_end_t;

typedef struct wl_tsf {
	uint32 tsf_l;
	uint32 tsf_h;
} wl_tsf_t;

typedef struct wl_nan_ev_rx_bcn {
	wl_tsf_t tsf;
	uint16   bcn_len;
	uint8    PAD[2];
	uint8    bcn[0];
} wl_nan_ev_rx_bcn_t;

/* reason of host assist request */
enum wl_nan_host_assist_reason {
	WL_NAN_HAST_REASON_NONE		= 0,

	/* reason for host assist request */
	WL_NAN_HAST_REASON_NO_CRB			= 1, /* NDL: no common NA */
	WL_NAN_HAST_REASON_NDC				= 2, /* NDL: NDC not compliant */
	WL_NAN_HAST_REASON_IMMUT			= 3, /* NDL: peer immutable schedule */
	WL_NAN_HAST_REASON_RNG				= 4, /* NDL: ranging schedule */
	WL_NAN_HAST_REASON_QOS				= 5, /* NDL: QoS not satisfied */
	WL_NAN_HAST_REASON_SVC_NDI_MISSING		= 6, /* SD: SVC NDI missing */
	WL_NAN_HAST_REASON_PEER_SCB_NORESOURCE		= 7, /* NDP: no more peer scb available */
	WL_NAN_HAST_REASON_NDP_PMK_MISSING		= 8, /* NDP: PMK needed from host */
	WL_NAN_HAST_REASON_SVC_NDI_AND_PMK_MISSING	= 9  /* PMK and SVC NDI needed from host */
};
typedef uint8 wl_nan_host_assist_reason_t;

/* WL_NAN_XTLV_HOST_ASSIST_REQ */
typedef struct wl_nan_host_assist_req {
	struct ether_addr peer_nmi;		/* peer nmi */
	struct ether_addr initiator_ndi;	/* initiator ndi */
	uint8 indp_id;				/* initiator NDP ID */
	wl_nan_frame_type_t frm_type;		/* received NAF type */
	wl_nan_host_assist_reason_t reason;	/* reason of host assist request */
	uint8 pub_id;				/* Publish ID (valid for WL_NAN_FRM_TYPE_DP_REQ) */
	uint8 PAD[2];
} wl_nan_host_assist_req_t;

/* nan sub-features */
enum wl_nan_fw_cap_flag1 {
	WL_NAN_FW_CAP_FLAG_NONE			= 0x00000000, /* dummy */
	WL_NAN_FW_CAP_FLAG1_AVAIL		= 0x00000001,
	WL_NAN_FW_CAP_FLAG1_DISC		= 0x00000002,
	WL_NAN_FW_CAP_FLAG1_DATA		= 0x00000004,
	WL_NAN_FW_CAP_FLAG1_SEC			= 0x00000008,
	WL_NAN_FW_CAP_FLAG1_RANGE		= 0x00000010,
	WL_NAN_FW_CAP_FLAG1_WFA_TB		= 0x00000020,
	WL_NAN_FW_CAP_FLAG1_DAM			= 0x00000040,
	WL_NAN_FW_CAP_FLAG1_DAM_STRICT		= 0x00000080,
	WL_NAN_FW_CAP_FLAG1_DAM_AUTO		= 0x00000100,
	WL_NAN_FW_CAP_FLAG1_DBG			= 0x00000200,
	WL_NAN_FW_CAP_FLAG1_BCMC_IN_NDC		= 0x00000400,
	WL_NAN_FW_CAP_FLAG1_CHSTATS		= 0x00000800,
	WL_NAN_FW_CAP_FLAG1_ASSOC_COEX		= 0x00001000,
	WL_NAN_FW_CAP_FLAG1_FASTDISC		= 0x00002000,
	WL_NAN_FW_CAP_FLAG1_NO_ID_GEN		= 0x00004000,
	WL_NAN_FW_CAP_FLAG1_DP_OPAQUE_DATA	= 0x00008000,
	WL_NAN_FW_CAP_FLAG1_NSR2		= 0x00010000,
	WL_NAN_FW_CAP_FLAG1_NSR2_SAVE		= 0x00020000,
	WL_NAN_FW_CAP_FLAG1_NANHO		= 0x00040000,
	WL_NAN_FW_CAP_FLAG1_NDPE		= 0x00080000,
	WL_NAN_FW_CAP_FLAG1_OOB_AF		= 0x00100000,
	WL_NAN_FW_CAP_FLAG1_PMK_PER_NDP		= 0x00200000,
	WL_NAN_FW_CAP_FLAG1_INSTANT_MODE	= 0x00400000,
	/* Group Addressed Frames Protect (GTK/IGTK/BIGTK) */
	WL_NAN_FW_CAP_FLAG1_GAF_PROTECT		= 0x00800000,
	WL_NAN_FW_CAP_FLAG1_REKEY		= 0x01000000,
	WL_NAN_FW_CAP_FLAG1_S3			= 0x02000000,
	WL_NAN_FW_CAP_FLAG1_PAIRING		= 0x04000000,
	WL_NAN_FW_CAP_FLAG1_6G			= 0x08000000,
	WL_NAN_FW_CAP_FLAG1_MCAST_AVAIL_BMP	= 0x10000000,
	WL_NAN_FW_CAP_FLAG1_WOW_OFFLOAD		= 0x20000000,
	WL_NAN_FW_CAP_FLAG1_SUSPENSION		= 0x40000000
};


/* WL_NAN_XTLV_GEN_FW_CAP */
typedef struct wl_nan_fw_cap {
	uint32 flags1;                  /* nan sub-features compiled in firmware */
	uint32 flags2;                  /* for more sub-features in future */
	uint8  max_svc_publishes;        /* max num of service publish */
	uint8  max_svc_subscribes;       /* max num of service subscribe */
	uint8  max_lcl_sched_maps;       /* max num of local schedule map */
	uint8  max_lcl_ndc_entries;      /* max num of local NDC entry */
	uint8  max_lcl_ndi_interfaces;   /* max num of local NDI interface */
	uint8  max_peer_entries;         /* max num of peer entry */
	uint8  max_ndp_sessions;         /* max num of NDP session */
	uint8  max_concurrent_nan_clusters; /* max num of concurrent clusters */
	uint16 max_service_name_len; /* max service name length */
	uint16 max_match_filter_len; /* max match filter length */
	uint16 max_total_match_filter_len; /* max total match filter length */
	uint16 max_service_specific_info_len; /* max service specific info length */
	uint16 max_vsa_data_len; /* max vendor specific attrib data length */
	uint16 max_mesh_data_len; /* max mesh data length */
	uint16 max_app_info_len; /* max app info length */
	uint16 max_sdea_svc_specific_info_len; /* max sdea ser specific info length */
	uint8  max_queued_tx_followup_msgs; /* max no. of queued tx followup msgs */
	uint8  max_subscribe_address; /* max subscribe addresses supported */
	uint8  ndp_supported_bands; /* number of ndp supported bands */
	uint8  is_ndp_security_supported; /* if secure ndp is supported */
	uint8  cipher_suites_supported_mask; /* bitmask for suites supported */
	uint8  cipher_suites_supported_mask1; /* 1 more bitmask for suites supported */
	uint8  PAD[2];
} wl_nan_fw_cap_t;

/* WL_NAN_XTLV_GEN_FW_CAP_V2 */
typedef struct wl_nan_fw_cap_v2 {
	uint32 flags1;			/* nan sub-features compiled in firmware */
	uint32 flags2;			/* for more sub-features in future */
	uint8  max_svc_publishes;	/* max num of service publish */
	uint8  max_svc_subscribes;	/* max num of service subscribe */
	uint8  max_lcl_sched_maps;	/* max num of local schedule map */
	uint8  max_lcl_ndc_entries;	/* max num of local NDC entry */
	uint8  max_lcl_ndi_interfaces;	/* max num of local NDI interface */
	uint8  max_peer_entries;	/* max num of peer entry */
	uint8  max_peer_sched_maps;	/* max num of peer schedule maps */
	uint8  max_ndp_sessions;	/* max num of NDP session */
	uint32 cipher_suites_supported_mask; /* bitmask for supported cipher suites */
	uint32 reserved_uint32_1;	/* reserved for future sub-features */
	uint32 reserved_uint32_2;	/* reserved for future sub-features */
	uint32 reserved_uint32_3;	/* reserved for future sub-features */
	uint32 reserved_uint32_4;	/* reserved for future sub-features */
} wl_nan_fw_cap_v2_t;

/* nan cipher suite support mask bits */
#define WL_NAN_CIPHER_SUITE_SHARED_KEY_128_MASK		0x01u
#define WL_NAN_CIPHER_SUITE_SHARED_KEY_256_MASK		0x02u
#define WL_NAN_CIPHER_SUITE_PUBLIC_KEY_128_MASK		0x04u
#define WL_NAN_CIPHER_SUITE_PUBLIC_KEY_256_MASK		0x08u
#define WL_NAN_CIPHER_SUITE_GROUP_KEY_128_MASK		0x10u
#define WL_NAN_CIPHER_SUITE_GROUP_KEY_256_MASK		0x20u
#define WL_NAN_CIPHER_SUITE_BIP_KEY_128_MASK		0x40u
#define WL_NAN_CIPHER_SUITE_BIP_KEY_256_MASK		0x80u

#define WL_NAN_CIPHER_SUITE_PK_PASN_128_MASK		0x100u
#define WL_NAN_CIPHER_SUITE_PK_PASN_256_MASK		0x200u

/* NAN Save Restore */
#define WL_NAN_NSR2_INFO_MAX_SIZE	2048 /* arbitrary */

/* WL_NAN_XTLV_NSR2_PEER */
typedef struct wl_nan_nsr_peer_info {
	struct ether_addr nmi;
	uint8 l_min_slots;	/* local QoS min slots */
	uint8 p_min_slots;	/* peer QoS min slots */
	uint16 l_max_latency;	/* local QoS max latency */
	uint16 p_max_latency;	/* peer QoS max latency */
	uint8 num_map;		/* num of NA map */
	uint8 PAD;
	uint16 attrs_len;	/* total len of following attrs */
	uint8 attrs[];		/* peer attributes (NA/NDC/ULW/DevCap/Element container) */
} wl_nan_nsr_peer_info_t;

enum wl_nan_nsr_ndp_flag {
	WL_NAN_NSR_NDP_FLAG_LCL_INITATOR	= 0x0001,
	WL_NAN_NSR_NDP_FLAG_MCAST		= 0x0002
};
typedef uint16 wl_nan_nsr_ndp_flag_t;

/* WL_NAN_XTLV_NSR2_NDP */
typedef struct wl_nan_nsr_ndp_info {
	struct ether_addr peer_nmi;
	struct ether_addr peer_ndi;
	struct ether_addr lcl_ndi;
	uint16 flags;		/* wl_nan_nsr_ndp_flag_t */
	uint8 pub_id;		/* publish id */
	uint8 indp_id;		/* initiator's ndp id */
	uint8 last_token;	/* last NDP dialog token */
	uint8 PAD;
} wl_nan_nsr_ndp_info_t;

/* NAN2.0 Ranging definitions */

/* result indication bit map */
#define NAN_RANGE_INDICATION_NONE		0
#define NAN_RANGE_INDICATION_CONT		(1<<0)
#define NAN_RANGE_INDICATION_INGRESS		(1<<1)
#define NAN_RANGE_INDICATION_EGRESS		(1<<2)

/* responder flags */
#define NAN_RANGE_FLAG_AUTO_ACCEPT	(1 << 0)
#define NAN_RANGE_FLAG_RESULT_REQUIRED	(1 << 1)

typedef struct wl_nan_range_req {
	struct ether_addr peer;
	uint8 publisher_id;
	uint8 indication; /* bit map for result event */
	uint32 resolution; /* default millimeters */
	uint32 ingress; /* ingress limit in mm */
	uint32 egress; /* egress limit in mm */
	uint32 interval; /* max interval(in TU) b/w two ranging measurements */
	uint8 num_meas; /* number of ftm measurement frames for ranging */
	uint8 pad[3];
} wl_nan_range_req_t;

#define NAN_RNG_REQ_IOV_LEN	24

typedef uint8 wl_nan_range_id;

typedef struct wl_nan_range_resp {
	wl_nan_range_id range_id;
	uint8 flags; /* auto response, range result required */
	uint8 status; /* accept, reject */
	uint8 indication; /* bit map for result event */
	uint32 resolution; /* default millimeters */
	uint32 ingress; /* ingress limit in mm */
	uint32 egress; /* egress limit in mm */
	uint32 interval; /* max interval(in TU) b/w two ranging measurements */
} wl_nan_range_resp_t;

#define NAN_RNG_RESP_IOV_LEN	20

#define NAN_RNG_TERM_FLAG_IMMEDIATE         (1u << 0u) /* Do not wait for TXS */
#define NAN_RNG_TERM_FLAG_SILENT_TEARDOWN   (1u << 1u) /* Do not TX rng_term */
#define NAN_RNG_TERM_FLAG_EVENT_HOST        (1u << 2u) /* Notify event to host */
#define NAN_RNG_TERM_FLAG_OPT_TLVS          (1u << 3u) /* opt tlvs present */

typedef struct wl_nan_range_cancel_ext {
	wl_nan_range_id range_id;
	uint8 flags;
	uint8 PAD[2];
} wl_nan_range_cancel_ext_t;

#define NAN_RNG_CANCEL_IOV_FIXED_LEN	4u

#define NAN_RNG_MAX_IOV_LEN	255

typedef struct wl_nan_ev_rng_req_ind {
	struct ether_addr peer_m_addr;
	uint8 rng_id;
	/* ftm parameters */
	uint8 max_burst_dur;
	uint8 min_ftm_delta;
	uint8 max_num_ftm;
	uint8 ftm_format_bw;
	/* location info availability bit map */
	uint8 lc_info_avail;
	/* Last movement indication */
	uint16 last_movement;
	uint8 PAD[2];
} wl_nan_ev_rng_req_ind_t;

#define NAN_RNG_REQ_IND_SIZE 14

typedef struct wl_nan_ev_rng_rpt_ind {
	uint32 dist_mm; /* in millimeter */
	struct ether_addr peer_m_addr;
	uint8 indication; /* indication definitions mentioned above */
	uint8 rng_id;
} wl_nan_ev_rng_rpt_ind_t;

#define NAN_RNG_RPT_IND_SIZE 12

/* number of continuous ranging crbs which can be idle,
* after which ranging session will be terminated.
* Default value is 5. Set to zero for disabling the
* idle timeout functionality
*/
typedef uint8 wl_nan_range_idle_count_t;

/* nan ranging termination reason codes */
#define NAN_RNG_TERM_REASON_CODE_BASE		100u
#define NAN_RNG_TERM_REASON_CODE(reason_code) \
	(NAN_RNG_TERM_REASON_CODE_BASE + (reason_code))

/* Reason Code Unspecified */
#define NAN_RNG_TERM_UNSPECIFIED	NAN_RNG_TERM_REASON_CODE(0u)
/* no ftms from peer */
#define NAN_RNG_TERM_IDLE_TIMEOUT	NAN_RNG_TERM_REASON_CODE(1u)
/* On Peer Termination */
#define NAN_RNG_TERM_PEER_REQ		NAN_RNG_TERM_REASON_CODE(2u)
/* On User or Host Termination */
#define NAN_RNG_TERM_USER_REQ		NAN_RNG_TERM_REASON_CODE(3u)
/* On FSM Timeout, waiting for Resp from peer */
#define NAN_RNG_TERM_RNG_RESP_TIMEOUT	NAN_RNG_TERM_REASON_CODE(4u)
/* On range resp, reject from peer */
#define NAN_RNG_TERM_RNG_RESP_REJ	NAN_RNG_TERM_REASON_CODE(5u)
/* On range req/resp txs fail */
#define NAN_RNG_TERM_RNG_TXS_FAIL	NAN_RNG_TERM_REASON_CODE(6u)

typedef struct wl_nan_ev_rng_term_ind {
	struct ether_addr peer_m_addr;
	uint8 reason_code;
	uint8 rng_id;
} wl_nan_ev_rng_term_ind_t;

#define NAN_RNG_TERM_IND_SIZE 8

typedef struct wl_nan_ev_rng_resp {
	struct ether_addr peer_m_addr;
	uint8 status;
	uint8 rng_id;
} wl_nan_ev_rng_resp_t;

/* Used by NDL schedule events -
 * WL_NAN_EVENT_PEER_SCHED_UPD_NOTIF, WL_NAN_EVENT_PEER_SCHED_REQ
 * WL_NAN_EVENT_PEER_SCHED_RESP, WL_NAN_EVENT_PEER_SCHED_CONF
 */
typedef struct wl_nan_ev_sched_info {
	struct ether_addr peer_nmi;
	uint8	ndl_status;	/* applies only to sched resp/conf */
	uint8	PAD;
	uint16	opt_tlv_len;
	uint8	opt_tlvs[];
} wl_nan_ev_sched_info_t;

/* WL_NAN_EVENT_CHAN_BOUNDARY */
typedef struct wl_nan_chbound_info {
	uint32	cluster_tsf_h;	/* Current Cluster TSF High */
	uint32	cluster_tsf_l;	/* Current Cluster TSF Low */
	uint16	cur_chspec;
	uint16	opt_tlvs_len;
	uint8	opt_tlvs[];
} wl_nan_chbound_info_t;

/* channel stats (includes nan & non-nan) */

/* WL_NAN_XTLV_CCA_STATS */
typedef struct wl_nan_cca_stats {
	uint16	chanspec;
	int16	noise;
	uint32	sample_dur;	/* CCA measurement duration (in ms) */

	uint32	congest_ibss;	/* IBSS & TxDUR (in us) */
	uint32	congest_obss;	/* OBSS & noctg (in us) */
	uint32	interference;	/* nopkt & ED (in us) */
} wl_nan_cca_stats_t;

/* WL_NAN_XTLV_PER_STATS */
typedef struct wl_nan_per_stats_s {
	uint16	chanspec;
	uint8	PAD[2];
	uint32	sample_dur;

	uint32	txframe;	/* tx data frames */
	uint32	txretrans;	/* tx mac retransmits */
	uint32	txerror;	/* tx data errors  */
	uint32	txctl;		/* tx management frames */
	uint32	txserr;		/* tx status errors */

	uint32	rxframe;	/* rx data frames */
	uint32	rxerror;	/* rx data errors */
	uint32	rxctl;		/* rx management frames */

	uint32	txbar;		/* tx bar */
	uint32	rxbar;		/* rx bar */
	uint32	txaction;	/* tx action frame */
	uint32	rxaction;	/* rx action frame */
	uint32  txlost;		/* lost packets reported in txs */
	uint32  rxback;		/* rx block ack */
	uint32  txback;		/* tx bloak ack */
} wl_nan_per_stats_t;

/* fast discovery beacon config
 * WL_NAN_XTLV_CFG_FDISC_TBMP
*/
typedef struct wl_nan_fastdisc_s {
	uint8 id;
	uint8 bitmap_len;
	uint8 PAD[2];
	uint8 bitmap[];
} wl_nan_fastdisc_t;

#define WL_NAN_FASTDISC_CFG_SIZE	1024 /* arbitrary */

/* NAN NDL schedule info for a particular negotiation.
 * WL_NAN_XTLV_NDL_SCHED_INFO
 */
typedef struct wl_nan_ndl_slot_info {
	uint16	  chanspec;
	uint8	  nss;
	uint8	  PAD;
} wl_nan_ndl_slot_info_t;

typedef struct wl_nan_ndl_sched_info {
	uint16	  num_slot;
	uint16	  period;
	wl_nan_ndl_slot_info_t	  slot[];
} wl_nan_ndl_sched_info_t;


/*
 * NAN multicast data slot bitmap config
 * WL_NAN_XTLV_CFG_MCAST_AVAIL_BMP
 */
typedef struct wl_nan_mcast_avail_bmp_s {
	uint8 mapid;
	uint8 bitmap_len;
	uint8 PAD[2];
	uint8 bitmap[BCM_FLEX_ARRAY];
} wl_nan_mcast_avail_bmp_t;
#define WL_NAN_MCAST_AVAIL_BMP_LEN_FIXED  (OFFSETOF(wl_nan_mcast_avail_bmp_t, bitmap))
#define WL_NAN_MCAST_AVAIL_BMP_LEN_MAX    (WL_NAN_MCAST_AVAIL_BMP_LEN_FIXED + 8u) /* 1024/16TU */

typedef struct wl_nan_mcast_avail_s {
	struct ether_addr ndi;
	uint8 PAD[2];
	wl_nan_mcast_avail_bmp_t bmp[];
} wl_nan_mcast_avail_t;
#define WL_NAN_MCAST_AVAIL_LEN_FIXED	(OFFSETOF(wl_nan_mcast_avail_t, bmp))
#define WL_NAN_MCAST_AVAIL_LEN_MAX	(WL_NAN_MCAST_AVAIL_LEN_FIXED + 64u) /* arbitrary */

/* ********************* end of NAN section ******************************** */
/* endif WL_NAN */

/* NAN P2P operation */
enum {
	WL_AVAIL_NONE		= 0x0000,
	WL_AVAIL_LOCAL		= 0x0001,
	WL_AVAIL_PEER		= 0x0002,
	WL_AVAIL_NDC		= 0x0003,
	WL_AVAIL_IMMUTABLE	= 0x0004,
	WL_AVAIL_RESPONSE	= 0x0005,
	WL_AVAIL_COUNTER	= 0x0006,
	WL_AVAIL_RANGING	= 0x0007,
	WL_AVAIL_UPD_POT	= 0x0008,	/* modify potential, keep committed/conditional */
	WL_AVAIL_UPD_COM_COND	= 0x0009,	/* modify committed/conditional, keep potential */
	WL_AVAIL_REMOVE_MAP	= 0x000A,	/* remove map */
	WL_AVAIL_FRM_TYPE	= 0x000B,	/* specify frame types containing NA */
	WL_AVAIL_SUBSLOT	= 0x000C,	/* Sub slot attribute */
	WL_AVAIL_TYPE_MAX	= WL_AVAIL_SUBSLOT	/* New ones before and update */
};
#define WL_AVAIL_TYPE_MASK		0x000F
#define WL_AVAIL_FLAG_ALL_MAPS		0x1000	/* All maps */
#define WL_AVAIL_FLAG_REMOVE		0x2000	/* remove schedule attr of given type & map id */
#define WL_AVAIL_FLAG_SELECTED_NDC	0x4000
#define WL_AVAIL_FLAG_RAW_MODE		0x8000
#define WL_AVAIL_FLAGS_MASK		0xFF00
#define WL_AVAIL_FLAGS_SHIFT		8

typedef int16 wl_avail_flags_t;

/* availability entry flags */
enum {
	WL_AVAIL_ENTRY_NONE		= 0x0000,
	WL_AVAIL_ENTRY_COM		= 0x0001,		/* committed */
	WL_AVAIL_ENTRY_POT		= 0x0002,		/* potential */
	WL_AVAIL_ENTRY_COND		= 0x0004,	/* conditional */
	WL_AVAIL_ENTRY_PAGED		= 0x0008,	/* P-NDL */
	WL_AVAIL_ENTRY_USAGE		= 0x0030,	/* usage preference */
	WL_AVAIL_ENTRY_BIT_DUR		= 0x00C0,	/* bit duration */
	WL_AVAIL_ENTRY_BAND_PRESENT	= 0x0100,	/* band present */
	WL_AVAIL_ENTRY_CHAN_PRESENT	= 0x0200,	/* channel information present */
	WL_AVAIL_ENTRY_CHAN_ENTRY_PRESENT = 0x0400,	/* channel entry (opclass+bitmap) */
	WL_AVAIL_ENTRY_SUBSLOT_IMMUTABLE  = 0x0800,	/* Sub slot is immutable */
	/* free to use 0x0800 */
	WL_AVAIL_ENTRY_RXNSS		= 0xF000	/* max num of spatial stream RX */
};

/* bit duration */
enum {
	WL_AVAIL_BIT_DUR_16	= 0,	/* 16TU */
	WL_AVAIL_BIT_DUR_32	= 1,	/* 32TU */
	WL_AVAIL_BIT_DUR_64	= 2,	/* 64TU */
	WL_AVAIL_BIT_DUR_128	= 3,	/* 128TU */
};

/* period */
enum {
	WL_AVAIL_PERIOD_0	= 0,	/* 0TU */
	WL_AVAIL_PERIOD_128	= 1,	/* 128TU */
	WL_AVAIL_PERIOD_256	= 2,	/* 256TU */
	WL_AVAIL_PERIOD_512	= 3,	/* 512TU */
	WL_AVAIL_PERIOD_1024	= 4,	/* 1024TU */
	WL_AVAIL_PERIOD_2048	= 5,	/* 2048TU */
	WL_AVAIL_PERIOD_4096	= 6,	/* 4096TU */
	WL_AVAIL_PERIOD_8192	= 7,	/* 8192TU */
};

/* Band ID values: See Table 99 in NAN R4 spec */
enum {
	WL_AVAIL_BAND_NONE	= 0,	/* reserved */
	WL_AVAIL_BAND_SUB1G	= 1,	/* sub-1 GHz */
	WL_AVAIL_BAND_2G	= 2,	/* 2.4 GHz */
	WL_AVAIL_BAND_3G	= 3,	/* reserved (for 3.6 GHz) */
	WL_AVAIL_BAND_5G	= 4,	/* 4.9 and 5 GHz */
	WL_AVAIL_BAND_60G	= 5,	/* reserved (for 60 GHz) */
	WL_AVAIL_BAND_45G	= 6,	/* 45 GHz */
	WL_AVAIL_BAND_6G	= 7,	/* 6 GHz */
};

#define WL_AVAIL_ENTRY_TYPE_MASK	0x000F
#define WL_AVAIL_ENTRY_USAGE_MASK	0x0030 /* up to 4 usage preferences */
#define WL_AVAIL_ENTRY_USAGE_SHIFT	4
#define WL_AVAIL_ENTRY_USAGE_VAL(_flags)	(((_flags) & WL_AVAIL_ENTRY_USAGE_MASK) \
	>> WL_AVAIL_ENTRY_USAGE_SHIFT)

#define WL_AVAIL_ENTRY_BIT_DUR_MASK	0x00C0 /* 0:16TU, 1:32TU, 2:64TU, 3:128TU */
#define WL_AVAIL_ENTRY_BIT_DUR_SHIFT	6
#define WL_AVAIL_ENTRY_BIT_DUR_VAL(_flags)	(((_flags) & WL_AVAIL_ENTRY_BIT_DUR_MASK) \
	>> WL_AVAIL_ENTRY_BIT_DUR_SHIFT)

#define WL_AVAIL_ENTRY_BAND_MASK	0x0100 /* 0=band not present, 1=present */
#define WL_AVAIL_ENTRY_BAND_SHIFT	8

#define WL_AVAIL_ENTRY_CHAN_MASK	0x0200 /* 0=channel info not present, 1=present */
#define WL_AVAIL_ENTRY_CHAN_SHIFT	9

#define WL_AVAIL_ENTRY_CHAN_ENTRY_MASK	0x0400 /* 0=chanspec, 1=hex channel entry */
#define WL_AVAIL_ENTRY_CHAN_ENTRY_SHIFT	10

#define WL_AVAIL_ENTRY_RXNSS_MASK	0xF000
#define WL_AVAIL_ENTRY_RXNSS_SHIFT	12
#define WL_AVAIL_ENTRY_RXNSS_VAL(_flags)	(((_flags) & WL_AVAIL_ENTRY_RXNSS_MASK) \
	>> WL_AVAIL_ENTRY_RXNSS_SHIFT)
#define WL_AVAIL_ENTRY_RXNSS_MAX	15	/* 0-15 */

/* mask for channel_entry (to be obsoleted) */
#define WL_AVAIL_ENTRY_OPCLASS_MASK		0xFF
#define WL_AVAIL_ENTRY_CHAN_BITMAP_MASK		0xFF00
#define WL_AVAIL_ENTRY_CHAN_BITMAP_SHIFT	8
#define WL_AVAIL_ENTRY_CHAN_BITMAP_VAL(_info)	(((_info) & WL_AVAIL_ENTRY_CHAN_BITMAP_MASK) \
	>> WL_AVAIL_ENTRY_CHAN_BITMAP_SHIFT)

/* Max allowed timebitmap bits length: Worst case allowed with 1tu duration for 1024tu period */
#define NAN_S3_MAX_TMB_LEN	(NAN_SLOT_DUR_1024TU / NBBY)

/* Used for raw channel entry field input */
#define MAX_CHAN_ENTRY_LEN 6

typedef struct wl_avail_entry {
	uint16		length;		/* total length */
	uint16		start_offset;	/* in TUs, multiply by 16 for total offset */
	union {
		uint32 channel_info;	/* either chanspec or hex channel entry (opclass +
					 * bitmap per NAN spec), as indicated by setting
					 * WL_AVAIL_ENTRY_HEX_CHAN_ENTRY flag
					 */
		uint32 band;		/* defined by WL_BAND enum, 2=2.4GHz, 4=5GHz */
		uint8 channel_entry[MAX_CHAN_ENTRY_LEN];
		uint8 align[8];		/* aligned len of union in structure (not for use)
					 * if member of union is changed,
					 * update length of align[] accordingly.
					 */
	} u;				/* band or channel value, 0=all band/channels */
	uint8		sched_map_id;	/* avail map id associated with sched entry */
	uint8		PAD;
	uint8		period;		/* in TUs, defined by WL_AVAIL_PERIOD enum
					 * 1:128, 2:256, 3:512, 4:1024, 5:2048, 6:4096,
					 * 7:8192
					 */
	uint8		bitmap_len;
	uint16		flags;		/* defined by avail entry flags enum:
					 * type, usage pref, bit duration, rx nss,
					 * and band, channel or channel entry
					 */
	uint8		bitmap[];	/* time bitmap */
} wl_avail_entry_t;

#define WL_AVAIL_VERSION	1	/* current wl_avail version */

typedef struct wl_avail {
	uint16		length;		/* total length */
	uint16		flags;		/* LSB - avail type (defined by WL_AVAIL enum)
					 * MSB - avail flags
					 */
	uint8		id;		/* id used for multiple maps/avail */
	uint8		lndc_id;	/* ndc id used in multi-ndc case */
	uint8		version;
	uint8		PAD;
	struct ether_addr addr;		/* peer mac address or ndc id */
	uint8		num_entries;
	uint8		unused_byte;
	/* add additional fields above this line */
	uint8		entry[];
} wl_avail_t;

#define WL_AVAIL_MIN_LEN(n) ((n) ? OFFSETOF(wl_avail_t, entry) + \
		((n) * OFFSETOF(wl_avail_entry_t, bitmap)) : 0)

/* unaligned schedule (window) */
typedef struct wl_avail_ulw {
	uint8	id;		/* schedule ID */
	uint8	overwrite;	/* bit 0: overwrite all
				 * 1-4: map ID if overwrite all is 0
				 */
	uint16	flags;
	uint32	start;		/* start time of first ULW, in us */
	uint32	dur;		/* duration of ULW, in us */
	uint32	period;		/* time between consecutive ULWs, in us */
	union {
		uint32 chanspec;
		uint32 band;
		uint8 chan_entry[MAX_CHAN_ENTRY_LEN];
		uint8 PAD[8];
	} u;
	uint8	cntdwn;		/* remaining ULWs before schedule ends */
	uint8	PAD[3];
} wl_avail_ulw_t;

/* unset: NAN is not available during ULW, set: NAN is avail depending on ctrl flags */
#define WL_NAN_ULW_CTRL_PRESENT		(1 << 0)
/* unset: band, set: channel */
#define WL_NAN_ULW_CTRL_TYPE		(1 << 1)
/* set: NAN is availabile on specified band/channel */
#define WL_NAN_ULW_CTRL_AVAIL		(1 << 2)
/* channel is provided in raw attribute format */
#define WL_NAN_ULW_CTRL_RAW_CHAN	(1 << 3)

/* nan wfa testmode operations */
enum {
	WL_NAN_WFA_TM_IGNORE_TERMINATE_NAF		= 0x00000001,
	WL_NAN_WFA_TM_IGNORE_RX_DATA_OUTSIDE_CRB	= 0x00000002,
	WL_NAN_WFA_TM_ALLOW_TX_DATA_OUTSIDE_CRB		= 0x00000004,
	WL_NAN_WFA_TM_ENFORCE_NDL_COUNTER		= 0x00000008,
	WL_NAN_WFA_TM_BYPASS_NDL_PROPOSAL_VALIDATION	= 0x00000010,
	/* allow data(pings) tx while ndp sec negotiation */
	WL_NAN_WFA_TM_SEC_SEND_PINGS_BYPASS_NDP_SM	= 0x00000020,
	/* generate and insert incorrect mic */
	WL_NAN_WFA_TM_SEC_INCORRECT_MIC		        = 0x00000040,
	/* send m4 reject deliberately */
	WL_NAN_WFA_TM_SEC_REJECT_STATUS4M4		= 0x00000080,
	/* send mgmt frame (for eg. ndp terminate) in clear txt (bypass security) */
	WL_NAN_WFA_TM_SEC_SEND_MGMT_CLEAR	        = 0x00000100,
	/* validate qos */
	WL_NAN_WFA_TM_NDL_QOS_VALIDATE			= 0x00000200,
	/* firmware generated schedule update */
	WL_NAN_WFA_TM_GEN_SCHED_UPD			= 0x00000400,
	/* add lower 4-bytes of TSF to configured start time */
	WL_NAN_WFA_TM_ULW_START_TIME			= 0x00000800,
	/* enable schedule validation for SDF */
	WL_NAN_WFA_TM_SDF_SCHED_VALIDATE		= 0x00001000,
	/* by pass faw na iovar */
	WL_NAN_WFA_TM_SKIP_RAW_NA_BLOB		        = 0x00002000,
	/* overwrite local NA with peer NA in received frame */
	WL_NAN_WFA_TM_LOCAL_NA_OVERWRITE		= 0x00004000,
	/* randomize and self configure ndl qos(needed at responder in auto mode) */
	WL_NAN_WFA_TM_SELF_CFG_NDL_QOS                  = 0x00008000,
	/* send NAF frames only in DW */
	WL_NAN_WFA_TM_SEND_NAF_IN_DW			= 0x00010000,
	/* restrict channels used for countered slots to Ch 6/149 only */
	WL_NAN_WFA_TM_RESTRICT_COUNTER_SLOTS_CHAN	= 0x00020000,
	/* NDPE negative test case (4.2.5 & 4.2.6) */
	WL_NAN_WFA_TM_NDPE_NEGATIVE_TEST_TB		= 0x00040000,
	/* Set NDPE(NAN3.0) capable bit in dev cap attr */
	WL_NAN_WFA_TM_ENABLE_NDPE_CAP                   = 0x00080000,
	/* NDPE negative test case (4.2.5.2). Enable both NDP and NDPE attributes */
	WL_NAN_WFA_TM_ENABLE_NDP_NDPE_ATTR		= 0x00100000,

	/* add above & update mask */
	WL_NAN_WFA_TM_FLAG_MASK                         = 0x001FFFFF
};
typedef uint32 wl_nan_wfa_testmode_t;

/* To be removed; replaced by wl_nan_vndr_payload */
typedef struct wl_nan_vndr_ie {
	uint32  flags;			/* bitmask indicating which packet(s) contain this IE */
	uint16  body_len;		/* length of body (does not include oui field) */
	uint8   PAD[2];
	uint8   oui[DOT11_OUI_LEN];
	uint8   PAD;
	uint8   body[];			/* vendor IE payload */
} wl_nan_vndr_ie_t;

typedef struct wl_nan_vndr_payload {
	uint32  flags;			/* bitmask indicating which packet(s) contain payload */
	uint16  payload_len;		/* length of payload */
	uint8   PAD[2];
	uint8   payload[];		/* payload to be appended to NAN frame */
} wl_nan_vndr_payload_t;

typedef struct wl_nan_dev_cap {
	uint8 bands[NAN_MAX_BANDS];
	uint8 awake_dw[NAN_MAX_BANDS];
	uint8 overwrite_mapid[NAN_MAX_BANDS];
	uint8 mapid;	/* dev cap mapid */
	uint8 all_maps;	/* applies to device */
	uint8 paging;
	uint8 PAD[3];
} wl_nan_dev_cap_t;

/* arbitrary max len for frame template */
#define WL_NAN_FRM_TPLT_MAX_LEN	1024

typedef struct wl_nan_frm_tplt {
	wl_nan_frame_type_t type;
	uint8   PAD;
	uint16  len;		/* length of template */
	uint8   data[];		/* template */
} wl_nan_frm_tplt_t;


#define RSSI_THRESHOLD_SIZE 16
#define MAX_IMP_RESP_SIZE 256

typedef struct wl_proxd_rssi_bias {
	int32		version;			/**< version */
	int32		threshold[RSSI_THRESHOLD_SIZE];	/**< threshold */
	int32		peak_offset;			/**< peak offset */
	int32		bias;				/**< rssi bias */
	int32		gd_delta;			/**< GD - GD_ADJ */
	int32		imp_resp[MAX_IMP_RESP_SIZE];	/**< (Hi*Hi)+(Hr*Hr) */
} wl_proxd_rssi_bias_t;

typedef struct wl_proxd_rssi_bias_avg {
	int32		avg_threshold[RSSI_THRESHOLD_SIZE];	/**< avg threshold */
	int32		avg_peak_offset;			/**< avg peak offset */
	int32		avg_rssi;				/**< avg rssi */
	int32		avg_bias;				/**< avg bias */
} wl_proxd_rssi_bias_avg_t;

#include <packed_section_start.h>
typedef BWL_PRE_PACKED_STRUCT struct wl_proxd_collect_info {
	uint16		type;  /**< type: 0 channel table, 1 channel smoothing table, 2 and 3 seq */
	uint16		index;		/**< The current frame index, from 1 to total_frames. */
	uint16		tof_cmd;	/**< M_TOF_CMD      */
	uint16		tof_rsp;	/**< M_TOF_RSP      */
	uint16		tof_avb_rxl;	/**< M_TOF_AVB_RX_L */
	uint16		tof_avb_rxh;	/**< M_TOF_AVB_RX_H */
	uint16		tof_avb_txl;	/**< M_TOF_AVB_TX_L */
	uint16		tof_avb_txh;	/**< M_TOF_AVB_TX_H */
	uint16		tof_id;		/**< M_TOF_ID */
	uint8		tof_frame_type;
	uint8		tof_frame_bw;
	int8		tof_rssi;
	int32		tof_cfo;
	int32		gd_adj_ns;	/**< gound delay */
	int32		gd_h_adj_ns;	/**< group delay + threshold crossing */
	int16		nfft;		/**< number of samples stored in H */
	uint8		num_max_cores;

} BWL_POST_PACKED_STRUCT wl_proxd_collect_info_t;
#include <packed_section_end.h>

#define K_TOF_COLLECT_H_PAD 1
#define K_TOF_COLLECT_SC_20MHZ (64)
/* Maximum possible size of sample capture */
#define K_TOF_COLLECT_SC_80MHZ (2*K_TOF_COLLECT_SC_20MHZ)
/* Maximum possible size of channel dump */
#define K_TOF_COLLECT_CHAN_SIZE (2*K_TOF_COLLECT_SC_80MHZ)

/*
A few extra samples are required to estimate frequency offset
Right now 16 samples are being used. Can be changed in future.
*/
#define K_TOF_COLLECT_SAMP_SIZE_20MHZ		((2u) * (K_TOF_COLLECT_SC_20MHZ) + \
							(16u) + (K_TOF_COLLECT_H_PAD))
#define K_TOF_COLLECT_RAW_SAMP_SIZE_20MHZ	((2u) * (K_TOF_COLLECT_SAMP_SIZE_20MHZ))
#define K_TOF_COLLECT_H_SIZE_20MHZ		(K_TOF_COLLECT_SAMP_SIZE_20MHZ)
#define K_TOF_COLLECT_HRAW_SIZE_20MHZ		(K_TOF_COLLECT_RAW_SAMP_SIZE_20MHZ)

#define K_TOF_COLLECT_SAMP_SIZE_80MHZ		((2u) * (K_TOF_COLLECT_SC_80MHZ) + \
							(16u) + (K_TOF_COLLECT_H_PAD))
#define K_TOF_COLLECT_RAW_SAMP_SIZE_80MHZ	((2u) * K_TOF_COLLECT_SAMP_SIZE_80MHZ)
#define K_TOF_COLLECT_H_SIZE_80MHZ		(K_TOF_COLLECT_SAMP_SIZE_80MHZ)
#define K_TOF_COLLECT_HRAW_SIZE_80MHZ		(K_TOF_COLLECT_RAW_SAMP_SIZE_80MHZ)
#define K_TOF_COLLECT_HRAW_SIZE_20MHZ_1FS	(K_TOF_COLLECT_SAMP_SIZE_20MHZ)
#define K_TOF_COLLECT_HRAW_SIZE_80MHZ_1FS	((2u) * (K_TOF_COLLECT_SAMP_SIZE_20MHZ))

#define WL_PROXD_COLLECT_DATA_VERSION_1		1
#include <packed_section_start.h>
typedef BWL_PRE_PACKED_STRUCT struct wl_proxd_collect_data_v1 {
	wl_proxd_collect_info_t  info;
	uint8	ri_rr[FTM_TPK_RI_RR_LEN];
	/**< raw data read from phy used to adjust timestamps */
	uint32	H[K_TOF_COLLECT_H_SIZE_20MHZ];
} BWL_POST_PACKED_STRUCT wl_proxd_collect_data_t_v1;
#include <packed_section_end.h>

#define WL_PROXD_COLLECT_DATA_VERSION_2		2
#include <packed_section_start.h>
typedef BWL_PRE_PACKED_STRUCT struct wl_proxd_collect_data_v2 {
	wl_proxd_collect_info_t  info;
	uint8   ri_rr[FTM_TPK_RI_RR_LEN_SECURE_2_0];
	/**< raw data read from phy used to adjust timestamps */
	uint32  H[K_TOF_COLLECT_H_SIZE_20MHZ];
} BWL_POST_PACKED_STRUCT wl_proxd_collect_data_t_v2;
#include <packed_section_end.h>

#define WL_PROXD_COLLECT_DATA_VERSION_3         3
typedef struct wl_proxd_collect_data_v3 {
	uint16			version;
	uint16			len;
	wl_proxd_collect_info_t	info;
	uint8			ri_rr[FTM_TPK_RI_RR_LEN_SECURE_2_0];
	/**< raw data read from phy used to adjust timestamps */
	uint32			H[K_TOF_COLLECT_H_SIZE_20MHZ];
	uint32			chan[4 * K_TOF_COLLECT_CHAN_SIZE];
} wl_proxd_collect_data_t_v3;

#define WL_PROXD_COLLECT_DATA_VERSION_4         4
typedef struct wl_proxd_collect_data_v4 {
	uint16			version;
	uint16			len;
	wl_proxd_collect_info_t	info;
	uint8			ri_rr[FTM_TPK_RI_RR_LEN_SECURE_2_0_5G];
	uint8			PAD[2];
	/**< raw data read from phy used to adjust timestamps */
	uint32			H[K_TOF_COLLECT_H_SIZE_20MHZ];
	uint32			chan[4 * K_TOF_COLLECT_CHAN_SIZE];
} wl_proxd_collect_data_t_v4;
#define WL_PROXD_COLLECT_DATA_VERSION_MAX	WL_PROXD_COLLECT_DATA_VERSION_4


typedef struct wl_proxd_debug_data {
	uint8		count;		/**< number of packets */
	uint8		stage;		/**< state machone stage */
	uint8		received;	/**< received or txed */
	uint8		paket_type;	/**< packet type */
	uint8		category;	/**< category field */
	uint8		action;		/**< action field */
	uint8		token;		/**< token number */
	uint8		follow_token;	/**< following token number */
	uint16		index;		/**< index of the packet */
	uint16		tof_cmd;	/**< M_TOF_CMD */
	uint16		tof_rsp;	/**< M_TOF_RSP */
	uint16		tof_avb_rxl;	/**< M_TOF_AVB_RX_L */
	uint16		tof_avb_rxh;	/**< M_TOF_AVB_RX_H */
	uint16		tof_avb_txl;	/**< M_TOF_AVB_TX_L */
	uint16		tof_avb_txh;	/**< M_TOF_AVB_TX_H */
	uint16		tof_id;		/**< M_TOF_ID */
	uint16		tof_status0;	/**< M_TOF_STATUS_0 */
	uint16		tof_status2;	/**< M_TOF_STATUS_2 */
	uint16		tof_chsm0;	/**< M_TOF_CHNSM_0 */
	uint16		tof_phyctl0;	/**< M_TOF_PHYCTL0 */
	uint16		tof_phyctl1;	/**< M_TOF_PHYCTL1 */
	uint16		tof_phyctl2;	/**< M_TOF_PHYCTL2 */
	uint16		tof_lsig;	/**< M_TOF_LSIG */
	uint16		tof_vhta0;	/**< M_TOF_VHTA0 */
	uint16		tof_vhta1;	/**< M_TOF_VHTA1 */
	uint16		tof_vhta2;	/**< M_TOF_VHTA2 */
	uint16		tof_vhtb0;	/**< M_TOF_VHTB0 */
	uint16		tof_vhtb1;	/**< M_TOF_VHTB1 */
	uint16		tof_apmductl;	/**< M_TOF_AMPDU_CTL */
	uint16		tof_apmdudlim;	/**< M_TOF_AMPDU_DLIM */
	uint16		tof_apmdulen;	/**< M_TOF_AMPDU_LEN */
} wl_proxd_debug_data_t;

/** version of the wl_wsec_info structure */
#define WL_WSEC_INFO_VERSION_1	 0x01

/** start enum value for BSS properties */
#define WL_WSEC_INFO_BSS_BASE 0x0100
/* for WFA testing (CTT testbed) */
#define WL_WSEC_INFO_TEST_BASE 0x0300
/** size of len and type fields of wl_wsec_info_tlv_t struct */
#define WL_WSEC_INFO_TLV_HDR_LEN OFFSETOF(wl_wsec_info_tlv_t, data)

/** Allowed wl_wsec_info properties; not all of them may be supported. */
typedef enum {
	WL_WSEC_INFO_NONE = 0,
	WL_WSEC_INFO_MAX_KEYS = 1,
	WL_WSEC_INFO_NUM_KEYS = 2,
	WL_WSEC_INFO_NUM_HW_KEYS = 3,
	WL_WSEC_INFO_MAX_KEY_IDX = 4,
	WL_WSEC_INFO_NUM_REPLAY_CNTRS = 5,
	WL_WSEC_INFO_SUPPORTED_ALGOS = 6,
	WL_WSEC_INFO_MAX_KEY_LEN = 7,
	WL_WSEC_INFO_FLAGS = 8,
	/* add global/per-wlc properties above */
	WL_WSEC_INFO_BSS_FLAGS = (WL_WSEC_INFO_BSS_BASE + 1),
	WL_WSEC_INFO_BSS_WSEC = (WL_WSEC_INFO_BSS_BASE + 2),
	WL_WSEC_INFO_BSS_TX_KEY_ID = (WL_WSEC_INFO_BSS_BASE + 3),
	WL_WSEC_INFO_BSS_ALGO = (WL_WSEC_INFO_BSS_BASE + 4),
	WL_WSEC_INFO_BSS_KEY_LEN = (WL_WSEC_INFO_BSS_BASE + 5),
	WL_WSEC_INFO_BSS_ALGOS = (WL_WSEC_INFO_BSS_BASE + 6),
	WL_WSEC_INFO_BSS_WPA_AP_RESTRICT = (WL_WSEC_INFO_BSS_BASE + 7),
	WL_WSEC_INFO_BSS_PMK_PASSPHRASE = (WL_WSEC_INFO_BSS_BASE + 8),
	WL_WSEC_INFO_BSS_SAE_PWE = (WL_WSEC_INFO_BSS_BASE + 9),
	WL_WSEC_INFO_BSS_SAE_PK = (WL_WSEC_INFO_BSS_BASE + 0xA),
	WL_WSEC_INFO_6G_LEGACY_SEC = (WL_WSEC_INFO_BSS_BASE + 0xB),
	WL_WSEC_INFO_BSS_TD_POLICY = (WL_WSEC_INFO_BSS_BASE + 0xC), /* set TD policy */
	WL_WSEC_INFO_SAE_GROUPS = (WL_WSEC_INFO_BSS_BASE + 0xD),
	WL_WSEC_INFO_OCV = (WL_WSEC_INFO_BSS_BASE + 0xE),
	WL_WSEC_INFO_BSS_KEY_IDLE_TIME = (WL_WSEC_INFO_BSS_BASE + 0xF),
	/* Include RSNXE in  pure WPA-PSK mode */
	WL_WSEC_INFO_BSS_INCLUDE_RSNXE = (WL_WSEC_INFO_BSS_BASE + 0x10),
	/* set/get OWE DH group Id */
	WL_WSEC_INFO_OWE_DH_GROUP = (WL_WSEC_INFO_BSS_BASE + 0x11),

	/*
	 * ADD NEW ENUM ABOVE HERE
	 */
	/* WPA3 CTT testbed specific requirement. refer to WFA CAPI command list */
	WL_WSEC_INFO_TEST_UNUSED = (WL_WSEC_INFO_TEST_BASE),	/* reserved for future use */
	WL_WSEC_INFO_TEST_SAE_GROUP_REJ = (WL_WSEC_INFO_TEST_BASE + 1), /* rejected group ID */
	WL_WSEC_INFO_TEST_SAE_INVALID_VEC = (WL_WSEC_INFO_TEST_BASE + 2), /* test SAE vector */
	WL_WSEC_INFO_TEST_PMK = (WL_WSEC_INFO_TEST_BASE + 3),		/* query PMK */
	WL_WSEC_INFO_TEST_UNUSED1 = (WL_WSEC_INFO_TEST_BASE + 4),	/* reserved for future */
	WL_WSEC_INFO_TEST_INVALID_OCI = (WL_WSEC_INFO_TEST_BASE + 5),	/* OCV invalid OCI */
	WL_WSEC_INFO_TEST_PMKSA_CACHE = (WL_WSEC_INFO_TEST_BASE + 6),	/* PMKSA cache on/off */
	WL_WSEC_INFO_TEST_IGNORE_CSA = (WL_WSEC_INFO_TEST_BASE + 7),	/* Ignore CSA */
	WL_WSEC_INFO_TEST_IGNORE_ASSOCRESP = (WL_WSEC_INFO_TEST_BASE + 8), /* Ignore reassoc_resp */
	WL_WSEC_INFO_TEST_DISASSOC_MFP_TMO = (WL_WSEC_INFO_TEST_BASE + 0xA),
	/* sending disassoc frame when MFP query timed out */

	/* add per-BSS properties above */
	WL_WSEC_INFO_MAX = 0xffff
} wl_wsec_info_type_t;

#define WL_WSEC_PMK_INFO_VERSION		0x0100 /**< version 1.0 */

typedef uint16 wl_wsec_info_pmk_info_flags_t;
typedef uint32 wl_wsec_info_pmk_lifetime_t;
typedef uint8 wl_wsec_info_akm_mask_t;
typedef uint16 wl_wsec_info_pmk_info_flags;

enum {
	WL_WSEC_PMK_INFO_SSID_PRESENT	= 0x1,
	WL_WSEC_PMK_DEFAULT_LIFETIME	= 0x2
};

struct bcm_xlo {
	uint16 len;
	uint16 off;
};
typedef struct bcm_xlo bcm_xlo_t;

/* Supported operations (actions) for PMK info */

/* Add passphrase/pmk entry with the info provided..this is default behaviour */
#define WL_WSEC_PMK_INFO_ADD 0x0u
/* Search passphrase entry with info provided and delete it */
#define WL_WSEC_PMK_INFO_DEL 0x1u

/*
** all offsets are from the beginning of the structure that starts
** with the version field and length field is the total length of the structure
** including the version and length fields
*/
typedef struct wl_wsec_info_pmk_info {
	uint16 version;  /* WL_WSEC_PMK_INFO_VERSION */
	uint16 len;
	uint16 next_offset;  /* If non zero, specifies offset of next next_offset field */
	wl_wsec_info_pmk_info_flags_t flags;	/* Fill in the input based on the flags */
	wl_wsec_info_pmk_lifetime_t pmk_lifetime;
	wl_wsec_info_akm_mask_t akm_mask;	/* RSN authenticated key management suite */
	uint8 action;		/* add/del */
	uint8 rsvd[2];		/* reserved for future use */
	bcm_xlo_t ssid; /* ssid - key, zero length is allowed for SSID */
	bcm_xlo_t bssid;	/* bssid - key, zero length = broadcast/wildcard */
	bcm_xlo_t pass_id;	/* key - optional password id for SAE */
	bcm_xlo_t pmk;	/* pmk - either 32 or 48 byte for SuiteB-192 */
	bcm_xlo_t passphrase;	/* passphrase info */
	/* data follows */
} wl_wsec_info_pmk_info_t;

typedef struct {
	uint32 algos; /* set algos to be enabled/disabled */
	uint32 mask; /* algos outside mask unaltered */
} wl_wsec_info_algos_t;

/** tlv used to return wl_wsec_info properties */
typedef struct {
	uint16 type;
	uint16 len;		/**< data length */
	uint8 data[1];	/**< data follows */
} wl_wsec_info_tlv_t;

/** input/output data type for wsec_info iovar */
typedef struct wl_wsec_info {
	uint8 version; /**< structure version */
	uint8 PAD[2];
	uint8 num_tlvs;
	wl_wsec_info_tlv_t tlvs[BCM_FLEX_ARRAY]; /**< tlv data follows */
} wl_wsec_info_t;

#define AP_BLOCK_NONE		0x00000000u	/* No restriction (default) */
/* Policy when WPA3/SAE is configured for the BSS */
#define AP_ALLOW_WPA2		0x00000001u	/* Allow WPA2PSK AP during join or roam */
#define AP_ALLOW_WPA3_TO_WPA2_TSN	0x00000002u	/* Replacement for AP_ALLOW_TSN */
#define AP_ALLOW_TSN			AP_ALLOW_WPA3_TO_WPA2_TSN
#define AP_ALLOW_WPA3_ONLY	0x00000004u	/* Allow WPA3 only AP during join or roam */
/* AP_ALLOW_WPA3_ONLY is write only
** supports AP_ALLOW_WPA3_2G_5G_ONLY and AP_ALLOW_WPA3_6G_ONLY
** DEPRECATED moving forward
*/
/* Policy when WPA2 PSK, but not SAE is configured for the BSS */
#define AP_WPA2_PSK_NO_MIX_SEC	0x00000008u	/* Disallow Mixed WPA/WPA2 security during roam */

#define AP_ALLOW_WPA2_to_WPA_NO_TSN	0x00000008u /* Replacement for AP_WPA2_PSK_NO_MIX_SEC */
#define AP_ALLOW_WPA3_TO_WPA_NO_TSN	0x00000040u /* Dont allow WPA3 to WPA TSN */
#define AP_ALLOW_NO_TSN			(AP_ALLOW_WPA3_TO_WPA_NO_TSN | AP_ALLOW_WPA2_to_WPA_NO_TSN)
#define AP_ALLOW_WPA3_2G_5G_ONLY 0x00000010u	/* Allow WPA3 only 2G/5G AP join or roam */
#define AP_ALLOW_WPA3_6G_ONLY	0x00000020u	/* Allow WPA3 only 6G AP during join or roam */
/* All flags */
#define AP_ALLOW_MAX	(AP_ALLOW_WPA2 | AP_ALLOW_TSN | \
	AP_ALLOW_WPA3_ONLY | AP_WPA2_PSK_NO_MIX_SEC) /* DEPRECATED moving forward */
/* AP_ALLOW_MAX  will be defined in the src component */

/* OWE roaming policy bit definitions */

#define AP_ALLOW_OPEN_ONLY		0x00010000u /* Allow roam to open APs (2G and 5G) */
#define AP_ALLOW_OWE_TRANS_2G_5G	0x00020000u /* Allow roam to owe transition n/w */
#define AP_ALLOW_OWE_ONLY_IN_2G_5G	0x00040000u /* Allow roam to owe only APs in 2G or 5G */
#define AP_ALLOW_OWE_ONLY_IN_6G	0x00080000u /* Allow roam to owe only APs in 6g only */

/* Mask to include WPA3 Roam Policy */
#define WPA3_ROAM_POLICY_MASK	(AP_ALLOW_WPA2 | AP_ALLOW_TSN | \
	AP_ALLOW_WPA3_ONLY | AP_WPA2_PSK_NO_MIX_SEC | \
	AP_ALLOW_WPA3_2G_5G_ONLY | AP_ALLOW_WPA3_6G_ONLY)

/* Mask to determine OWE Roam Policy */
#define OWE_ROAM_POLICY_MASK	(AP_ALLOW_OPEN_ONLY | AP_ALLOW_OWE_TRANS_2G_5G | \
	AP_ALLOW_OWE_ONLY_IN_6G | AP_ALLOW_OWE_ONLY_IN_2G_5G)

/*  If MLO is enabled, then restrict MLO-OWE connection to only OWE APs */
#define OWE_MLO_ROAM_POLICY_MASK	(AP_ALLOW_OWE_ONLY_IN_6G | AP_ALLOW_OWE_ONLY_IN_2G_5G)

typedef struct {
	uint32 wpa_ap_restrict; /* set WPA2 / WPA3 AP restriction policy */
} wl_wsec_info_wpa_ap_restrict_t;

/* SAE PWE derivation method */
#define SAE_PWE_INVALID	0x0u
#define SAE_PWE_LOOP	0x1u
#define SAE_PWE_H2E	0x2u

/* SAE PK modes */
#define WSEC_SAE_PK_NONE	0u
#define WSEC_SAE_PK_ENABLED	0x1u
#define WSEC_SAE_PK_ONLY	0x2u

/* HE 6Ghz security bitmap */
#define WL_HE_6G_SEC_DISABLE	0x00u	/* HE 6G Open Security support disable */
#define WL_HE_6G_SEC_OPEN	0x01u	/* HE 6G Open Security support */

/*
 * randmac definitions
 */
#define WL_RANDMAC_MODULE			"randmac"
#define WL_RANDMAC_API_VERSION		0x0100 /**< version 1.0 */
#define WL_RANDMAC_API_MIN_VERSION	0x0100 /**< version 1.0 */

/** subcommands that can apply to randmac */
enum {
	WL_RANDMAC_SUBCMD_NONE				= 0,
	WL_RANDMAC_SUBCMD_GET_VERSION			= 1,
	WL_RANDMAC_SUBCMD_ENABLE			= 2,
	WL_RANDMAC_SUBCMD_DISABLE			= 3,
	WL_RANDMAC_SUBCMD_CONFIG			= 4,
	WL_RANDMAC_SUBCMD_STATS				= 5,
	WL_RANDMAC_SUBCMD_CLEAR_STATS			= 6,

	WL_RANDMAC_SUBCMD_MAX
};
typedef int16 wl_randmac_subcmd_t;

/* Common IOVAR struct */
typedef struct wl_randmac {
	uint16 version;
	uint16 len;			/* total length */
	wl_randmac_subcmd_t subcmd_id;	/* subcommand id */
	uint8 data[0];			/* subcommand data */
} wl_randmac_t;

#define WL_RANDMAC_IOV_HDR_SIZE OFFSETOF(wl_randmac_t, data)

/* randmac version subcommand */
typedef struct wl_randmac_version {
	uint16 version;  /* Randmac method version info */
	uint8 PAD[2];    /* Align on 4 byte boundary */
} wl_randmac_version_t;

/*
 * Bitmask for methods supporting MAC randomization feature
 */
#define WL_RANDMAC_USER_NONE		0x0000
#define WL_RANDMAC_USER_FTM		0x0001
#define WL_RANDMAC_USER_NAN		0x0002
#define WL_RANDMAC_USER_SCAN		0x0004
#define WL_RANDMAC_USER_ANQP		0x0008
#define WL_RANDMAC_USER_ALL		0xFFFF
typedef uint16 wl_randmac_method_t;

enum {
	WL_RANDMAC_FLAGS_NONE	= 0x00,
	WL_RANDMAC_FLAGS_ADDR	= 0x01,
	WL_RANDMAC_FLAGS_MASK	= 0x02,
	WL_RANDMAC_FLAGS_METHOD	= 0x04,
	WL_RANDMAC_FLAGS_ALL	= 0xFF
};
typedef uint8 wl_randmac_flags_t;

/* randmac statistics subcommand */
typedef struct wl_randmac_stats {
	uint32 set_ok;		/* Set random addr success count */
	uint32 set_fail;	/* Set random addr failed count */
	uint32 set_reqs;	/* Set random addr count */
	uint32 reset_reqs;	/* Restore random addr count */
	uint32 restore_ok;	/* Restore random addr succes count */
	uint32 restore_fail;	/* Restore random addr failed count */
	uint32 events_sent;	/* randmac module events count */
	uint32 events_rcvd;	/* randmac events received count */
} wl_randmac_stats_t;

/* randmac config subcommand */
typedef struct wl_randmac_config {
	struct ether_addr addr;			/* Randomized MAC address */
	struct ether_addr addr_mask;		/* bitmask for randomization */
	wl_randmac_method_t method;		/* Enabled methods */
	wl_randmac_flags_t flags;		/* What config info changed */
	uint8	PAD;
} wl_randmac_config_t;

enum {
	WL_RANDMAC_EVENT_NONE			= 0,	/**< not an event, reserved */
	WL_RANDMAC_EVENT_BSSCFG_ADDR_SET	= 1,	/* bsscfg addr randomized */
	WL_RANDMAC_EVENT_BSSCFG_ADDR_RESTORE	= 2,	/* bsscfg addr restored */
	WL_RANDMAC_EVENT_ENABLED		= 3,	/* randmac module enabled */
	WL_RANDMAC_EVENT_DISABLE		= 4,	/* randmac module disabled */
	WL_RANDMAC_EVENT_BSSCFG_STATUS	= 5,	/* bsscfg enable/disable */

	WL_RANDMAC_EVENT_MAX
};
typedef int16 wl_randmac_event_type_t;
typedef int32 wl_randmac_status_t;
typedef uint32 wl_randmac_event_mask_t;

#define WL_RANDMAC_EVENT_MASK_ALL 0xfffffffe
#define WL_RANDMAC_EVENT_MASK_EVENT(_event_type) (1 << (_event_type))
#define WL_RANDMAC_EVENT_ENABLED(_mask, _event_type) (\
	((_mask) & WL_RANDMAC_EVENT_MASK_EVENT(_event_type)) != 0)

/** tlv IDs - data length 4 bytes unless overridden by type, alignment 32 bits */
enum {
	WL_RANDMAC_TLV_NONE		= 0,
	WL_RANDMAC_TLV_METHOD		= 1,
	WL_RANDMAC_TLV_ADDR		= 2,
	WL_RANDMAC_TLV_MASK		= 3
};
typedef uint16 wl_randmac_tlv_id_t;

typedef struct wl_randmac_tlv {
	wl_randmac_tlv_id_t id;
	uint16 len;		/* Length of variable */
	uint8  data[1];
} wl_randmac_tlv_t;

/** randmac event */
typedef struct wl_randmac_event {
	uint16			version;
	uint16			len;			/* Length of all variables */
	wl_randmac_event_type_t	type;
	wl_randmac_method_t	method;
	uint8			PAD[2];
	wl_randmac_tlv_t	tlvs[BCM_FLEX_ARRAY];	/**< variable */
} wl_randmac_event_t;

/*
 * scan MAC definitions
 */

/** common iovar struct */
typedef struct wl_scanmac {
	uint16 subcmd_id;	/**< subcommand id */
	uint16 len;		/**< total length of data[] */
	uint8 data[];		/**< subcommand data */
} wl_scanmac_t;

/* subcommand ids */
#define WL_SCANMAC_SUBCMD_ENABLE   0
#define WL_SCANMAC_SUBCMD_BSSCFG   1u   /**< only GET supported */
#define WL_SCANMAC_SUBCMD_CONFIG   2u
#define WL_SCANMAC_SUBCMD_MACADDR  3u

/** scanmac enable data struct */
typedef struct wl_scanmac_enable {
	uint8 enable;	/**< 1 - enable, 0 - disable */
	uint8 PAD[3];	/**< 4-byte struct alignment */
} wl_scanmac_enable_t;

/** scanmac bsscfg data struct */
typedef struct wl_scanmac_bsscfg {
	uint32 bsscfg;	/**< bsscfg index */
} wl_scanmac_bsscfg_t;

/** scanmac config data struct */
typedef struct wl_scanmac_config {
	struct ether_addr mac;	/**< 6 bytes of MAC address or MAC prefix (i.e. OUI) */
	struct ether_addr random_mask;	/**< randomized bits on each scan */
	uint16 scan_bitmap;	/**< scans to use this MAC address */
	uint8 PAD[2];	/**< 4-byte struct alignment */
} wl_scanmac_config_t;

/** scanmac mac addr data struct */
typedef struct wl_scanmac_macaddr {
	struct ether_addr mac;	/* last mac address used for scan. either randomized or permanent */
} wl_scanmac_macaddr_t;

/* scan bitmap */
#define WL_SCANMAC_SCAN_UNASSOC         (0x01 << 0u)	/**< unassociated scans */
#define WL_SCANMAC_SCAN_ASSOC_ROAM	(0x01 << 1u)	/**< associated roam scans */
#define WL_SCANMAC_SCAN_ASSOC_PNO	(0x01 << 2u)	/**< associated PNO scans */
#define WL_SCANMAC_SCAN_ASSOC_HOST	(0x01 << 3u)	/**< associated host scans */
#define WL_SCANMAC_SCAN_RAND_PERPRQ	(0x01 << 4u)	/* enable per probe rand */
#define WL_SCANMAC_SCAN_RAND_SCANCH	(0x01 << 5u)	/* enable scan chan rand */


#define WL_SCAN_EVENT_VER1	1
#define WL_SCAN_EVENT_VER2	2

#define WL_SCAN_TYPE_ASSOC      0x1   /* Assoc scan     */
#define WL_SCAN_TYPE_ROAM       0x2   /* Roam scan     */
#define WL_SCAN_TYPE_FWSCAN     0x4   /* Other FW scan     */
#define WL_SCAN_TYPE_HOSTSCAN   0x8   /* Host scan     */

typedef struct scan_event_data {
	uint32 version;
	uint32 flags;
	uint16 num_chan_slice0;
	uint16 num_chan_slice1;
	/* Will contain num_chan_slice0 followed by num_chan_slice1 chanspecs */
	chanspec_t scan_chan_list[];
} scan_event_data_v1_t;

/** tlv used to return chanspec list of each slice */
typedef struct scan_chan_tlv {
	uint16 type; /* slice index */
	uint16 len; /* data length */
	uint8 data[BCM_FLEX_ARRAY]; /* chanspec list */
} scan_chan_tlv_t;

typedef struct scan_event_data_v2 {
	uint32 version;
	uint32 flags;
	uint32 num_tlvs; /* no of chanspec list tlvs */
	uint8  tlvs[BCM_FLEX_ARRAY];
} scan_event_data_v2_t;
#define WL_SCAN_EVENT_FIXED_LEN_V2	OFFSETOF(scan_event_data_v2_t, tlvs)


/*
 * bonjour dongle offload definitions
 */

/* common iovar struct */
typedef struct wl_bdo {
	uint16 subcmd_id;	/* subcommand id */
	uint16 len;		/* total length of data[] */
	uint8 data[];		/* subcommand data */
} wl_bdo_t;

/* subcommand ids */
#define WL_BDO_SUBCMD_DOWNLOAD		0	/* Download flattened database  */
#define WL_BDO_SUBCMD_ENABLE		1	/* Start bonjour after download */
#define WL_BDO_SUBCMD_MAX_DOWNLOAD	2	/* Get the max download size    */

/* maximum fragment size */
#define BDO_MAX_FRAGMENT_SIZE	1024

/* download flattened database
 *
 * BDO must be disabled before database download else fail.
 *
 * If database size is within BDO_MAX_FRAGMENT_SIZE then only a single fragment
 * is required (i.e. frag_num = 0, total_size = frag_size).
 * If database size exceeds BDO_MAX_FRAGMENT_SIZE then multiple fragments are required.
 */
typedef struct wl_bdo_download {
	uint16 total_size;	/* total database size */
	uint16 frag_num;	/* fragment number, 0 for first fragment, N-1 for last fragment */
	uint16 frag_size;	/* size of fragment (max BDO_MAX_FRAGMENT_SIZE) */
	uint8 PAD[2];		/* 4-byte struct alignment */
	uint8 fragment[BDO_MAX_FRAGMENT_SIZE];  /* fragment data */
} wl_bdo_download_t;

/* enable
 *
 * Enable requires a downloaded database else fail.
 */
typedef struct wl_bdo_enable {
	uint8 enable;	/* 1 - enable, 0 - disable */
	uint8 PAD[3];	/* 4-byte struct alignment */
} wl_bdo_enable_t;

/*
 * Get the max download size for Bonjour Offload.
 */
typedef struct wl_bdo_max_download {
	uint16 size;	/* Max download size in bytes */
	uint8 PAD[2];	/* 4-byte struct alignment    */
} wl_bdo_max_download_t;

/*
 * TCP keepalive offload definitions
 */

/* common iovar struct */
typedef struct wl_tko {
	uint16 subcmd_id;	/* subcommand id */
	uint16 len;		/* total length of data[] */
	uint8 data[];		/* subcommand data */
} wl_tko_t;

/* subcommand ids */
#define WL_TKO_SUBCMD_MAX_TCP		0	/* max TCP connections supported */
#define WL_TKO_SUBCMD_PARAM		1	/* configure offload common parameters  */
#define WL_TKO_SUBCMD_CONNECT		2	/* TCP connection info */
#define WL_TKO_SUBCMD_ENABLE		3	/* enable/disable */
#define WL_TKO_SUBCMD_STATUS		4	/* TCP connection status */

/* WL_TKO_SUBCMD_MAX_CONNECT subcommand data */
typedef struct wl_tko_max_tcp {
	uint8 max;	/* max TCP connections supported */
	uint8 PAD[3];	/* 4-byte struct alignment */
} wl_tko_max_tcp_t;

/* WL_TKO_SUBCMD_PARAM subcommand data */
typedef struct wl_tko_param {
	uint16 interval;	/* keepalive tx interval (secs) */
	uint16 retry_interval;	/* keepalive retry interval (secs) */
	uint16 retry_count;	/* retry_count */
	uint8 PAD[2];		/* 4-byte struct alignment */
} wl_tko_param_t;

/* WL_TKO_SUBCMD_CONNECT subcommand data
 * invoke with unique 'index' for each TCP connection
 */
typedef struct wl_tko_connect {
	uint8 index;		/* TCP connection index, 0 to max-1 */
	uint8 ip_addr_type;	/* 0 - IPv4, 1 - IPv6 */
	uint16 local_port;	/* local port */
	uint16 remote_port;	/* remote port */
	uint16 PAD;
	uint32 local_seq;	/* local sequence number */
	uint32 remote_seq;	/* remote sequence number */
	uint16 request_len;	/* TCP keepalive request packet length */
	uint16 response_len;	/* TCP keepalive response packet length */
	uint8 data[];		/* variable length field containing local/remote IPv4/IPv6,
				 * TCP keepalive request packet, TCP keepalive response packet
				 *    For IPv4, length is 4 * 2 + request_length + response_length
				 *       offset 0 - local IPv4
				 *       offset 4 - remote IPv4
				 *       offset 8 - TCP keepalive request packet
				 *       offset 8+request_length - TCP keepalive response packet
				 *    For IPv6, length is 16 * 2 + request_length + response_length
				 *       offset 0 - local IPv6
				 *       offset 16 - remote IPv6
				 *       offset 32 - TCP keepalive request packet
				 *       offset 32+request_length - TCP keepalive response packet
				 */
} wl_tko_connect_t;

/* WL_TKO_SUBCMD_CONNECT subcommand data to GET configured info for specific index */
typedef struct wl_tko_get_connect {
	uint8 index;		/* TCP connection index, 0 to max-1 */
	uint8 PAD[3];		/* 4-byte struct alignment */
} wl_tko_get_connect_t;

typedef struct wl_tko_enable {
	uint8 enable;	/* 1 - enable, 0 - disable */
	uint8 PAD[3];	/* 4-byte struct alignment */
} wl_tko_enable_t;

/* WL_TKO_SUBCMD_STATUS subcommand data */
/* must be invoked before tko is disabled else status is unavailable */
typedef struct wl_tko_status {
	uint8 count;		/* number of status entries (i.e. equals
				 * max TCP connections supported)
	                         */
	uint8 status[BCM_FLEX_ARRAY];	/* variable length field contain status for
				 * each TCP connection index
				 */
} wl_tko_status_t;

typedef enum {
	TKO_STATUS_NORMAL			= 0,	/* TCP connection normal, no error */
	TKO_STATUS_NO_RESPONSE			= 1,	/* no response to TCP keepalive */
	TKO_STATUS_NO_TCP_ACK_FLAG		= 2,	/* TCP ACK flag not set */
	TKO_STATUS_UNEXPECT_TCP_FLAG		= 3,	/* unexpect TCP flags set other than ACK */
	TKO_STATUS_SEQ_NUM_INVALID		= 4,	/* ACK != sequence number */
	TKO_STATUS_REMOTE_SEQ_NUM_INVALID	= 5,	/* SEQ > remote sequence number */
	TKO_STATUS_TCP_DATA			= 6,	/* TCP data available */
	TKO_STATUS_UNAVAILABLE			= 255,	/* not used/configured */
} tko_status_t;

enum rssi_reason {
	RSSI_REASON_UNKNOW = 0,
	RSSI_REASON_LOWRSSI = 1,
	RSSI_REASON_NSYC = 2,
	RSSI_REASON_TIMEOUT = 3
};

enum tof_reason {
	TOF_REASON_OK = 0,
	TOF_REASON_REQEND = 1,
	TOF_REASON_TIMEOUT = 2,
	TOF_REASON_NOACK = 3,
	TOF_REASON_INVALIDAVB = 4,
	TOF_REASON_INITIAL = 5,
	TOF_REASON_ABORT = 6
};

enum rssi_state {
	RSSI_STATE_POLL = 0,
	RSSI_STATE_TPAIRING = 1,
	RSSI_STATE_IPAIRING = 2,
	RSSI_STATE_THANDSHAKE = 3,
	RSSI_STATE_IHANDSHAKE = 4,
	RSSI_STATE_CONFIRMED = 5,
	RSSI_STATE_PIPELINE = 6,
	RSSI_STATE_NEGMODE = 7,
	RSSI_STATE_MONITOR = 8,
	RSSI_STATE_LAST = 9
};

enum tof_state {
	TOF_STATE_IDLE	 = 0,
	TOF_STATE_IWAITM = 1,
	TOF_STATE_TWAITM = 2,
	TOF_STATE_ILEGACY = 3,
	TOF_STATE_IWAITCL = 4,
	TOF_STATE_TWAITCL = 5,
	TOF_STATE_ICONFIRM = 6,
	TOF_STATE_IREPORT = 7
};

enum tof_mode_type {
	TOF_LEGACY_UNKNOWN	= 0,
	TOF_LEGACY_AP		= 1,
	TOF_NONLEGACY_AP	= 2
};

enum tof_way_type {
	TOF_TYPE_ONE_WAY = 0,
	TOF_TYPE_TWO_WAY = 1,
	TOF_TYPE_REPORT = 2
};

enum tof_rate_type {
	TOF_FRAME_RATE_VHT = 0,
	TOF_FRAME_RATE_LEGACY = 1
};

#define TOF_ADJ_TYPE_NUM	4	/**< number of assisted timestamp adjustment */
enum tof_adj_mode {
	TOF_ADJ_SOFTWARE = 0,
	TOF_ADJ_HARDWARE = 1,
	TOF_ADJ_SEQ = 2,
	TOF_ADJ_NONE = 3
};

#define FRAME_TYPE_NUM		4	/**< number of frame type */
enum frame_type {
	FRAME_TYPE_CCK	= 0,
	FRAME_TYPE_OFDM	= 1,
	FRAME_TYPE_11N	= 2,
	FRAME_TYPE_11AC	= 3
};

typedef struct wl_proxd_status_iovar {
	uint16			method;				/**< method */
	uint8			mode;				/**< mode */
	uint8			peermode;			/**< peer mode */
	uint8			state;				/**< state */
	uint8			reason;				/**< reason code */
	uint8			PAD[2];
	uint32			distance;			/**< distance */
	uint32			txcnt;				/**< tx pkt counter */
	uint32			rxcnt;				/**< rx pkt counter */
	struct ether_addr	peer;				/**< peer mac address */
	int8			avg_rssi;			/**< average rssi */
	int8			hi_rssi;			/**< highest rssi */
	int8			low_rssi;			/**< lowest rssi */
	uint8			PAD[3];
	uint32			dbgstatus;			/**< debug status */
	uint16			frame_type_cnt[FRAME_TYPE_NUM];	/**< frame types */
	uint8			adj_type_cnt[TOF_ADJ_TYPE_NUM];	/**< adj types HW/SW */
} wl_proxd_status_iovar_t;

/* ifdef NET_DETECT */
typedef struct net_detect_adapter_features {
	uint8	wowl_enabled;
	uint8	net_detect_enabled;
	uint8	nlo_enabled;
} net_detect_adapter_features_t;

typedef enum net_detect_bss_type {
	nd_bss_any = 0,
	nd_ibss,
	nd_ess
} net_detect_bss_type_t;

typedef struct net_detect_profile {
	wlc_ssid_t		ssid;
	net_detect_bss_type_t   bss_type;	/**< Ignore for now since Phase 1 is only for ESS */
	uint32			cipher_type;	/**< DOT11_CIPHER_ALGORITHM enumeration values */
	uint32			auth_type;	/**< DOT11_AUTH_ALGORITHM enumeration values */
} net_detect_profile_t;

typedef struct net_detect_profile_list {
	uint32			num_nd_profiles;
	net_detect_profile_t	nd_profile[];
} net_detect_profile_list_t;

typedef struct net_detect_config {
	uint8			    nd_enabled;
	uint8				PAD[3];
	uint32			    scan_interval;
	uint32			    wait_period;
	uint8			    wake_if_connected;
	uint8			    wake_if_disconnected;
	uint8				PAD[2];
	net_detect_profile_list_t   nd_profile_list;
} net_detect_config_t;

typedef enum net_detect_wake_reason {
	nd_reason_unknown,
	nd_net_detected,
	nd_wowl_event,
	nd_ucode_error
} net_detect_wake_reason_t;

typedef struct net_detect_wake_data {
	net_detect_wake_reason_t    nd_wake_reason;
	uint32			    nd_wake_date_length;
	uint8			    nd_wake_data[0];	    /**< Wake data (currently unused) */
} net_detect_wake_data_t;

/* endif NET_DETECT */

/* (unversioned, deprecated) */
typedef struct bcnreq {
	uint8 bcn_mode;
	uint8 PAD[3];
	int32 dur;
	int32 channel;
	struct ether_addr da;
	uint16 random_int;
	wlc_ssid_t ssid;
	uint16 reps;
	uint8 PAD[2];
} bcnreq_t;

#define WL_RRM_BCN_REQ_VER	1
typedef struct bcn_req {
	uint8 version;
	uint8 bcn_mode;
	uint8 pad_1[2];
	int32 dur;
	int32 channel;
	struct ether_addr da;
	uint16 random_int;
	wlc_ssid_t ssid;
	uint16 reps;
	uint8 req_elements;
	uint8 pad_2;
	chanspec_list_t chspec_list;
} bcn_req_t;

#define WL_RRM_BCN_REQ_VER_2	2u
struct wl_bcn_req_v2 {
	uint8	version;		/* size to be compatible with older version */
	uint8	pad1[1];
	uint16	length;			/* length for fixed struct + variable chanspec list */
	uint8	bcn_mode;
	uint8	last_bcn_rpt_ind;	/* Last Beacon Report Indication */
	uint8	bw_ind;			/* Bandwidth Indication */
	uint8	pad2[1];
	int32	dur;
	int32	channel;
	struct ether_addr da;
	uint16	random_int;
	wlc_ssid_t ssid;
	uint16	reps;
	uint8	req_elements;
	uint8	pad3[1];
	chanspec_list_t chspec_list;
};

typedef struct rrmreq {
	struct ether_addr da;
	uint8 reg;
	uint8 chan;
	uint16 random_int;
	uint16 dur;
	uint16 reps;
} rrmreq_t;

typedef struct framereq {
	struct ether_addr da;
	uint8 reg;
	uint8 chan;
	uint16 random_int;
	uint16 dur;
	struct ether_addr ta;
	uint16 reps;
} framereq_t;

typedef struct statreq {
	struct ether_addr da;
	struct ether_addr peer;
	uint16 random_int;
	uint16 dur;
	uint8 group_id;
	uint8 PAD;
	uint16 reps;
} statreq_t;

typedef struct txstrmreq {
	struct ether_addr da;	/* Destination address */
	uint16 random_int;	/* Random interval for measurement start */
	uint16 dur;		/* Measurement duration */
	uint16 reps;		/* number of repetitions */
	struct ether_addr peer;	/* Peer MAC address */
	uint8 tid;		/* Traffic ID */
	uint8 bin0_range;	/* Delay range of the first bin */
} txstrmreq_t;

typedef struct lcireq {
	struct ether_addr da;	/* Destination address */
	uint16 reps;		/* number of repetitions */
	uint8 subj;		/* Local/Remote/Thid party */
	uint8 lat_res;		/* Latitude requested Resolution */
	uint8 lon_res;		/* Longitude requested Resolution */
	uint8 alt_res;		/* Altitude requested Resolution */
} lcireq_t;

typedef struct civicreq {
	struct ether_addr da;	/* Destination address */
	uint16 reps;		/* number of repetitions */
	uint8 subj;		/* Local/Remote/Thid party */
	uint8 civloc_type;	/* Format of location info */
	uint8 siu;		/* Unit of Location service interval */
	uint8 PAD;
	uint16 si;		/* Location service interval */
} civicreq_t;

typedef struct locidreq {
	struct ether_addr da;	/* Destination address */
	uint16 reps;		/* number of repetitions */
	uint8 subj;		/* Local/Remote/Thid party */
	uint8 siu;		/* Unit of Location service interval */
	uint16 si;		/* Location service interval */
} locidreq_t;

typedef struct wl_rrm_config_ioc {
	uint16 version; /* command version */
	uint16 id;      /* subiovar cmd ID */
	uint16 len;     /* total length of all bytes in data[] */
	uint16 PAD;     /* 4-byte boundary padding */
	uint8 data[BCM_FLEX_ARRAY];  /* payload */
} wl_rrm_config_ioc_t;

enum {
	WL_RRM_CONFIG_NONE	= 0,	/* reserved */
	WL_RRM_CONFIG_GET_LCI	= 1,	/* get LCI */
	WL_RRM_CONFIG_SET_LCI	= 2,	/* set LCI */
	WL_RRM_CONFIG_GET_CIVIC	= 3,	/* get civic location */
	WL_RRM_CONFIG_SET_CIVIC	= 4,	/* set civic location */
	WL_RRM_CONFIG_GET_LOCID	= 5,	/* get location identifier */
	WL_RRM_CONFIG_SET_LOCID	= 6,	/* set location identifier */
	WL_RRM_CONFIG_MAX	= 7
};

#define WL_RRM_CONFIG_NAME "rrm_config"
#define WL_RRM_CONFIG_MIN_LENGTH OFFSETOF(wl_rrm_config_ioc_t, data)

enum {
	WL_RRM_EVENT_NONE		= 0,	/* not an event, reserved */
	WL_RRM_EVENT_FRNG_REQ	= 1,	/* Receipt of FRNG request frame */
	WL_RRM_EVENT_FRNG_REP	= 2,	/* Receipt of FRNG report frame */

	WL_RRM_EVENT_MAX
};
typedef int16 wl_rrm_event_type_t;

typedef struct frngreq_target {
	uint32 bssid_info;
	uint8 channel;
	uint8 phytype;
	uint8 reg;
	uint8 PAD;
	struct ether_addr bssid;
	chanspec_t chanspec;
	uint32 sid;
} frngreq_target_t;

typedef struct frngreq {
	wl_rrm_event_type_t event;			/* RRM event type */
	struct ether_addr da;
	uint16 max_init_delay;	/* Upper bound of random delay, in TUs */
	uint8 min_ap_count;		/* Min FTM ranges requested (1-15) */
	uint8 num_aps;			/* Number of APs to range, at least min_ap_count */
	uint16 max_age;			/* Max elapsed time before FTM request, 0xFFFF = any */
	uint16 reps;			/* Number of repetitions of this measurement type */
	frngreq_target_t targets[BCM_FLEX_ARRAY];	/* Target BSSIDs to range */
} frngreq_t;

typedef struct frngrep_range {
	uint32 start_tsf;		/* 4 lsb of tsf */
	struct ether_addr bssid;
	uint8 PAD[2];
	uint32 range;
	uint32 max_err;
	uint8  rsvd;
	uint8 PAD[3];
} frngrep_range_t;

typedef struct frngrep_error {
	uint32 start_tsf;		/* 4 lsb of tsf */
	struct ether_addr bssid;
	uint8  code;
	uint8 PAD[1];
} frngrep_error_t;

typedef struct frngrep {
	wl_rrm_event_type_t event;			/* RRM event type */
	struct ether_addr da;
	uint8 range_entry_count;
	uint8 error_entry_count;
	uint16 dialog_token;				/* dialog token */
	frngrep_range_t range_entries[DOT11_FTM_RANGE_ENTRY_MAX_COUNT];
	frngrep_error_t error_entries[DOT11_FTM_RANGE_ERROR_ENTRY_MAX_COUNT];
} frngrep_t;

typedef struct wl_rrm_frng_ioc {
	uint16 version; /* command version */
	uint16 id;      /* subiovar cmd ID */
	uint16 len;     /* total length of all bytes in data[] */
	uint16 PAD;     /* 4-byte boundary padding */
	uint8 data[];  /* payload */
} wl_rrm_frng_ioc_t;

enum {
	WL_RRM_FRNG_NONE	= 0,	/* reserved */
	WL_RRM_FRNG_SET_REQ	= 1,	/* send ftm ranging request */
	WL_RRM_FRNG_MAX		= 2
};

#define WL_RRM_FRNG_NAME "rrm_frng"
#define WL_RRM_FRNG_MIN_LENGTH OFFSETOF(wl_rrm_frng_ioc_t, data)

#define WL_RRM_RPT_VER		0
#define WL_RRM_RPT_MAX_PAYLOAD	256
#define WL_RRM_RPT_MIN_PAYLOAD	7
#define WL_RRM_RPT_FALG_ERR	0
#define WL_RRM_RPT_FALG_GRP_ID_PROPR	(1 << 0)
#define WL_RRM_RPT_FALG_GRP_ID_0	(1 << 1)
typedef struct {
	uint16 ver;		/**< version */
	struct ether_addr addr;	/**< STA MAC addr */
	uint32 timestamp;	/**< timestamp of the report */
	uint16 flag;		/**< flag */
	uint16 len;		/**< length of payload data */
	uint8 data[WL_RRM_RPT_MAX_PAYLOAD];
} statrpt_t;

typedef struct wlc_dwds_config {
	uint32		enable;
	uint32		mode; /**< STA/AP interface */
	struct ether_addr ea;
	uint8  PAD[2];
} wlc_dwds_config_t;

typedef struct wl_el_set_params_s {
	uint8 set;	/**< Set number */
	uint8  PAD[3];
	uint32 size;	/**< Size to make/expand */
} wl_el_set_params_t;

typedef struct wl_el_tag_params_s {
	uint16 tag;
	uint8 set;
	uint8 flags;
} wl_el_tag_params_t;

#define EVENT_LOG_SET_TYPE_VERSION_0 0u
typedef struct wl_el_set_type_s {
	uint16	version;
	uint16	len;
	uint8	set;	/* Set number */
	uint8	type;	/* Type- EVENT_LOG_SET_TYPE_DEFAULT or EVENT_LOG_SET_TYPE_PRSRV */
	uint16	PAD;
} wl_el_set_type_t;

#define EVENT_LOG_SET_TYPE_ALL_V1 1u

typedef struct wl_el_set_type_s_v1 {
	uint8 set_val;
	uint8 type_val;
} wl_el_set_type_v1_t;

typedef struct wl_el_set_all_type_s_v1 {
	uint16	version;
	uint16	len;
	uint32	max_sets;
	wl_el_set_type_v1_t set_type[BCM_FLEX_ARRAY]; /* set-Type Values Array */
} wl_el_set_all_type_v1_t;

typedef struct wl_staprio_cfg {
	struct ether_addr ea;	/**< mac addr */
	uint8 prio;		/**< scb priority */
} wl_staprio_cfg_t;

#define STAMON_STACONFIG_VER    1
/* size of struct wlc_stamon_sta_config_t elements */
#define STAMON_STACONFIG_LENGTH 20

typedef enum wl_stamon_cfg_cmd_type {
	STAMON_CFG_CMD_DEL = 0,
	STAMON_CFG_CMD_ADD = 1,
	STAMON_CFG_CMD_ENB = 2,
	STAMON_CFG_CMD_DSB = 3,
	STAMON_CFG_CMD_CNT = 4,
	STAMON_CFG_CMD_RSTCNT = 5,
	STAMON_CFG_CMD_GET_STATS = 6,
	STAMON_CFG_CMD_SET_MONTIME = 7
} wl_stamon_cfg_cmd_type_t;

typedef struct wlc_stamon_sta_config {
	wl_stamon_cfg_cmd_type_t cmd;	/**< 0 - delete, 1 - add */
	struct ether_addr ea;
	uint16	version;		/* Command structure version */
	uint16	length;			/* Command structure length */
	uint8	PAD[2];
	/* Time (ms) for which STA's are monitored. Value ZERO indicates no time limit */
	uint32	monitor_time;
} wlc_stamon_sta_config_t;

/* ifdef SR_DEBUG */
typedef struct /* pmu_reg */{
	uint32  pmu_control;
	uint32  pmu_capabilities;
	uint32  pmu_status;
	uint32  res_state;
	uint32  res_pending;
	uint32  pmu_timer1;
	uint32  min_res_mask;
	uint32  max_res_mask;
	uint32  pmu_chipcontrol1[4];
	uint32  pmu_regcontrol[5];
	uint32  pmu_pllcontrol[5];
	uint32  pmu_rsrc_up_down_timer[31];
	uint32  rsrc_dep_mask[31];
} pmu_reg_t;
/* endif SR_DEBUG */

typedef struct wl_taf_define {
	struct ether_addr ea;	/**< STA MAC or 0xFF... */
	uint16 version;         /**< version */
	uint32 sch;             /**< method index */
	uint32 prio;            /**< priority */
	uint32 misc;            /**< used for return value */
	uint8  text[];         /**< used to pass and return ascii text */
} wl_taf_define_t;

/** Received Beacons lengths information */
#define WL_LAST_BCNS_INFO_FIXED_LEN		OFFSETOF(wlc_bcn_len_hist_t, bcnlen_ring)
typedef struct wlc_bcn_len_hist {
	uint16	ver;				/**< version field */
	uint16	cur_index;			/**< current pointed index in ring buffer */
	uint32	max_bcnlen;		/**< Max beacon length received */
	uint32	min_bcnlen;		/**< Min beacon length received */
	uint32	ringbuff_len;		/**< Length of the ring buffer 'bcnlen_ring' */
	uint32	bcnlen_ring[BCM_FLEX_ARRAY];	/**< ring buffer storing received beacon lengths */
} wlc_bcn_len_hist_t;

/* WDS net interface types */
#define WL_WDSIFTYPE_NONE  0x0 /**< The interface type is neither WDS nor DWDS. */
#define WL_WDSIFTYPE_WDS   0x1 /**< The interface is WDS type. */
#define WL_WDSIFTYPE_DWDS  0x2 /**< The interface is DWDS type. */

typedef struct wl_bssload_static {
	uint8 is_static;
	uint8  PAD;
	uint16 sta_count;
	uint8 chan_util;
	uint8  PAD;
	uint16 aac;
} wl_bssload_static_t;

/** IO Var Operations - the Value of iov_op In wlc_ap_doiovar */
typedef enum wlc_ap_iov_bss_operation {
	WLC_AP_IOV_OP_DELETE                   = -1,
	WLC_AP_IOV_OP_DISABLE                  = 0,
	WLC_AP_IOV_OP_ENABLE                   = 1,
	WLC_AP_IOV_OP_MANUAL_AP_BSSCFG_CREATE  = 2,
	WLC_AP_IOV_OP_MANUAL_STA_BSSCFG_CREATE = 3,
	WLC_AP_IOV_OP_MOVE                     = 4
} wlc_ap_iov_bss_oper_t;

/* LTE coex info */
/* Analogue of HCI Set MWS Signaling cmd */
typedef struct {
	int16	mws_rx_assert_offset;
	int16	mws_rx_assert_jitter;
	int16	mws_rx_deassert_offset;
	int16	mws_rx_deassert_jitter;
	int16	mws_tx_assert_offset;
	int16	mws_tx_assert_jitter;
	int16	mws_tx_deassert_offset;
	int16	mws_tx_deassert_jitter;
	int16	mws_pattern_assert_offset;
	int16	mws_pattern_assert_jitter;
	int16	mws_inact_dur_assert_offset;
	int16	mws_inact_dur_assert_jitter;
	int16	mws_scan_freq_assert_offset;
	int16	mws_scan_freq_assert_jitter;
	int16	mws_prio_assert_offset_req;
} wci2_config_t;

/** Analogue of HCI MWS Channel Params */
typedef struct {
	uint16	mws_rx_center_freq; /**< MHz */
	uint16	mws_tx_center_freq;
	uint16	mws_rx_channel_bw;  /**< KHz */
	uint16	mws_tx_channel_bw;
	uint8	mws_channel_en;
	uint8	mws_channel_type;   /**< Don't care for WLAN? */
} mws_params_t;

#define LTECX_MAX_NUM_PERIOD_TYPES	7

/* LTE Frame params */
typedef struct {
	uint16	mws_frame_dur;
	int16	mws_framesync_assert_offset;
	uint16	mws_framesync_assert_jitter;
	uint16  mws_period_dur[LTECX_MAX_NUM_PERIOD_TYPES];
	uint8	mws_period_type[LTECX_MAX_NUM_PERIOD_TYPES];
	uint8	mws_num_periods;
} mws_frame_config_t;

/** MWS wci2 message */
typedef struct {
	uint8	mws_wci2_data; /**< BT-SIG msg */
	uint8	PAD;
	uint16	mws_wci2_interval; /**< Interval in us */
	uint16	mws_wci2_repeat; /**< No of msgs to send */
} mws_wci2_msg_t;
/* MWS ANT map */
typedef struct {
	uint16	combo1; /* mws ant selection 1 */
	uint16	combo2; /* mws ant selection 2 */
	uint16	combo3; /* mws ant selection 3 */
	uint16	combo4; /* mws ant selection 4 */
} mws_ant_map_t;

/* MWS ANT map 2nd generation */
typedef struct {
	uint16	combo[16]; /* mws ant selection 2nd */
} mws_ant_map_t_2nd;

#define LTECX_MWS_ANTMAP_VERSION_V3 3u

/** flags indicating changed field */
enum {
	WL_MWS_ANT_MAP_2G		= 1,  /* 2g filed updated */
	WL_MWS_ANT_MAP_5G		= 2,  /* 5g filed updated */
	WL_MWS_ANT_MAP_6G		= 3   /* 6g filed updated */
};

/* MWS ANT map 3rd generation */
typedef struct {
	uint16	version;	/* Structure version */
	uint16	length;		/* Length of whole struct */
	uint16	band;		/* 2G/5G/6G */
	uint16	combo_2g[16];	/* mws ant selection 2g */
	uint16	combo_5g[16];	/* mws ant selection 5g */
	uint16	combo_6g[16];	/* mws ant selection 6g */
	uint8	PAD[2];		/* Padding for 4byte alignment */
} mws_ant_map_v3_t;

/* MWS Coex bitmap v2 map for Type0/Type6 */
typedef struct {
	uint16	bitmap_2G;     /* 2G Bitmap */
	uint16	bitmap_5G_lo;  /* 5G lo bitmap */
	uint16	bitmap_5G_mid; /* 5G mid bitmap */
	uint16	bitmap_5G_hi;  /* 5G hi bitmap */
} mws_coex_bitmap_v2_t;

/* MWS Coex bitmap v3 map for LTECX features */

#define LTECX_COEX_BITMAP_VERSION_V3 3u

/** flags indicating changed field */
enum {
	WL_MWS_COEX_BITMAP_2G		= (1 << 0),  /* 2g filed updated */
	WL_MWS_COEX_BITMAP_5G		= (1 << 1),  /* 5g filed updated */
	WL_MWS_COEX_BITMAP_6G		= (1 << 2)  /* 6g filed updated */
};

typedef struct {
	uint16	version;		/* Structure version */
	uint16	length;			/* Length of whole struct */
	uint8	flags;			/* Flags to indicate the updated field */
	uint8	PAD;
	uint16	bitmap_2G;		/* 2G Bitmap */
	uint16	bitmap_5G_lo;		/* 5G lo bitmap */
	uint16	bitmap_5G_mid;		/* 5G mid bitmap */
	uint16	bitmap_5G_hi;		/* 5G hi bitmap */
	uint16	bitmap_6G_lo_unii5;	/* 6G lo bitmap UNII5 */
	uint16	bitmap_6G_hi_unii5;	/* 6G hi bitmap UNII5 */
	uint16	bitmap_6G_unii6;	/* 6G UNII6 bitmap */
	uint16	bitmap_6G_unii7;	/* 6G UNII7 bitmap */
	uint16	bitmap_6G_unii8;	/* 6G UNII8 bitmap */
} mws_coex_bitmap_v3_t;

/* MWS OCL bitmap v2 map for LTECX features */
/* v2 map is for 4388 and future chips */
/* to cover 6g Channels */
/* The first generation map is defined */
/* by wl_mws_ocl_override_t */
/* Not a ltecoex sub commands */

#define LTECX_OCL_BITMAP_VERSION_V2 2u

/** flags indicating changed field */
enum {
	WL_MWS_OCL_BITMAP_2G		= (1 << 0),  /* 2g field updated */
	WL_MWS_OCL_BITMAP_5G		= (1 << 1),  /* 5g field updated */
	WL_MWS_OCL_BITMAP_6G		= (1 << 2)  /* 6g field updated */
};

typedef struct {
	uint16	version;		/* Structure version */
	uint16	length;			/* Length of whole struct */
	uint8	flags;			/* Flags to indicate the updated field */
	uint8	PAD;
	uint16	bitmap_2G;		/* 2G Bitmap */
	uint16	bitmap_5G_lo;		/* 5G lo bitmap */
	uint16	bitmap_5G_mid;		/* 5G mid bitmap */
	uint16	bitmap_5G_hi;		/* 5G hi bitmap */
	uint16	bitmap_6G_lo_unii5;	/* 6G lo bitmap UNII5 */
	uint16	bitmap_6G_hi_unii5;	/* 6G hi bitmap UNII5 */
	uint16	bitmap_6G_unii6;	/* 6G UNII6 bitmap */
	uint16	bitmap_6G_unii7;	/* 6G UNII7 bitmap */
	uint16	bitmap_6G_unii8;	/* 6G UNII8 bitmap */
} mws_ocl_bitmap_t;

/* To define the ctrl map */
/* to enable or disable spmi tx message */
#define LTECX_SPMITX_CTRL_VER 1u

typedef struct {
	uint16	version;		/* Structure version */
	uint16	length;			/* Length of whole struct */
	uint16	spmimsg_type;		/* SPMI MSG type */
	uint16	spmimsg_id;		/* SPMI MSG id */
	uint16	spmimsg_dis;		/* Disable or enable */
} mws_spmitx_ctrl_v1_t;

/* MWS SCAN_REQ Bitmap */
typedef struct mws_scanreq_params {
	uint16 idx;
	uint16 bm_2g;
	uint16 bm_5g_lo;
	uint16 bm_5g_mid;
	uint16 bm_5g_hi;
} mws_scanreq_params_t;

/* MWS BLNK bitmap v2 map for LTECX features */
/* v2 map is for 4388 and future chips */
/* to cover all 2g/5g/6g Channels */
/* The first generation map is defined */
/* by mws_scanreq_params_t */
/* Not a ltecoex sub commands */

#define LTECX_BLNK_BITMAP_VERSION_V2 2u

/** flags indicating changed field */
enum {
	WL_MWS_BLNK_BITMAP_2G		= (1 << 0),  /* 2g field updated */
	WL_MWS_BLNK_BITMAP_5G		= (1 << 1),  /* 5g field updated */
	WL_MWS_BLNK_BITMAP_6G		= (1 << 2)  /* 6g field updated */
};

typedef struct {
	uint16	version;	/* Structure version */
	uint16	length;		/* Length of whole struct */
	uint8	flags;		/* Flags to indicate the updated field */
	uint8	PAD;
	uint16	idx;
	uint16	bm_2G;		/* 2G Bitmap */
	uint16	bm_5G_lo;	/* 5G lo bitmap */
	uint16	bm_5G_mid;	/* 5G mid bitmap */
	uint16	bm_5G_hi;	/* 5G hi bitmap */
	uint16	bm_6G_lo_unii5;	/* 6G lo bitmap UNII5 */
	uint16	bm_6G_hi_unii5;	/* 6G hi bitmap UNII5 */
	uint16	bm_6G_unii6;	/* 6G UNII6 bitmap */
	uint16	bm_6G_unii7;	/* 6G UNII7 bitmap */
	uint16	bm_6G_unii8;	/* 6G UNII8 bitmap */
	uint8	PAD1[2];	/* Additional padding for 4byte alignment */
} mws_blnk_bitmap_t;

/* MWS rFEM RX mode map for LTECX features */

#define  LTECX_RFEM_MODEMAP_VERSION 1u

/** flags indicating changed field */
enum {
	WL_MWS_RFEM_MODEMAP_2G		= (1 << 0),  /* 2g filed updated */
	WL_MWS_RFEM_MODEMAP_5G		= (1 << 1),  /* 5g filed updated */
	WL_MWS_RFEM_MODEMAP_6G		= (1 << 2)  /* 6g filed updated */
};

typedef struct {
	uint16	version;		/* Structure version */
	uint16	length;			/* Length of whole struct */
	uint8	flags;			/* Flags to indicate the updated field */
	uint8	PAD;
	uint16	modemap_2G;		/* 2G Bitmap */
	uint16	modemap_5G_lo;		/* 5G lo bitmap */
	uint16	modemap_5G_mid;		/* 5G mid bitmap */
	uint16	modemap_5G_hi;		/* 5G hi bitmap */
	uint16	modemap_6G_lo_unii5;	/* 6G lo bitmap UNII5 */
	uint16	modemap_6G_hi_unii5;	/* 6G hi bitmap UNII5 */
	uint16	modemap_6G_unii6;	/* 6G UNII6 bitmap */
	uint16	modemap_6G_unii7;	/* 6G UNII7 bitmap */
	uint16	modemap_6G_unii8;	/* 6G UNII8 bitmap */
} mws_rfem_modemap_t;

#define LTECX_SCANPROT_CONFIG_VERSION 1u

/** flags indicating changed field */
enum {
	WL_MWS_SCANPROT_CONFIG_2G	= (1 << 0), /* 2g field updated */
	WL_MWS_SCANPROT_CONFIG_5G	= (1 << 1), /* 5g field updated */
	WL_MWS_SCANPROT_CONFIG_6G	= (1 << 2)  /* 6g field updated */
};

typedef struct {
	uint16	version;		 /* Structure version */
	uint16	length;			 /* Length of whole struct */
	uint8	flags;			 /* Flags to indicate the updated field */
	uint8	PAD;
	uint16	idx;
	uint16	config_2G;		 /* 2G Bitmap */
	uint16	config_5G_lo;		 /* 5G lo bitmap */
	uint16	config_5G_mid;		 /* 5G mid bitmap */
	uint16	config_5G_hi;		 /* 5G hi bitmap */
	uint16	config_6G_lo_unii5;	 /* 6G lo bitmap UNII5 */
	uint16	config_6G_hi_unii5;	 /* 6G hi bitmap UNII5 */
	uint16	config_6G_unii6;	 /* 6G UNII6 bitmap */
	uint16	config_6G_unii7;	 /* 6G UNII7 bitmap */
	uint16	config_6G_unii8;	 /* 6G UNII8 bitmap */
	uint16	PAD_ADDITIONAL;		 /* Additional padding for 4byte alignment */
} mws_scanprot_config_t;

#define LTECX_MWS_COND_ID_BITMAP_VERSION 1u

/** LTE coex MWS (Mobile Wireless Standard) flags indicating 2G/5G/6G changed field */
enum {
	WL_LTECX_MWS_COND_ID_BITMAP_2G		= (1u << 0),  /* 2g field updated */
	WL_LTECX_MWS_COND_ID_BITMAP_5G		= (1u << 1),  /* 5g field updated */
	WL_LTECX_MWS_COND_ID_BITMAP_6G		= (1u << 2)   /* 6g field updated */
};

/* LTE coex MWS (Mobile Wireless Standard) bitmap for 2G/5G/6G channels */
typedef struct mws_channel_bm {
	uint16	bm_2G;		/* 2G Bitmap */
	uint16	bm_5G_lo;	/* 5G lo bitmap */
	uint16	bm_5G_mid;	/* 5G mid bitmap */
	uint16	bm_5G_hi;	/* 5G hi bitmap */
	uint16	bm_6G_lo_unii5;	/* 6G lo bitmap UNII5 */
	uint16	bm_6G_hi_unii5;	/* 6G hi bitmap UNII5 */
	uint16	bm_6G_unii6;	/* 6G UNII6 bitmap */
	uint16	bm_6G_unii7;	/* 6G UNII7 bitmap */
	uint16	bm_6G_unii8;	/* 6G UNII8 bitmap */
} ltecx_mws_channel_bm_t;

/* LTE coex MWS (Mobile Wireless Standard) subcomand for configuring condition ids */
typedef struct mws_cond_id_bitmap {
	uint16			version;	/* Structure version */
	uint16			length;		/* Length of whole struct */
	uint8			flags;		/* Flags to indicate the updated field */
	uint8			cond_id;	/* condition id */
	ltecx_mws_channel_bm_t	channel_bm;	/* Bitmap of WLAN 2G/5G/6G channels */
} ltecx_mws_cond_id_bitmap_t;

#define LTECX_ASSOC_PROT_BITMAP_VERSION 1u

/** flags indicating changed field */
enum {
	MWS_ASSOC_PROT_BITMAP_2G		= (1 << 0),  /* 2g field updated */
	MWS_ASSOC_PROT_BITMAP_5G		= (1 << 1),  /* 5g field updated */
	MWS_ASSOC_PROT_BITMAP_6G		= (1 << 2)  /* 6g field updated */
};

typedef struct {
	uint16	version;		/* Structure version */
	uint16	length;			/* Length of whole struct */
	uint8	flags;			/* Flags to indicate the updated field */
	uint8	PAD;
	uint16	bitmap_2G;		/* 2G Bitmap */
	uint16	bitmap_5G_lo;		/* 5G lo bitmap */
	uint16	bitmap_5G_mid;		/* 5G mid bitmap */
	uint16	bitmap_5G_hi;		/* 5G hi bitmap */
	uint16	bitmap_6G_lo_unii5;	/* 6G lo bitmap UNII5 */
	uint16	bitmap_6G_hi_unii5;	/* 6G hi bitmap UNII5 */
	uint16	bitmap_6G_unii6;	/* 6G UNII6 bitmap */
	uint16	bitmap_6G_unii7;	/* 6G UNII7 bitmap */
	uint16	bitmap_6G_unii8;	/* 6G UNII8 bitmap */
} mws_assoc_prot_bitmap_t;

/* Definitions for LTE coex iovar */
#define WL_LTECX_VERSION_1	 1

/* LTE coex IOV sub command IDs */
typedef enum ltecx_cmd_id {
	WL_LTECX_CMD_VER		= 0,	/* LTECX version sub command */
	WL_LTECX_TYPE7_2G_COEX_BITMAP	= 1,	/* Type7 enable/disable bitmap for 2G */
	WL_LTECX_COEX_BITMAP		= 2,	/* bitmaps to enable/disable Coex */
	WL_LTECX_OCL_BITMAP		= 3,	/* bitmaps to enable/disable OCL by ltecoex */
	WL_LTECX_ANT_MAP		= 4,	/* bitmaps for antenna selection */
	WL_LTECX_SPMITX_CTRL		= 5,	/* to enable/disable SPMI TX MSG */
	WL_LTECX_TYPE7_BITMAP		= 6,	/* bitmaps to enable/disable sending type7 */
	WL_LTECX_BLNKREQ_BM		= 7,	/* to enable/disable blnk req */
	WL_LTECX_RFEMMODE_MAP		= 8,	/* to set wifi rFEM mode bitmap  */
	WL_LTECX_UNUSED			= 9,	/* Note:This can be used for future ltecx sub cmd */
	WL_LTECX_COND_ID_BM		= 10,	/* bitmap for condition ids */
	WL_LTECX_DISABLE_MSG_48_END	= 11,	/* to disable/enable Tx of msg 48 End */
	WL_LTECX_SCANPROT_CONFIG	= 12,	/* to determine TXblank or LAA throttling */
	WL_LTECX_ASSOC_PROT_MAP		= 13	/* wifi assoc prot request channel bitmap */
} ltecx_cmd_id_t;

/* MWS Type7 bitmap for LTECX feature */
/* v1 bitmap is for 4388 and future chips */
#define LTECX_TYPE7_BITMAP_VERSION_V1 1u

/** flags indicating changed field */
enum {
	WL_MWS_TYPE7_BITMAP_2G		= (1 << 0),  /* 2g field updated */
	WL_MWS_TYPE7_BITMAP_5G		= (1 << 1),  /* 5g field updated */
	WL_MWS_TYPE7_BITMAP_6G		= (1 << 2)  /* 6g field updated */
};

typedef struct {
	uint16	version;		/* Structure version */
	uint16	length;			/* Length of whole struct */
	uint8	flags;			/* Flags to indicate the updated field */
	uint8	PAD;
	uint16	bitmap_2G;		/* 2G Bitmap */
	uint16	bitmap_5G_lo;		/* 5G lo bitmap */
	uint16	bitmap_5G_mid;		/* 5G mid bitmap */
	uint16	bitmap_5G_hi;		/* 5G hi bitmap */
	uint16	bitmap_6G_lo_unii5;	/* 6G lo bitmap UNII5 */
	uint16	bitmap_6G_hi_unii5;	/* 6G hi bitmap UNII5 */
	uint16	bitmap_6G_unii6;	/* 6G UNII6 bitmap */
	uint16	bitmap_6G_unii7;	/* 6G UNII7 bitmap */
	uint16	bitmap_6G_unii8;	/* 6G UNII8 bitmap */
} mws_type7_bitmap_v1_t;

/* MWS NR Coex Channel map */
#define WL_MWS_NR_COEXMAP_VERSION_1	 1
typedef struct wl_mws_nr_coexmap {
	uint16  version;    /* Structure version */
	uint16	bitmap_5g_lo;  /* bitmap for 5G low channels by 2:
				*34-48, 52-56, 60-64, 100-102
				*/
	uint16	bitmap_5g_mid; /* bitmap for 5G mid channels by 2:
				* 104, 108-112, 116-120, 124-128,
				* 132-136, 140, 149-151
				*/
	uint16	bitmap_5g_high; /* bitmap for 5G high channels by 2
				* 153, 157-161, 165
				*/
} wl_mws_nr_coexmap_t;

typedef struct {
	uint32 config;	/**< MODE: AUTO (-1), Disable (0), Enable (1) */
	uint32 status;	/**< Current state: Disabled (0), Enabled (1) */
} wl_config_t;

#define WLC_RSDB_MODE_AUTO_MASK 0x80
#define WLC_RSDB_EXTRACT_MODE(val) ((int8)((val) & (~(WLC_RSDB_MODE_AUTO_MASK))))

typedef struct {
	uint16  request; /* type of sensor hub request */
	uint16  enable; /* enable/disable response for specified request */
	uint16  interval; /* interval between responses to the request */
} shub_req_t;


typedef struct wl_band {
	uint16		bandtype;	/**< WL_BAND_2G, WL_BAND_5G */
	uint16		bandunit;	/**< bandstate[] index */
	uint16		phytype;	/**< phytype */
	uint16		phyrev;
} wl_band_t;

#define	WL_ROAM_STATS_VER_1 (1u)	/**< current version of wl_if_stats structure */

/** roam statistics counters */
typedef struct {
	uint16	version;		/**< version of the structure */
	uint16	length;			/**< length of the entire structure */
	uint32	initial_assoc_time;
	uint32	prev_roam_time;
	uint32	last_roam_event_type;
	uint32	last_roam_event_status;
	uint32	last_roam_event_reason;
	uint16	roam_success_cnt;
	uint16	roam_fail_cnt;
	uint16	roam_attempt_cnt;
	uint16	max_roam_target_cnt;
	uint16	min_roam_target_cnt;
	uint16	max_cached_ch_cnt;
	uint16	min_cached_ch_cnt;
	uint16	partial_roam_scan_cnt;
	uint16	full_roam_scan_cnt;
	uint16	most_roam_reason;
	uint16	most_roam_reason_cnt;
	uint16	PAD;
} wl_roam_stats_v1_t;

#define	WL_WLC_VERSION_T_VERSION_1	 1 /**< current version of wlc_version structure */

/** wlc interface version */
typedef struct wl_wlc_version {
	uint16	version;		/**< version of the structure */
	uint16	length;			/**< length of the entire structure */

	/* epi version numbers */
	uint16	epi_ver_major;		/**< epi major version number */
	uint16	epi_ver_minor;		/**< epi minor version number */
	uint16	epi_rc_num;		/**< epi RC number */
	uint16	epi_incr_num;		/**< epi increment number */

	/* wlc interface version numbers */
	uint16	wlc_ver_major;		/**< wlc interface major version number */
	uint16	wlc_ver_minor;		/**< wlc interface minor version number */
}

wl_wlc_version_t;

#define	WL_SCAN_VERSION_T_VERSION_1	 1 /**< current version of scan_version structure */
/** scan interface version */
typedef struct wl_scan_version {
	uint16	version;		/**< version of the structure */
	uint16	length;			/**< length of the entire structure */

	/* scan interface version numbers */
	uint16	scan_ver_major;		/**< scan interface major version number */
} wl_scan_version_t;

#define	WL_JOIN_VERSION_T_VERSION_1	1u /**< current version of join_version structure */
/* version of join to be returned as part of wl_join_version structure */
#define WL_JOIN_VERSION_MAJOR_0	0u
#define WL_JOIN_VERSION_MAJOR_1	1u
#define WL_JOIN_VERSION_MAJOR_2	2u
#define WL_JOIN_VERSION_MAJOR_3	3u
/** join interface version */
typedef struct wl_join_version_v1 {
	uint16	version;		/**< version of the structure */
	uint16	length;			/**< length of the entire structure */

	/* join interface version numbers */
	uint16	join_ver_major;		/**< join interface major version number */
	uint8	pad[2];
} wl_join_version_v1_t;

/* begin proxd definitions */
#include <packed_section_start.h>

#define WL_PROXD_API_VERSION_3	 0x0300u /**< version 3.0 */

/* proxd version with 11az */
#define WL_PROXD_11AZ_API_VERSION_1 0x0400u

/** Minimum supported API version */
#define WL_PROXD_API_MIN_VERSION_3 0x0300u

/** proximity detection methods */
enum {
	WL_PROXD_METHOD_NONE	= 0,
	WL_PROXD_METHOD_RSVD1	= 1, /**< backward compatibility - RSSI, not supported */
	WL_PROXD_METHOD_TOF	= 2, /**< 11v+BCM proprietary */
	WL_PROXD_METHOD_RSVD2	= 3, /**< 11v only - if needed */
	WL_PROXD_METHOD_FTM	= 4, /**< IEEE rev mc/2014 */
	WL_PROXD_METHOD_MAX
};
typedef int16 wl_proxd_method_t;

/** global and method configuration flags */
enum {
	WL_PROXD_FLAG_NONE			= 0x00000000,
	WL_PROXD_FLAG_RX_ENABLED		= 0x00000001, /* respond to requests, per bss */
	WL_PROXD_FLAG_RX_RANGE_REQ		= 0x00000002, /* 11mc range requests enabled */
	WL_PROXD_FLAG_TX_LCI			= 0x00000004, /* tx lci, if known */
	WL_PROXD_FLAG_TX_CIVIC			= 0x00000008, /* tx civic, if known */
	WL_PROXD_FLAG_RX_AUTO_BURST		= 0x00000010, /* auto respond w/o host action */
	WL_PROXD_FLAG_TX_AUTO_BURST		= 0x00000020, /* continue tx w/o host action */
	WL_PROXD_FLAG_AVAIL_PUBLISH		= 0x00000040, /* publish availability */
	WL_PROXD_FLAG_AVAIL_SCHEDULE		= 0x00000080, /* schedule using availability */
	WL_PROXD_FLAG_ASAP_CAPABLE		= 0x00000100, /* ASAP capable */
	WL_PROXD_FLAG_MBURST_FOLLOWUP		= 0x00000200, /* new multi-burst algorithm */
	WL_PROXD_FLAG_SECURE			= 0x00000400, /* per bsscfg option */
	WL_PROXD_FLAG_NO_TSF_SYNC		= 0x00000800, /* disable tsf sync */
	WL_PROXD_FLAG_AVB_TS			= 0x00001000, /* Force AVB TimeStamping */
	WL_PROXD_FLAG_SIGNED_RTT		= 0x00002000, /* Send negative RTT to host */
	WL_PROXD_FLAG_ALL			= 0xffffffff
};
typedef uint32 wl_proxd_flags_t;

#define WL_PROXD_FLAGS_AVAIL (WL_PROXD_FLAG_AVAIL_PUBLISH | \
	WL_PROXD_FLAG_AVAIL_SCHEDULE)

typedef enum wl_proxd_session_flags {
	WL_PROXD_SESSION_FLAG_NONE              = 0x00000000,  /**< no flags */
	WL_PROXD_SESSION_FLAG_INITIATOR         = 0x00000001,  /**< local device is initiator */
	WL_PROXD_SESSION_FLAG_TARGET            = 0x00000002,  /**< local device is target */
	WL_PROXD_SESSION_FLAG_ONE_WAY		= 0x00000004,  /**< (initiated) 1-way rtt */
	WL_PROXD_SESSION_FLAG_AUTO_BURST	= 0x00000008,  /**< created w/ rx_auto_burst */
	WL_PROXD_SESSION_FLAG_PERSIST		= 0x00000010,  /**< good until cancelled */
	WL_PROXD_SESSION_FLAG_RTT_DETAIL	= 0x00000020,  /**< rtt detail in results */
	WL_PROXD_SESSION_FLAG_SECURE		= 0x00000040,  /**< session is secure */
	WL_PROXD_SESSION_FLAG_AOA		= 0x00000080,  /**< AOA along w/ RTT */
	WL_PROXD_SESSION_FLAG_RX_AUTO_BURST	= 0x00000100,  /**< Same as proxd flags above */
	WL_PROXD_SESSION_FLAG_TX_AUTO_BURST	= 0x00000200,  /**< Same as proxd flags above */
	WL_PROXD_SESSION_FLAG_NAN_BSS		= 0x00000400,  /**< Use NAN BSS, if applicable */
	WL_PROXD_SESSION_FLAG_TS1		= 0x00000800,  /**< e.g. FTM1 - ASAP-capable */
	WL_PROXD_SESSION_FLAG_RANDMAC		= 0x00001000,  /**< use random mac */
	WL_PROXD_SESSION_FLAG_REPORT_FAILURE	= 0x00002000, /**< report failure to target */
	WL_PROXD_SESSION_FLAG_INITIATOR_RPT	= 0x00004000, /**< report distance to target */
	WL_PROXD_SESSION_FLAG_NOCHANSWT		= 0x00008000,
	WL_PROXD_SESSION_FLAG_NETRUAL		= 0x00010000, /**< netrual mode */
	WL_PROXD_SESSION_FLAG_SEQ_EN		= 0x00020000, /**< Toast */
	WL_PROXD_SESSION_FLAG_NO_PARAM_OVRD	= 0x00040000, /**< no param override from target */
	WL_PROXD_SESSION_FLAG_ASAP		= 0x00080000, /**< ASAP session */
	WL_PROXD_SESSION_FLAG_REQ_LCI		= 0x00100000, /**< transmit LCI req */
	WL_PROXD_SESSION_FLAG_REQ_CIV		= 0x00200000, /**< transmit civic loc req */
	WL_PROXD_SESSION_FLAG_PRE_SCAN		= 0x00400000, /* enable pre-scan for asap=1 */
	WL_PROXD_SESSION_FLAG_AUTO_VHTACK	= 0x00800000, /* use vhtack based on brcm ie */
	WL_PROXD_SESSION_FLAG_VHTACK		= 0x01000000, /* vht ack is in use - output only */
	WL_PROXD_SESSION_FLAG_BDUR_NOPREF	= 0x02000000, /* burst-duration: no preference */
	WL_PROXD_SESSION_FLAG_NUM_FTM_NOPREF	= 0x04000000, /* num of FTM frames: no preference */
	WL_PROXD_SESSION_FLAG_FTM_SEP_NOPREF	= 0x08000000, /* time btw FTM frams: no pref */
	WL_PROXD_SESSION_FLAG_NUM_BURST_NOPREF	= 0x10000000, /* num of bursts: no pref */
	WL_PROXD_SESSION_FLAG_BURST_PERIOD_NOPREF = 0x20000000, /* burst period: no pref */
	WL_PROXD_SESSION_FLAG_MBURST_FOLLOWUP	= 0x40000000, /* new mburst algo  - reserved */
	WL_PROXD_SESSION_FLAG_MBURST_NODELAY	= 0x80000000, /**< good until cancelled */
	/* core rotation on initiator (reuse ONE_WAY bit) */
	WL_PROXD_SESSION_FLAG_CORE_ROTATE	= 0x00000004,
	WL_PROXD_SESSION_FLAG_ALL		= 0xffffffff
} wl_proxd_session_flags_t;


/** time units - mc supports up to 0.1ns resolution */
enum {
	WL_PROXD_TMU_TU			= 0,	/**< 1024us */
	WL_PROXD_TMU_SEC		= 1,
	WL_PROXD_TMU_MILLI_SEC		= 2,
	WL_PROXD_TMU_MICRO_SEC		= 3,
	WL_PROXD_TMU_NANO_SEC		= 4,
	WL_PROXD_TMU_PICO_SEC		= 5
};
typedef int16 wl_proxd_tmu_t;

/** time interval e.g. 10ns */
struct wl_proxd_intvl {
	uint32 intvl;
	wl_proxd_tmu_t tmu;
	uint8	pad[2];
};
typedef struct wl_proxd_intvl wl_proxd_intvl_t;

/** commands that can apply to proxd, method or a session */
enum {
	WL_PROXD_CMD_NONE			= 0,
	WL_PROXD_CMD_GET_VERSION		= 1,
	WL_PROXD_CMD_ENABLE			= 2,
	WL_PROXD_CMD_DISABLE			= 3,
	WL_PROXD_CMD_CONFIG			= 4,
	WL_PROXD_CMD_START_SESSION		= 5,
	WL_PROXD_CMD_BURST_REQUEST		= 6,
	WL_PROXD_CMD_STOP_SESSION		= 7,
	WL_PROXD_CMD_DELETE_SESSION		= 8,
	WL_PROXD_CMD_GET_RESULT			= 9,
	WL_PROXD_CMD_GET_INFO			= 10,
	WL_PROXD_CMD_GET_STATUS			= 11,
	WL_PROXD_CMD_GET_SESSIONS		= 12,
	WL_PROXD_CMD_GET_COUNTERS		= 13,
	WL_PROXD_CMD_CLEAR_COUNTERS		= 14,
	WL_PROXD_CMD_COLLECT			= 15,	/* not supported, see 'wl proxd_collect' */
	WL_PROXD_CMD_TUNE			= 16,	/* not supported, see 'wl proxd_tune' */
	WL_PROXD_CMD_DUMP			= 17,
	WL_PROXD_CMD_START_RANGING		= 18,
	WL_PROXD_CMD_STOP_RANGING		= 19,
	WL_PROXD_CMD_GET_RANGING_INFO		= 20,
	WL_PROXD_CMD_IS_TLV_SUPPORTED		= 21,
	WL_PROXD_CMD_TEST_MODE			= 22,	/* for test mode (i.e. WFA CTT) */
	WL_PROXD_CMD_DBG_EVENT_CHECK		= 23,	/* for debugging */

	WL_PROXD_CMD_MAX
};
typedef int16 wl_proxd_cmd_t;

/* session ids:
 * id 0 is reserved
 * ids 1..0x7fff - allocated by host/app
 * ids 0x8000..0xffff - allocated by firmware, used for auto-rx
 */
enum {
	WL_PROXD_SESSION_ID_GLOBAL = 0
};

/* Externally allocated sids */
#define WL_PROXD_SID_EXT_MAX 0x7fff
#define WL_PROXD_SID_EXT_ALLOC(_sid) ((_sid) > 0 && (_sid) <= WL_PROXD_SID_EXT_MAX)

/* block size for reserved sid blocks */
#define WL_PROXD_SID_EXT_BLKSZ 256
#define WL_PROXD_SID_EXT_BLK_START(_i) (WL_PROXD_SID_EXT_MAX - (_i) * WL_PROXD_SID_EXT_BLKSZ + 1)
#define WL_PROXD_SID_EXT_BLK_END(_start) ((_start) + WL_PROXD_SID_EXT_BLKSZ - 1)

/* rrm block */
#define WL_PROXD_SID_RRM_START WL_PROXD_SID_EXT_BLK_START(1)
#define WL_PROXD_SID_RRM_END WL_PROXD_SID_EXT_BLK_END(WL_PROXD_SID_RRM_START)

/* nan block */
#define WL_PROXD_SID_NAN_START WL_PROXD_SID_EXT_BLK_START(2)
#define WL_PROXD_SID_NAN_END WL_PROXD_SID_EXT_BLK_END(WL_PROXD_SID_NAN_START)

#ifdef CHRE
/* chre block */
#define WL_PROXD_SID_CHRE_START WL_PROXD_SID_EXT_BLK_START(3)
#define WL_PROXD_SID_CHRE_END WL_PROXD_SID_EXT_BLK_END(WL_PROXD_SID_CHRE_START)
#endif /* CHRE */

/* host block */
#define WL_PROXD_SID_HOST_START WL_PROXD_SID_EXT_BLK_START(4)
#define WL_PROXD_SID_HOST_END WL_PROXD_SID_EXT_BLK_END(WL_PROXD_SID_HOST_START)

/** maximum number sessions that can be allocated, may be less if tunable */
#define WL_PROXD_MAX_SESSIONS 16

#ifndef FTM
typedef struct wl_proxd_tlv {
	uint16 id;
	uint16 len;
	uint8  data[1];
} wl_proxd_tlv_t;

typedef uint8		wl_proxd_type_t;
typedef uint16 wl_proxd_session_id_t;

/** proxd iovar - applies to proxd, method or session */
typedef struct wl_proxd_iov {
	uint16			version;
	uint16			len;
	wl_proxd_cmd_t		cmd;
	wl_proxd_method_t		method;
	wl_proxd_session_id_t	sid;
	wl_proxd_type_t		ftm_type;	/* 11az ftm type. valid for PROXD ver >= 0x0400 */
	uint8			PAD[1];
	wl_proxd_tlv_t		tlvs[BCM_FLEX_ARRAY];	/**< variable */
} wl_proxd_iov_t;

#else /* !FTM */
typedef wl_ftm_session_id_t wl_proxd_session_id_t;
typedef wl_ftm_tlv_t wl_proxd_tlv_t;
typedef wl_ftm_iov_t wl_proxd_iov_t;
#endif /* FTM */

/* Use WL_PROXD_E_* errorcodes from this file if BCMUTILS_ERR_CODES not defined */
#ifndef BCMUTILS_ERR_CODES

/** status - TBD BCME_ vs proxd status - range reserved for BCME_ */
enum {
	WL_PROXD_E_LAST			= -1059,
	WL_PROXD_E_CHANSW       		= -1059,
	WL_PROXD_E_PKTFREED		= -1058,
	WL_PROXD_E_ASSOC_INPROG         = -1057,
	WL_PROXD_E_NOAVAIL		= -1056,
	WL_PROXD_E_EXT_SCHED		= -1055,
	WL_PROXD_E_NOT_BCM		= -1054,
	WL_PROXD_E_FRAME_TYPE		= -1053,
	WL_PROXD_E_VERNOSUPPORT		= -1052,
	WL_PROXD_E_SEC_NOKEY		= -1051,
	WL_PROXD_E_SEC_POLICY		= -1050,
	WL_PROXD_E_SCAN_INPROCESS	= -1049,
	WL_PROXD_E_BAD_PARTIAL_TSF	= -1048,
	WL_PROXD_E_SCANFAIL		= -1047,
	WL_PROXD_E_NOTSF		= -1046,
	WL_PROXD_E_POLICY		= -1045,
	WL_PROXD_E_INCOMPLETE		= -1044,
	WL_PROXD_E_OVERRIDDEN		= -1043,
	WL_PROXD_E_ASAP_FAILED		= -1042,
	WL_PROXD_E_NOTSTARTED		= -1041,
	WL_PROXD_E_INVALIDMEAS		= -1040,
	WL_PROXD_E_INCAPABLE		= -1039,
	WL_PROXD_E_MISMATCH		= -1038,
	WL_PROXD_E_DUP_SESSION		= -1037,
	WL_PROXD_E_REMOTE_FAIL		= -1036,
	WL_PROXD_E_REMOTE_INCAPABLE	= -1035,
	WL_PROXD_E_SCHED_FAIL		= -1034,
	WL_PROXD_E_PROTO		= -1033,
	WL_PROXD_E_EXPIRED		= -1032,
	WL_PROXD_E_TIMEOUT		= -1031,
	WL_PROXD_E_NOACK		= -1030,
	WL_PROXD_E_DEFERRED		= -1029,
	WL_PROXD_E_INVALID_SID		= -1028,
	WL_PROXD_E_REMOTE_CANCEL	= -1027,
	WL_PROXD_E_CANCELED		= -1026,	/**< local */
	WL_PROXD_E_INVALID_SESSION	= -1025,
	WL_PROXD_E_BAD_STATE		= -1024,
	WL_PROXD_E_START		= -1024,
	WL_PROXD_E_ERROR		= -1,
	WL_PROXD_E_OK			= 0
};
typedef int32 wl_proxd_status_t;

#endif	/* BCMUTILS_ERR_CODES */

/* proxd errors from phy */
#define PROXD_TOF_INIT_ERR_BITS 16

enum {
	WL_PROXD_PHY_ERR_LB_CORR_THRESH    = (1 << 0), /* Loopback Correlation threshold */
	WL_PROXD_PHY_ERR_RX_CORR_THRESH    = (1 << 1), /* Received Correlation threshold */
	WL_PROXD_PHY_ERR_LB_PEAK_POWER     = (1 << 2), /* Loopback Peak power   */
	WL_PROXD_PHY_ERR_RX_PEAK_POWER     = (1 << 3), /* Received Peak power   */
	WL_PROXD_PHY_ERR_BITFLIP           = (1 << 4), /* Bitflips */
	WL_PROXD_PHY_ERR_SNR               = (1 << 5), /* SNR */
	WL_PROXD_PHY_RX_STRT_WIN_OFF       = (1 << 6), /* Receive start window is off */
	WL_PROXD_PHY_RX_END_WIN_OFF        = (1 << 7), /* Receive End window is off */
	WL_PROXD_PHY_ERR_LOW_CONFIDENCE    = (1 << 15), /* Low confidence on meas distance */
};
typedef uint32 wl_proxd_phy_error_t;

/** PROXD session states */
enum {
	WL_PROXD_SESSION_STATE_NONE			= 0,
	WL_PROXD_SESSION_STATE_CREATED			= 1,
	WL_PROXD_SESSION_STATE_CONFIGURED		= 2,
	WL_PROXD_SESSION_STATE_STARTED			= 3,
	WL_PROXD_SESSION_STATE_DELAY			= 4,
	WL_PROXD_SESSION_STATE_USER_WAIT		= 5,
	WL_PROXD_SESSION_STATE_SCHED_WAIT		= 6,
	WL_PROXD_SESSION_STATE_BURST			= 7,
	WL_PROXD_SESSION_STATE_STOPPING			= 8,
	WL_PROXD_SESSION_STATE_ENDED			= 9,
	WL_PROXD_SESSION_STATE_START_WAIT		= 10,
	WL_PROXD_SESSION_STATE_DESTROYING		= -1
};

typedef int16 wl_proxd_session_state_t;

/** RTT sample flags */
enum {
	WL_PROXD_RTT_SAMPLE_NONE	= 0x00,
	WL_PROXD_RTT_SAMPLE_DISCARD	= 0x01
};
typedef uint8 wl_proxd_rtt_sample_flags_t;
typedef int16 wl_proxd_rssi_t;
typedef uint16 wl_proxd_snr_t;
typedef uint16 wl_proxd_bitflips_t;
typedef uint32 wl_proxd_phy_gd_variance_t;

/** result flags */
enum {
	WL_PRXOD_RESULT_FLAG_NONE	= 0x0000,
	WL_PROXD_RESULT_FLAG_NLOS	= 0x0001,	/**< LOS - if available */
	WL_PROXD_RESULT_FLAG_LOS	= 0x0002,	/**< NLOS - if available */
	WL_PROXD_RESULT_FLAG_FATAL	= 0x0004,	/**< Fatal error during burst */
	WL_PROXD_RESULT_FLAG_VHTACK	= 0x0008,	/* VHTACK or Legacy ACK used */
	WL_PROXD_REQUEST_SENT		= 0x0010,	/* FTM request was sent */
	WL_PROXD_REQUEST_ACKED		= 0x0020,	/* FTM request was acked */
	WL_PROXD_LTFSEQ_STARTED		= 0x0040,	/* LTF sequence started */
	WL_PROXD_RESULT_SIGNED		= 0x0080,	/* RTT result is negative */
	WL_PROXD_RESULT_FLAG_ALL	= 0xffff
};
typedef int16 wl_proxd_result_flags_t;

#define WL_PROXD_RTT_SAMPLE_VERSION_1	1
typedef struct wl_proxd_rtt_sample_v1 {
	uint8				id;		/**< id for the sample - non-zero */
	wl_proxd_rtt_sample_flags_t	flags;
	wl_proxd_rssi_t			rssi;
	wl_proxd_intvl_t		rtt;		/**< round trip time */
	uint32				ratespec;
	wl_proxd_snr_t                  snr;
	wl_proxd_bitflips_t             bitflips;
	wl_proxd_status_t               status;
	int32                           distance;
	wl_proxd_phy_error_t		tof_phy_error;
	wl_proxd_phy_error_t		tof_tgt_phy_error; /* target phy error bit map */
	wl_proxd_snr_t                  tof_tgt_snr;
	wl_proxd_bitflips_t             tof_tgt_bitflips;
	uint8                           coreid;
	uint8                           pad[3];
} wl_proxd_rtt_sample_v1_t;

#define WL_PROXD_RTT_RESULT_VERSION_1	1
/** rtt measurement result */
typedef struct wl_proxd_rtt_result_v1 {
	wl_proxd_session_id_t		sid;
	wl_proxd_result_flags_t		flags;
	wl_proxd_status_t		status;
	struct ether_addr		peer;
	wl_proxd_session_state_t	state;		/**< current state */
	union {
		wl_proxd_intvl_t	retry_after;	/* hint for errors */
		wl_proxd_intvl_t	burst_duration; /* burst duration */
	} u;
	wl_proxd_rtt_sample_v1_t	avg_rtt;
	uint32				avg_dist;	/* 1/256m units */
	uint16				sd_rtt;		/* RTT standard deviation */
	uint8				num_valid_rtt;	/* valid rtt cnt */
	uint8				num_ftm;	/* actual num of ftm cnt (Configured) */
	uint16				burst_num;	/* in a session */
	uint16				num_rtt;	/* 0 if no detail */
	uint16				num_meas;	/* number of ftm frames seen OTA */
	uint8				pad[2];
	wl_proxd_rtt_sample_v1_t	rtt[BCM_FLEX_ARRAY];		/* variable */
} wl_proxd_rtt_result_v1_t;

#define WL_PROXD_RTT_SAMPLE_VERSION_2	2
typedef struct wl_proxd_rtt_sample_v2 {
	uint16				version;
	uint16				length;
	uint8				id;		/**< id for the sample - non-zero */
	wl_proxd_rtt_sample_flags_t	flags;
	wl_proxd_rssi_t			rssi;
	wl_proxd_intvl_t		rtt;		/**< round trip time */
	uint32				ratespec;
	wl_proxd_snr_t                  snr;
	wl_proxd_bitflips_t             bitflips;
	wl_proxd_status_t               status;
	int32                           distance;
	wl_proxd_phy_error_t		tof_phy_error;
	wl_proxd_phy_error_t		tof_tgt_phy_error; /* target phy error bit map */
	wl_proxd_snr_t                  tof_tgt_snr;
	wl_proxd_bitflips_t             tof_tgt_bitflips;
	uint8                           coreid;
	uint8                           pad[3];
	uint32				chanspec;
} wl_proxd_rtt_sample_v2_t;

#define WL_PROXD_RTT_RESULT_VERSION_2	2
/** rtt measurement result */
typedef struct wl_proxd_rtt_result_v2 {
	uint16				version;
	uint16				length;		/* up to rtt[] */
	wl_proxd_session_id_t		sid;
	wl_proxd_result_flags_t		flags;
	wl_proxd_status_t		status;
	struct ether_addr		peer;
	wl_proxd_session_state_t	state;		/**< current state */
	union {
		wl_proxd_intvl_t	retry_after;	/* hint for errors */
		wl_proxd_intvl_t	burst_duration; /* burst duration */
	} u;
	uint32				avg_dist;	/* 1/256m units */
	uint16				sd_rtt;		/* RTT standard deviation */
	uint8				num_valid_rtt;	/* valid rtt cnt */
	uint8				num_ftm;	/* actual num of ftm cnt (Configured) */
	uint16				burst_num;	/* in a session */
	uint16				num_rtt;	/* 0 if no detail */
	uint16				num_meas;	/* number of ftm frames seen OTA */
	uint8				pad[2];
	wl_proxd_rtt_sample_v2_t	rtt[BCM_FLEX_ARRAY];	/* variable len: first is avg_rtt */
} wl_proxd_rtt_result_v2_t;

#define WL_PROXD_RTT_SAMPLE_VERSION_3	3
typedef struct wl_proxd_rtt_sample_v3 {
	uint16				version;
	uint16				length;
	uint8				id;		/**< id for the sample - non-zero */
	wl_proxd_rtt_sample_flags_t	flags;
	wl_proxd_rssi_t			rssi;
	wl_proxd_intvl_t		rtt;		/**< round trip time */
	uint32				ratespec;
	wl_proxd_snr_t                  snr;
	wl_proxd_bitflips_t             bitflips;
	wl_proxd_status_t               status;
	int32                           distance;
	wl_proxd_phy_error_t		tof_phy_error;
	wl_proxd_phy_error_t		tof_tgt_phy_error; /* target phy error bit map */
	wl_proxd_snr_t                  tof_tgt_snr;
	wl_proxd_bitflips_t             tof_tgt_bitflips;
	uint8                           coreid;
	uint8                           pad[3];
	uint32				chanspec;
	uint32				gd_variance; /* group delay variance */
} wl_proxd_rtt_sample_v3_t;

#define WL_PROXD_RTT_RESULT_VERSION_3	3
/** rtt measurement result */
typedef struct wl_proxd_rtt_result_v3 {
	uint16				version;
	uint16				length;		/* up to rtt[] */
	wl_proxd_session_id_t		sid;
	wl_proxd_result_flags_t		flags;
	wl_proxd_status_t		status;
	struct ether_addr		peer;
	wl_proxd_session_state_t	state;		/**< current state */
	union {
		wl_proxd_intvl_t	retry_after;	/* hint for errors */
		wl_proxd_intvl_t	burst_duration; /* burst duration */
	} u;
	uint32				avg_dist;	/* 1/256m units */
	uint16				sd_rtt;		/* RTT standard deviation */
	uint8				num_valid_rtt;	/* valid rtt cnt */
	uint8				num_ftm;	/* actual num of ftm cnt (Configured) */
	uint16				burst_num;	/* in a session */
	uint16				num_rtt;	/* 0 if no detail */
	uint16				num_meas;	/* number of ftm frames seen OTA */
	uint8				pad[2];
	wl_proxd_rtt_sample_v3_t	rtt[];		/* variable len: first is avg_rtt */
} wl_proxd_rtt_result_v3_t;

/** aoa measurement result */
typedef struct wl_proxd_aoa_result {
	wl_proxd_session_id_t		sid;
	wl_proxd_result_flags_t		flags;
	wl_proxd_status_t		status;
	struct ether_addr		peer;
	wl_proxd_session_state_t	state;
	uint16				burst_num;
	uint8				PAD[2];
	/* wl_proxd_aoa_sample_t sample_avg; TBD */
} BWL_POST_PACKED_STRUCT wl_proxd_aoa_result_t;
#include <packed_section_end.h>

/** global stats */
typedef struct wl_proxd_counters {
	uint32 tx;			/* tx frame count */
	uint32 rx;			/* rx frame count */
	uint32 burst;			/* total number of burst */
	uint32 sessions;		/* total number of sessions */
	uint32 max_sessions;		/* max concurrency */
	uint32 sched_fail;		/* scheduling failures */
	uint32 timeouts;		/* timeouts */
	uint32 protoerr;		/* protocol errors */
	uint32 noack;			/* tx w/o ack */
	uint32 txfail;			/* any tx falure */
	uint32 lci_req_tx;		/* tx LCI requests */
	uint32 lci_req_rx;		/* rx LCI requests */
	uint32 lci_rep_tx;		/* tx LCI reports */
	uint32 lci_rep_rx;		/* rx LCI reports */
	uint32 civic_req_tx;		/* tx civic requests */
	uint32 civic_req_rx;		/* rx civic requests */
	uint32 civic_rep_tx;		/* tx civic reports */
	uint32 civic_rep_rx;		/* rx civic reports */
	uint32 rctx;			/* ranging contexts created */
	uint32 rctx_done;		/* count of ranging done */
	uint32 publish_err;		/* availability publishing errors */
	uint32 on_chan;			/* count of scheduler onchan */
	uint32 off_chan;		/* count of scheduler offchan */
	uint32 tsf_lo;			/* local tsf or session tsf */
	uint32 tsf_hi;
	uint32 num_meas;		/* number of measurements */
} wl_proxd_counters_t;
typedef struct wl_proxd_counters wl_proxd_session_counters_t;
typedef struct wl_proxd_counters wl_ftm_counters_v1_t;

enum {
	WL_PROXD_CAP_NONE		= 0x0000,
	WL_PROXD_CAP_ALL		= 0xffff
};
typedef int16 wl_proxd_caps_t;

/** method capabilities */
enum {
	WL_PROXD_FTM_CAP_NONE = 0x0000,
	WL_PROXD_FTM_CAP_FTM1 = 0x0001
};
typedef uint16 wl_proxd_ftm_caps_t;

typedef struct wl_proxd_tlv_id_list {
	uint16			num_ids;
	uint16			ids[BCM_FLEX_ARRAY];
} wl_proxd_tlv_id_list_t;

typedef struct wl_proxd_session_id_list {
	uint16 num_ids;
	wl_proxd_session_id_t ids[BCM_FLEX_ARRAY];
} wl_proxd_session_id_list_t;

typedef struct wl_proxd_tpk {
	struct ether_addr	peer;
	uint8 tpk[TPK_FTM_LEN];
} wl_proxd_tpk_t;

/* tlvs returned for get_info on ftm method
 *	configuration:
 *	proxd flags
 *	event mask
 *	debug mask
 *	session defaults (session tlvs)
 * status tlv - not supported for ftm method
 * info tlv
 */
typedef struct wl_proxd_ftm_info {
	wl_proxd_ftm_caps_t caps;
	uint16 max_sessions;
	uint16 num_sessions;
	uint16 rx_max_burst;
} wl_proxd_ftm_info_t;

enum {
	WL_PROXD_WAIT_NONE	= 0x0000,
	WL_PROXD_WAIT_KEY	= 0x0001,
	WL_PROXD_WAIT_SCHED	= 0x0002,
	WL_PROXD_WAIT_TSF	= 0x0004
};
typedef int16 wl_proxd_wait_reason_t;

/* tlvs returned for get_info on session
 * session config (tlvs)
 * session info tlv
 */
typedef struct wl_proxd_ftm_session_info {
	uint16 sid;
	uint8 bss_index;
	uint8 pad;
	struct ether_addr bssid;
	wl_proxd_session_state_t state;
	wl_proxd_status_t status;
	uint16	burst_num;
	wl_proxd_wait_reason_t wait_reason;
	uint32	meas_start_lo; /* sn tsf of 1st meas for cur/prev burst */
	uint32	meas_start_hi;
} wl_proxd_ftm_session_info_t;

typedef struct wl_proxd_ftm_session_status {
	uint16 sid;
	wl_proxd_session_state_t state;
	wl_proxd_status_t status;
	uint16	burst_num;
	uint16 core_info;
} wl_proxd_ftm_session_status_t;

/** rrm range request */
typedef struct wl_proxd_range_req {
	uint16			num_repeat;
	uint16			init_delay_range;	/**< in TUs */
	uint8			PAD;
	uint8			num_nbr;		/**< number of (possible) neighbors */
	nbr_element_t		nbr[BCM_FLEX_ARRAY];
} wl_proxd_range_req_t;

#define WL_PROXD_LCI_LAT_OFF	0
#define WL_PROXD_LCI_LONG_OFF	5
#define WL_PROXD_LCI_ALT_OFF	10

#define WL_PROXD_LCI_GET_LAT(_lci, _lat, _lat_err) { \
	unsigned _off = WL_PROXD_LCI_LAT_OFF; \
	_lat_err = (_lci)->data[(_off)] & 0x3f; \
	_lat = (_lci)->data[(_off)+1]; \
	_lat |= (_lci)->data[(_off)+2] << 8; \
	_lat |= (_lci)->data[_(_off)+3] << 16; \
	_lat |= (_lci)->data[(_off)+4] << 24; \
	_lat <<= 2; \
	_lat |= (_lci)->data[(_off)] >> 6; \
}

#define WL_PROXD_LCI_GET_LONG(_lci, _lcilong, _long_err) { \
	unsigned _off = WL_PROXD_LCI_LONG_OFF; \
	_long_err = (_lci)->data[(_off)] & 0x3f; \
	_lcilong = (_lci)->data[(_off)+1]; \
	_lcilong |= (_lci)->data[(_off)+2] << 8; \
	_lcilong |= (_lci)->data[_(_off)+3] << 16; \
	_lcilong |= (_lci)->data[(_off)+4] << 24; \
	__lcilong <<= 2; \
	_lcilong |= (_lci)->data[(_off)] >> 6; \
}

#define WL_PROXD_LCI_GET_ALT(_lci, _alt_type, _alt, _alt_err) { \
	unsigned _off = WL_PROXD_LCI_ALT_OFF; \
	_alt_type = (_lci)->data[_off] & 0x0f; \
	_alt_err = (_lci)->data[(_off)] >> 4; \
	_alt_err |= ((_lci)->data[(_off)+1] & 0x03) << 4; \
	_alt = (_lci)->data[(_off)+2]; \
	_alt |= (_lci)->data[(_off)+3] << 8; \
	_alt |= (_lci)->data[_(_off)+4] << 16; \
	_alt <<= 6; \
	_alt |= (_lci)->data[(_off) + 1] >> 2; \
}

#define WL_PROXD_LCI_VERSION(_lci) ((_lci)->data[15] >> 6)

/* availability. advertising mechanism bss specific */
/** availablity flags */
enum {
	WL_PROXD_AVAIL_NONE = 0,
	WL_PROXD_AVAIL_NAN_PUBLISHED = 0x0001,
	WL_PROXD_AVAIL_SCHEDULED = 0x0002        /**< scheduled by proxd */
};
typedef int16 wl_proxd_avail_flags_t;

/** time reference */
enum {
	WL_PROXD_TREF_NONE = 0,
	WL_PROXD_TREF_DEV_TSF = 1,
	WL_PROXD_TREF_NAN_DW = 2,
	WL_PROXD_TREF_TBTT = 3,
	WL_PROXD_TREF_MAX		/* last entry */
};
typedef int16 wl_proxd_time_ref_t;

/** proxd channel-time slot */
typedef struct {
	wl_proxd_intvl_t start;         /**< from ref */
	wl_proxd_intvl_t duration;      /**< from start */
	uint32  chanspec;
} wl_proxd_time_slot_t;

typedef struct wl_proxd_avail24 {
	wl_proxd_avail_flags_t flags; /**< for query only */
	wl_proxd_time_ref_t time_ref;
	uint16	max_slots; /**< for query only */
	uint16  num_slots;
	wl_proxd_time_slot_t slots[1];	/**< ROM compat - not used */
	wl_proxd_intvl_t	repeat;
	wl_proxd_time_slot_t ts0[BCM_FLEX_ARRAY];
} wl_proxd_avail24_t;
#define WL_PROXD_AVAIL24_TIMESLOT(_avail24, _i) (&(_avail24)->ts0[(_i)])
#define WL_PROXD_AVAIL24_TIMESLOT_OFFSET(_avail24) OFFSETOF(wl_proxd_avail24_t, ts0)
#define WL_PROXD_AVAIL24_TIMESLOTS(_avail24) WL_PROXD_AVAIL24_TIMESLOT(_avail24, 0)
#define WL_PROXD_AVAIL24_SIZE(_avail24, _num_slots) (\
	WL_PROXD_AVAIL24_TIMESLOT_OFFSET(_avail24) + \
	(_num_slots) * sizeof(*WL_PROXD_AVAIL24_TIMESLOT(_avail24, 0)))

typedef struct wl_proxd_avail {
	wl_proxd_avail_flags_t flags; /**< for query only */
	wl_proxd_time_ref_t time_ref;
	uint16	max_slots; /**< for query only */
	uint16  num_slots;
	wl_proxd_intvl_t	repeat;
	wl_proxd_time_slot_t slots[BCM_FLEX_ARRAY];
} wl_proxd_avail_t;
#define WL_PROXD_AVAIL_TIMESLOT(_avail, _i) (&(_avail)->slots[(_i)])
#define WL_PROXD_AVAIL_TIMESLOT_OFFSET(_avail) OFFSETOF(wl_proxd_avail_t, slots)

#define WL_PROXD_AVAIL_TIMESLOTS(_avail) WL_PROXD_AVAIL_TIMESLOT(_avail, 0)
#define WL_PROXD_AVAIL_SIZE(_avail, _num_slots) (\
	WL_PROXD_AVAIL_TIMESLOT_OFFSET(_avail) + \
	(_num_slots) * sizeof(*WL_PROXD_AVAIL_TIMESLOT(_avail, 0)))

/* collect support TBD */

/** debugging */
enum {
	WL_PROXD_DEBUG_NONE		= 0x00000000,
	WL_PROXD_DEBUG_LOG		= 0x00000001,
	WL_PROXD_DEBUG_IOV		= 0x00000002,
	WL_PROXD_DEBUG_EVENT		= 0x00000004,
	WL_PROXD_DEBUG_SESSION		= 0x00000008,
	WL_PROXD_DEBUG_PROTO		= 0x00000010,
	WL_PROXD_DEBUG_SCHED		= 0x00000020,
	WL_PROXD_DEBUG_RANGING		= 0x00000040,
	WL_PROXD_DEBUG_NAN		= 0x00000080,
	WL_PROXD_DEBUG_PKT		= 0x00000100,
	WL_PROXD_DEBUG_SEC		= 0x00000200,
	WL_PROXD_DEBUG_FSM		= 0x00000400,	/* BCM FSM log for FTM session */
	WL_PROXD_DEBUG_CSI		= 0x00000800,	/* Enable CSI logging */
	WL_PROXD_DEBUG_AZ_MEAS		= 0x00001000,	/* AZ measurement substate */
	WL_PROXD_DEBUG_PM		= 0x00002000,
	WL_PROXD_DEBUG_EVENTLOG		= 0x80000000,	/* map/enable EVNET_LOG_TAG_PROXD_INFO */
	WL_PROXD_DEBUG_ALL		= 0xffffffff
};
typedef uint32 wl_proxd_debug_mask_t;

/** tlv IDs - data length 4 bytes unless overridden by type, alignment 32 bits */
typedef enum {
	WL_PROXD_TLV_ID_NONE			= 0,
	WL_PROXD_TLV_ID_METHOD			= 1,
	WL_PROXD_TLV_ID_FLAGS			= 2,
	WL_PROXD_TLV_ID_CHANSPEC		= 3,	/**< note: uint32 */
	WL_PROXD_TLV_ID_TX_POWER		= 4,
	WL_PROXD_TLV_ID_RATESPEC		= 5,
	WL_PROXD_TLV_ID_BURST_DURATION		= 6,	/**< intvl - length of burst */
	WL_PROXD_TLV_ID_BURST_PERIOD		= 7,	/**< intvl - between bursts */
	WL_PROXD_TLV_ID_BURST_FTM_SEP		= 8,	/**< intvl - between FTMs */
	WL_PROXD_TLV_ID_BURST_NUM_FTM		= 9,	/**< uint16 - per burst */
	WL_PROXD_TLV_ID_NUM_BURST		= 10,	/**< uint16 */
	WL_PROXD_TLV_ID_FTM_RETRIES		= 11,	/**< uint16 at FTM level */
	WL_PROXD_TLV_ID_BSS_INDEX		= 12,	/**< uint8 */
	WL_PROXD_TLV_ID_BSSID			= 13,
	WL_PROXD_TLV_ID_INIT_DELAY		= 14,	/**< intvl - optional,non-standalone only */
	WL_PROXD_TLV_ID_BURST_TIMEOUT		= 15,	/**< expect response within - intvl */
	WL_PROXD_TLV_ID_EVENT_MASK		= 16,	/**< interested events - in/out */
	WL_PROXD_TLV_ID_FLAGS_MASK		= 17,	/**< interested flags - in only */
	WL_PROXD_TLV_ID_PEER_MAC		= 18,	/**< mac address of peer */
	WL_PROXD_TLV_ID_FTM_REQ			= 19,	/**< dot11_ftm_req */
	WL_PROXD_TLV_ID_LCI_REQ			= 20,
	WL_PROXD_TLV_ID_LCI			= 21,
	WL_PROXD_TLV_ID_CIVIC_REQ		= 22,
	WL_PROXD_TLV_ID_CIVIC			= 23,
	WL_PROXD_TLV_ID_AVAIL24			= 24,	/**< ROM compatibility */
	WL_PROXD_TLV_ID_SESSION_FLAGS		= 25,
	WL_PROXD_TLV_ID_SESSION_FLAGS_MASK	= 26,	/**< in only */
	WL_PROXD_TLV_ID_RX_MAX_BURST		= 27,	/**< uint16 - limit bursts per session */
	WL_PROXD_TLV_ID_RANGING_INFO		= 28,	/**< ranging info */
	WL_PROXD_TLV_ID_RANGING_FLAGS		= 29,	/**< uint16 */
	WL_PROXD_TLV_ID_RANGING_FLAGS_MASK	= 30,	/**< uint16, in only */
	WL_PROXD_TLV_ID_NAN_MAP_ID		= 31,
	WL_PROXD_TLV_ID_DEV_ADDR		= 32,
	WL_PROXD_TLV_ID_AVAIL			= 33,	/**< wl_proxd_avail_t  */
	WL_PROXD_TLV_ID_TLV_ID			= 34,	/* uint16 tlv-id */
	WL_PROXD_TLV_ID_FTM_REQ_RETRIES		= 35,	/* uint16 FTM request retries */
	WL_PROXD_TLV_ID_TPK			= 36,	/* 32byte TPK  */
	WL_PROXD_TLV_ID_RI_RR			= 36,	/* RI_RR */
	WL_PROXD_TLV_ID_TUNE			= 37,	/* wl_proxd_pararms_tof_tune_t */
	WL_PROXD_TLV_ID_CUR_ETHER_ADDR		= 38,	/* Source Address used for Tx */
	WL_PROXD_TLV_ID_RESERVED_ID39		= 39,	/* ftm component only */
	WL_PROXD_TLV_ID_RESERVED_ID40		= 40,	/* ftm component only */
	WL_PROXD_TLV_ID_RESERVED_ID41		= 41,	/* ftm component only */
	WL_PROXD_TLV_ID_RESERVED_ID42		= 42,	/* ftm component only */
	WL_PROXD_TLV_ID_RESERVED_ID43		= 43,	/* ftm component only */
	WL_PROXD_TLV_ID_RESERVED_ID44		= 44,	/* ftm component only */
	WL_PROXD_TLV_ID_RESERVED_ID45		= 45,	/* ftm component only */
	WL_PROXD_TLV_ID_RESERVED_ID46		= 46,	/* ftm component only */
	WL_PROXD_TLV_ID_RESERVED_ID47		= 47,	/* ftm component only */
	WL_PROXD_TLV_ID_RESERVED_ID48		= 48,	/* ftm component only */
	WL_PROXD_TLV_ID_RESERVED_ID49		= 49,	/* ftm component only */
	WL_PROXD_TLV_ID_RESERVED_ID50		= 50,	/* ftm component only */
	WL_PROXD_TLV_ID_RESERVED_ID51		= 51,	/* ftm component only */
	WL_PROXD_TLV_ID_RESERVED_ID52		= 52,	/* ftm component only */
	WL_PROXD_TLV_ID_RESERVED_ID53		= 53,	/* ftm component only */

	/* output - 512 + x */
	WL_PROXD_TLV_ID_STATUS			= 512,
	WL_PROXD_TLV_ID_COUNTERS		= 513,	/* wl_proxd_counters_t */
	WL_PROXD_TLV_ID_INFO			= 514,
	WL_PROXD_TLV_ID_RTT_RESULT		= 515,
	WL_PROXD_TLV_ID_AOA_RESULT		= 516,
	WL_PROXD_TLV_ID_SESSION_INFO		= 517,
	WL_PROXD_TLV_ID_SESSION_STATUS		= 518,
	WL_PROXD_TLV_ID_SESSION_ID_LIST		= 519,
	WL_PROXD_TLV_ID_RTT_RESULT_V2		= 520,
	WL_PROXD_TLV_ID_RTT_RESULT_V3		= 521,
	WL_PROXD_TLV_ID_RESERVED_ID522		= 522,	/* ftm component only */
	WL_PROXD_TLV_ID_RESERVED_ID523		= 523,	/* ftm component only */
	WL_PROXD_TLV_ID_RESERVED_ID524		= 524,	/* ftm component only */
	WL_PROXD_TLV_ID_RESERVED_ID525		= 525,	/* ftm component only */
	WL_PROXD_TLV_ID_RESERVED_ID526		= 526,	/* ftm component only */

	/* debug tlvs can be added starting 1024 */
	WL_PROXD_TLV_ID_DEBUG_MASK		= 1024,
	WL_PROXD_TLV_ID_COLLECT			= 1025,	/**< output only */
	WL_PROXD_TLV_ID_STRBUF			= 1026,

	WL_PROXD_TLV_ID_COLLECT_HEADER		= 1025,	/* wl_proxd_collect_header_t */
	WL_PROXD_TLV_ID_COLLECT_INFO		= 1028,	/* wl_proxd_collect_info_t */
	WL_PROXD_TLV_ID_COLLECT_DATA		= 1029,	/* wl_proxd_collect_data_t */
	WL_PROXD_TLV_ID_COLLECT_CHAN_DATA	= 1030,	/* wl_proxd_collect_data_t */
	WL_PROXD_TLV_ID_MF_STATS_DATA		= 1031,	/* mf_stats_buffer */

	WL_PROXD_TLV_ID_COLLECT_INLINE_HEADER	= 1032,
	WL_PROXD_TLV_ID_COLLECT_INLINE_FRAME_INFO	= 1033,
	WL_PROXD_TLV_ID_COLLECT_INLINE_FRAME_DATA	= 1034,
	WL_PROXD_TLV_ID_COLLECT_INLINE_RESULTS	= 1035,
	WL_PROXD_TLV_ID_RESERVED_ID1036		= 1036,	/* ftm component only */
	WL_PROXD_TLV_ID_RESERVED_ID1037		= 1037,	/* ftm component only */
	WL_PROXD_TLV_ID_RESERVED_ID1038		= 1038,	/* ftm component only */
	WL_PROXD_TLV_ID_RESERVED_ID1039		= 1039,	/* ftm component only */
	WL_PROXD_TLV_ID_RESERVED_ID1040		= 1040,	/* ftm component only */

	WL_PROXD_TLV_ID_MAX
} wl_proxd_tlv_types_t;

#define TOF_COLLECT_INLINE_HEADER_INFO_VER_1	1

typedef struct wl_proxd_collect_inline_header_info_v1
{
	uint16			version;
	uint16			PAD;
	uint32			ratespec;		/* override */
	chanspec_t		chanspec;
	uint16			num_ftm;
	struct ether_addr	peer_mac;
	struct ether_addr	cur_ether_addr;		/* source address for Tx */
} wl_proxd_collect_inline_header_info_v1_t;


#define TOF_COLLECT_INLINE_RESULTS_VER_1		1
typedef struct wl_proxd_collect_inline_results_info_v1
{
	uint16 version;
	uint16 pad1;
	uint32 meanrtt;
	uint32 distance;
	uint16 num_rtt;
	uint16 pad2;
	int32 status;
	uint32 ratespec;
} wl_proxd_collect_inline_results_info_v1_t;

#define TOF_COLLECT_INLINE_FRAME_INFO_VER_1	1
typedef struct wl_proxd_collect_inline_frame_info_v1
{
	uint16 version;
	uint16 pad1;
	int32 gd;
	uint32 T[4];
	uint32 prev_t1;
	uint32 prev_t4;
	int32 hadj;
	int8 rssi;
	uint8 pad3[3];
} wl_proxd_collect_inline_frame_info_v1_t;

#define TOF_COLLECT_INLINE_FRAME_INFO_VER_2	2
typedef struct wl_proxd_collect_inline_frame_info_v2
{
	uint16 version;
	uint16 pad1;
	int32 gd;
	uint32 T[4];
	int32 hadj;
	int8 rssi;
	uint8 pad[3];
} wl_proxd_collect_inline_frame_info_v2_t;

#define WL_PROXD_IOV_HDR_SIZE OFFSETOF(wl_proxd_iov_t, tlvs)

/* The following event definitions may move to bcmevent.h, but sharing proxd types
 * across needs more invasive changes unrelated to proxd
 */
enum {
	WL_PROXD_EVENT_NONE			= 0,	/**< not an event, reserved */
	WL_PROXD_EVENT_SESSION_CREATE		= 1,
	WL_PROXD_EVENT_SESSION_START		= 2,
	WL_PROXD_EVENT_FTM_REQ			= 3,
	WL_PROXD_EVENT_BURST_START		= 4,
	WL_PROXD_EVENT_BURST_END		= 5,
	WL_PROXD_EVENT_SESSION_END		= 6,
	WL_PROXD_EVENT_SESSION_RESTART		= 7,
	WL_PROXD_EVENT_BURST_RESCHED		= 8,	/**< burst rescheduled-e.g. partial TSF */
	WL_PROXD_EVENT_SESSION_DESTROY		= 9,
	WL_PROXD_EVENT_RANGE_REQ		= 10,
	WL_PROXD_EVENT_FTM_FRAME		= 11,
	WL_PROXD_EVENT_DELAY			= 12,
	WL_PROXD_EVENT_VS_INITIATOR_RPT		= 13,	/**< (target) rx initiator-report */
	WL_PROXD_EVENT_RANGING			= 14,
	WL_PROXD_EVENT_LCI_MEAS_REP		= 15,	/* LCI measurement report */
	WL_PROXD_EVENT_CIVIC_MEAS_REP		= 16,	/* civic measurement report */
	WL_PROXD_EVENT_COLLECT			= 17,
	WL_PROXD_EVENT_START_WAIT		= 18,	/* waiting to start */
	WL_PROXD_EVENT_MF_STATS			= 19,	/* mf stats event */
	WL_PROXD_EVENT_RES_WAIT			= 20,	/* waiting to resources */
	WL_PROXD_EVENT_SESSION_READY		= 21,	/* ready to burst */
	WL_PROXD_EVENT_RESERVED22		= 22,	/* reserved by ftm */
	WL_PROXD_EVENT_RESERVED23		= 23,	/* reserved by ftm */
	WL_PROXD_EVENT_RESERVED24		= 24,	/* reserved by ftm */
	WL_PROXD_EVENT_MAX
};
typedef int16 wl_proxd_event_type_t;

/** proxd event mask - upto 32 events for now */
typedef uint32 wl_proxd_event_mask_t;

#define WL_PROXD_EVENT_MASK_ALL 0xfffffffe
#define WL_PROXD_EVENT_MASK_EVENT(_event_type) (1 << (_event_type))
#define WL_PROXD_EVENT_ENABLED(_mask, _event_type) (\
	((_mask) & WL_PROXD_EVENT_MASK_EVENT(_event_type)) != 0)

/** proxd event - applies to proxd, method or session */
typedef struct wl_proxd_event {
	uint16			version;
	uint16			len;
	wl_proxd_event_type_t	type;
	wl_proxd_method_t	method;
	wl_proxd_session_id_t	sid;
	uint8			pad[2];		/* This field is used fragmentation purpose */
	wl_proxd_tlv_t tlvs[BCM_FLEX_ARRAY];	/**< variable */
} wl_proxd_event_t;

enum {
	WL_PROXD_RANGING_STATE_NONE = 0,
	WL_PROXD_RANGING_STATE_NOTSTARTED = 1,
	WL_PROXD_RANGING_STATE_INPROGRESS = 2,
	WL_PROXD_RANGING_STATE_DONE = 3
};
typedef int16 wl_proxd_ranging_state_t;

/** proxd ranging flags */
enum {
	WL_PROXD_RANGING_FLAG_NONE = 0x0000,  /**< no flags */
	WL_PROXD_RANGING_FLAG_DEL_SESSIONS_ON_STOP = 0x0001,
	WL_PROXD_RANGING_FLAG_ALL = 0xffff
};
typedef uint16 wl_proxd_ranging_flags_t;

struct wl_proxd_ranging_info {
	wl_proxd_status_t   status;
	wl_proxd_ranging_state_t state;
	wl_proxd_ranging_flags_t flags;
	uint16	num_sids;
	uint16	num_done;
};
typedef struct wl_proxd_ranging_info wl_proxd_ranging_info_t;

#include <packed_section_start.h>
/* Legacy platform i.e. 43342/43430 */
#define WL_PROXD_COLLECT_EVENT_DATA_VERSION_1		1
typedef BWL_PRE_PACKED_STRUCT struct wl_proxd_collect_event_data_v1 {
	uint32                  H_LB[K_TOF_COLLECT_H_SIZE_20MHZ];
	uint32                  H_RX[K_TOF_COLLECT_H_SIZE_20MHZ];
	uint8                   ri_rr[FTM_TPK_LEN];
	wl_proxd_phy_error_t    phy_err_mask;
} BWL_POST_PACKED_STRUCT wl_proxd_collect_event_data_v1_t;

/* Secured 2.0 supoorted devices i.e. 4364 */
#define WL_PROXD_COLLECT_EVENT_DATA_VERSION_2		2
typedef BWL_PRE_PACKED_STRUCT struct wl_proxd_collect_event_data_v2 {
	uint32                  H_LB[K_TOF_COLLECT_H_SIZE_20MHZ];
	uint32                  H_RX[K_TOF_COLLECT_H_SIZE_20MHZ];
	uint8                   ri_rr[FTM_TPK_RI_RR_LEN_SECURE_2_0];
	wl_proxd_phy_error_t    phy_err_mask;
} BWL_POST_PACKED_STRUCT wl_proxd_collect_event_data_v2_t;
#include <packed_section_end.h>

#define WL_PROXD_COLLECT_EVENT_DATA_VERSION_3		3
typedef struct wl_proxd_collect_event_data_v3 {
	uint16			version;
	uint16			length;
	uint32                  H_LB[K_TOF_COLLECT_H_SIZE_20MHZ];
	uint32                  H_RX[K_TOF_COLLECT_H_SIZE_20MHZ];
	uint8                   ri_rr[FTM_TPK_RI_RR_LEN_SECURE_2_0];
	wl_proxd_phy_error_t    phy_err_mask;
} wl_proxd_collect_event_data_v3_t;

#define WL_PROXD_COLLECT_EVENT_DATA_VERSION_4		4
typedef struct wl_proxd_collect_event_data_v4 {
	uint16			version;
	uint16			length;
	uint32                  H_LB[K_TOF_COLLECT_H_SIZE_20MHZ];
	uint32                  H_RX[K_TOF_COLLECT_H_SIZE_20MHZ];
	uint8                   ri_rr[FTM_TPK_RI_RR_LEN_SECURE_2_0_5G];
	uint8                   PAD[2];
	wl_proxd_phy_error_t    phy_err_mask;
} wl_proxd_collect_event_data_v4_t;

#define WL_PROXD_COLLECT_EVENT_DATA_VERSION_MAX	WL_PROXD_COLLECT_EVENT_DATA_VERSION_4

/** Data returned by the bssload_report iovar. This is also the WLC_E_BSS_LOAD event data */
#include <packed_section_start.h>
typedef BWL_PRE_PACKED_STRUCT struct wl_bssload {
	uint16 sta_count;		/**< station count */
	uint16 aac;			/**< available admission capacity */
	uint8 chan_util;		/**< channel utilization */
} BWL_POST_PACKED_STRUCT wl_bssload_t;
#include <packed_section_end.h>

/**
 * Maximum number of configurable BSS Load levels.  The number of BSS Load
 * ranges is always 1 more than the number of configured levels.  eg. if
 * 3 levels of 10, 20, 30 are configured then this defines 4 load ranges:
 * 0-10, 11-20, 21-30, 31-255.  A WLC_E_BSS_LOAD event is generated each time
 * the utilization level crosses into another range, subject to the rate limit.
 */
#define MAX_BSSLOAD_LEVELS 8
#define MAX_BSSLOAD_RANGES (MAX_BSSLOAD_LEVELS + 1)

/** BSS Load event notification configuration. */
typedef struct wl_bssload_cfg {
	uint32 rate_limit_msec;	/**< # of events posted to application will be limited to
				 * one per specified period (0 to disable rate limit).
				 */
	uint8 num_util_levels;	/**< Number of entries in util_levels[] below */
	uint8 util_levels[MAX_BSSLOAD_LEVELS];
				/**< Variable number of BSS Load utilization levels in
				 * low to high order.  An event will be posted each time
				 * a received beacon's BSS Load IE channel utilization
				 * value crosses a level.
				 */
	uint8 PAD[3];
} wl_bssload_cfg_t;

/** User roam cache support */
#define WL_USER_ROAM_CACHE_VER_1	1u
#define WL_USER_ROAM_CACHE_VER		WL_USER_ROAM_CACHE_VER_1

#define WL_USER_ROAM_CACHE_GET		0u	/**< Read back the chanspec[s] */
#define WL_USER_ROAM_CACHE_ADD		1u	/**< Add chanspec[s] */
#define WL_USER_ROAM_CACHE_DEL		2u	/**< Delete chanspec[s] */
#define WL_USER_ROAM_CACHE_CLR		3u	/**< Delete all chanspec[s] */
#define WL_USER_ROAM_CACHE_OVERRIDE	4u	/**< Set to use roam cached chanspec only */

typedef struct wl_user_roamcache {
	uint16	version;
	uint16	length;		/**< Total length including version and length */
	uint32	subcmd;		/**< Sub-command for chanspec add/rel etc.. */
	union {
		uint32	val;	/**< Command value when applicable */
		struct {
			uint16	num_ch;	/**< Number of chanspecs in the following array */
			chanspec_t chanspecs[];
		} chlist;
	} u;
} wl_user_roamcache_t;

/** Multiple roaming profile suport */
#define WL_MAX_ROAM_PROF_BRACKETS	4

#define WL_ROAM_PROF_VER_0	0
#define WL_ROAM_PROF_VER_1	1
#define WL_ROAM_PROF_VER_2	2
#define WL_ROAM_PROF_VER_3	3
#define WL_ROAM_PROF_VER_4	4
#define WL_ROAM_PROF_VER_5	5

#define WL_MAX_ROAM_PROF_VER   WL_ROAM_PROF_VER_1

#define WL_ROAM_PROF_NONE		(0u << 0u)
#define WL_ROAM_PROF_LAZY		(1u << 0u)
#define WL_ROAM_PROF_NO_CI		(1u << 1u)
#define WL_ROAM_PROF_SUSPEND		(1u << 2u)
#define WL_ROAM_PROF_EXTSCAN		(1u << 3u)
#define WL_ROAM_FIND_HIGHER_BAND_ONLY	(1u << 4u) /* Flag to find better higher band AP
						  * Not valid 4388 onwards
						  */
#define WL_ROAM_PROF_SYNC_DTIM		(1u << 6u)
#define WL_ROAM_PROF_DEFAULT		(1u << 7u) /* backward compatible single default profile */
#define WL_ROAM_PROF_5G_PREF		(1u << 8u) /* Flag to restrict low rssi roam scan to 5G */
#define WL_ROAM_PROF_6G_PREF		(1u << 9u) /* Flag to restrict low rssi roam scan to 6G */
#define WL_ROAM_FORCE_6G_SC		(1u << 10u) /* Force 6G lowrssi LP roam scan on Scan core */
#define WL_ROAM_PROF_TIE_BREAKER_POLICY	(1u << 11u) /* Prefer higher band target with same score */
#define WL_ROAM_PROF_NONDFS_PREF	(1u << 12u) /* Prefer non-DFS target with similar score */
/* 2-bit flags to indicate AP with preferred TX power */
#define WL_ROAM_PROF_POWER_PREF_BIT_OFFSET 13u	/* Offset for Power Pref bit */
#define WL_ROAM_PROF_POWER_PREF_BIT_MASK   0x3u	/* Bit mask: bits 13 and 14 indicate power pref */
#define WL_ROAM_PROF_SKIP_FILS		(1u << 15u) /* Flag to skip FILS */

typedef enum wl_roam_prof_power_pref {
	WL_NO_POWER_PREF = 0x0u,
	WL_LOW_POWER_PREF = 0x1u,
	WL_HIGH_POWER_PREF = 0x2u
} wl_roam_prof_power_pref_t;

#define WL_FACTOR_TABLE_MAX_LIMIT 5

#define WL_CU_2G_ROAM_TRIGGER (-60)
#define WL_CU_5G_ROAM_TRIGGER (-70)

#define WL_CU_SCORE_DELTA_DEFAULT 20

#define WL_MAX_CHANNEL_USAGE 0x0FF
#define WL_CU_PERCENTAGE_DISABLE 0
#define WL_CU_PERCENTAGE_DEFAULT 70
#define WL_CU_PERCENTAGE_MAX 100
#define WL_CU_CALC_DURATION_DEFAULT 10 /* seconds */
#define WL_CU_CALC_DURATION_MAX 60 /* seconds */
#define WL_CU_NOT_AVAIL -1

#define WL_ESTM_LOW_TRIGGER_DISABLE 0
#define WL_ESTM_LOW_TRIGGER_DEFAULT 5 /* Mbps */
#define WL_ESTM_LOW_TRIGGER_MAX 250  /* Mbps */
#define WL_ESTM_ROAM_DELTA_DEFAULT 10

typedef struct wl_rssi_boost_v1 {
	int8	thresh;	/**< Min RSSI to qualify for RSSI boost */
	int8	delta;	/**< RSSI boost for AP in the other band */
} wl_rssi_boost_v1_t;


typedef struct wl_roam_prof_v6 {
	uint32	roam_flags;		/**< bit flags */
	int8	roam_trigger;		/**< RSSI trigger level per profile/RSSI bracket */
	int8	rssi_lower;
	int8	roam_delta;
	int8	pad1;
	/* if channel_usage if zero, roam_delta is rssi delta required for new AP */
	/* if channel_usage if non-zero, roam_delta is score delta(%) required for new AP */
	uint16	nfscan;			/**< number of full scan to start with */
	uint16	fullscan_period;
	uint16	init_scan_period;
	uint16	backoff_multiplier;
	uint16	max_scan_period;
	uint8	channel_usage;
	uint8	cu_avg_calc_dur;
	uint16	estm_low_trigger;	/**< ESTM low throughput roam trigger */
	int8	estm_roam_delta;	/**< ESTM low throughput roam delta */
	uint8	boost_bitmap;		/* bitmap boost configuration for bands
					 * provided by user
					 */
	uint16  cca_meas_span;		/* cca meas start time in s before partial
					 * or full scan period
					 */
	uint16	lp_roamscan_period;
	uint16	max_fullscan_period;
	wl_rssi_boost_v1_t rssi_boost[WL_BAND_MAX_CNT];
} wl_roam_prof_v6_t;

typedef struct wl_roam_prof_v5 {
	uint8	roam_flags;		/**< bit flags */
	int8	roam_trigger;		/**< RSSI trigger level per profile/RSSI bracket */
	int8	rssi_lower;
	int8	roam_delta;
	/* if channel_usage if zero, roam_delta is rssi delta required for new AP */
	/* if channel_usage if non-zero, roam_delta is score delta(%) required for new AP */
	int8	rssi_boost_thresh;	/**< Min RSSI to qualify for RSSI boost */
	int8	rssi_boost_delta;	/**< RSSI boost for AP in the other band */
	uint16	nfscan;			/**< number of full scan to start with */
	uint16	fullscan_period;
	uint16	init_scan_period;
	uint16	backoff_multiplier;
	uint16	max_scan_period;
	uint8	channel_usage;
	uint8	cu_avg_calc_dur;
	uint16	estm_low_trigger;	/**< ESTM low throughput roam trigger */
	int8	estm_roam_delta;	/**< ESTM low throughput roam delta */
	int8	pad;
	uint16  cca_meas_span;		/* cca meas start time in s before partial
					 * or full scan period
					 */
	uint16	lp_roamscan_period;
	uint16	max_fullscan_period;
} wl_roam_prof_v5_t;

typedef struct wl_roam_prof_v4 {
	uint8	roam_flags;		/**< bit flags */
	int8	roam_trigger;		/**< RSSI trigger level per profile/RSSI bracket */
	int8	rssi_lower;
	int8	roam_delta;

	/* if channel_usage if zero, roam_delta is rssi delta required for new AP */
	/* if channel_usage if non-zero, roam_delta is score delta(%) required for new AP */
	int8	rssi_boost_thresh;	/**< Min RSSI to qualify for RSSI boost */
	int8	rssi_boost_delta;	/**< RSSI boost for AP in the other band */
	uint16	nfscan;			/**< number of full scan to start with */
	uint16	fullscan_period;
	uint16	init_scan_period;
	uint16	backoff_multiplier;
	uint16	max_scan_period;
	uint8	channel_usage;
	uint8	cu_avg_calc_dur;
	uint16	estm_low_trigger;	/**< ESTM low throughput roam trigger */
	int8	estm_roam_delta;	/**< ESTM low throughput roam delta */
	int8	PAD[3];
	uint16	lp_roamscan_period;
	uint16	max_fullscan_period;
} wl_roam_prof_v4_t;

typedef struct wl_roam_prof_v3 {
	uint8	roam_flags;		/**< bit flags */
	int8	roam_trigger;		/**< RSSI trigger level per profile/RSSI bracket */
	int8	rssi_lower;
	int8	roam_delta;

	/* if channel_usage if zero, roam_delta is rssi delta required for new AP */
	/* if channel_usage if non-zero, roam_delta is score delta(%) required for new AP */
	int8	rssi_boost_thresh;	/**< Min RSSI to qualify for RSSI boost */
	int8	rssi_boost_delta;	/**< RSSI boost for AP in the other band */
	uint16	nfscan;			/**< number of full scan to start with */
	uint16	fullscan_period;
	uint16	init_scan_period;
	uint16	backoff_multiplier;
	uint16	max_scan_period;
	uint8	channel_usage;
	uint8	cu_avg_calc_dur;
	uint16	estm_low_trigger; /**< ESTM low throughput roam trigger */
	int8	estm_roam_delta; /**< ESTM low throughput roam delta */
	uint8 PAD;
} wl_roam_prof_v3_t;

typedef struct wl_roam_prof_v2 {
	int8	roam_flags;		/**< bit flags */
	int8	roam_trigger;		/**< RSSI trigger level per profile/RSSI bracket */
	int8	rssi_lower;
	int8	roam_delta;

	/* if channel_usage if zero, roam_delta is rssi delta required for new AP */
	/* if channel_usage if non-zero, roam_delta is score delta(%) required for new AP */
	int8	rssi_boost_thresh;	/**< Min RSSI to qualify for RSSI boost */
	int8	rssi_boost_delta;	/**< RSSI boost for AP in the other band */
	uint16	nfscan;			/**< number of full scan to start with */
	uint16	fullscan_period;
	uint16	init_scan_period;
	uint16	backoff_multiplier;
	uint16	max_scan_period;
	uint8	channel_usage;
	uint8	cu_avg_calc_dur;
	uint8	PAD[2];
} wl_roam_prof_v2_t;

typedef struct wl_roam_prof_v1 {
	int8	roam_flags;		/**< bit flags */
	int8	roam_trigger;		/**< RSSI trigger level per profile/RSSI bracket */
	int8	rssi_lower;
	int8	roam_delta;

	/* if channel_usage if zero, roam_delta is rssi delta required for new AP */
	/* if channel_usage if non-zero, roam_delta is score delta(%) required for new AP */
	int8	rssi_boost_thresh;	/**< Min RSSI to qualify for RSSI boost */
	int8	rssi_boost_delta;	/**< RSSI boost for AP in the other band */
	uint16	nfscan;			/**< number of full scan to start with */
	uint16	fullscan_period;
	uint16	init_scan_period;
	uint16	backoff_multiplier;
	uint16	max_scan_period;
} wl_roam_prof_v1_t;

typedef struct wl_roam_prof_band_v6 {
	uint32	band;			/**< Must be just one band */
	uint16	ver;			/**< version of this struct */
	uint16	len;			/**< length in bytes of this structure */
	wl_roam_prof_v6_t roam_prof[WL_MAX_ROAM_PROF_BRACKETS];
} wl_roam_prof_band_v6_t;

typedef struct wl_roam_prof_band_v5 {
	uint32	band;			/**< Must be just one band */
	uint16	ver;			/**< version of this struct */
	uint16	len;			/**< length in bytes of this structure */
	wl_roam_prof_v5_t roam_prof[WL_MAX_ROAM_PROF_BRACKETS];
} wl_roam_prof_band_v5_t;

typedef struct wl_roam_prof_band_v4 {
	uint32	band;			/**< Must be just one band */
	uint16	ver;			/**< version of this struct */
	uint16	len;			/**< length in bytes of this structure */
	wl_roam_prof_v4_t roam_prof[WL_MAX_ROAM_PROF_BRACKETS];
} wl_roam_prof_band_v4_t;

typedef struct wl_roam_prof_band_v3 {
	uint32	band;			/**< Must be just one band */
	uint16	ver;			/**< version of this struct */
	uint16	len;			/**< length in bytes of this structure */
	wl_roam_prof_v3_t roam_prof[WL_MAX_ROAM_PROF_BRACKETS];
} wl_roam_prof_band_v3_t;

typedef struct wl_roam_prof_band_v2 {
	uint32	band;			/**< Must be just one band */
	uint16	ver;			/**< version of this struct */
	uint16	len;			/**< length in bytes of this structure */
	wl_roam_prof_v2_t roam_prof[WL_MAX_ROAM_PROF_BRACKETS];
} wl_roam_prof_band_v2_t;

typedef struct wl_roam_prof_band_v1 {
	uint32	band;			/**< Must be just one band */
	uint16	ver;			/**< version of this struct */
	uint16	len;			/**< length in bytes of this structure */
	wl_roam_prof_v1_t roam_prof[WL_MAX_ROAM_PROF_BRACKETS];
} wl_roam_prof_band_v1_t;

#define BSS_MAXTABLE_SIZE 10
#define WNM_BSS_SELECT_FACTOR_VERSION_1   1
typedef struct wnm_bss_select_factor_params {
	uint8 low;
	uint8 high;
	uint8 factor;
	uint8 PAD;
} wnm_bss_select_factor_params_t;

#define WNM_BSS_SELECT_FIXED_SIZE OFFSETOF(wnm_bss_select_factor_cfg_t, params)
typedef struct wnm_bss_select_factor_cfg {
	uint8 version;
	uint8 band;
	uint16 type;
	uint16 PAD;
	uint16 count;
	wnm_bss_select_factor_params_t params[BCM_FLEX_ARRAY];
} wnm_bss_select_factor_cfg_t;

#define WNM_BSS_SELECT_WEIGHT_VERSION   1
typedef struct wnm_bss_select_weight_cfg {
	uint8 version;
	uint8 band;
	uint16 type;
	uint16 weight; /* weightage for each type between 0 to 100 */
} wnm_bss_select_weight_cfg_t;

/* For branches before koala .. wbtext is part
 * of wnm need to use below type only
 */
typedef struct wnm_btm_default_score_cfg {
	uint32 default_score; /* default score */
	uint8 band;
	uint8 PAD[3];
} wnm_btm_default_score_cfg_t;

/* For branches from koala and above .. wbtext is
 * seperate module..need to use below type only
 */
typedef struct wbtext_btm_default_score_cfg {
	uint32 default_score; /* default score */
	uint8 band;
	uint8 PAD[3];
} wbtext_btm_default_score_cfg_t;

#define WNM_BSS_SELECT_TYPE_RSSI   0
#define WNM_BSS_SELECT_TYPE_CU   1
#define WNM_BSS_SELECT_TYPE_ESTM_DL   2

#define WNM_BSSLOAD_MONITOR_VERSION_1   1
typedef struct wnm_bssload_monitor_cfg {
	uint8 version;
	uint8 band;
	uint8 duration; /* duration between 1 to 20sec */
} wnm_bssload_monitor_cfg_t;

#define WNM_ROAM_TRIGGER_VERSION_1   1
typedef struct wnm_roam_trigger_cfg {
	uint8 version;
	uint8 band;
	uint16 type;
	int16 trigger; /* trigger for each type in new roam algorithm */
} wnm_roam_trigger_cfg_t;

#define WNM_NOQBSS_SCORE_MODE_VERSION_1		1
#define WNM_NOQBSS_SCORE_MODE_RSSI		0
#define WNM_NOQBSS_SCORE_MODE_CAPACITY		1

typedef struct wnm_noqbss_score_mode_v1 {
	uint16 version;
	uint16 len;
	uint8 mode; /* 0 - RSSI based, 1 - Capacity based */
	uint8 pad[3];
} wnm_noqbss_score_mode_v1_t;

/* Data structures for Interface Create/Remove  */

#define WL_INTERFACE_CREATE_VER_0	0
#define WL_INTERFACE_CREATE_VER_1	1
#define WL_INTERFACE_CREATE_VER_2	2
#define WL_INTERFACE_CREATE_VER_3	3

/*
 * The flags filed of the wl_interface_create is designed to be
 * a Bit Mask. As of now only Bit 0 and Bit 1 are used as mentioned below.
 * The rest of the bits can be used, incase we have to provide
 * more information to the dongle
 */

/*
 * Bit 0 of flags field is used to inform whether the interface requested to
 * be created is STA or AP.
 * 0 - Create a STA interface
 * 1 - Create an AP interface
 * NOTE: This Bit 0 is applicable for the WL_INTERFACE_CREATE_VER < 2
 */
#define WL_INTERFACE_CREATE_STA	(0u << 0u)
#define WL_INTERFACE_CREATE_AP	(1u << 0u)

/*
 * From revision >= 2 Bit 0 of flags field will not used be for STA or AP interface creation.
 * "iftype" field shall be used for identifying the interface type.
 */
typedef enum wl_interface_type {
	WL_INTERFACE_TYPE_STA = 0,
	WL_INTERFACE_TYPE_AP = 1,


	WL_INTERFACE_TYPE_NAN = 3,
	WL_INTERFACE_TYPE_P2P_GO = 4,
	WL_INTERFACE_TYPE_P2P_GC = 5,
	WL_INTERFACE_TYPE_P2P_DISC = 6,
	WL_INTERFACE_TYPE_IBSS = 7,
	WL_INTERFACE_TYPE_MESH = 8,
	WL_INTERFACE_TYPE_MAX
} wl_interface_type_t;

/*
 * Bit 1 of flags field is used to inform whether MAC is present in the
 * data structure or not.
 * 0 - Ignore mac_addr field
 * 1 - Use the mac_addr field
 */
#define WL_INTERFACE_MAC_DONT_USE	(0u << 1u)
#define WL_INTERFACE_MAC_USE		(1u << 1u)

/*
 * Bit 2 of flags field is used to inform whether core or wlc index
 * is present in the data structure or not.
 * 0 - Ignore wlc_index field
 * 1 - Use the wlc_index field
 */
#define WL_INTERFACE_WLC_INDEX_DONT_USE	(0u << 2u)
#define WL_INTERFACE_WLC_INDEX_USE	(1u << 2u)

/*
 * Bit 3 of flags field is used to create interface on the host requested interface index
 * 0 - Ignore if_index field
 * 1 - Use the if_index field
 */
#define WL_INTERFACE_IF_INDEX_USE       (1u << 3u)

/*
 * Bit 4 of flags field is used to assign BSSID
 * 0 - Ignore bssid field
 * 1 - Use the bssid field
 */
#define WL_INTERFACE_BSSID_INDEX_USE	(1u << 4u)

/*
 * Bit 5 of flags field indicates mesh usage on this interface
 */
#define WL_INTERFACE_MESH_USE		(1u << 5u)
#define WL_INTERFACE_NAN_TETHER_USE	(1u << 6u)

#define WL_INTERFACE_IOV_ALLOWED_FLAGS (WL_INTERFACE_CREATE_AP | WL_INTERFACE_MAC_USE | \
		WL_INTERFACE_WLC_INDEX_USE | WL_INTERFACE_IF_INDEX_USE | \
		WL_INTERFACE_BSSID_INDEX_USE | WL_INTERFACE_MESH_USE | WL_INTERFACE_NAN_TETHER_USE)

#define WL_INTERFACE_IOV_FLAGS_VALID(_f) (((_f) & ~WL_INTERFACE_IOV_ALLOWED_FLAGS) == 0u)

typedef struct wl_interface_create_v0 {
	uint16	ver;			/**< version of this struct */
	uint16	PAD;
	uint32  flags;			/**< flags that defines the operation */
	struct	ether_addr   mac_addr;	/**< Optional Mac address */
	uint8   PAD[2];                /**< Padding bytes */
} wl_interface_create_v0_t;

typedef struct wl_interface_create {
	uint16  ver;                    /**< version of this struct */
	uint8   PAD[2];                /**< Padding bytes */
	uint32  flags;                  /**< flags that defines the operation */
	struct  ether_addr   mac_addr;  /**< Optional Mac address */
	uint8   PAD[2];                /**< Padding bytes */
	uint32  wlc_index;              /**< Optional wlc index */
} wl_interface_create_v1_t;

typedef struct wl_interface_create_v2 {
	uint16  ver;                    /**< version of this struct */
	uint8   PAD[2];                /**< Padding bytes */
	uint32  flags;                  /**< flags that defines the operation */
	struct  ether_addr   mac_addr;  /**< Optional Mac address */
	uint8   iftype;                 /**< Type of interface created */
	uint8   PAD;                   /**< Padding bytes */
	uint32  wlc_index;              /**< Optional wlc index */
} wl_interface_create_v2_t;

typedef struct wl_interface_create_v3 {
	uint16	ver;			/**< version of this struct */
	uint16	len;			/**< length of whole structure including variable length */
	uint16	fixed_len;		/**< Fixed length of this structure excluding data[] */
	uint8	iftype;			/**< Type of interface created */
	uint8	wlc_index;		/**< Optional wlc index */
	uint32  flags;			/**< flags that defines the operation */
	struct	ether_addr   mac_addr;	/**< Optional Mac address */
	struct	ether_addr   bssid;	/**< Optional BSSID */
	uint8	if_index;		/**< interface index requested by Host */
	uint8	PAD[3];			/**< Padding bytes to ensure data[] is at 32 bit aligned */
	uint8	data[];			/**< Optional application/Module specific data */
} wl_interface_create_v3_t;

#define WL_INTERFACE_INFO_VER_0		0
#define WL_INTERFACE_INFO_VER_1		1
#define WL_INTERFACE_INFO_VER_2		2

typedef struct wl_interface_info_v0 {
	uint16	ver;			/**< version of this struct */
	struct ether_addr    mac_addr;	/**< MAC address of the interface */
	char	ifname[BCM_MSG_IFNAME_MAX]; /**< name of interface */
	uint8	bsscfgidx;		/**< source bsscfg index */
	uint8	PAD;
} wl_interface_info_v0_t;

typedef struct wl_interface_info_v1 {
	uint16  ver;                    /**< version of this struct */
	struct ether_addr    mac_addr;  /**< MAC address of the interface */
	char    ifname[BCM_MSG_IFNAME_MAX]; /**< name of interface */
	uint8   bsscfgidx;              /**< source bsscfg index */
	uint8	PAD;
} wl_interface_info_v1_t;

typedef struct wl_interface_info_v2 {
	uint16                  ver;                    /**< version of this struct */
	uint16                  length;                 /**< length of the whole structure */
	struct  ether_addr      mac_addr;               /**< MAC address of the interface */
	uint8                   bsscfgidx;              /**< source bsscfg index */
	uint8                   if_index;               /**< Interface index allocated by FW */
	char                    ifname[BCM_MSG_IFNAME_MAX]; /**< name of interface */
} wl_interface_info_v2_t;

#define PHY_RXIQEST_AVERAGING_DELAY 10

typedef struct wl_iqest_params {
	uint32 rxiq;
	uint8 niter;
	uint8 delay;
	uint8 PAD[2];
} wl_iqest_params_t;

typedef struct wl_iqest_sweep_params {
	wl_iqest_params_t params;
	uint8 nchannels;
	uint8 channel[3];	/** variable */
} wl_iqest_sweep_params_t;

typedef struct wl_iqest_value {
	uint8 channel;
	uint8 PAD[3];
	uint32 rxiq;
} wl_iqest_value_t;

typedef struct wl_iqest_result {
	uint8 nvalues;
	uint8 PAD[3];
	wl_iqest_value_t value[BCM_FLEX_ARRAY];
} wl_iqest_result_t;

#define WL_PRIO_ROAM_PROF_V1 (1u)

typedef struct wl_prio_roam_prof_v1 {
	uint16 version; /* Version info */
	uint16 length; /* byte length of this structure */
	uint8 prio_roam_mode; /* Roam mode RCC/RCC+Full Scan */
	uint8 PAD[3];
} wl_prio_roam_prof_v1_t;

typedef enum wl_prio_roam_mode {
	PRIO_ROAM_MODE_OFF = 0, /* Prio_Roam feature disable */
	PRIO_ROAM_MODE_RCC_ONLY = 1, /* Scan RCC list only */
	PRIO_ROAM_MODE_RCC_FULLSCAN = 2, /* Scan RCC list + Full scan */
	PRIO_ROAM_MODE_FULLSCAN_ONLY = 3 /* Full Scan only */
} wl_prio_roam_mode_t;

/* BTCX AIBSS (Oxygen) Status */
typedef struct wlc_btc_aibss_info {
	uint32	prev_tsf_l;		// Lower 32 bits of last read of TSF
	uint32	prev_tsf_h;		// Higher 32 bits of last read of TSF
	uint32	last_btinfo;		// Last read of BT info
	uint32	local_btinfo;		// Local BT INFO BitMap
	uint8	bt_out_of_sync_cnt;	// BT not in sync with strobe
	uint8	esco_off_cnt;		// Count incremented when ESCO is off
	uint8	strobe_enabled;		// Set only in AIBSS mode
	uint8	strobe_on;		// strobe to BT is on for Oxygen
	uint8	local_bt_in_sync;	// Sync status of local BT when strobe is on
	uint8	other_bt_in_sync;	// Sync state of BT in other devices in AIBSS
	uint8	local_bt_is_master;	// Local BT is master
	uint8	sco_prot_on;		// eSCO Protection on in local device
	uint8	other_esco_present;	// eSCO status in other devices in AIBSS
	uint8	rx_agg_change;		// Indicates Rx Agg size needs to change
	uint8	rx_agg_modified;	// Rx Agg size modified
	uint8	acl_grant_set;		// ACL grants on for speeding up sync
	uint8	write_ie_err_cnt;	// BTCX Ie write error cnt
	uint8	parse_ie_err_cnt;	// BTCX IE parse error cnt
	uint8	wci2_fail_cnt;		// WCI2 init failure cnt
	uint8	strobe_enable_err_cnt;	// Strobe enable err cnt
	uint8	strobe_init_err_cnt;	// Strobe init err cnt
	uint8	tsf_jump_cnt;		// TSF jump cnt
	uint8	acl_grant_cnt;		// ALC grant cnt
	uint8	PAD;
	uint16	ibss_tsf_shm;		// SHM address of strobe TSF
	uint16	pad2;
} wlc_btc_aibss_info_t;

#define WLC_BTC_AIBSS_STATUS_VER	1
#define WLC_BTC_AIBSS_STATUS_LEN	(sizeof(wlc_btc_aibss_status_t) - 2 * (sizeof(uint16)))

typedef struct wlc_btc_aibss_status {
	uint16	version;		// Version #
	uint16	len;			// Length of the structure(excluding len & version)
	int32	mode;			// Current value of btc_mode
	uint16	bth_period;             // bt coex period. read from shm.
	uint16	agg_off_bm;		// AGG OFF BM read from SHM
	uint8	bth_active;             // bt active session
	uint8	PAD[3];
	wlc_btc_aibss_info_t aibss_info;	// Structure definition above
} wlc_btc_aibss_status_t;

/* Beacon trim feature statistics */
/* configuration */
#define BCNTRIMST_PER			0	/* Number of beacons to trim (0: disable) */
#define BCNTRIMST_TIMEND		1	/* Number of bytes till TIM IE */
#define BCNTRIMST_TSFLMT		2	/* TSF tolerance value (usecs) */
/* internal use */
#define BCNTRIMST_CUR			3	/* PSM's local beacon trim counter */
#define BCNTRIMST_PREVLEN		4	/* Beacon length excluding the TIM IE */
#define BCNTRIMST_TIMLEN		5	/* TIM IE Length */
#define BCNTRIMST_RSSI			6	/* Partial beacon RSSI */
#define BCNTRIMST_CHAN			7	/* Partial beacon channel */
/* debug stat (off by default) */
#define BCNTRIMST_DUR			8	/* RX duration until beacon trimmed */
#define BCNTRIMST_RXMBSS		9	/* MYBSSID beacon received */
#define BCNTRIMST_CANTRIM		10	/* # beacons which were trimmed */
#define BCNTRIMST_LENCHG		11	/* # beacons not trimmed due to length change */
#define BCNTRIMST_TSFDRF		12	/* # beacons not trimmed due to large TSF delta */
#define BCNTRIMST_NOTIM			13	/* # beacons not trimmed due to TIM missing */

#define BCNTRIMST_NUM			14

#define WL_BCNTRIM_STATUS_VERSION_1	1
#define WL_BCNTRIM_STATUS_VERSION_2	2	/* current version of
	                                         * struct wl_bcntrim_status_v2_t and
	                                         * struct wl_bcntrim_status_query_v2_t
	                                         * changes in v2: curr_slice_id also include
	                                         * beacon offload state
	                                         */

typedef struct wl_bcntrim_status_query_v1 {
	uint16  version;
	uint16  len;     /* Total length includes fixed fields */
	uint8   reset;   /* reset after reading the stats */
	uint8   PAD[3];  /* 4-byte alignment */
} wl_bcntrim_status_query_v1_t;

/* bits for curr_slice_id */
#define WL_BCNTRIM_CURR_SLICE_ID_MASK        0x0Fu	/* bits 0-3 for curr_slice_id */
#define WL_BCNTRIM_SC_OFFLOAD_ACTIVE_MASK    0x80u	/* mask for sc beacon offload */
#define WL_BCNTRIM_SC_OFFLOAD_ACTIVE_FLAG    (1u << 7u)	/* MSB of curr_slice_id is used
	                                                 * to indicate if the offload is
	                                                 * currently active or not
	                                                 */
typedef struct wl_bcntrim_status_v1 {
	uint16  version;
	uint16  len;            /* Total length includes fixed fields and variable data[] */
	uint8   curr_slice_id;  /* slice index of the interface */
	uint8   applied_cfg;    /* applied bcntrim N threshold */
	uint8   pad[2];         /* 4-byte alignment */
	uint32  fw_status;      /* Bits representing bcntrim disable reason in FW */
	uint32  total_disable_dur;    /* total duration (msec) bcntrim remains
	                                 disabled due to FW disable reasons
	                               */
	uint32  data[];         /* variable length data containing stats */
} wl_bcntrim_status_v1_t;

/* v1 and v2 struct format for query and status are identical */
typedef wl_bcntrim_status_v1_t wl_bcntrim_status_v2_t;
typedef wl_bcntrim_status_query_v1_t wl_bcntrim_status_query_v2_t;

#define BCNTRIM_STATS_MAX		10	/* Total stats part of the status data[] */

/* Bits for FW status */
#define WL_BCNTRIM_DISABLE_HOST		0x1	/* Host disabled bcntrim through bcntrim IOVar */
#define WL_BCNTRIM_DISABLE_PHY_RATE	0x2	/* bcntrim disabled because beacon rx rate is
		                                 * higher than phy_rate_thresh
		                                 */
#define WL_BCNTRIM_DISABLE_QUIET_IE	0x4	/* bcntrim disable when Quiet IE present */
#define WL_BCNTRIM_DISABLE_QBSSLOAD_IE	0x8	/* bcntrim disable when QBSS Load IE present */
#define WL_BCNTRIM_DISABLE_OPERMODE_IE	0x10	/* bcntrim disable when opermode IE is present */
#define WL_BCNTRIM_DISABLE_CSA_IE	0x20	/* bcntrim dsiable when CSA IE is present */
#define WL_BCNTRIM_DISABLE_SC_OFFLOAD	0x40	/* bcntrim disable on SC */

#define BCNTRIM_DISABLE_THRESHOLD_TIME  1000 * 10	/* enable bcntrim after a threshold (10sec)
							 * when disabled due to above mentioned IE's
							 */
#define WL_BCNTRIM_CFG_VERSION_1     1
/* Common IOVAR struct */
typedef struct wl_bcntrim_cfg_v1 {
	uint16  version;
	uint16 len;          /* Total length includes fixed fields and variable data[] */
	uint16 subcmd_id;    /* subcommand id */
	uint16 pad;          /* pad/reserved */
	uint8 data[];        /* subcommand data; could be empty */
} wl_bcntrim_cfg_v1_t;

/* subcommands ids */
enum {
	WL_BCNTRIM_CFG_SUBCMD_PHY_RATE_THRESH =       0,   /* PHY rate threshold above
	                                                    * which bcntrim is not applied
	                                                    */
	WL_BCNTRIM_CFG_SUBCMD_OVERRIDE_DISABLE_MASK = 1,   /* Override bcntrim disable reasons */
	WL_BCNTRIM_CFG_SUBCMD_TSF_DRIFT_LIMIT =       2,   /* TSF drift limit to consider bcntrim */
	WL_BCNTRIM_CFG_SUBCMD_SC_BCNTRIM =            3    /* config bcntrim on SC */
};

#define BCNTRIM_MAX_PHY_RATE	48     /* in 500Kbps */
#define BCNTRIM_MAX_TSF_DRIFT   65535  /* in usec */
#define WL_BCNTRIM_OVERRIDE_DISABLE_MASK \
		(WL_BCNTRIM_DISABLE_QUIET_IE | WL_BCNTRIM_DISABLE_QBSSLOAD_IE)

/* WL_BCNTRIM_CFG_SUBCMD_PHY_RATE_TRESH */
typedef struct wl_bcntrim_cfg_phy_rate_thresh {
	uint32 rate;      /* beacon rate (in 500kbps units)  */
} wl_bcntrim_cfg_phy_rate_thresh_t;

/* WL_BCNTRIM_CFG_SUBCMD_OVERRIDE_DISABLE_MASK */
typedef struct wl_bcntrim_cfg_override_disable_mask {
	uint32  mask;     /* bits representing individual disable reason to override */
} wl_bcntrim_cfg_override_disable_mask_t;

/* WL_BCNTRIM_CFG_SUBCMD_TSF_DRIFT_LIMIT */
typedef struct wl_bcntrim_cfg_tsf_drift_limit {
	uint16   drift;   /* tsf drift limit specified in usec */
	uint8    pad[2];  /* 4-byte alignment */
} wl_bcntrim_cfg_tsf_drift_limit_t;

/* WL_BCNTRIM_CFG_SUBCMD_SC_BCNTRIM */
typedef struct wl_bcntrim_cfg_sc_bcntrim {
	uint32	sc_config; /* 0 disable or 1 enable sc bcntrim */
} wl_bcntrim_cfg_sc_bcntrim_t;

/* --------------  TX Power Cap --------------- */
#define TXPWRCAP_MAX_NUM_CORES 8
#define TXPWRCAP_MAX_NUM_ANTENNAS (TXPWRCAP_MAX_NUM_CORES * 2)

#define TXPWRCAP_MAX_NUM_CORES_V3 4
#define TXPWRCAP_MAX_NUM_ANTENNAS_V3 (TXPWRCAP_MAX_NUM_CORES_V3 * 2)

#define TXPWRCAP_NUM_SUBBANDS 5
#define TXPWRCAP_MAX_NUM_SUBGRPS 10

/* IOVAR txcapconfig enum's */
#define TXPWRCAPCONFIG_WCI2 0u
#define TXPWRCAPCONFIG_HOST 1u
#define TXPWRCAPCONFIG_WCI2_AND_HOST 2u
#define TXPWRCAPCONFIG_NONE 0xFFu

/* IOVAR txcapstate enum's */
#define TXPWRCAPSTATE_LOW_CAP  0
#define TXPWRCAPSTATE_HIGH_CAP 1
#define TXPWRCAPSTATE_HOST_LOW_WCI2_LOW_CAP	0
#define TXPWRCAPSTATE_HOST_LOW_WCI2_HIGH_CAP	1
#define TXPWRCAPSTATE_HOST_HIGH_WCI2_LOW_CAP	2
#define TXPWRCAPSTATE_HOST_HIGH_WCI2_HIGH_CAP	3

/* IOVAR txcapconfig and txcapstate structure is shared: SET and GET */
#define TXPWRCAPCTL_VERSION_2 2
#define TXPWRCAPCTL_VERSION_3 3
#define TXPWRCAPCTL_VERSION_4 4

typedef struct wl_txpwrcap_ctl {
	uint8   version;
	uint8   ctl[TXPWRCAP_NUM_SUBBANDS];
} wl_txpwrcap_ctl_t;

typedef struct wl_txpwrcap_ctl_v3 {
	uint8   version;
	uint8   ctl[TXPWRCAP_MAX_NUM_SUBGRPS];
} wl_txpwrcap_ctl_v3_t;

typedef struct wl_txpwrcap_ctl_v4 {
	uint8   version; /* TXPWRCAPCTL_VERSION_4 */
	uint8   len; /* length of whole structure */
	uint8   ctl; /* one single value common for all subgroup. */
} wl_txpwrcap_ctl_v4_t;

/* IOVAR txcapdump structure: GET only */
#define TXPWRCAP_DUMP_VERSION_2	 2
typedef struct wl_txpwrcap_dump {
	uint8   version;
	uint8	pad0;
	uint8   current_country[2];
	uint32	current_channel;
	uint8   config[TXPWRCAP_NUM_SUBBANDS];
	uint8   state[TXPWRCAP_NUM_SUBBANDS];
	uint8	high_cap_state_enabled;
	uint8	wci2_cell_status_last;
	uint8   download_present;
	uint8	num_subbands;
	uint8	num_antennas;
	uint8   num_antennas_per_core[TXPWRCAP_MAX_NUM_CORES];
	uint8	num_cc_groups;
	uint8   current_country_cc_group_info_index;
	int8    low_cap[TXPWRCAP_MAX_NUM_ANTENNAS*TXPWRCAP_NUM_SUBBANDS];
	int8    high_cap[TXPWRCAP_MAX_NUM_ANTENNAS*TXPWRCAP_NUM_SUBBANDS];
	uint8	PAD[3];
} wl_txpwrcap_dump_t;

typedef struct wl_txpwrcap_dump_v3 {
	uint8   version;
	uint8	pad0;
	uint8   current_country[2];
	uint32	current_channel;
	uint8   config[TXPWRCAP_NUM_SUBBANDS];
	uint8   state[TXPWRCAP_NUM_SUBBANDS];
	uint8	high_cap_state_enabled;
	uint8	wci2_cell_status_last;
	uint8   download_present;
	uint8	num_subbands;
	uint8	num_antennas;
	uint8   num_antennas_per_core[TXPWRCAP_MAX_NUM_CORES];
	uint8	num_cc_groups;
	uint8   current_country_cc_group_info_index;
	uint8	cap_states_per_cc_group;
	int8    host_low_wci2_low_cap[TXPWRCAP_MAX_NUM_ANTENNAS*TXPWRCAP_NUM_SUBBANDS];
	int8    host_low_wci2_high_cap[TXPWRCAP_MAX_NUM_ANTENNAS*TXPWRCAP_NUM_SUBBANDS];
	int8    host_high_wci2_low_cap[TXPWRCAP_MAX_NUM_ANTENNAS*TXPWRCAP_NUM_SUBBANDS];
	int8    host_high_wci2_high_cap[TXPWRCAP_MAX_NUM_ANTENNAS*TXPWRCAP_NUM_SUBBANDS];
	uint8	PAD[2];
} wl_txpwrcap_dump_v3_t;

/*
* Capability flag for wl_txpwrcap_tbl_v2_t and wl_txpwrcap_t
* The index into pwrs will be:  0: onbody-cck, 1: onbody-ofdm, 2:offbody-cck, 3:offbody-ofdm
*
* For 5G power in SDB case as well as for non-SDB case, the value of flag will be: CAP_ONOFF_BODY
* The index into pwrs will be:  0: onbody, 1: offbody-ofdm
*/

#define CAP_ONOFF_BODY	(0x1)	/* on/off body only */
#define CAP_CCK_OFDM	(0x2)	/* cck/ofdm capability only */
#define CAP_LTE_CELL	(0x4)	/* cell on/off capability; required for iOS builds */
#define CAP_HEAD_BODY	(0x8)	/* head/body capability */
#define CAP_2G_DEPON_5G (0x10)	/* 2G pwr caps depend on other slice 5G subband */
#define CAP_SISO_MIMO	(0x20)	/* Siso/Mimo Separate Power Caps */
#define CAP_ANT_TX	(0x40)	/* Separate Power Caps based on cell ant tx value */
#define CAP_LTE_PQBIT	(0x100u) /* QPBit is enabled */
#define CAP_RC1		(0x200u) /* RC1 is enabled */
#define CAP_BT_EPA	(0x400u) /* BT_EPA is enabled */

#define CAP_ONOFF_BODY_CCK_OFDM	(CAP_ONOFF_BODY | CAP_CCK_OFDM)
#define CAP_TXPWR_ALL	(CAP_ONOFF_BODY|CAP_CCK_OFDM|CAP_LTE_CELL|\
	CAP_SISO_MIMO|CAP_HEAD_BODY|CAP_ANT_TX)

#define TXHDR_SEC_MAX		5u	/* Deprecated.  Kept till removed in all branches */
#define TXPWRCAP_MAX_STATES	4u
#define TXPWRCAP_MAX_STATES_V3	10u
#define TXPWRCAP_CCKOFDM_ONOFFBODY_MAX_STATES 4u
#define TXPWRCAP_ONOFFBODY_MAX_STATES 2u
#define TXPWRCAP_ONOFFCELL_MAX_STATES 2u

#define TXHDR_SEC_NONSDB_MAIN_2G	0
#define TXHDR_SEC_NONSDB_MAIN_5G	1
#define TXHDR_SEC_NONSDB_AUX_2G		2
#define TXHDR_SEC_NONSDB_AUX_5G		3
#define TXHDR_SEC_SDB_MAIN_2G		4
#define TXHDR_SEC_SDB_MAIN_5G		5
#define TXHDR_SEC_SDB_AUX_2G		6
#define TXHDR_SEC_SDB_AUX_5G		7
#define TXHDR_MAX_SECTION		8
#define TXHDR_SEC_NONSDB_MAIN_6G	8
#define TXHDR_SEC_NONSDB_AUX_6G		9
#define TXHDR_SEC_SDB_MAIN_6G		10
#define TXHDR_SEC_SDB_AUX_6G		11
#define TXHDR_MAX_SECTION_6G		12

#define WL_TXPWRCAP_MAX_SLICES	2
#define WL_TXPWRCAPDUMP_VER	4

#define WL_TXPWRCAP_VERSION_2 2
#define WL_TXPWRCAP_VERSION_3 3

typedef struct wl_txpwrcap {
	uint8   capability;
	uint8	num_cap_states;
	uint8   section; /* Index from above,eg. TXHDR_SEC_NONSDB */
	int8    pwrs[][TXPWRCAP_NUM_SUBBANDS][TXPWRCAP_MAX_NUM_CORES];
} wl_txpwrcap_t;

typedef struct {
	uint8   capability;
	uint8	num_cap_states;
	uint8   num_subgrps;
	uint8   section; /* Index from above,eg. TXHDR_SEC_NONSDB */
	int8    pwrs[][TXPWRCAP_MAX_NUM_SUBGRPS][TXPWRCAP_MAX_NUM_ANTENNAS_V3];
} wl_txpwrcap_v2_t;

#define TXPWRCAP_DUMP_VERSION_4 4u
#define TXPWRCAP_DUMP_VERSION_5 5u
#define TXPWRCAP_DUMP_VERSION_6 6u
#define TXPWRCAP_DUMP_VERSION_7 7u

typedef struct wl_txpwrcap_dump_v4 {
	uint8		version;
	uint8		num_pwrcap;
	uint8		current_country[2];
	uint32		current_channel;
	uint8		download_present;
	uint8		num_cores;	/* number cores on slice */
	uint8		num_cc_groups;	/* number cc groups */
	uint8		current_country_cc_group_info_index;
	/* first power cap always exist
	* On main,-non-sdb follows by sdb2g and then sdb5g
	* On aux slice - aux2g then aux5g.
	*/
	uint8 pwrcap[BCM_FLEX_ARRAY]; /* variable size power caps (wl_txpwrcap_t) */
} wl_txpwrcap_dump_v4_t;

typedef struct wl_txpwrcap_dump_v5 {
	uint8	version;
	uint8	num_pwrcap;
	uint8	current_country[2];
	uint8	current_channel;
	uint8	high_cap_state_enabled;
	uint8	reserved[2];
	uint8	download_present;
	uint8	num_ants;	/* number antenna slice */
	uint8	num_cc_groups;	/* number cc groups */
	uint8	current_country_cc_group_info_index;
	uint8	ant_tx; /* current value of ant_tx */
	uint8	cell_status; /* current value of cell status */
	uint8	pwrcap[]; /* variable size power caps (wl_txpwrcap_v2_t) */
} wl_txpwrcap_dump_v5_t;

typedef struct wl_txpwrcap_dump_v6 {
	uint8   version;
	uint8   num_pwrcap;
	uint8   current_country[2];
	uint8   current_channel;
	uint8   high_cap_state_enabled;
	uint8   rc1;
	uint8   bt_epa;
	uint8   download_present;
	uint8   num_ants;       /* number antenna slice */
	uint8   num_cc_groups;  /* number cc groups */
	uint8   current_country_cc_group_info_index;
	uint8   ant_tx; /* current value of ant_tx */
	uint8   cell_status; /* current value of cell status */
	uint16  capability[TXHDR_MAX_SECTION]; /* capabilities */
	uint8   pwrcap[]; /* variable size power caps (wl_txpwrcap_v2_t) */
} wl_txpwrcap_dump_v6_t;

typedef struct wl_txpwrcap_dump_v7 {
	uint8   version;
	uint8   num_pwrcap;
	uint8   current_country[2];
	uint8   current_channel;
	uint8   high_cap_state_enabled;
	uint8   rc1;
	uint8   bt_epa;
	uint8   download_present;
	uint8   num_ants;       /* number antenna slice */
	uint8   num_cc_groups;  /* number cc groups */
	uint8   current_country_cc_group_info_index;
	uint8   ant_tx; /* current value of ant_tx */
	uint8   cell_status; /* current value of cell status */
	uint16  capability[TXHDR_MAX_SECTION_6G]; /* capabilities */
	int8    pwrcap[]; /* variable size power caps (wl_txpwrcap_v2_t) */
} wl_txpwrcap_dump_v7_t;

#define TXCAPINFO_VERSION_1 1
typedef struct wl_txpwrcap_ccgrp_info {
	uint8   num_cc;
	char    cc_list[1][2]; /* 2 letters for each country. At least one country */
} wl_txpwrcap_ccgrp_info_t;

typedef struct {
	uint16  version;
	uint16  length; /* length in bytes */
	uint8   num_ccgrp;
	/* followed by one or more wl_txpwrcap_ccgrp_info_t */
	wl_txpwrcap_ccgrp_info_t ccgrp_data[BCM_FLEX_ARRAY];
} wl_txpwrcap_info_t;

typedef struct wl_txpwrcap_tbl {
	uint8 num_antennas_per_core[TXPWRCAP_MAX_NUM_CORES];
	/* Stores values for valid antennas */
	int8 pwrcap_cell_on[TXPWRCAP_MAX_NUM_ANTENNAS]; /* qdBm units */
	int8 pwrcap_cell_off[TXPWRCAP_MAX_NUM_ANTENNAS]; /* qdBm units */
} wl_txpwrcap_tbl_t;

typedef struct wl_txpwrcap_tbl_v2 {
	uint8 version;
	uint8 length;		/* size of entire structure, including the pwrs */
	uint8 capability;	/* capability bitmap */
	uint8 num_cores;	/* number of cores i.e. entries in each cap state row */
	/*
	* pwrs array has TXPWRCAP_MAX_STATES rows - one for each cap state.
	* Each row has up to TXPWRCAP_MAX_NUM_CORES entries - one for each core.
	*/
	uint8 pwrs[][TXPWRCAP_MAX_NUM_CORES];  /* qdBm units */
} wl_txpwrcap_tbl_v2_t;

/* commands that go into the id of wl_txcap_ioc_t */
#define TXCAP_NONE_CMD          0u
#define TXCAP_TRIM_CMD          1u
#define TXCAP_CC_LIST_CMD       2u

/* structure for "txcap" iovar */
typedef struct wl_txcap_ioc {
	uint16 id; /* subcommand id */
	uint16 len; /* length of structure including data */
	uint8 data[BCM_FLEX_ARRAY]; /* subcommand data */
} wl_txcap_ioc_t;

/* When command is TXCAP_TRIM_CMD, the data of wl_txcap_ioc_t is wl_txcap_trim_t
 * trim modes:
 * TXCAP_TRIM_MODE_NOW: trim all cc groups except current
 * TXCAP_TRIM_MODE_AHEAD: accepts an expected country code.  Upon next time
 * we load the txcap, we'll trim cc groups other
 * than current and the expected country code
 */
/* When command is TXCAP_TRIM_CMD, the data of wl_txcap_ioc_t is wl_txcap_trim_t
* trim modes:
* TXCAP_TRIM_MODE_NONE: clear expected country value set by MODE_AHEAD.
*   Only used if memory is not yet trimmed.
* TXCAP_TRIM_MODE_NOW: trim all cc groups except current.
*   Can use this to unset value by MODE_AHEAD if memory is already trimmed.
* TXCAP_TRIM_MODE_AHEAD: accepts an expected country code.  Upon next time
*   we load the txcap, we'll trim cc groups other
*   than current and the expected country code
*/
#define TXCAP_TRIM_MODE_NONE	0u
#define TXCAP_TRIM_MODE_NOW	1u
#define TXCAP_TRIM_MODE_AHEAD	2u
typedef struct wl_txcap_trim {
	uint8 mode;
	uint8 ccode[2u];
	uint8 pad;
} wl_txcap_trim_t;

/* xtlvid encoded in wl_txpwrcap_cclist_t */
#define TXCAP_ID_CC_LIST_CURRENT	1u
#define TXCAP_ID_CC_LIST_TRIMMED	2u

#define TXCAP_CCLIST_VER_1	1u
/*
 * When command is TXCAP_CC_LIST_CMD, the data of wl_txcap_ioc_t is wl_txpwrcap_cclist_t
 * The data of wl_txpwrcap_cclist_t is a bunch of xtlvid
 */
typedef struct wl_txpwrcap_cclist {
	uint16 version;
	uint16 len;
	uint8 data[BCM_FLEX_ARRAY]; /* one or more xltlvs */
} wl_txpwrcap_cclist_t;

/* Supported sar modes value for sar_enable IOVAR */
typedef enum {
	SAR_MODE_DISABLE	= (0u),
	SAR_MODE_HEAD		= (1u << 0u),
	SAR_MODE_GRIP		= (1u << 1u),
	SAR_MODE_NR_MW		= (1u << 2u),
	SAR_MODE_NR_SUB6	= (1u << 3u),
	SAR_MODE_BT		= (1u << 4u),
	SAR_MODE_HS		= (1u << 5u),
	SAR_MODE_MHS		= (1u << 6u),
	SAR_MODE_FLIP		= (1u << 7u),
	SAR_MODE_MAX		= (1u << 8u)
} sar_modes_new;

/* sub6 bandinfo valuse for s6bandinfo iovar */
typedef enum {
	NR_SUB6_BAND2		= 2u,
	NR_SUB6_BAND25		= 25u,
	NR_SUB6_BAND41		= 41u,
	NR_SUB6_BAND48		= 48u,
	NR_SUB6_BAND66		= 66u,
	NR_SUB6_BAND77		= 77u,
	NR_SUB6_BAND_UN		= 0u
} sar_sub6bandinfo_mode;

/* sar cap states values */
typedef enum {
	SAR_CAP_AIR		= (1u << 0u),
	SAR_CAP_HEAD		= (1u << 1u),
	SAR_CAP_GRIP		= (1u << 2u),
	SAR_CAP_NRMW		= (1u << 3u),
	SAR_CAP_NRS6		= (1u << 4u),
	SAR_CAP_BT		= (1u << 5u),
	SAR_CAP_HS		= (1u << 6u),
	SAR_CAP_RU		= (1u << 7u),
	SAR_CAP_MHS		= (1u << 8u),
	SAR_CAP_S6_BAND2	= (1u << 9u),
	SAR_CAP_S6_BAND25	= (1u << 10u),
	SAR_CAP_S6_BAND41	= (1u << 11u),
	SAR_CAP_S6_BAND48	= (1u << 12u),
	SAR_CAP_S6_BAND66	= (1u << 13u),
	SAR_CAP_S6_BAND77	= (1u << 14u),
	SAR_CAP_FLIP		= (1u << 15u),
	SAR_CAP_MAX		= SAR_CAP_FLIP
} sar_caps;

#define SAR_DISABLE_PWR_BIT		(1u << 31u)
#define SAR_ALL_MODE_CAP_MASK (SAR_CAP_HEAD | SAR_CAP_GRIP | SAR_CAP_NRMW | SAR_CAP_NRS6 |\
					SAR_CAP_BT | SAR_CAP_HS | SAR_CAP_MHS | SAR_CAP_FLIP)

#define SAR_ABS_CAP_MASK	(SAR_CAP_HEAD | SAR_CAP_GRIP | SAR_CAP_BT | SAR_CAP_HS |\
					SAR_CAP_MHS | SAR_CAP_FLIP)
#define SAR_COND_CAP_MASK	(SAR_CAP_NRMW | SAR_CAP_NRS6)
#define SAR_OFFSET_CAP_MASK	(SAR_CAP_NRMW | SAR_CAP_NRS6)
#define SAR_SUB6_BAND_CAP_MASK	(SAR_CAP_S6_BAND2 | SAR_CAP_S6_BAND25 | SAR_CAP_S6_BAND41 | \
					SAR_CAP_S6_BAND48 | SAR_CAP_S6_BAND66 | SAR_CAP_S6_BAND77)

#define TXPWRCAP_SAR_STATE_SU		0u
#define TXPWRCAP_SAR_MAX_STATES_SU_V3	1u

#define TXPWRCAP_SAR_STATE_RU_26		1u
#define TXPWRCAP_SAR_STATE_RU_56		2u
#define TXPWRCAP_SAR_STATE_RU_106		3u
#define TXPWRCAP_SAR_MAX_STATES_RU_V3		3u
#define TXPWRCAP_SAR_MAX_STATES_SU_RU_V3	4u

typedef struct wl_txpwrcap_tbl_v3 {
	uint8 version;
	uint8 length;		/* size of entire structure, including the pwrs */
	uint8 capability;	/* capability bitmap */
	uint8 num_cores;	/* number of cores */
	uint8 num_antennas_per_core[TXPWRCAP_MAX_NUM_CORES_V3];
	/*
	* pwrs array has TXPWRCAP_MAX_STATES rows - one for each cap state.
	* Each row has up to TXPWRCAP_MAX_NUM_ANTENNAS entries - for each antenna.
	* Included in the rows of powers are rows for fail safe.
	*/
	int8 pwrs[][TXPWRCAP_MAX_NUM_ANTENNAS_V3];	/* qdBm units */
} wl_txpwrcap_tbl_v3_t;

/* dynamic sar iovar subcommand ids */
enum {
	IOV_DYNSAR_MODE		= 1,
	IOV_DYNSAR_PWR_OFF	= 2,
	IOV_DYNSAR_STAT_SUM	= 3,
	IOV_DYNSAR_STAT_DET	= 4,
	IOV_DYNSAR_TS		= 5,
	IOV_DYNSAR_OPT_DUR	= 6,
	IOV_DYNSAR_OPT_TXDC	= 7,
	IOV_DYNSAR_STATUS	= 8,
	IOV_DYNSAR_EVENT	= 9,
	IOV_DYNSAR_VAR		= 10,
	IOV_DYNSAR_SUM_AGG	= 11,
	IOV_DYNSAR_PROFILE	= 12,
	IOV_DYNSAR_PROFILES	= 13,
	IOV_DYNSAR_STAT_DET_V2	= 14,
	IOV_DYNSAR_CMD_LAST
};

/* when subcommand is IOV_DYNSAR_MODE, the mode can be one of the below */
enum {
	IOV_DYNSAR_MODE_OFF	   = 0, /* DSA optimization turned off */
	IOV_DYNSAR_MODE_PWR	   = 1, /* DSA Power optimization mode */
	IOV_DYNSAR_MODE_HBR_NOMUTE = 2, /* DSA Hybrid power and nomute optimization mode */

	IOV_DYNSAR_MODE_MAX
};

#define DYNSAR_CNT_VERSION_V1	1u
#define DYNSAR_CNT_VERSION_V2	2u
#define DYNSAR_CNT_VERSION_V3	3u
#define DYNSAR_CNT_VERSION_V4	4u
#define DYNSAR_STS_OBS_WIN	20u
#define DYNSAR_MAX_ANT		WL_STA_ANT_MAX
#define DYNSAR_MAX_AGG_IDX (DYNSAR_MAX_ANT << 1u) /* max antenna aggregation index */
#define DYNSAR_MAC_NUM		2u

/* Error bits */
#define DYNSAR_NO_TXCAP		(1u << 0u)
#define DYNSAR_NO_CLM		(1u << 1u)
#define DYNSAR_TDMTX_DISABLED	(1u << 2u)
#define DYNSAR_VIOLATION	(1u << 3u)
#define DYNSAR_ANT_NUM_MISMATCH	(1u << 4u)
#define DYNSAR_COUNTRY_DISABLED	(1u << 5u)

/* TLVs for DYNSAR related IOVARs */
typedef enum wl_dynsar_tlv {
	DYNSAR_AGG_IDX       = 1u,
	DYNSAR_FS_RSN        = 2u
} wl_dynsar_tlv_t;

/* bit position corresponding to each failsafe reason */
typedef enum wlc_fs_rsn_bit {
	FS_FORCED = 0, /* Forced */
	FS_UX_SAFE = 1, /* Ux Safe exceeds limit */
	FS_UTIL_MEAN = 2, /* Mean Util exceeds limit */
	FS_TXDC_AVG = 3, /* Average Txdc exceeds limit */
} wlc_dynsar_fs_rsn_bit_t;

typedef uint8 wlc_dynsar_fs_rsn_t;

typedef struct wlc_dynsar_sts_mon_ctr_st {
	uint32 tx_dur;		/* in usec */
	uint32 tx_dur_raw;	/* in usec */
	uint32 plim_avg;	/* In uw. plim averaged over mon win. */
	uint32 energy;		/* pavg * dur in mw * ms */
	uint32 qsar;		/* plim * dur in mw * ms */
	uint16 fs;		/* failsafe duration in usec */
	uint8  util_hist;       /* utilization in past observe sec */
	uint8  util_pred;       /* utilization of past (observe - budget) & predicted budget sec */
} wlc_dynsar_sts_mon_ctr_t;

typedef struct wlc_dynsar_sts_mon_ctr_v2_st {
	uint32 tx_dur;          /* in usec */
	uint32 tx_dur_raw;      /* in usec */
	uint32 plim_avg;        /* In uw. plim averaged over mon win. */
	uint32 energy;          /* pavg * dur in mw * ms */
	uint32 qsar;            /* plim * dur in mw * ms */
	uint16 fs;              /* failsafe duration in usec */
	uint16 util_hist;       /* 10 x utilization in past observe sec */
	uint16 util_pred;       /* 10 x utilization of past (observe - budget) & predicted
				* budget sec
				*/
	uint16 pad;
} wlc_dynsar_sts_mon_ctr_v2_t;

/* DSA v2 support */
typedef struct wlc_dynsar_sts_mon_ctr_v3 {
	uint32 tx_dur;          /* in usec */
	uint16 util;            /* 10 x Tmon utilization */
	uint16 util_hist;       /* 10 x Twin utilization */
	uint16 util_pred;       /* 10 x Twin utilization forecast after budget num sec */
	uint16 pad;
} wlc_dynsar_sts_mon_ctr_v3_t;

typedef struct wlc_dynsar_sts_obs_win {
	uint8 opt;
	uint8 valid;
	uint8 start;
	uint8 PAD;
	uint32 dur;	/* monitor duration in usec */
	uint64 ts;	/* timestamp in usec */
} wlc_dynsar_sts_obs_win_t;

typedef struct dynsar_agg_entry {
	uint32 util;
	uint32 util_sqr;
	uint32 mean_util;
	uint32 var;
} dynsar_agg_ent_t;

typedef struct dynsar_agg_stat {
	/* variable length */
	uint16 len;	/* length of this structure including data */
	uint16 num_ent; /* number of entries per aggregated slot */
	uint16 num_agg; /* number of aggregated slots */
	uint16 PAD;	/* pad */
	uint64 buf[];	/* num_ent entries wlc_dynsar_sts_obs_win_t
			* followed by num_ent entries dynsar_agg_ent_t
			*/
} dynsar_agg_stat_t;

/* Common header for the detailed stats IOVAR */
typedef struct dynsar_cnt_head {
	uint16 ver;
	uint16 len;     /* length of this structure */
	uint8  num_ant; /* num_antennas */
	uint8  win;     /* number of valid entries in the observe window */
	uint8  slice;
	uint8  PAD;
	uint64 sync_ts; /* time of first mon period collection after last sync */
} dynsar_cnt_head_t;

/* structure holding dynsar per slice counters that interface to iovar */
typedef struct dynsar_cnt_v1 {
	uint16 ver;
	uint16 len;	/* length of this structure */
	uint8  num_ant;	/* num_antennas */
	uint8  win;	/* number of valid entries in the observe window */
	uint8  slice;
	uint8  PAD;	/* num_antennas */
	uint64 sync_ts;	/* time of first mon period collection after last sync */
	wlc_dynsar_sts_obs_win_t obs[DYNSAR_STS_OBS_WIN];
	wlc_dynsar_sts_mon_ctr_t mon_ctr[DYNSAR_STS_OBS_WIN][DYNSAR_MAX_ANT];
} dynsar_cnt_v1_t;

/* structure holding dynsar per slice counters that interface to iovar */
typedef struct dynsar_cnt_v2 {
	uint16 ver;
	uint16 len;     /* length of this structure */
	uint8  num_ant; /* num_antennas */
	uint8  win;     /* number of valid entries in the observe window */
	uint8  slice;
	uint8  PAD;
	uint64 sync_ts; /* time of first mon period collection after last sync */
	wlc_dynsar_sts_obs_win_t    obs[DYNSAR_STS_OBS_WIN];
	wlc_dynsar_sts_mon_ctr_v2_t mon_ctr[DYNSAR_STS_OBS_WIN][DYNSAR_MAX_ANT];
} dynsar_cnt_v2_t;

/* structure holding dynsar per slice counters that interface to iovar */
typedef struct dynsar_cnt_v3 {
	uint16 ver;
	uint16 len;     /* length of this structure */
	uint8  num_ant; /* num_antennas */
	uint8  win;     /* number of valid entries in the observe window */
	uint8  slice;
	uint8  PAD;
	uint64 sync_ts; /* time of first mon period collection after last sync */
	wlc_dynsar_sts_obs_win_t    obs[DYNSAR_STS_OBS_WIN];
	wlc_dynsar_sts_mon_ctr_v3_t mon_ctr[DYNSAR_STS_OBS_WIN][DYNSAR_MAX_ANT];
} dynsar_cnt_v3_t;

typedef struct dynsar_shared_ant_stats {
	uint32 tx_dur;		/* tx duration */
	uint8  sar_util;	/* sar utilization */
	uint8  PAD[3];		/* pad */
} dynsar_shared_ant_stats_t;

typedef struct dynsar_unshared_ant_stats {
	uint32 qsar;	/* mw * ms */
	uint32 energy;	/* mw * ms */
	uint32 tx_dur;	/* tx duration */
} dynsar_unshared_ant_stats_t;

typedef struct dynsar_ant_stats {
	uint32 tx_dur;		/* tx duration     */
	uint16 sar_util;	/* sar utilization */
	uint8  valid;		/* valid record    */
	uint8  PAD;		/* pad */
} dynsar_ant_stats_t;

typedef struct dynsar_sum_v1 {
	uint16 ver;
	uint16 len;      /* length of this structure */
	uint32 dur;      /* duration in us */
	uint64 ts;       /* time stamp of report in us */
	uint64 sync_ts;  /* time of first mon period collection after last sync */
	uint8  slice;
	uint8  num_ant;
	uint8  opt;
	uint8  sync;
	/* per antenna counters aggregated if shared between radios */
	struct {
		uint32 tx_dur;  /* tx duration */
		uint8 sar_util; /* sar utilization */
		uint8 PAD[3];   /* pad */
	} shared[DYNSAR_MAX_ANT];

	/* per antenna counters not aggregated between radios */
	struct {
		uint32 qsar;    /* mw * ms */
		uint32 energy;  /* mw * ms */
	} unshared[DYNSAR_MAX_ANT];
	uint8  PAD[4];
} dynsar_sum_v1_t;

typedef struct dynsar_sum_v2 {
	uint16 ver;
	uint16 len;      /* length of this structure */
	uint32 dur;      /* duration in us */
	uint64 ts;       /* time stamp of report in us */
	uint64 sync_ts;  /* time of first mon period collection after last sync */
	uint8  num_ant;  /* max number of antennas between 2 slices */
	uint8  opt;
	uint8  sync;
	uint8  max_mac;  /* number of slices */
	uint8  num_agg;  /* number of aggregated antennas */
	uint8  offset_shared;   /* offset from beginning of structure to physical antenna data */
	uint8  offset_unshared; /* offset from beginning of structure to aggregated antenna data */
	uint8  PAD;
	/* Variable length data sections follow as per above offsets:
	* DYNSAR_CNT_VERSION_V2:
	* dynsar_unshared_ant_stats_t [max_mac][num_ant]
	* dynsar_shared_ant_stats_t [num_agg]
	*
	* DYNSAR_CNT_VERSION_V4:
	* dynsar_ant_stats_t [max_mac][num_ant] // Per Physical antenna
	* dynsar_ant_stats_t [num_agg]		// Per Aggregated antenna
	*/
} dynsar_sum_v2_t;

typedef struct dynsar_var_info {
	uint32 lim; /* variance limit */
	uint32 off; /* hysterysis offset applied to variance while optimized */
} dynsar_var_info_t;

typedef struct dynsar_status_v3 {
	uint16 ver;
	uint16 len;		/* length of this structure */
	uint8  slice;		/* slice number */
	uint8  mode;		/* optimization mode */
	uint8  util_thrhd;	/* utilization threshold */
	uint8  opt_txdc;	/* txdc prediction percentage */
	uint8  opt_dur;		/* optimization prediction duration */
	uint8  event;		/* if wl event is configured */
	uint8  time_sync;	/* if gpio pulse is configured */
	uint8  power_off;	/* power offset in db */
	uint8  num_ant;		/* num antenna */
	uint8  status;		/* status bitmap. e.g. WL_DYNSAR_STS_PWR_OPT.
				* These are same as status field in wl_event
				*/
	uint8  error;		/* error bits */
	uint8  gpio_pin;	/* gpio pin */
	dynsar_var_info_t var;  /* variance information */
	uint8  data[];	        /* variable length data of type bcm_xtlv_t */
} dynsar_status_v3_t;

typedef struct dynsar_opt_profile_key {
	uint8 pwr_off;
	uint8 mode;
} dynsar_opt_profile_key_t;

typedef struct dynsar_opt_profile_v1 {
	uint32 var_lim; /* variance limit */
	uint32 var_off; /* hysterysis offset applied to variance while optimized */
	uint8 pwr_off; /* power boost offset */
	uint8 mode; /* DSA mode */
	/* optimization parameters */
	uint8 opt_dur;     /* number of mon periods in future to predict optimization */
	uint8 util_thrhd;  /* Max history utilization before turning off optimization */
	uint8 util_mean;   /* Mean utilization percentage before turning off optimization */
	/* failsafe parameters */
	uint8 fs;          /* historical util percentage to start failsafe */
	uint8 util_mean_fs; /* Mean utilization to force failsafe */
	uint8 avg_txdc_fs;   /* mean txdc threshold for failsafe */
	/* txdc limits */
	uint8 util_mean_ddc; /* mean utilization threshold to apply avg_txdc_th */
	uint8 opt_txdc;    /* txdc cap when optimized */
	uint8 avg_txdc_th;   /* mean txdc threshold for throttling txdc */
	uint8 opt_txdc_tgt; /* target txdc */
} dynsar_opt_profile_v1_t;

typedef struct dynsar_opt_profile_v2 {
	uint32 var_lim; /* variance limit */
	uint32 var_off; /* hysterysis offset applied to variance while optimized */
	uint8 pwr_off; /* power boost offset */
	uint8 mode; /* DSA mode */
	/* optimization parameters */
	uint8 opt_dur;     /* number of mon periods in future to predict optimization */
	uint8 util_thrhd;  /* Max history utilization before turning off optimization */
	uint8 util_mean;   /* Mean utilization percentage before turning off optimization */
	/* failsafe parameters */
	uint8 fs;          /* historical util percentage to start failsafe */
	uint8 util_mean_fs; /* Mean utilization to force failsafe */
	uint8 avg_txdc_fs;   /* mean txdc threshold for failsafe */
	/* txdc limits */
	uint8 util_mean_ddc; /* mean utilization threshold to apply avg_txdc_th */
	uint8 opt_txdc;    /* txdc cap when optimized */
	uint8 avg_txdc_th;   /* mean txdc threshold for throttling txdc */
	uint8 opt_txdc_tgt; /* target txdc */
	uint8 twin; /* Twin in seconds */
	uint8 pad[3];
} dynsar_opt_profile_v2_t;

typedef struct dynsar_opt_profiles_v1 {
	uint16 ver;
	uint16 len;	/* length of this structure */
	uint16 active;  /* active profile */
	uint16 num_profiles;  /* number of profiles in variable length below */
	dynsar_opt_profile_v1_t profiles[];
} dynsar_opt_profiles_v1_t;

typedef struct dynsar_opt_profiles_v2 {
	uint16 ver;
	uint16 len;	/* length of this structure */
	uint16 active;  /* active profile */
	uint16 num_profiles;  /* number of profiles in variable length below */
	dynsar_opt_profile_v2_t profiles[];
} dynsar_opt_profiles_v2_t;

typedef dynsar_opt_profiles_v2_t dynsar_opt_profiles_t;

typedef struct wl_dynsar_ioc {
	uint16 id;	/* ID of the sub-command */
	uint16 len;	/* total length of all data[] */
	uint8 PAD[4];
	union { /* var len payload */
		uint8 cnt;
		dynsar_cnt_v1_t det;
		dynsar_cnt_v2_t detv2;
		dynsar_cnt_v3_t detv3;
		dynsar_agg_stat_t agg_stat;
		dynsar_sum_v1_t sum;
		dynsar_sum_v2_t sumv2;
		dynsar_status_v3_t statusv3;
		dynsar_var_info_t var;
		dynsar_opt_profiles_v1_t profiles;
		dynsar_opt_profiles_v2_t profilesv2;
	} data;
} wl_dynsar_ioc_t;

typedef struct wlc_dynsar_status {
	uint16 ver;
	uint16 len;      /* length of this structure */
} wl_dynsar_status_t;

/* -------------- dynamic BTCOEX --------------- */
#define DCTL_TROWS	2			/**< currently practical number of rows  */
#define DCTL_TROWS_MAX	4			/**<  2 extra rows RFU */
/* DYNCTL profile flags */
#define DCTL_FLAGS_DISABLED	0		/**< default value: all features disabled */
#define DCTL_FLAGS_DYNCTL	(1 << 0)	/**<  1 - enabled, 0 - legacy only */
#define DCTL_FLAGS_DESENSE	(1 << 1)	/**< auto desense is enabled */
#define DCTL_FLAGS_MSWITCH	(1 << 2)	/**< mode switching is enabled */
#define DCTL_FLAGS_PWRCTRL	(1 << 3)	/**< Tx power control is enabled */
/* for now AGG on/off is handled separately  */
#define DCTL_FLAGS_TX_AGG_OFF	(1 << 4)	/**< TBD: allow TX agg Off */
#define DCTL_FLAGS_RX_AGG_OFF	(1 << 5)	/**< TBD: allow RX agg Off */
/* used for dry run testing only */
#define DCTL_FLAGS_DRYRUN	(1 << 7)	/**< Enables dynctl dry run mode  */
#define IS_DYNCTL_ON(prof)	((prof->flags & DCTL_FLAGS_DYNCTL) != 0)
#define IS_DESENSE_ON(prof)	((prof->flags & DCTL_FLAGS_DESENSE) != 0)
#define IS_MSWITCH_ON(prof)	((prof->flags & DCTL_FLAGS_MSWITCH) != 0)
#define IS_PWRCTRL_ON(prof)	((prof->flags & DCTL_FLAGS_PWRCTRL) != 0)
/* desense level currently in use */
#define DESENSE_OFF	0
#define DFLT_DESENSE_MID	12
#define DFLT_DESENSE_HIGH	2

/**
 * dynctl data points(a set of btpwr & wlrssi thresholds)
 * for mode & desense switching
 */
typedef struct btc_thr_data {
	int8	mode;	/**< used by desense sw */
	int8	bt_pwr;	/**< BT tx power threshold */
	int8	bt_rssi;	/**< BT rssi threshold */
	/* wl rssi range when mode or desense change may be needed */
	int8	wl_rssi_high;
	int8	wl_rssi_low;
} btc_thr_data_t;

/* dynctl. profile data structure  */
#define DCTL_PROFILE_VER 0x01
#include <packed_section_start.h>
typedef BWL_PRE_PACKED_STRUCT struct  dctl_prof {
	uint8 version;  /**< dynctl profile version */
	/* dynctl profile flags bit:0 - dynctl On, bit:1 dsns On, bit:2 mode sw On, */
	uint8 flags;  /**< bit[6:3] reserved, bit7 - Dryrun (sim) - On */
	/**  wl desense levels to apply */
	uint8	dflt_dsns_level;
	uint8	low_dsns_level;
	uint8	mid_dsns_level;
	uint8	high_dsns_level;
	/** mode switching hysteresis in dBm */
	int8	msw_btrssi_hyster;
	/** default btcoex mode */
	uint8	default_btc_mode;
	/** num of active rows in mode switching table */
	uint8	msw_rows;
	/** num of rows in desense table */
	uint8	dsns_rows;
	/** dynctl mode switching data table  */
	btc_thr_data_t msw_data[DCTL_TROWS_MAX];
	/** dynctl desense switching data table */
	btc_thr_data_t dsns_data[DCTL_TROWS_MAX];
} BWL_POST_PACKED_STRUCT dctl_prof_t;
#include <packed_section_end.h>

/**  dynctl status info */
#include <packed_section_start.h>
typedef BWL_PRE_PACKED_STRUCT struct  dynctl_status {
	uint8 sim_on;	/**< true if simulation is On */
	uint16	bt_pwr_shm; /**< BT per/task power as read from ucode  */
	int8	bt_pwr;		/**< BT pwr extracted & converted to dBm */
	int8	bt_rssi;	/**< BT rssi in dBm */
	int8	wl_rssi;	/**< last wl rssi reading used by btcoex */
	uint8	dsns_level; /**< current desense level */
	uint8	btc_mode;   /**< current btcoex mode */
	/* add more status items if needed,  pad to 4 BB if needed */
} BWL_POST_PACKED_STRUCT dynctl_status_t;
#include <packed_section_end.h>

/**  dynctl simulation (dryrun data) */
#include <packed_section_start.h>
typedef BWL_PRE_PACKED_STRUCT struct  dynctl_sim {
	uint8 sim_on;	/**< simulation mode on/off */
	int8 btpwr;		/**< simulated BT power in dBm */
	int8 btrssi;	/**< simulated BT rssi in dBm */
	int8 wlrssi;	/**< simulated WL rssi in dBm */
} BWL_POST_PACKED_STRUCT dynctl_sim_t;
/* no default structure packing */
#include <packed_section_end.h>

/** PTK key maintained per SCB */
#define RSN_TEMP_ENCR_KEY_LEN 16
typedef struct wpa_ptk {
	uint8 kck[RSN_KCK_LENGTH]; /**< EAPOL-Key Key Confirmation Key (KCK) */
	uint8 kek[RSN_KEK_LENGTH]; /**< EAPOL-Key Key Encryption Key (KEK) */
	uint8 tk1[RSN_TEMP_ENCR_KEY_LEN]; /**< Temporal Key 1 (TK1) */
	uint8 tk2[RSN_TEMP_ENCR_KEY_LEN]; /**< Temporal Key 2 (TK2) */
} wpa_ptk_t;

/** GTK key maintained per SCB */
typedef struct wpa_gtk {
	uint32 idx;
	uint32 key_len;
	uint8  key[DOT11_MAX_KEY_SIZE];
} wpa_gtk_t;

/** FBT Auth Response Data structure */
typedef struct wlc_fbt_auth_resp {
	uint8 macaddr[ETHER_ADDR_LEN]; /**< station mac address */
	uint8 PAD[2];
	uint8 pmk_r1_name[WPA2_PMKID_LEN];
	wpa_ptk_t ptk; /**< pairwise key */
	wpa_gtk_t gtk; /**< group key */
	uint32 ie_len;
	uint8 status;  /**< Status of parsing FBT authentication
					Request in application
					*/
	uint8 ies[BCM_FLEX_ARRAY]; /**< IEs contains MDIE, RSNIE,
					FBTIE (ANonce, SNonce,R0KH-ID, R1KH-ID)
					*/
} wlc_fbt_auth_resp_t;

/** FBT Action Response frame */
typedef struct wlc_fbt_action_resp {
	uint16 version; /**< structure version */
	uint16 length; /**< length of structure */
	uint8 macaddr[ETHER_ADDR_LEN]; /**< station mac address */
	uint8 data_len;  /**< len of ie from Category */
	uint8 data[BCM_FLEX_ARRAY]; /**< data contains category, action, sta address, target ap,
						status code,fbt response frame body
						*/
} wlc_fbt_action_resp_t;

#define MACDBG_PMAC_ADDR_INPUT_MAXNUM 16
#define MACDBG_PMAC_OBJ_TYPE_LEN 8

typedef struct _wl_macdbg_pmac_param_t {
	char type[MACDBG_PMAC_OBJ_TYPE_LEN];
	uint8 step;
	uint8 w_en;
	uint16 num;
	uint32 bitmap;
	uint8 addr_raw;
	uint8 addr_num;
	uint16 addr[MACDBG_PMAC_ADDR_INPUT_MAXNUM];
	uint8 pad0[2];
	uint32 w_val;
} wl_macdbg_pmac_param_t;

/** IOVAR 'svmp_sampcol' parameter. Used to set and read SVMP_SAMPLE_COLLECT's setting */
typedef struct wl_svmp_sampcol_param {
	uint32 version;           /* version */
	uint8  enable;
	uint8  trigger_mode;      /* SVMP_SAMPCOL_TRIGGER */
	uint8  trigger_mode_s[2]; /* SVMP_SAMPCOL_PKTPROC */
	uint8  data_samplerate;   /* SVMP_SAMPCOL_SAMPLERATE */
	uint8  data_sel_phy1;     /* SVMP_SAMPCOL_PHY1MUX */
	uint8  data_sel_rx1;      /* SVMP_SAMPCOL_RX1MUX without iqCompOut */
	uint8  data_sel_dualcap;  /* SVMP_SAMPCOL_RX1MUX */
	uint8  pack_mode;         /* SVMP_SAMPCOL_PACK */
	uint8  pack_order;
	uint8  pack_cfix_fmt;
	uint8  pack_1core_sel;
	uint16 waitcnt;
	uint16 caplen;
	uint32 buff_addr_start;   /* in word-size (2-bytes) */
	uint32 buff_addr_end;     /* note: Tcl in byte-size, HW in vector-size (8-bytes) */
	uint8  int2vasip;
	uint8  PAD;
	uint16 status;
} wl_svmp_sampcol_t;

#define WL_SVMP_SAMPCOL_PARAMS_VERSION	1

enum {
	SVMP_SAMPCOL_TRIGGER_PKTPROC_TRANSITION = 0,
	SVMP_SAMPCOL_TRIGGER_FORCE_IMMEDIATE,
	SVMP_SAMPCOL_TRIGGER_RADAR_DET
};

enum {
	SVMP_SAMPCOL_PHY1MUX_GPIOOUT = 0,
	SVMP_SAMPCOL_PHY1MUX_FFT,
	SVMP_SAMPCOL_PHY1MUX_DBGHX,
	SVMP_SAMPCOL_PHY1MUX_RX1MUX
};

enum {
	SVMP_SAMPCOL_RX1MUX_FARROWOUT = 4,
	SVMP_SAMPCOL_RX1MUX_IQCOMPOUT,
	SVMP_SAMPCOL_RX1MUX_DCFILTEROUT,
	SVMP_SAMPCOL_RX1MUX_RXFILTEROUT,
	SVMP_SAMPCOL_RX1MUX_ACIFILTEROUT
};

enum {
	SVMP_SAMPCOL_SAMPLERATE_1XBW = 0,
	SVMP_SAMPCOL_SAMPLERATE_2XBW
};

enum {
	SVMP_SAMPCOL_PACK_DUALCAP = 0,
	SVMP_SAMPCOL_PACK_4CORE,
	SVMP_SAMPCOL_PACK_2CORE,
	SVMP_SAMPCOL_PACK_1CORE
};

enum {
	SVMP_SAMPCOL_PKTPROC_RESET = 0,
	SVMP_SAMPCOL_PKTPROC_CARRIER_SEARCH,
	SVMP_SAMPCOL_PKTPROC_WAIT_FOR_NB_PWR,
	SVMP_SAMPCOL_PKTPROC_WAIT_FOR_W1_PWR,
	SVMP_SAMPCOL_PKTPROC_WAIT_FOR_W2_PWR,
	SVMP_SAMPCOL_PKTPROC_OFDM_PHY,
	SVMP_SAMPCOL_PKTPROC_TIMING_SEARCH,
	SVMP_SAMPCOL_PKTPROC_CHAN_EST_1,
	SVMP_SAMPCOL_PKTPROC_LEG_SIG_DEC,
	SVMP_SAMPCOL_PKTPROC_SIG_DECODE_1,
	SVMP_SAMPCOL_PKTPROC_SIG_DECODE_2,
	SVMP_SAMPCOL_PKTPROC_HT_AGC,
	SVMP_SAMPCOL_PKTPROC_CHAN_EST_2,
	SVMP_SAMPCOL_PKTPROC_PAY_DECODE,
	SVMP_SAMPCOL_PKTPROC_DSSS_CCK_PHY,
	SVMP_SAMPCOL_PKTPROC_WAIT_ENERGY_DROP,
	SVMP_SAMPCOL_PKTPROC_WAIT_NCLKS,
	SVMP_SAMPCOL_PKTPROC_PAY_DEC_EXT,
	SVMP_SAMPCOL_PKTPROC_SIG_FAIL_DELAY,
	SVMP_SAMPCOL_PKTPROC_RIFS_SEARCH,
	SVMP_SAMPCOL_PKTPROC_BOARD_SWITCH_DIV_SEARCH,
	SVMP_SAMPCOL_PKTPROC_DSSS_CCK_BOARD_SWITCH_DIV_SEARCH,
	SVMP_SAMPCOL_PKTPROC_CHAN_EST_3,
	SVMP_SAMPCOL_PKTPROC_CHAN_EST_4,
	SVMP_SAMPCOL_PKTPROC_FINE_TIMING_SEARCH,
	SVMP_SAMPCOL_PKTPROC_SET_CLIP_GAIN,
	SVMP_SAMPCOL_PKTPROC_NAP,
	SVMP_SAMPCOL_PKTPROC_VHT_SIGA_DEC,
	SVMP_SAMPCOL_PKTPROC_VHT_SIGB_DEC,
	SVMP_SAMPCOL_PKTPROC_PKT_ABORT,
	SVMP_SAMPCOL_PKTPROC_DCCAL
};

/** IOVAR 'svmp_mem' parameter. Used to read/clear svmp memory */
typedef struct svmp_mem {
	uint32 addr;	/**< offset to read svmp memory from vasip base address */
	uint16 len;	/**< length in count of uint16's */
	uint16 val;	/**< set the range of addr/len with a value */
} svmp_mem_t;

/** IOVAR 'mu_rate' parameter. read/set mu rate for upto four users */
#define MU_RATE_CFG_VERSION	1
typedef struct mu_rate {
	uint16	version;	/**< version of the structure as defined by MU_RATE_CFG_VERSION */
	uint16	length;		/**< length of entire structure */
	uint8	auto_rate;	/**< enable/disable auto rate */
	uint8	PAD;
	uint16	rate_user[4];	/**< rate per each of four users, set to -1 for no change */
} mu_rate_t;

/** IOVAR 'mu_group' parameter. Used to set and read MU group recommendation setting */
#define WL_MU_GROUP_AUTO_COMMAND      -1
#define WL_MU_GROUP_PARAMS_VERSION_3   3
#define WL_MU_GROUP_METHOD_NAMELEN    64
#define WL_MU_GROUP_NGROUP_MAX        15
#define WL_MU_GROUP_NUSER_MAX          4
#define WL_MU_GROUP_METHOD_MIN         0
#define WL_MU_GROUP_NUMBER_AUTO_MIN    1
#define WL_MU_GROUP_NUMBER_AUTO_MAX   15
#define WL_MU_GROUP_NUMBER_FORCED_MAX  8
#define WL_MU_GROUP_METHOD_OLD         0
#define WL_MU_GROUP_MODE_AUTO          0
#define WL_MU_GROUP_MODE_FORCED        1
#define WL_MU_GROUP_FORCED_1GROUP      1
#define WL_MU_GROUP_ENTRY_EMPTY       -1
typedef struct mu_group {
	uint32 version;          /* version */
	int16  forced;           /* forced group recommendation */
	int16  forced_group_mcs; /* forced group with mcs */
	int16  forced_group_num; /* forced group number */
	int16  group_option[WL_MU_GROUP_NGROUP_MAX][WL_MU_GROUP_NUSER_MAX];
	                         /* set mode for forced grouping and read mode for auto grouping */
	int16  group_GID[WL_MU_GROUP_NGROUP_MAX];
	int16  group_method;     /* methof for VASIP group recommendation */
	int16  group_number;     /* requested number for VASIP group recommendation */
	int16  auto_group_num;   /* exact number from VASIP group recommendation */
	int8   group_method_name[WL_MU_GROUP_METHOD_NAMELEN];
	uint8  PAD[2];
} mu_group_t;

typedef struct mupkteng_sta {
	struct ether_addr ea;
	uint8  PAD[2];
	int32 nrxchain;
	int32 idx;
} mupkteng_sta_t;

typedef struct mupkteng_client {
	ratespec_t rspec;
	int32 idx;
	int32 flen;
	int32 nframes;
} mupkteng_client_t;

typedef struct mupkteng_tx {
	mupkteng_client_t client[8];
	int32 nclients;
	int32 ntx;
} mupkteng_tx_t;

/*
 * MU Packet engine interface.
 * The following two definitions will go into
 * wlioctl_defs.h
 * when wl utility changes are merged to EAGLE TOB & Trunk
 */

#define WL_MUPKTENG_PER_TX_START		0x10
#define WL_MUPKTENG_PER_TX_STOP		        0x20

/** IOVAR 'mu_policy' parameter. Used to configure MU admission control policies */
#define WL_MU_POLICY_PARAMS_VERSION_1   1
#define WL_MU_POLICY_SCHED_DEFAULT	60
#define WL_MU_POLICY_DISABLED		0
#define WL_MU_POLICY_ENABLED		1
#define WL_MU_POLICY_NRX_MIN		1
#define WL_MU_POLICY_NRX_MAX		2
typedef struct mu_policy {
	uint16 version;
	uint16 length;
	uint32 sched_timer;
	uint32 pfmon;
	uint32 pfmon_gpos;
	uint32 samebw;
	uint32 nrx;
	uint32 max_muclients;
} mu_policy_t;

#define WL_NAN_BAND_STR_SIZE 5       /* sizeof ("auto") */

/** Definitions of different NAN Bands */
/* do not change the order */
enum {
		NAN_BAND_B = 0,
		NAN_BAND_A,
		NAN_BAND_AUTO,
		NAN_BAND_INVALID = 0xFF
};

/* ifdef WL11ULB */
/* ULB Mode configured via "ulb_mode" IOVAR */
enum {
	ULB_MODE_DISABLED = 0,
	ULB_MODE_STD_ALONE_MODE = 1,    /* Standalone ULB Mode */
	ULB_MODE_DYN_MODE = 2,      /* Dynamic ULB Mode */
	/* Add all other enums before this */
	MAX_SUPP_ULB_MODES
};

/* ULB BWs configured via "ulb_bw" IOVAR during Standalone Mode Only.
 * Values of this enumeration are also used to specify 'Current Operational Bandwidth'
 * and 'Primary Operational Bandwidth' sub-fields in 'ULB Operations' field (used in
 * 'ULB Operations' Attribute or 'ULB Mode Switch' Attribute)
 */
typedef enum {
	ULB_BW_DISABLED = 0,
	ULB_BW_10MHZ    = 1,    /* Standalone ULB BW in 10 MHz BW */
	ULB_BW_5MHZ = 2,    /* Standalone ULB BW in 5 MHz BW */
	ULB_BW_2P5MHZ   = 3,    /* Standalone ULB BW in 2.5 MHz BW */
	/* Add all other enums before this */
	MAX_SUPP_ULB_BW
} ulb_bw_type_t;
/* endif WL11ULB */


#define WL_MESH_IOCTL_VERSION_1   1
#define MESH_IOC_BUFSZ            512 /* sufficient ioc buff size for mesh */

#ifdef WLMESH
typedef struct mesh_peer_info_ext {
	mesh_peer_info_t        peer_info;
	uint8   PAD;
	uint16  local_aid;      /* AID generated by *local* to peer */
	uint32  entry_state;    /* see MESH_PEER_ENTRY_STATE_ACTIVE etc; valid
				* ONLY for internal peering requests
				*/
	int8 rssi;
	uint8 PAD;
	struct ether_addr ea; /* peer ea */
} mesh_peer_info_ext_t;

typedef struct mesh_peer_info_dump {
	uint32 buflen;
	uint32 version;
	uint16 count;   /* number of results */
	uint16 remaining; /* remaining rsults */
	mesh_peer_info_ext_t    mpi_ext[BCM_FLEX_ARRAY];
} mesh_peer_info_dump_t;

#define WL_MESH_PEER_RES_FIXED_SIZE (OFFSETOF(mesh_peer_info_dump_t, mpi_ext))
#endif /* WLMESH */

/* container for mesh ioctls & events */
typedef struct wl_mesh_ioc {
	uint16  version;        /* interface command or event version */
	uint16  id;             /* mesh ioctl cmd  ID  */
	uint16  len;            /* total length of all tlv records in data[]  */
	uint16  PAD;            /* pad to be 32 bit aligment */
	uint8   data[];       /* var len payload of bcm_xtlv_t type */
} wl_mesh_ioc_t;

enum wl_mesh_cmds {
	WL_MESH_CMD_ENABLE = 1,
	WL_MESH_CMD_JOIN = 2,
	WL_MESH_CMD_PEER_STATUS = 3,
	WL_MESH_CMD_ADD_ROUTE = 4,
	WL_MESH_CMD_DEL_ROUTE = 5,
	WL_MESH_CMD_ADD_FILTER = 6,
	WL_MESH_CMD_ENAB_AL_METRIC = 7,
	WL_MESH_CMD_START_AUTOPEER = 8
};

enum wl_mesh_cmd_xtlv_id {
	WL_MESH_XTLV_ENABLE = 1,
	WL_MESH_XTLV_JOIN = 2,
	WL_MESH_XTLV_STATUS = 3,
	WL_MESH_XTLV_ADD_ROUTE = 4,
	WL_MESH_XTLV_DEL_ROUTE = 5,
	WL_MESH_XTLV_ADD_FILTER = 6,
	WL_MESH_XTLV_ENAB_AIRLINK = 7,
	WL_MESH_XTLV_START_AUTOPEER = 8
};
/* endif WLMESH */

/* Fast BSS Transition parameter configuration */
#define FBT_PARAM_VERSION_0	0u

typedef struct _wl_fbt_params {
	uint16	version;		/* version of the structure
					* as defined by FBT_PARAM_CURRENT_VERSION
					*/
	uint16	length;			/* length of the entire structure */

	uint16 param_type;		/* type of parameter defined below */
	uint16 param_len;		/* length of the param_value */
	uint8 param_value[BCM_FLEX_ARRAY];		/* variable length */
} wl_fbt_params_t;

#define WL_FBT_PARAM_TYPE_RSNIE			0
#define WL_FBT_PARAM_TYPE_FTIE			0x1
#define WL_FBT_PARAM_TYPE_SNONCE		0x2
#define WL_FBT_PARAM_TYPE_MDE			0x3
#define WL_FBT_PARAM_TYPE_PMK_R0_NAME		0x4
#define WL_FBT_PARAM_TYPE_R0_KHID		0x5
#define WL_FBT_PARAM_TYPE_R1_KHID		0x6
#define WL_FBT_PARAM_TYPE_FIRST_INVALID		0x7

/* Assoc Mgr commands for fine control of assoc */
#define WL_ASSOC_MGR_VERSION_0	0u

typedef struct {
	uint16	version;		/* version of the structure as
					 * defined by WL_ASSOC_MGR_CURRENT_VERSION
					 */
	uint16	length;			/* length of the entire structure */

	uint16 cmd;

	union {				/* the command buffer may end in a simple uint16 params, */
		uint16	params;		/* or may have a variable length set of parameters */
		uint8	data[1];	/* represented by the 'data' member. */
	};
} wl_assoc_mgr_cmd_t;

enum wl_sae_auth_xtlv_id {
	WL_SAE_AUTH_XTLV_CONTAINER	= 0xa1,
	WL_SAE_AUTH_XTLV_BSSID		= 0xa2,
	WL_SAE_AUTH_XTLV_CYCLIC_GROUP	= 0xa3,
	WL_SAE_AUTH_XTLV_SCALAR		= 0xa4,
	WL_SAE_AUTH_XTLV_ELEMENTS	= 0xa5,
	WL_SAE_AUTH_XTLV_ANTI_CLOGGING  = 0xa6,
	WL_SAE_AUTH_XTLV_SEND_CONFIRM	= 0xa7,
	WL_SAE_AUTH_XTLV_CONFIRM	= 0xa8,
	WL_SAE_AUTH_XTLV_STATUS		= 0xa9,
	WL_SAE_AUTH_XTLV_LAST		= 0xac
};

#define WL_ASSOC_MGR_CMD_PAUSE_ON_EVT		0 /* have assoc pause on certain events */
#define WL_ASSOC_MGR_CMD_ABORT_ASSOC		1
#define WL_ASSOC_MGR_CMD_SET_SAE_FRAME		2
#define WL_ASSOC_MGR_CMD_SEND_AUTH		3


#define WL_ASSOC_MGR_PARAMS_EVENT_NONE			0 /* use this to resume as well as clear */
#define WL_ASSOC_MGR_PARAMS_PAUSE_EVENT_AUTH_RESP	1

#define WL_WINVER_STRUCT_VER_1 (1)

typedef struct wl_winver {

	/* Version and length of this structure. Length includes all fields in wl_winver_t */
	uint16 struct_version;
	uint16 struct_length;

	/* Windows operating system version info (Microsoft provided) */
	struct {
		uint32 major_ver;
		uint32 minor_ver;
		uint32 build;
	} os_runtime;

	/* NDIS runtime version (Microsoft provided) */
	struct {
		uint16 major_ver;
		uint16 minor_ver;
	} ndis_runtime;

	/* NDIS Driver version (Broadcom provided) */
	struct {
		uint16 major_ver;
		uint16 minor_ver;
	} ndis_driver;

	/* WDI Upper Edge (UE) Driver version (Microsoft provided) */
	struct {
		uint8 major_ver;
		uint8 minor_ver;
		uint8 suffix;
	} wdi_ue;

	/* WDI Lower Edge (LE) Driver version (Broadcom provided) */
	struct {
		uint8 major_ver;
		uint8 minor_ver;
		uint8 suffix;
	} wdi_le;
	uint8 PAD[2];
} wl_winver_t;

/* defined(WLRCC) || defined(ROAM_CHANNEL_CACHE) */
#define MAX_ROAM_CHANNEL      20
typedef struct {
	int32 n;
	chanspec_t channels[MAX_ROAM_CHANNEL];
} wl_roam_channel_list_t;
/* endif RCC || ROAM_CHANNEL_CACHE */

/* values for IOV_MFP arg */
enum {
	WL_MFP_NONE = 0,
	WL_MFP_CAPABLE,
	WL_MFP_REQUIRED
};

typedef enum {
/* The complete enum definition should be moved to bcmevent.h.
 * When add new, add it to bcmevent.h
 */
#if !defined(WL_CHANSW_REASONS_0TO13_INCLUDED)
	CHANSW_UNKNOWN = 0,	/* channel switch due to unknown reason */
	CHANSW_SCAN = 1,	/* channel switch due to scan */
	CHANSW_PHYCAL = 2,	/* channel switch due to phy calibration */
	CHANSW_INIT = 3,	/* channel set at WLC up time */
	CHANSW_ASSOC = 4,	/* channel switch due to association */
	CHANSW_ROAM = 5,	/* channel switch due to roam */
	CHANSW_MCHAN = 6,	/* channel switch triggered by mchan module */
	CHANSW_IOVAR = 7,	/* channel switch due to IOVAR */
	CHANSW_CSA_DFS = 8,	/* channel switch due to chan switch  announcement from AP */
	CHANSW_APCS = 9,	/* Channel switch from AP channel select module */
	CHANSW_FBT = 11,	/* Channel switch from FBT module for action frame response */
	CHANSW_UPDBW = 12,	/* channel switch at update bandwidth */
	CHANSW_ULB = 13,	/* channel switch at ULB */
#endif	/* WL_CHANSW_REASONS_0TO13_INCLUDED */
	CHANSW_DUMMY		/* dummy entry to avoid empty enum */
} chansw_reason_t;

/*
 * WOWL unassociated mode power svae pattern.
 */
typedef struct wowl_radio_duty_cycle {
	uint16 wake_interval;
	uint16  sleep_interval;
} wowl_radio_duty_cycle_t;

typedef struct nd_ra_ol_limits {
	uint16 version;         /* version of the iovar buffer */
	uint16 type;            /* type of data provided */
	uint16 length;          /* length of the entire structure */
	uint16 PAD;            /* pad union to 4 byte boundary */
	union {
		struct {
			uint16 min_time;         /* seconds, min time for RA offload hold */
			uint16 lifetime_percent;
			/* percent, lifetime percentage for offload hold time */
		} lifetime_relative;
		struct {
			uint16 hold_time;        /* seconds, RA offload hold time */
			uint16 PAD;             /* unused */
		} fixed;
	} limits;
} nd_ra_ol_limits_t;

#define ND_RA_OL_LIMITS_VER 1

/* nd_ra_ol_limits sub-types */
#define ND_RA_OL_LIMITS_REL_TYPE   0     /* relative, percent of RA lifetime */
#define ND_RA_OL_LIMITS_FIXED_TYPE 1     /* fixed time */

/* buffer lengths for the different nd_ra_ol_limits types */
#define ND_RA_OL_LIMITS_REL_TYPE_LEN   12
#define ND_RA_OL_LIMITS_FIXED_TYPE_LEN  10

/*
 * Temperature Throttling control mode
 */
typedef struct wl_temp_control {
	uint8 enable;
	uint8 PAD;
	uint16 control_bit;
} wl_temp_control_t;

/* SensorHub Interworking mode */

#define SHUB_CONTROL_VERSION_1	1u
#define SHUB_CONTROL_LEN	12u

typedef struct {
	uint16  verison;
	uint16  length;
	uint16  cmd;
	uint16  op_mode;
	uint16  interval;
	uint16  enable;
} shub_control_t;

/* WLC_MAJOR_VER <= 5 */
/* Data structures for non-TLV format */

/* Data structures for rsdb caps */
/*
 * The flags field of the rsdb_caps_response is designed to be
 * a Bit Mask. As of now only Bit 0 is used as mentioned below.
 */

/* Bit-0 in flags is used to indicate if the cores can operate synchronously
* i.e either as 2x2 MIMO or 2(1x1 SISO). This is true only for 4349 variants
* 0 - device can operate only in rsdb mode (eg: 4364)
* 1 - device can operate in both rsdb and mimo (eg : 4359 variants)
*/

#define WL_RSDB_CAPS_VER 2
#define SYNCHRONOUS_OPERATION_TRUE	(1 << 0)
#define WL_RSDB_CAPS_FIXED_LEN  OFFSETOF(rsdb_caps_response_t, num_chains)

typedef struct rsdb_caps_response {
	uint8 ver;		/* Version */
	uint8 len;		/* length of this structure excluding ver and len */
	uint8 rsdb;		/* TRUE for rsdb chip */
	uint8 num_of_cores;	/* no of d11 cores */
	uint16 flags;		/* Flags to indicate various capabilities */
	uint8 num_chains[BCM_FLEX_ARRAY];	/* Tx/Rx chains for each core */
} rsdb_caps_response_t;

/* Data structures for rsdb bands */

#define WL_RSDB_BANDS_VER       2
#define WL_RSDB_BANDS_FIXED_LEN  OFFSETOF(rsdb_bands_t, band)

typedef struct rsdb_bands
{
	uint8 ver;
	uint8 len;
	uint16 num_cores;	/* num of D11 cores */
	int16 band[BCM_FLEX_ARRAY];		/* The band operating on each of the d11 cores */
} rsdb_bands_t;

/* rsdb config */

#define WL_RSDB_CONFIG_VER 3
#define ALLOW_SIB_PARALLEL_SCAN	(1 << 0)
#define MAX_BANDS 2

#define WL_RSDB_CONFIG_LEN sizeof(rsdb_config_t)


typedef uint8 rsdb_opmode_t;
typedef uint32 rsdb_flags_t;

typedef enum rsdb_modes {
	WLC_SDB_MODE_NOSDB_MAIN = 1, /* 2X2 or MIMO mode (applicable only for 4355) */
	WLC_SDB_MODE_NOSDB_AUX = 2,
	WLC_SDB_MODE_SDB_MAIN = 3, /* This is RSDB mode(default) applicable only for 4364 */
	WLC_SDB_MODE_SDB_AUX = 4,
	WLC_SDB_MODE_SDB_AUTO = 5, /* Same as WLC_RSDB_MODE_RSDB(1+1) mode above */
} rsdb_modes_t;

typedef struct rsdb_config {
	uint8 ver;
	uint8 len;
	uint16 reserved;
	rsdb_opmode_t non_infra_mode;
	rsdb_opmode_t infra_mode[MAX_BANDS];
	rsdb_flags_t flags[MAX_BANDS];
	rsdb_opmode_t current_mode;   /* Valid only in GET, returns the current mode */
	uint8  PAD[3];
} rsdb_config_t;

/* WLC_MAJOR_VER > =5 */
/* TLV definitions and data structures for rsdb subcmds */

enum wl_rsdb_cmd_ids {
	/* RSDB ioctls */
	WL_RSDB_CMD_VER = 0,
	WL_RSDB_CMD_CAPS = 1,
	WL_RSDB_CMD_BANDS = 2,
	WL_RSDB_CMD_CONFIG = 3,
	/* Add before this !! */
	WL_RSDB_CMD_LAST
};
#define WL_RSDB_IOV_VERSION	0x1

typedef struct rsdb_caps_response_v1 {
	uint8 rsdb;		/* TRUE for rsdb chip */
	uint8 num_of_cores;	/* no of d11 cores */
	uint16 flags;		/* Flags to indicate various capabilities */
	uint8 num_chains[MAX_NUM_D11CORES];	/* Tx/Rx chains for each core */
	uint8 band_cap[MAX_NUM_D11CORES]; /* band cap bitmask per slice */
} rsdb_caps_response_v1_t;

typedef struct rsdb_bands_v1
{
	uint8 num_cores;		/* num of D11 cores */
	uint8 PAD;			/* padding bytes for 4 byte alignment */
	int8 band[MAX_NUM_D11CORES];	/* The band operating on each of the d11 cores */
} rsdb_bands_v1_t;

typedef struct rsdb_config_xtlv {
	rsdb_opmode_t reserved1;	/* Non_infra mode is no more applicable */
	rsdb_opmode_t infra_mode[MAX_BANDS]; /* Target mode for Infra association */
	uint8 PAD;	/* pad bytes for 4 byte alignment */
	rsdb_flags_t  flags[MAX_BANDS];
	rsdb_opmode_t current_mode; /* GET only; has current mode of operation */
	uint8 PAD[3];
} rsdb_config_xtlv_t;

/* Definitions for slot_bss chanseq iovar */
#define WL_SLOT_BSS_VERSION_1 1

/* critical slots max size */
#define WL_SLOTTED_BSS_CS_BMP_CFG_MAX_SZ	128 /* arbitrary */

/* enums for critical slots XTLVS per slice */
typedef enum wl_slotted_bss_cs_xtlv {
	WL_SLOTTED_BSS_XTLV_CS_MAIN	= 0,
	WL_SLOTTED_BSS_XTLV_CS_AUX	= 1,
	WL_SLOTTED_BSS_XTLV_CS_MAX
} wl_sbss_bss_cs_xtlv_t;

enum wl_slotted_bss_cmd_id {
	WL_SLOTTED_BSS_CMD_VER			= 0,
	WL_SLOTTED_BSS_CMD_CHANSEQ		= 1,
	WL_SLOTTED_BSS_CMD_CS_BMP		= 2,	/* critical slots bitmap */
	WL_SLOTTED_BSS_CMD_PERSLICE_CS_BMP	= 3,	/* critical slots per slice */
	WL_SLOTTED_BSS_CMD_FW_CAP		= 4
};

typedef uint16 chan_seq_type_t;
enum chan_seq_type {
	CHAN_SEQ_TYPE_AWDL	= 1,
	CHAN_SEQ_TYPE_SLICE	= 2,
	CHAN_SEQ_TYPE_NAN	= 3, /* NAN avail XTLV */
	CHAN_SEQ_TYPE_NANHO	= 4  /* NANHO channel schedule XTLV */
};

typedef uint8 sched_flag_t;
enum sched_flag {
	NO_SDB_SCHED = 0x1,
	SDB_TDM_SCHED = 0x2,
	SDB_SPLIT_BAND_SCHED = 0x4, /* default mode for 4357 */
	MAIN_ONLY = 0x8,
	AUX_ONLY = 0x10,
	SDB_DUAL_TIME = (MAIN_ONLY | AUX_ONLY),
	NO_SDB_MAIN_ONLY = (NO_SDB_SCHED | MAIN_ONLY), /* default mode for 4364 */
	SDB_TDM_SCHED_MAIN = (SDB_TDM_SCHED | MAIN_ONLY),
	SDB_TDM_SCHED_AUX = (SDB_TDM_SCHED | AUX_ONLY),
	SDB_TDM_SCHED_DUAL_TIME = (SDB_TDM_SCHED | SDB_DUAL_TIME),
	SDB_SPLIT_BAND_SCHED_DUAL_TIME = (SDB_SPLIT_BAND_SCHED | SDB_DUAL_TIME)
};

typedef struct chan_seq_tlv_data {
	uint32 flags;
	uint8 data[1];
} chan_seq_tlv_data_t;

typedef struct chan_seq_tlv {
	chan_seq_type_t type;
	uint16 len;
	chan_seq_tlv_data_t chanseq_data[1];
} chan_seq_tlv_t;

typedef struct sb_channel_sequence {
	sched_flag_t sched_flags; /* (sdb-tdm or sdb-sb or Dual-Time) */
	uint8 num_seq; /* number of chan_seq_tlv following */
	uint16 PAD;
	chan_seq_tlv_t seq[BCM_FLEX_ARRAY];
} sb_channel_sequence_t;

typedef struct slice_chan_seq {
	uint8 slice_index;  /* 0(Main) or 1 (Aux) */
	uint8 num_chanspecs;
	uint8 dur;
	uint8 PAD;
	chanspec_t chanspecs[BCM_FLEX_ARRAY];
} slice_chan_seq_t;

#define SLOT_BSS_SLICE_TYPE_DUR_MAX_RANGE	2u
#define SLOTTED_BSS_AGGR_EN			(1 << 0)    /* Bitmap of mode */
#define SLOTTED_BSS_AGGR_LIMIT_DUR	        (1 << 1)    /* Jira 49554 */
#define SLOTTED_BSS_HE_1024_QAM_SUPPORT		(1 << 2)    /* MCS10-11 Support */
#define SLOTTED_BSS_EHT_4096_QAM_SUPPORT	(1 << 3)    /* EHT support */


#define WL_SLICE_CHAN_SEQ_FIXED_LEN   OFFSETOF(slice_chan_seq_t, chanspecs)
/* Definitions for slotted_bss stats */

#define SBSS_STATS_VERSION_1	1

#define SBSS_MAX_CHAN_STATS 4

typedef struct sbss_core_stats {
	uint32 sb_slot_start;
	uint32 sb_slot_end;
	uint32 sb_slot_skip;
	uint32 mismatch_count;
} sbss_core_stats_t;

typedef struct sbss_chan_stats {
	chanspec_t chanspec;
	uint16 PAD;
	uint32 slot_start;
	uint32 slot_end;
	uint32 slot_skip;
} sbss_chan_stats_t;

typedef struct sbss_stats_v1 {
	uint16 version;
	uint16 length;
	sbss_core_stats_t corestats[MAX_NUM_D11CORES];
	sbss_chan_stats_t sbss_chanstats[MAX_NUM_D11CORES][SBSS_MAX_CHAN_STATS];
} sbss_stats_t;

/* slotted bss critical slots */
typedef struct wl_sbss_cs_bmp_s {
	uint8 bitmap_len;
	uint8 PAD[3];
	uint8 bitmap[];
} wl_sbss_cs_bmp_t;

typedef struct wl_sbss_perslice_cs_bmp_s {
	uint8 num_xtlv;		/* number of XTLVs passsed */
	uint8 pad[3];
	uint8 xtlv_params[];	/* list of XTLVs */
} wl_sbss_perslice_cs_bmp_t;

#define SBSS_BTCOEX_PERSLICE_ENABLED	0u	/* flag bit for btcoex per slice capability */
typedef struct wl_sbss_fw_cap {
	uint32 cap;
} wl_sbss_fw_cap_t;

typedef struct sim_pm_params {
	uint32 enabled;
	uint16 cycle;
	uint16 up;
} sim_pm_params_t;

/* Digital napping status */
#define WL_NAP_STATUS_VERSION_1		1
typedef struct wl_nap_status_v1 {
	uint16 version;			/* structure version  */
	uint16 len;			/* length of returned data */
	uint16 fw_status;		/* bitmask of FW disable reasons */
	uint8  hw_status;		/* bitmask for actual HW state info */
	uint8  slice_index;		/* which slice this represents */
	uint32 total_disable_dur;       /* total time (ms) disabled for fw_status */
} wl_nap_status_v1_t;

/* Bits for fw_status */
#define NAP_DISABLED_HOST		0x0001   /* Host has disabled through nap_enable */
#define NAP_DISABLED_RSSI		0x0002   /* Disabled because of nap_rssi_threshold */
#define NAP_DISABLED_SCAN		0x0004   /* Disabled because of scan */
#define NAP_DISABLED_ASSOC		0x0008   /* Disabled because of association */
#define NAP_DISABLED_LTE		0x0010   /* Disabled because of LTE */
#define NAP_DISABLED_ACI		0x0020   /* Disabled because of ACI mitigation */
#define NAP_DISABLED_SEQ_RANGE		0x0040   /* Disabled during SEQ Ranging */
#define NAP_DISABLED_CHANSWITCH		0x0080   /* Disabled during channel switch */
#define NAP_DISABLED_AZ		        0x0100   /* Disabled during 802.11az ranging */
#define NAP_DISABLED_PHYTS		0x0200   /* Disabled during PHYTS */
#define NAP_DISABLED_CAL		0x0400   /* Disabled during rxiqcal */
#define NAP_DISABLED_SLICE		0x0800   /* Disabled on this slice */

/* Bits for hw_status */
#define NAP_HWCFG			0x01   /* State of NAP config bit in phy HW */
#define NAP_NOCLK			0x80   /* No clock to read HW (e.g. core down) */

/* ifdef WL_NATOE */
#define WL_NATOE_IOCTL_VERSION		1
#define WL_NATOE_IOC_BUFSZ		512	/* sufficient ioc buff size for natoe */
#define WL_NATOE_DBG_STATS_BUFSZ	2048
#define NATOE_FLAGS_ENAB_MASK		0x1
#define NATOE_FLAGS_ACTIVE_MASK		0x2
#define NATOE_FLAGS_PUBNW_MASK		0x4
#define NATOE_FLAGS_PVTNW_MASK		0x8
#define NATOE_FLAGS_ENAB_SHFT_MASK	0
#define NATOE_FLAGS_ACTIVE_SHFT_MASK	1
#define NATOE_FLAGS_PUBNW_SHFT_MASK	2
#define NATOE_FLAGS_PVTNW_SHFT_MASK	3
#define NATOE_FLAGS_PUB_NW_UP	(1 << NATOE_FLAGS_PUBNW_SHFT_MASK)
#define NATOE_FLAGS_PVT_NW_UP	(1 << NATOE_FLAGS_PVTNW_SHFT_MASK)

#define PCIE_FRWDPKT_STATS_VERSION	1

/* Module version is 1 for IGUANA */
#define WL_NATOE_MODULE_VER_1	1
/* Module version is 2 for Lemur */
#define WL_NATOE_MODULE_VER_2	2

/* WL_NATOE_CMD_MOD_VER */
typedef uint16 wl_natoe_ver_t;
/* config natoe STA and AP IP's structure */
typedef struct {
	uint32 sta_ip;
	uint32 sta_netmask;
	uint32 sta_router_ip;
	uint32 sta_dnsip;
	uint32 ap_ip;
	uint32 ap_netmask;
} wl_natoe_config_ips_t;

/* natoe ports config structure */
typedef struct {
	uint16 start_port_num;
	uint16 no_of_ports;
} wl_natoe_ports_config_t;

/* natoe ports exception info */
typedef struct {
	uint16 sta_port_num;
	uint16 dst_port_num;    /* for SIP type protocol, dst_port_num info can be ignored by FW */
	uint32 ip;              /* for SIP ip is APcli_ip and for port clash it is dst_ip */
	uint8  entry_type;      /* Create/Destroy */
	uint8  PAD[3];
} wl_natoe_exception_port_t;

/* container for natoe ioctls & events */
typedef struct wl_natoe_ioc {
	uint16  version;        /* interface command or event version */
	uint16  id;             /* natoe ioctl cmd  ID  */
	uint16  len;            /* total length of all tlv records in data[]  */
	uint16  PAD;            /* pad to be 32 bit aligment */
	uint8   data[];       /* var len payload of bcm_xtlv_t type */
} wl_natoe_ioc_t;

typedef struct wl_natoe_pool_stats_v1 {
	/* For debug purposes */
	uint16 poolreorg_cnt;
	uint16 poolrevert_cnt;
	uint16 txfrag_state;
	uint16 rxfrag_state;
	uint16 txfrag_plen;
	uint16 rxfrag_plen;
	uint16 tx_pavail;
	uint16 rx_pavail;
	uint16 txmin_bkup_bufs;
	uint16 rxmin_bkup_bufs;
	uint16 pktpool_sbuf_alloc;
	uint16 pktpool_plen;
	uint16 pktpool_pavail;
	/* Peak shared buffer count in all iterations */
	uint16 sbuf_peak;
	/* Peak shared buffer count in current D3 iteration */
	uint16 sbuf_peak_cur;
} wl_natoe_pool_stats_v1_t;

typedef struct wl_natoe_arp_entry_v1 {
	struct ipv4_addr ip;
	struct ether_addr mac_addr;
	uint8  lifetime;
	uint8 flags;
} wl_natoe_arp_entry_v1_t;

typedef struct wl_natoe_dbg_arp_tbl_info_v1 {
	uint8 valid_arp_entries;
	uint8 PAD[3];
	wl_natoe_arp_entry_v1_t arp_ent[];
} wl_natoe_dbg_arp_tbl_info_v1_t;

typedef struct wl_natoe_skip_port_entry_v1 {
	struct ipv4_addr srcip;
	uint16 src_port;
	uint16 lifetime;
} wl_natoe_skip_port_entry_v1_t;

typedef struct wl_natoe_skip_port_info_v1 {
	uint8 valid_entries;
	uint8 PAD[3];
	wl_natoe_skip_port_entry_v1_t skip_port_ent[];
} wl_natoe_skip_port_info_v1_t;

typedef struct wl_natoe_dbg_stats_v1 {
	uint16 active_nat_entries;
	uint16 active_dns_entries;
	uint16 active_icmp_entries;
	uint16 valid_arp_entries;
	uint16 prev_nat_entries;
	uint16 prev_dns_entries;
	uint16 tcp_fast_reclaim_cnt;
	uint16 mcast_packets;
	uint16 bcast_packets;
	uint16 port_commands_rcvd;
	uint16 unsupported_prot;
	uint16 arp_req_sent;
	uint16 arp_rsp_rcvd;
	uint16 non_ether_frames;
	uint16 port_alloc_fail;
	uint16 srcip_tbl_full;
	uint16 dstip_tbl_full;
	uint16 nat_tbl_full;
	uint16 icmp_error_cnt;
	uint16 pkt_drops_resource;
	uint32 frwd_nat_pkt_cnt;
	uint32 reverse_nat_pkt_cnt;
	uint16 pub_nw_chspec;
	uint16 pvt_nw_chspec;
	uint8 pubnw_cfg_idx;
	uint8 pvtnw_cfg_idx;
	uint8 pubnw_cfg_ID;
	uint8 pvtnw_cfg_ID;
	uint16 natoe_flags;
	uint16 PAD;
} wl_natoe_dbg_stats_v1_t;

typedef struct wl_natoe_exception_port_inf_v1 {
	uint16 except_bmap_size;
	uint8 port_except_bmap[];
} wl_natoe_exception_port_inf_v1_t;

typedef struct wl_natoe_dstnat_entry_v1 {
	struct ipv4_addr clientip;
	struct ether_addr client_mac_addr;
	uint16 client_listenport;
	uint8 opcode;
	uint8 PAD;
} wl_natoe_dstnat_entry_v1_t;


typedef struct wl_pcie_frwd_stats_v1 {
	uint16 version;
	uint16 len;
	uint16 frwd_txfrag_q_cnt; /* no. of txfrags in frwd_txfrag_list */
	/* no. of outstanding lbufs in txpath on if0/ifx */
	uint16 tx_frwd_n_lb_if0;
	uint16 tx_frwd_n_lb_ifx;
	/* no. of outstanding lfrags in txpath on if0/ifx */
	uint16 tx_frwd_n_lf_if0;
	uint16 tx_frwd_n_lf_ifx;
	/* no. of pending frwd pkts dropped upon d3 entry */
	uint16 tx_frwd_d3_drop_cnt;
	/* Total no. of lbufs frwded in txpath on if0/ifx */
	uint32 tx_frwd_n_lb_if0_cnt;
	uint32 tx_frwd_n_lb_ifx_cnt;
	/* Total no. of lfrags frwded in txpath on if0/ifx */
	uint32 tx_frwd_n_lf_if0_cnt;
	uint32 tx_frwd_n_lf_ifx_cnt;
	uint32 frwd_tx_drop_thr_cnt; /* no. of pkts dropped due to txfrag threshold */
	uint32 frwd_tx_drop_err_cnt; /* no. of pkts dropped due to txfrags not avail / errors */
} wl_pcie_frwd_stats_v1_t;

enum wl_natoe_cmds {
	WL_NATOE_CMD_MOD_VER = 0,
	WL_NATOE_CMD_ENABLE = 1,
	WL_NATOE_CMD_CONFIG_IPS = 2,
	WL_NATOE_CMD_CONFIG_PORTS = 3,
	WL_NATOE_CMD_DBG_STATS = 4,
	WL_NATOE_CMD_EXCEPTION_PORT = 5,
	WL_NATOE_CMD_SKIP_PORT = 6,
	WL_NATOE_CMD_TBL_CNT = 7,
	WL_NATOE_CMD_CONFIG_DSTNAT = 8,
	WL_NATOE_CMD_CTRL = 9
};

enum wl_natoe_cmd_xtlv_id {
	WL_NATOE_XTLV_MOD_VER = 0,
	WL_NATOE_XTLV_ENABLE = 1,
	WL_NATOE_XTLV_CONFIG_IPS = 2,
	WL_NATOE_XTLV_CONFIG_PORTS = 3,
	WL_NATOE_XTLV_DBG_STATS = 4,
	WL_NATOE_XTLV_EXCEPTION_PORT = 5,
	WL_NATOE_XTLV_SKIP_PORT = 6,
	WL_NATOE_XTLV_TBL_CNT = 7,
	WL_NATOE_XTLV_ARP_TBL = 8,
	WL_NATOE_XTLV_POOLREORG = 9,
	WL_NATOE_XTLV_CONFIG_DSTNAT = 10,
	WL_NATOE_XTLV_CTRL = 11
};

/* endif WL_NATOE */

enum wl_idauth_cmd_ids {
	WL_IDAUTH_CMD_CONFIG				= 1,
	WL_IDAUTH_CMD_PEER_INFO				= 2,
	WL_IDAUTH_CMD_COUNTERS				= 3,
	WL_IDAUTH_CMD_LAST
};
enum wl_idauth_xtlv_id {
	WL_IDAUTH_XTLV_AUTH_ENAB			= 0x1,
	WL_IDAUTH_XTLV_GTK_ROTATION			= 0x2,
	WL_IDAUTH_XTLV_EAPOL_COUNT			= 0x3,
	WL_IDAUTH_XTLV_EAPOL_INTRVL			= 0x4,
	WL_IDAUTH_XTLV_BLKLIST_COUNT			= 0x5,
	WL_IDAUTH_XTLV_BLKLIST_AGE			= 0x6,
	WL_IDAUTH_XTLV_PEERS_INFO			= 0x7,
	WL_IDAUTH_XTLV_COUNTERS				= 0x8,
	WL_IDAUTH_XTLV_PTK_ROTATION			= 0x9
};
enum wl_idauth_stats {
	WL_AUTH_PEER_STATE_AUTHORISED			= 0x01,
	WL_AUTH_PEER_STATE_BLACKLISTED			= 0x02,
	WL_AUTH_PEER_STATE_4WAY_HS_ONGOING		= 0x03,
	WL_AUTH_PEER_STATE_LAST
};
typedef struct {
	uint16 state;				/* Peer State: Authorised or Blacklisted */
	struct ether_addr peer_addr;		/* peer Address */
	uint32 blklist_end_time;		/* Time of blacklist end */
} auth_peer_t;
typedef struct wl_idauth_counters {
	uint32 auth_reqs;			/* No of auth req recvd */
	uint32 mic_fail;			/* No of mic fails */
	uint32 four_way_hs_fail;		/* No of 4-way handshake fails */
} wl_idauth_counters_t;

#define WLC_UTRACE_LEN		(1024u * 4u) // default length
#define WLC_UTRACE_LEN_AUX	(1024u * 3u) // reduced length to fit smaller AUX BM
#define WLC_UTRACE_LEN_SC	(1024u * 3u) // reduced length to fit smaller Scan core BM

#define WLC_UTRACE_READ_END 0
#define WLC_UTRACE_MORE_DATA 1

typedef struct wl_utrace_capture_args_v1 {
	uint32 length;
	uint32 flag;
} wl_utrace_capture_args_v1_t;

#define UTRACE_CAPTURE_VER_2	2
typedef struct wl_utrace_capture_args_v2 {
	/* structure control */
	uint16 version;		/**< structure version */
	uint16 length;		/**< length of the response */
	uint32 flag;		/* Indicates if there is more data or not */
} wl_utrace_capture_args_v2_t;

/* Signal read end. */
#define WLC_REGVAL_READ_END 0
/* Signal more data pending. */
#define WLC_REGVAL_MORE_DATA 1
/* Internal read state. */
#define WLC_REGVAL_READ_CONTINUE 2

#define WLC_REGVAL_DUMP_PHYREG 0
#define WLC_REGVAL_DUMP_RADREG 1
#define WLC_REGVAL_DUMP_RFEMREG 2
#define WLC_REGVAL_DUMP_SRCBREG 3

#define PHYREGVAL_CAPTURE_BUFFER_LEN 2048

typedef struct wl_regval_capture_args {
	uint32 control_flag;	/* Carries status information. */
} wl_regval_capture_args_t;

/* XTLV IDs for the Health Check "hc" iovar top level container */
enum {
	WL_HC_XTLV_ID_CAT_HC = 1,		/* category for HC as a whole */
	WL_HC_XTLV_ID_CAT_DATAPATH_TX = 2,	/* Datapath Tx */
	WL_HC_XTLV_ID_CAT_DATAPATH_RX = 3,	/* Datapath Rx */
	WL_HC_XTLV_ID_CAT_SCAN	= 4,		/* Scan */
	WL_HC_XTLV_ID_CAT_EVENTMASK	= 5,		/* Health Check event mask. */
};

/* Health Check: Common XTLV IDs for sub-elements in the top level container
 * Number starts at 0x8000 to be out of the way for category specific IDs.
 */
enum {
	WL_HC_XTLV_ID_ERR       = 0x8000,       /* for sub-command  err return */
	WL_HC_XTLV_ID_IDLIST    = 0x8001,       /* container for uint16 IDs */
};

/* Health Check: Datapath TX IDs */
enum {
	WL_HC_TX_XTLV_ID_VAL_STALL_THRESHOLD   = 1,     /* stall_threshold */
	WL_HC_TX_XTLV_ID_VAL_STALL_SAMPLE_SIZE = 2,     /* stall_sample_size */
	WL_HC_TX_XTLV_ID_VAL_STALL_TIMEOUT     = 3,     /* stall_timeout */
	WL_HC_TX_XTLV_ID_VAL_STALL_FORCE       = 4,     /* stall_force */
	WL_HC_TX_XTLV_ID_VAL_STALL_EXCLUDE     = 5,     /* stall_exclude */
	WL_HC_TX_XTLV_ID_VAL_FC_TIMEOUT        = 6,     /* flow ctl timeout */
	WL_HC_TX_XTLV_ID_VAL_FC_FORCE          = 7,     /* flow ctl force failure */
	WL_HC_TX_XTLV_ID_VAL_DELAY_TO_TRAP     = 8,     /* delay threshold for forced trap */
	WL_HC_TX_XTLV_ID_VAL_DELAY_TO_RPT      = 9,     /* delay threshold for event log report */
	WL_HC_TX_XTLV_ID_VAL_FAILURE_TO_RPT    = 10,	/* threshold for consecutive TX failures */
};

/* Health Check: Datapath RX IDs */
enum {
	WL_HC_RX_XTLV_ID_VAL_DMA_STALL_TIMEOUT = 1,     /* dma_stall_timeout */
	WL_HC_RX_XTLV_ID_VAL_DMA_STALL_FORCE   = 2,     /* dma_stall test trigger */
	WL_HC_RX_XTLV_ID_VAL_STALL_THRESHOLD   = 3,     /* stall_threshold */
	WL_HC_RX_XTLV_ID_VAL_STALL_SAMPLE_SIZE = 4,     /* stall_sample_size */
	WL_HC_RX_XTLV_ID_VAL_STALL_FORCE       = 5,     /* stall test trigger */
	WL_HC_RX_XTLV_ID_VAL_STALL_UC_DECRYPT_FAIL = 6,  /* trigger uc decrypt failures */
	WL_HC_RX_XTLV_ID_VAL_STALL_BCMC_DECRYPT_FAIL = 7, /* trigger bcmc decrypt failures */
	WL_HC_RX_XTLV_ID_VAL_STALL_NOUDATA_TO_RXRTS = 8, /* No data to RX CTS TX CTS */
};

/* Health Check: Datapath SCAN IDs */
enum {
	WL_HC_XTLV_ID_VAL_SCAN_STALL_THRESHOLD	= 1,	/* scan stall threshold */
};

/* Health check: PHY IDs */
/* Needed for iguana 13.35 branch */
typedef enum {
	PHY_HC_DD_ALL		= 0,
	PHY_HC_DD_TEMPSENSE	= 1,
	PHY_HC_DD_VCOCAL	= 2,
	PHY_HC_DD_RX		= 3,
	PHY_HC_DD_TX		= 4,
	PHY_HC_DD_LAST		/* This must be the last entry */
} phy_hc_dd_type_t;

typedef enum {
	PHY_HC_DD_TEMP_FAIL	= 0,
	PHY_HC_DD_VCO_FAIL	= 1,
	PHY_HC_DD_RXDSN_FAIL	= 2,
	PHY_HC_DD_TXPOW_FAIL	= 3,
	PHY_HC_DD_END		/* This must be the last entry */
} phy_hc_dd_type_v2_t;

/* IDs of Health Check report structures for sub types of health checks within WL */
typedef enum wl_hc_dd_type {
	WL_HC_DD_PCIE		= 0,	/* PCIe */
	WL_HC_DD_RX_DMA_STALL	= 1,	/* RX DMA stall check */
	WL_HC_DD_RX_STALL	= 2,	/* RX stall check */
	WL_HC_DD_TX_STALL	= 3,	/* TX stall check */
	WL_HC_DD_SCAN_STALL	= 4,	/* SCAN stall check */
	WL_HC_DD_PHY		= 5,	/* PHY health check */
	WL_HC_DD_REINIT		= 6,	/* Reinit due to other reasons */
	WL_HC_DD_TXQ_STALL	= 7,	/* TXQ stall */
	WL_HC_DD_RX_STALL_V2	= 8,	/* RX stall check v2 */


	WL_HC_DD_SBSS		=10,	/* Slotted bss health check */
	WL_HC_DD_NAN		=11,	/* NAN health check */
	WL_HC_DD_CHSW		=12,	/* Channel Switch health check */
	WL_HC_DD_LHL		=13,	/* LHL timer health check */
	WL_HC_DD_RX_STALL_V3	=14,	/* RX stall check v3 */
	WL_HC_DD_TX_STALL_V2	=15,	/* TX stall check V2 */
	WL_HC_DD_MAX
} wl_hc_dd_type_t;

/* RX stall reason codes sent with wl_rx_hc_info_v2_t */
typedef enum bcm_rx_hc_stall_reason {
	BCM_RX_HC_RESERVED		= 0,
	BCM_RX_HC_UNSPECIFIED		= 1,	/* All other. Catch all */
	BCM_RX_HC_UNICAST_DECRYPT_FAIL	= 2,	/* Unicast decrypt fail */
	BCM_RX_HC_BCMC_DECRYPT_FAIL	= 3,	/* BCMC decrypt fail */
	BCM_RX_HC_UNICAST_REPLAY	= 4,	/* Unicast replay */
	BCM_RX_HC_BCMC_REPLAY		= 5,	/* BCMC replay */
	BCM_RX_HC_AMPDU_DUP		= 6,	/* AMPDU DUP */
	BCM_RX_HC_BCMC_KEYIDMATCH_FAIL	= 7,	/* BCMC decrypt fail due to key index mismatch */
	/* Stall because no unicast data frames for RX RTS TX CTS transaction */
	BCM_RX_HC_NOUDATA_RXRTS_TXCTS = 8,
	BCM_RX_HC_TOO_MANY_SF_IN_AMSDU	= 9,	/* Too many SFs in AMSDU */
	BCM_RX_HC_MAX
} bcm_rx_hc_stall_reason_t;

/*
 * Health Check report structures for sub types of health checks within WL
 */

/* Health Check report structure for Rx DMA Stall check */
typedef struct {
	uint16 type;
	uint16 length;
	uint16 timeout;
	uint16 stalled_dma_bitmap;
} wl_rx_dma_hc_info_t;

/* Health Check report structure for Tx packet failure check */
typedef struct {
	uint16 type;
	uint16 length;
	uint32 stall_bitmap;
	uint32 stall_bitmap1;
	uint32 failure_ac;
	uint32 threshold;
	uint32 tx_all;
	uint32 tx_failure_all;
} wl_tx_hc_info_t;

/* Health Check report v2 structure for Tx packet failure check */
typedef struct {
	uint16 type; /* WL_HC_DD_TX_STALL_V2 */
	uint16 length;
	uint8 if_idx; /* interface index on which issue is reported */
	uint8 ac; /* access category on which this problem is seen */
	uint8 link_idx;
	uint8 PAD;
	uint32 stall_bitmap_low;
	uint32 stall_bitmap_high;
	uint32 tx_all;
	uint32 tx_failure_all;
	uint32 threshold;
	struct ether_addr peer_ea;
	uint8 PAD[2]; /* Reserved */
} wl_tx_hc_info_v2_t;

/* Health Check report structure for Rx dropped packet failure check */
typedef struct {
	uint16 type; /* WL_HC_RX_DD_STALL */
	uint16 length;
	uint32 bsscfg_idx;
	uint32 rx_hc_pkts;
	uint32 rx_hc_dropped_all;
	uint32 rx_hc_alert_th;
} wl_rx_hc_info_t;

/* Health Check report structure for Rx dropped packet failure check */
typedef struct {
	uint16 type; /* WL_HC_RX_DD_STALL_V2 */
	uint16 length;
	uint8 if_idx; /* interface index on which issue is reported */
	uint8 ac; /* access category on which this problem is seen */
	uint8 pad[2]; /* Reserved */
	uint32 rx_hc_pkts;
	uint32 rx_hc_dropped_all;
	uint32 rx_hc_alert_th;
	uint32 reason;	/* refer to bcm_rx_hc_stall_reason_t above */
	struct ether_addr peer_ea;
	uint8 PAD[2]; /* Reserved */
} wl_rx_hc_info_v2_t;

/* Health Check report structure for Rx dropped packet failure check */
typedef struct {
	uint16 type; /* WL_HC_RX_DD_STALL_V3 */
	uint16 length;
	uint8 if_idx; /* interface index on which issue is reported */
	uint8 ac; /* access category on which this problem is seen */
	uint8 link_idx;
	uint8 PAD;
	uint32 rx_hc_pkts;
	uint32 rx_hc_dropped_all;
	uint32 rx_hc_alert_th;
	uint32 reason;	/* refer to bcm_rx_hc_stall_reason_t above */
	struct ether_addr peer_ea;
	uint8 PAD[2]; /* Reserved */
} wl_rx_hc_info_v3_t;

/* HE top level command IDs */
enum {
	WL_HE_CMD_ENAB				= 0u,
	WL_HE_CMD_FEATURES			= 1u,
	WL_HE_CMD_TWT_SETUP			= 2u,
	WL_HE_CMD_TWT_TEARDOWN			= 3u,
	WL_HE_CMD_TWT_INFO			= 4u,
	WL_HE_CMD_BSSCOLOR			= 5u,
	WL_HE_CMD_PARTIAL_BSSCOLOR		= 6u,
	WL_HE_CMD_CAP				= 7u,
	WL_HE_CMD_STAID				= 8u,
	WL_HE_CMD_MUEDCA			= 9u,
	WL_HE_CMD_RTSDURTHRESH			= 10u,
	WL_HE_CMD_PEDURATION			= 11u,
	WL_HE_CMD_TESTBED_MODE			= 12u,
	WL_HE_CMD_OMI_CONFIG			= 13u,
	WL_HE_CMD_OMI_STATUS			= 14u,
	WL_HE_CMD_OMI_ULMU_THROTTLE		= 15u,
	WL_HE_CMD_ULMU_DISABLE_POLICY		= 16u,
	WL_HE_CMD_ULMU_DISABLE_STATS		= 17u,
	WL_HE_CMD_OMI_DLMU_RSD_RCM_MPF_MAP	= 18u,
	WL_HE_CMD_SR_PROHIBIT			= 19u,
	WL_HE_CMD_TRIG_PREFERRED_AC_ENAB	= 20u,
	WL_HE_CMD_ULMU_DISABLE_NS_TXPWR		= 21u,
	WL_HE_CMD_ULMU_FAKE_NS_TXPWR		= 22u,
	WL_HE_CMD_RXMPDU_MAXLEN_4K_ENAB		= 23u,
	WL_HE_CMD_LAST
};

enum {
	WL_HE_MUEDCA_IE		= 0,
	WL_HE_MUEDCA_SHM	= 1,
	WL_HE_MUEDCA_LAST
};

#ifdef WL11AX

/* struct for dump MU EDCA IE/SHM paramters */
typedef struct wl_he_muedca_ie_v1 {
	uint16 version;	/* structure version */
	uint16 length;	/* data length (starting after this field) */
	uint8 mu_qos_info;
	he_mu_ac_param_record_t param_ac[AC_COUNT]; /* 3*4 */
	uint8 PAD;
} wl_he_muedca_ie_v1_t;

typedef wl_he_muedca_ie_v1_t	wl_he_muedca_ie_t;

#define WL_HE_MUEDCA_VER_1	1

#endif /* WL11AX */

/* TWT top level command IDs */
enum {
	WL_TWT_CMD_ENAB			= 0,
	WL_TWT_CMD_SETUP		= 1,
	WL_TWT_CMD_TEARDOWN		= 2,
	WL_TWT_CMD_INFO			= 3,
	WL_TWT_CMD_AUTOSCHED		= 4,
	WL_TWT_CMD_STATS		= 5,
	WL_TWT_CMD_EARLY_TERM_TIME	= 6,
	WL_TWT_CMD_RESP_CONFIG		= 7,
	WL_TWT_CMD_SPPS_ENAB		= 8,
	WL_TWT_CMD_CAP			= 9,
	WL_TWT_CMD_STATUS		= 10,
	WL_TWT_CMD_CONFIG		= 11,
	WL_TWT_CMD_BTWT_CONFIG		= 12,
	WL_TWT_CMD_LAST
};

#define WL_HEB_VER_1	1

/* HEB top level command IDs */
enum {
	WL_HEB_CMD_ENAB			= 0,
	WL_HEB_CMD_NUM_HEB		= 1,
	WL_HEB_CMD_COUNTERS		= 2,
	WL_HEB_CMD_CLEAR_COUNTERS	= 3,
	WL_HEB_CMD_CONFIG		= 4,
	WL_HEB_CMD_STATUS		= 5,
	WL_HEB_CMD_LAST
};

/* HEB counters structures */
typedef struct wl_heb_int_cnt_v1 {
	uint16 pre_event;
	uint16 start_event;
	uint16 end_event;
	uint16 missed;
} wl_heb_int_cnt_v1_t;

typedef struct wl_heb_cnt_v1 {
	/* structure control */
	uint16 version;	/* structure version */
	uint16 length;	/* data length (starting after this field) */
	wl_heb_int_cnt_v1_t heb_int_cnt[BCM_FLEX_ARRAY];
} wl_heb_cnt_v1_t;

// struct for configuring HEB
typedef struct wl_config_heb_fill_v1 {
	uint16 version;	/* structure version */
	uint16 length;	/* data length (starting after this field) */
	uint32 duration;
	uint32 periodicity;
	uint16 heb_idx;
	uint16 preeventtime;
	uint8 count;
	uint8 PAD[3];
} wl_config_heb_fill_v1_t;

typedef struct wl_heb_blk_params_v1 {
	/* Don't change the order of following elements. This is as per the HEB HW spec */
	uint32 event_int_val_l;
	uint32 event_int_val_h;
	uint32 param2;
	uint32 param3;
	uint32 pre_event_intmsk_bmp;
	uint32 start_event_intmsk_bmp;
	uint32 end_event_intmsk_bmp;
	uint32 event_driver_info;
	uint16 param1;
	uint8 event_count;
	uint8 noa_invert;
	uint32 pre_end_event_intmsk_bmp;
} wl_heb_blk_params_v1_t;

typedef struct wl_heb_int_status_v1 {
	uint32 heb_idx;
	wl_heb_blk_params_v1_t blk_params;
} wl_heb_reg_status_v1_t;

typedef struct wl_heb_status_v1 {
	uint16 version;	/* structure version */
	uint16 length;	/* data length (starting after this field) */
	wl_heb_reg_status_v1_t heb_status[BCM_FLEX_ARRAY];
} wl_heb_status_v1_t;


/* HWA */
#define WL_HWA_VER_1	1

/* HWA top level command IDs */
typedef enum wl_hwa_cmd_type {
	WL_HWA_CMD_ENAB		= 0,
	WL_HWA_CMD_CAPS		= 1,
	WL_HWA_CMD_COUNTERS	= 2,
	WL_HWA_CMD_CLRCNTS	= 3,
	WL_HWA_CMD_REGDUMP	= 4,
	WL_HWA_CMD_INDUCE_ERR	= 5,
	WL_HWA_CMD_LAST
} wl_hwa_cmd_type_t;

typedef struct wl_hwa_cnts_info_v1 {
	uint16	cnt_rxs_filter;                  /* #filters added */
	uint16	cnt_rxs_chainable;               /* #rxchainable matched */
} wl_hwa_cnts_info_v1_t;

/* HWA dump info structures */
typedef struct wl_hwa_hwcaps_info_v1 {
	uint16	up;                              /* is hwa init'd/deint'd */
	uint16	corerev;                         /* hwa core revision */
	uint32	submodules_mask;                 /* mask for hwa submodules that are enabled */
} wl_hwa_hwcaps_info_v1_t;

typedef struct wl_hwa_cnts_v1 {
	/* structure control */
	uint16	version;                         /* structure version */
	uint16	length;                          /* data length (starting after this field) */
	wl_hwa_cnts_info_v1_t hwa_cnts_info[];   /* variable length array with hwa counters */
} wl_hwa_cnts_v1_t;

/* All submodules, order is important and define order of initialization. */
/* Not use enumeration here because these defines are also used in macro */
#define	HWA_SUBMODULES_COMMON		0	/**< Common */
#define	HWA_SUBMODULES_TXPOST		1u	/**< TxPost 3a */
#define	HWA_SUBMODULES_RXPOSTFILL	2u	/**< RxPost and Fill 1a/1b */
#define	HWA_SUBMODULES_TXDMA		3u	/**< TxDMA 3b */
#define	HWA_SUBMODULES_TXS		4u	/**< TxStatus 4a */
#define	HWA_SUBMODULES_BUFMGR		5u	/**< Buffer Manager, RX and TX. Do this last */
#define	HWA_SUBMODULES_CPL		6u	/**< Completion 2b/4b */
#define	HWA_SUBMODULES_RXS		7u	/**< RxStatus 2a */
#define	HWA_SUBMODULES_NUM		8u	/**< number of submodules */

#define HWA_SUBMODULES_ALL		0xFF	/* Bitmaps for all submodules */
#ifdef HWA
#define HWA_SUBMODULE_MASK(submodule)	(1u << (submodule))
#else
#define HWA_SUBMODULE_MASK(submodule)	(0)
#endif /* HWA */
/*
 * NOTES:
 * wl_twt_sdesc_t is used to support both broadcast TWT and individual TWT.
 * Value in bit[0:2] in 'flow_id' field is interpreted differently:
 * - flow id for individual TWT (when WL_TWT_FLOW_FLAG_BROADCAST bit is NOT set
 *   in 'flow_flags' field)
 * - flow id as defined in Table 8-248l1 for broadcast TWT (when
 *   WL_TWT_FLOW_FLAG_BROADCAST bit is set)
 * In latter case other bits could be used to differentiate different flows
 * in order to support multiple broadcast TWTs with the same flow id.
 */

/* TWT Setup descriptor */


typedef struct wl_twt_sdesc_v0 {
	/* Setup Command. */
	uint8 setup_cmd;	/* See TWT_SETUP_CMD_XXXX in 802.11ah.h */
	uint8 flow_flags;	/* Flow attributes. See WL_TWT_FLOW_FLAG_XXXX below */
	uint8 flow_id;		/* must be between 0 and 7. Set 0xFF for auto assignment */
	uint8 bid;		/* must be between 0 and 31. Set 0xFF for auto assignment */
	uint8 channel;		/* Twt channel - Not used for now */
	uint8 negotiation_type;	/* Negotiation Type: See macros TWT_NEGO_TYPE_X */
	uint8 frame_recomm;	/* frame recommendation for broadcast TWTs - Not used for now	 */
	uint8 wake_type;	/* See WL_TWT_TIME_TYPE_XXXX below */
	uint32 wake_time_h;	/* target wake time - BSS TSF (us) */
	uint32 wake_time_l;
	uint32 wake_dur;	/* target wake duration in unit of microseconds */
	uint32 wake_int;	/* target wake interval */
	uint32 btwt_persistence;	/* Broadcast TWT Persistence */
	uint32 wake_int_max;	/* max wake interval(uS) for TWT */
	uint8 duty_cycle_min;	/* min duty cycle for TWT(Percentage) */
	uint8 PAD;
	/* deprecated - to be removed */
	uint16 li;
} wl_twt_sdesc_v0_t;

#define WL_TWT_SETUP_DESC_VER	1u
/* TWT Setup descriptor (Version controlled) */
typedef struct wl_twt_sdesc_v1 {
	/* structure control */
	uint16 version;		/* structure version */
	uint16 length;		/* data length (starting after this field) */
	uint8 setup_cmd;	/* See TWT_SETUP_CMD_XXXX in 802.11ah.h */
	uint8 flow_flags;	/* Flow attributes. See WL_TWT_FLOW_FLAG_XXXX below */
	uint8 flow_id;		/* must be between 0 and 7. Set 0xFF for auto assignment */
	uint8 bid;		/* must be between 0 and 31. Set 0xFF for auto assignment */
	uint8 channel;		/* Twt channel - Not used for now */
	uint8 negotiation_type;	/* Negotiation Type: See macros TWT_NEGO_TYPE_X */
	uint8 frame_recomm;	/* frame recommendation for broadcast TWTs - Not used for now	 */
	uint8 wake_type;	/* See WL_TWT_TIME_TYPE_XXXX below */
	uint32 wake_time_h;	/* target wake time - BSS TSF (us) */
	uint32 wake_time_l;
	uint32 wake_dur;	/* target wake duration in unit of microseconds */
	uint32 wake_int;	/* target wake interval */
	uint32 btwt_persistence;	/* Broadcast TWT Persistence */
	uint32 wake_int_max;	/* max wake interval(uS) for TWT */
	uint32 wake_int_min;	/* Min. wake interval allowed for TWT Setup */
	uint32 wake_dur_min;	/* Min. wake duration allowed for TWT Setup */
	uint32 wake_dur_max;	/* Max. wake duration allowed for TWT Setup */
} wl_twt_sdesc_v1_t;

#define WL_TWT_CONFIG_DESC_VER	1u

/* TWT config descriptor */
typedef struct wl_twt_cdesc {
	/* structure control */
	uint16 version;		/* structure version */
	uint16 length;		/* data length (starting after this field) */
	uint8 negotiation_type;	/* Negotiation Type: See macros TWT_NEGO_TYPE_X */
	uint8 configID;		/* TWT Configuration ID */
	uint8 flow_flags;	/* Flow Flags Configuration. See WL_TWT_FLOW_FLAG_XXXX */
	uint8 PAD;
	uint32 wake_time_h;	/* target wake time - BSS TSF (us) */
	uint32 wake_time_l;
	uint32 wake_dur;	/* target wake duration in unit of microseconds */
	uint32 wake_int;	/* target wake interval */
	uint32 wake_int_max;	/* max wake interval(uS) for TWT */
	uint32 wake_int_min;	/* Min. wake interval allowed for TWT Setup */
	uint32 wake_dur_min;	/* Min. wake duration allowed for TWT Setup */
	uint32 wake_dur_max;	/* Max. wake duration allowed for TWT Setup */
	uint32 avg_pkt_num;	/* Average Number of Packets per interval */
	uint32 avg_pkt_size;	/* Average packet size for TWT SP */
} wl_twt_cdesc_t;

/* Flow flags */
#define WL_TWT_FLOW_FLAG_UNANNOUNCED	(1u << 0u)
#define WL_TWT_FLOW_FLAG_TRIGGER	(1u << 1u)
#define WL_TWT_FLOW_FLAG_REQUEST	(1u << 2u)
#define WL_TWT_FLOW_FLAG_PROTECT	(1u << 3u)
#define WL_TWT_FLOW_FLAG_RESPONDER_PM	(1u << 4u)
#define WL_TWT_FLOW_FLAG_UNSOLICITED	(1u << 5u)
#define WL_TWT_FLOW_FLAG_WAKEDUR_UNIT_1ms	(1u << 6u)
#define WL_TWT_FLOW_FLAG_INFO_FRM_DISABLED	(1u << 7u)

/* Deprecated - To be removed */
#define WL_TWT_FLOW_FLAG_BROADCAST	(1u << 5u)
#define WL_TWT_FLOW_FLAG_IMPLICIT	(1u << 7u)

/* Flow id */
#define WL_TWT_FLOW_ID_FID	0x07u	/* flow id */
#define WL_TWT_FLOW_ID_GID_MASK	0x70u	/* group id - broadcast TWT only */
#define WL_TWT_FLOW_ID_GID_SHIFT 4u

#define WL_TWT_INV_BCAST_ID	0xFFu
#define WL_TWT_INV_FLOW_ID	0xFFu
#define WL_TWT_INV_CONFIG_ID	0xFFu
#define WL_TWT_ALL_TWT_CONFIG_ID 0u	/* ConfigID 0 corresponds to All TWT */

#define WL_TWT_INV_WAKE_DUR	0xFFFFFFFFu
#define WL_TWT_INV_WAKE_INT	0xFFFFFFFFu
#define WL_TWT_INV_PKT_NUM	0xFFFFFFFFu
#define WL_TWT_INV_PKT_SIZE	0xFFFFFFFFu
#define WL_TWT_INV_WAKE_TIME	0xFFFFFFFFu

/* auto flow_id */
#define WL_TWT_SETUP_FLOW_ID_AUTO	0xFFu
/* auto broadcast ID */
#define WL_TWT_SETUP_BCAST_ID_AUTO	0xFFu
/* Infinite persistence for broadcast schedule */
#define WL_TWT_INFINITE_BTWT_PERSIST	0xFFFFFFFFu

/* Maximum individual & broadcast TWT supported */
#define WL_TWT_MAX_ITWT	4u
#define WL_TWT_MAX_BTWT	4u

/* should be larger than what chip supports */
#define WL_TWT_STATS_MAX_BTWT	WL_TWT_MAX_BTWT
#define WL_TWT_STATS_MAX_ITWT	WL_TWT_MAX_ITWT

/* TWT States */
#define WL_TWT_INACTIVE		0u	/* Resource is not allotted */
#define WL_TWT_ACTIVE		1u	/* Resource is allotted and HEB is programmed */
#define WL_TWT_SUSPEND		2u	/* Resource is suspended and HEB released */
#define WL_TWT_RESERVED		3u	/* Resource is allotted but HEB is not yet programmed */

/* Wake type */
/* TODO: not yet finalized */
#define WL_TWT_TIME_TYPE_BSS	0u	/* The time specified in wake_time_h/l is
					 * the BSS TSF time.
					 */
#define WL_TWT_TIME_TYPE_OFFSET	1u	/* The time specified in wake_time_h/l is an offset
					 * of the TSF time when the iovar is processed.
					 */
#define WL_TWT_TIME_TYPE_AUTO	2u	/* The target wake time is chosen internally by the FW */


#define WL_TWT_SETUP_VER	0u


/* HE TWT Setup command */
typedef struct wl_twt_setup_v0 {
	/* structure control */
	uint16 version;	/* structure version */
	uint16 length;	/* data length (starting after this field) */
	struct ether_addr peer;	/* Peer address - leave it all 0s' for AP */
	uint8 PAD[2];
	struct wl_twt_sdesc_v0 desc;
	uint16 dialog;		/* Deprecated - to be removed */
	uint8 PAD[2];
} wl_twt_setup_v0_t;

/* HE TWT Setup command */
typedef struct wl_twt_setup_v1 {
	/* structure control */
	uint16 version;	/* structure version */
	uint16 length;	/* data length (starting after this field) */
	struct ether_addr peer;	/* Peer address - leave it all 0s' for AP */
	uint8 PAD[2];
	struct wl_twt_sdesc_v1 desc;
	uint16 dialog;		/* Deprecated - to be removed */
	uint8 PAD[2];
} wl_twt_setup_v1_t;

#define WL_TWT_CONFIG_VER	0u

/*  TWT Config command */
typedef struct wl_twt_config {
	/* structure control */
	uint16 version;		/* structure version */
	uint16 length;		/* data length (starting after this field) */
	struct ether_addr peer;	/* Peer address. leave it all 0s' for AP */
	uint8 PAD[2];
	wl_twt_cdesc_t desc;	/* Config Descriptor */
} wl_twt_config_t;

#define WL_TWT_TEARDOWN_VER	0u

/* twt teardown descriptor */
typedef struct wl_twt_teardesc {
	uint8 negotiation_type;
	uint8 flow_id;		/* must be between 0 and 7 */
	uint8 bid;		/* must be between 0 and 31 */
	bool alltwt;		/* all twt teardown - 0 or 1 */
} wl_twt_teardesc_t;

/* HE TWT Teardown command */
typedef struct wl_twt_teardown {
	/* structure control */
	uint16 version;	/* structure version */
	uint16 length;	/* data length (starting after this field) */
	/* peer address */
	struct ether_addr peer;	/* leave it all 0s' for AP */
	wl_twt_teardesc_t teardesc;	/* Teardown descriptor */

	/* deprecated - to be removed - Start here */
	uint8 flow_flags;
	uint8 flow_id;
	uint8 bid;
	/* deprecated - to be removed - End here */

	uint8 configID;	/* TWT Configuration ID */
} wl_twt_teardown_t;

/* twt information descriptor */
typedef struct wl_twt_infodesc {
	uint8 flow_flags;	/* See WL_TWT_INFO_FLAG_XXX below */
	uint8 flow_id;
	uint8 PAD[2];
	uint32 next_twt_h;
	uint32 next_twt_l;
	/* deprecated - to be removed */
	uint8 wake_type;
	uint8 PAD[3];
} wl_twt_infodesc_t;

/* Flow flags */
#define WL_TWT_INFO_FLAG_ALL_TWT	(1u << 0u)	/* All TWT */
#define WL_TWT_INFO_FLAG_RESUME		(1u << 1u)	/* 1 is TWT Resume, 0 is TWT Suspend */

/* deprecated - to be removed */
#define WL_TWT_INFO_FLAG_RESP_REQ	(1 << 0)	/* Response Requested */
#define WL_TWT_INFO_FLAG_NEXT_TWT_REQ	(1 << 1)	/* Next TWT Request */
#define WL_TWT_INFO_FLAG_BTWT_RESCHED	(1 << 2)	/* Broadcast Reschedule */
typedef wl_twt_infodesc_t wl_twt_idesc_t;

#define WL_TWT_INFO_VER	0u

/* HE TWT Information command */
typedef struct wl_twt_info {
	/* structure control */
	uint16 version;	/* structure version */
	uint16 length;	/* data length (starting after this field) */
	/* peer address */
	struct ether_addr peer;	/* leave it all 0s' for AP */
	uint8 configID;	/* TWT Configuration ID */
	uint8 PAD[1];
	wl_twt_infodesc_t infodesc;	/* information descriptor */
	/* deprecated - to be removed */
	wl_twt_idesc_t desc;
} wl_twt_info_t;

/* wl twt status */
#define WL_TWT_CMD_STATUS_VERSION_1	1u

#define WL_TWT_STATUS_FLAG_SPPS_ENAB		(1u << 0u)
#define WL_TWT_STATUS_FLAG_WAKE_STATE		(1u << 1u)
#define WL_TWT_STATUS_FLAG_WAKE_OVERRIDE	(1u << 2u)

typedef struct wl_twt_status_v0 {
	uint8	state;		/* TWT State */
	uint8	heb_id;		/* HEB ID */
	uint8	configID;		/* TWT Configuration ID */
	uint8	PAD[1];
	struct	ether_addr peer;
	uint8	PAD[2];
	uint32	avg_pkt_num;	/* Average Packet number per TWT SP Interval */
	uint32	avg_pkt_size;	/* Average Packet size for TWT SP */
	struct wl_twt_sdesc_v0 desc;
} wl_twt_status_v0_t;

typedef wl_twt_status_v0_t wl_twt_status_t;

/* wl twt status output */
typedef struct wl_twt_status_v1 {
	uint16	version;
	uint16	length;
	uint8	num_fid;		/* Number of individual TWT setup */
	uint8	num_bid;	/* Number of Broadcast TWT setup */
	uint16	status_flags;	/* see WL_TWT_STATUS_FLAGS_XX */
	wl_twt_status_v0_t itwt_status[WL_TWT_MAX_ITWT];
	wl_twt_status_v0_t btwt_status[WL_TWT_MAX_BTWT];
} wl_twt_status_v1_t;

/* wl twt status command input */
typedef struct wl_twt_status_cmd_v1 {
	uint16	version;
	uint16	length;
	struct	ether_addr peer;
	uint8	configID;
	uint8	PAD;
} wl_twt_status_cmd_v1_t;

#define WL_TWT_PEER_STATS_VERSION_1	1u
typedef struct wl_twt_peer_stats_v1 {
	uint16	version;
	uint16	length;
	struct	ether_addr peer;
	uint8	PAD[2];
	uint8	id;
	uint8	flow_flags;
	uint8	PAD[2];
	uint32	sp_seq;		/* sequence number of the service period */
	uint32	tx_ucast_pkts;
	uint32	tx_pkts_min;
	uint32	tx_pkts_max;
	uint32	tx_pkts_avg;
	uint32	tx_failures;
	uint32	rx_ucast_pkts;
	uint32	rx_pkts_min;
	uint32	rx_pkts_max;
	uint32	rx_pkts_avg;
	uint32	rx_pkts_retried;
} wl_twt_peer_stats_v1_t;

#define WL_TWT_STATS_VERSION_1		1
typedef struct wl_twt_stats_v1 {
	uint16	version;
	uint16	length;
	uint32	num_stats;	/* number of peer stats in the peer_stats_list */
	wl_twt_peer_stats_v1_t	peer_stats_list[];
} wl_twt_stats_v1_t;

#define WL_TWT_PEER_STATS_VERSION_2	2u
typedef struct wl_twt_peer_stats_v2 {
	uint16	version;
	uint16	length;
	struct	ether_addr peer;
	uint8	id;		/* TWT session ID */
	uint8	flow_flags;
	uint8	configID;		/* TWT Configuration ID */
	uint8	PAD[3];
	uint32	sp_seq;		/* sequence number of the service period */
	uint32	tx_ucast_pkts;	/* Number of unicast Tx packets in TWT SPs */
	uint32	tx_pkts_min;	/* Minimum number of Tx packets in a TWT SP */
	uint32	tx_pkts_max;	/* Maximum number of Tx packets in a TWT SP */
	uint32	tx_pkts_avg;	/* Average number of Tx packets in each TWT SP */
	uint32	tx_failures;	/* Tx packets failure count */
	uint32	rx_ucast_pkts;	/* Number of unicast Rx packets in TWT SPs */
	uint32	rx_pkts_min;	/* Minimum number of Rx packets in a TWT SP */
	uint32	rx_pkts_max;	/* Maximum number of Rx packets in a TWT SP */
	uint32	rx_pkts_avg;	/* Average number of Rx packets in each TWT SP */
	uint32	rx_pkts_retried;	/* retried Rx packets count */
	uint32	tx_pkt_sz_avg;	/* Average Tx packet size in TWT SPs */
	uint32	rx_pkt_sz_avg;	/* Average Rx Packet size in TWT SPs */
	uint32	eosp_dur_avg;	/* Average Wake duration in SPs ended due to EOSP */
	uint32	eosp_count;	/* Count of TWT SPs ended due to EOSP */
} wl_twt_peer_stats_v2_t;

#define WL_TWT_STATS_VERSION_2		2u
typedef struct wl_twt_stats_v2 {
	uint16	version;
	uint16	length;
	uint32	num_stats;	/* number of peer stats in the peer_stats_list */
	wl_twt_peer_stats_v2_t	peer_stats_list[];
} wl_twt_stats_v2_t;

#define WL_TWT_STATS_CMD_VERSION_1	1
#define WL_TWT_STATS_CMD_FLAGS_RESET	(1u << 0u)
/* HE TWT stats command */
typedef struct wl_twt_stats_cmd_v1 {
	uint16	version;
	uint16	length;
	struct ether_addr peer;
	uint8	configID;		/* TWT Configuration ID */
	uint8	PAD;
	uint16	flags;		/* see WL_TWT_STATS_CMD_FLAGS */
	uint8	num_fid;
	uint8	num_bid;
	uint8	fid_list[WL_TWT_STATS_MAX_ITWT];
	uint8	bid_list[WL_TWT_STATS_MAX_BTWT];
} wl_twt_stats_cmd_v1_t;

#define WL_TWT_RESP_CFG_VER	0u

#define WL_TWT_CMD_RESP_CFG_TYPE_ALTERNATE	0u
#define WL_TWT_CMD_RESP_CFG_TYPE_DICTATE	1u
/* HE TWT resp command */
typedef struct wl_twt_resp_cfg {
	/* structure control */
	uint16 version;		/* Structure version */
	uint16 length;		/* Data length (starting after this field) */
	uint8 dc_max;		/* Max supported duty cycle for single TWT */
	uint8 resp_type;	/* Resp. type(Alt/dict) if duty cycle>max duty cycle */
	bool twt_resp_enab;	/* TWT Responder Mode enable/disable */
	uint8 PAD;
} wl_twt_resp_cfg_t;

#define WL_TWT_CAP_CMD_VERSION_1	1u

#define WL_TWT_CAP_FLAGS_REQ_SUPPORT	(1u << 0u)
#define WL_TWT_CAP_FLAGS_RESP_SUPPORT	(1u << 1u)
#define WL_TWT_CAP_FLAGS_BTWT_SUPPORT	(1u << 2u)
#define WL_TWT_CAP_FLAGS_FLEX_SUPPORT	(1u << 3u)
#define WL_TWT_CAP_FLAGS_TWT_REQUIRED	(1u << 4u)

/* HE TWT capabilities output */
typedef struct wl_twt_cap {
	uint16	version;
	uint16	length;
	uint16 device_cap;		/* see WL_TWT_CAP_CMD_FLAGS */
	uint16 peer_cap;		/* see WL_TWT_CAP_CMD_FLAGS */
} wl_twt_cap_t;

/* HE TWT capabilities command input */
typedef struct wl_twt_cap_cmd {
	uint16	version;
	uint16	length;
	struct ether_addr peer;
	uint8	PAD[2];
} wl_twt_cap_cmd_t;

#define WL_TWT_BTWT_CONFIG_SCHED_BID0	(1u << 0u) /* Enable/disable BTWT id 0 schedule */
#define WL_TWT_BTWT_TEARDOWN_BID0	(1u << 1u) /* Enable/disable teardown of BTWT
						* with BTWT id 0
						*/

/* EHT sub command IDs */
enum {
	WL_EHT_CMD_ENAB		= 0u,	/* enable/disable EHT feature as a whole */
	WL_EHT_CMD_FEATURES	= 2u,	/* configure EHT sub-features */

	WL_EHT_CMD_SIGMCS	= 3u,	/* configure EHT SIG MCS Index */
	WL_EHT_CMD_NUMLTF	= 4u,	/* configure EHT Number of LTFs */
	WL_EHT_CMD_NONHT_PUNC_PATT	= 5u,	/* configure nonHT puncture pattern */
	WL_EHT_CMD_PUNC_PAT	= 6u,	/* configure per BSS puncture pattern */
	WL_EHT_CMD_TESTBED_MODE	= 7u,	/* EHT Testbed STA Configuration */

	/* Add new sub command IDs here... */

	/* debug/test related sub-commands, mogrify? */
	WL_EHT_CMD_DBG		= 0x1000u,	/* configure EHT debug facilities */
};

/* MLO sub command IDs */
enum {
	WL_MLO_CMD_ENAB			= 0u,	/* enable/disable MLO feature as a whole */
	WL_MLO_CMD_CONFIG		= 1u,	/* configure MLO feature - bsscfg specific */
	WL_MLO_CMD_STATUS		= 2u,	/* status on MLO feature - interface specific */
	WL_MLO_CMD_EMLSR_CTRL		= 3u,	/* emlsr control - interface specific */
	WL_MLO_CMD_TID_MAP		= 4u,	/* configure TID-To-Link Mapping */
	WL_MLO_CMD_CAP			= 5u,	/* capability of MLO feature as a whole */
	WL_MLO_CMD_ACT_LINK_BMAP	= 6u,	/* Set active link for MLO TX and RX */
	WL_MLO_CMD_MULTILINK_ACTIVE	= 7u,	/* Set use of multi links in MLO mode */
	WL_MLO_CMD_LINK_PS_BMAP		= 8u,	/* Modify PS state of a particular link in MLO. */
	WL_MLO_CMD_LINK_DORMANT_BMAP	= 9u,	/* Bitmap to configure dormant state for links */
	WL_MLO_CMD_REC_LINK_BMAP	= 10u,	/* Bitmap to configure recommended links */
	WL_MLO_CMD_CONFIG_PREF		= 11u,	/* Configure mlo mode and band preferences */
	WL_MLO_CMD_MAX_MLO_LINKS	= 12u,	/* set/get max MLO links supported */
	WL_MLO_CMD_FEATURE_EN		= 13u,	/* Enable/Disable a given feature */
	WL_MLO_CMD_NPLINK_CONFIG	= 14u,	/* configure nplink op upon offchannel of plink */
	WL_MLO_CMD_STATS		= 15u,	/* stats on MLO feature */
	/* Add new sub command IDs here... */

	/* debug/test related sub-commands, mogrify? */
	WL_MLO_CMD_MLD_PRB		= 0x1000u,	/* send a mld probe request frame */
	WL_MLO_CMD_TID_MAP_NEG		= 0x1001u,	/* start a TID2Link Mapping negotiation */
	WL_MLO_CMD_MLD_AP_OP		= 0x1002u,	/* add/remove MLD AP(s) to/from MLD */
	WL_MLO_CMD_ML_OP_UPD		= 0x1003u,	/* send a ML Op Upd request frame */
	WL_MLO_CMD_LINK_RECFG_NOTIF	= 0x1004u,	/* send a Link Reconfig notify frame */
	WL_MLO_CMD_LINK_RECFG_REQ	= 0x1005u,	/* send a Link Reconfig request frame */
	WL_MLO_CMD_EPCS_ENAB_REQ	= 0x1006u,	/* send a EPCS Pri Access Req Enab frame */
	WL_MLO_CMD_EPCS_TEARDN		= 0x1007u,	/* send a EPCS Pri Access Teardown frame */
	WL_MLO_CMD_MAX_CST		= 0x1008u,	/* send a MCST IE in the bcn frame */
	WL_MLO_CMD_MLOSIM		= 0x2000u,	/* to set mlo simulation option */
};

/* MLO multilink active modes */
#define	WL_MLO_MULTILINK_DISABLE	0u	/* Proces beacons and data on single link */
#define	WL_MLO_MULTILINK_ML_BCN		1u	/* Process beacons & data on all links */
#define	WL_MLO_MULTILINK_SL_BCN		2u	/* Process beacons on single link & data
						 * on all links
						 */
#define WL_MLO_MULTILINK_ML_BCN_SL_DATA	3u	/* Process beacons on both links and data only
						 * on preferred link
						 */

/* MLO config Flags definition */
#define WL_MLO_USE_FW_GEN_LINKADDR	(1u << 0u) /* fw generates the link addresses */
#define WL_MLO_USE_FW_GEN_MLDADDR	(1u << 1u) /* fw generates the mld address */
#define WL_MLO_UPDATE_CHANNELS		(1u << 2u) /* channels are present in the config req */
#define WL_MLO_LINK_INTERFACES		(1u << 3u) /* link existing interfaces as mlo links */
#define WL_MLO_MLSR_MODE_ENAB		(1u << 4u) /* Enable MLSR mode */

typedef struct wl_mlo_link_config_v1 {
	struct ether_addr link_addr;	/* Link specific address */
	chanspec_t	chspec;		/* Chanspec */
} wl_mlo_link_config_v1_t;

/* MLO modes of operation */
#define MLO_STR			0u	/* STR Mode */
#define MLO_TDM			1u	/* Deprecated over MLO_EMLSR */
#define MLO_EMLSR		1u	/* eMLSR Mode */
#define MLO_AUTO		2u	/* Best possible based on scoring */
#define MLO_MON			3u	/* Monitor Mode */
#define MLO_3LINK		4u	/* 3 Link association (STR+eMLSR) Mode */
#define MLO_1LINK		5u	/* Single Link MLO Mode */
#define MLO_NONML		6u	/* Non-MLO association (legacy) */
#define WL_MLO_MODE_INVALID	0xFFu	/* Invalid mode */

#define WL_MLO_CONFIG_VER_1	1u

/* mlo status structure for an interface */
/* ===================================== */
#define WL_MLO_STATUS_VER_1	1u

typedef struct wl_mlo_config_v1 {
	uint16	version;			/* Structure version */
	uint16	length;				/* Length of structure */
	uint32	flags;				/* Flags */
	uint8	num_links;			/* Number of links */
	uint8	mode;				/* Mode of operation */
	struct ether_addr mld_addr;		/* MLD mac address */
	wl_mlo_link_config_v1_t link_config[];	/* Link addresses based on num_links */
} wl_mlo_config_v1_t;

/* MLO config_pref Flags definition */
#define WL_MLO_UPDATE_MODE_PREF		(1u << 0u) /* Mode is updated in the config req */
#define WL_MLO_UPDATE_BAND_PREF		(1u << 1u) /* Band pref is updated in the config req */
#define WL_MLO_RESET_CONFIG_PREF	(1u << 2u) /* Reset the pref_config to invalid */
#define WL_MLO_STRICT_ORDER_CONFIG_PREF	(1u << 3u) /* Strict preference order in config_pref */

#define WL_MLO_CONFIG_PREF_VER_1	1u

/* Max number of mode_pref which can be given. MLO_MON is NOT valid to give in mode_pref  */
#define MLO_MODE_PREF_MAX_V1		6u

typedef struct wl_mlo_config_pref_v1 {
	uint16	version;			/* Structure version */
	uint16	length;				/* Length of structure */
	uint32	flags;				/* Flags */
	uint8	num_mode_pref;			/* Number of valid modes in mode array */
	uint8	num_band_pref;			/* Number of valid bands in pref_band array */
	uint8	mode_pref[MLO_MODE_PREF_MAX_V1]; /* MLO mode priority order */
	uint8	band_pref[WL_BAND_MAX_CNT];	/* Preferred band priority order */
	uint8	strict_order;			/* Strict preference order */
} wl_mlo_config_pref_v1_t;

#define WL_MLO_FEATURE_EN_VER_1		1u
#define WL_MLO_FEATURE_MASK_VER_1	(WL_MLO_FEA_EMLSR_MASK | WL_MLO_FEA_STR_ON_SCAN_MASK | \
					WL_MLO_FEA_LGCY_SCORE_ML_ASSOC_MASK | \
					WL_MLO_FEA_STR_ON_SB_MASK)

/* Following mask are used in wl_mlo_feature_en_v1_t to find the value of enab field
 * corresponding to a feature. Please update WL_MLO_FEATURE_MASK_VER_1 whenever
 * changing the value of following macros. If a new MACRO is added and that macro is not
 * used by older branches using this IOVAR, then create a new version of mask and use that
 * in the new branch.
 */
#define WL_MLO_FEA_EMLSR_MASK			0x01	/* eMLSR in ML association mask */
#define WL_MLO_FEA_EMLSR_SHIFT			0u	/* Enable eMLSR assoc shift */

#define WL_MLO_FEA_STR_ON_SCAN_MASK		0x06	/* STR mode during scan mask */
#define WL_MLO_FEA_STR_ON_SCAN_SHIFT		1u	/* Enable STR on scan shift */

#define WL_MLO_FEA_LGCY_SCORE_ML_ASSOC_MASK	0x08	/* Legacy scoring for MLO AP mask */
#define WL_MLO_FEA_LGCY_SCORE_ML_ASSOC_SHIFT	3u	/* Legacy scoring for MLO AP shift */

#define WL_MLO_FEA_STR_ON_SB_MASK		0x30	/* STR during Slotted BSS oper mask */
#define WL_MLO_FEA_STR_ON_SB_SHIFT		4u	/* STR during Slotted BSS oper shift */

#define WL_MLO_FEA_EMLSR_DISABLE		0u	/* Disable eMLSR mode */
#define WL_MLO_FEA_EMLSR_ENABLE			1u	/* Enable eMLSR mode */

#define WL_MLO_FEA_STR_ON_SCAN_DISABLE		0u	/* STR operation for scan disabled */
#define WL_MLO_FEA_SLINK_STR_ON_SCAN		1u	/* 1 link STR operation on scan */
#define WL_MLO_FEA_MLINK_STR_ON_SCAN		2u	/* Multi link STR operation on scan */

#define WL_MLO_FEA_LGCY_SCORE_ML_ASSOC_DISABLE	0u	/* Do ML scoring for MLO APs */
#define WL_MLO_FEA_LGCY_SCORE_ML_ASSOC_ENABLE	1u	/* Do 1-link scoring for MLO APS */

#define WL_MLO_FEA_STR_ON_SB_DISABLE		0u	/* STR operation for AWDL/NAN Disabled */
#define WL_MLO_FEA_SLINK_STR_ON_SB		1u	/* 1 link STR operation on  AWDL/NAN */
#define WL_MLO_FEA_MLINK_STR_ON_SB		2u	/* Multi link STR operation on AWDL/NAN */

/* Mask in the following structure defines which bit is valid in the enab field. If a bit in
 * mask field is zero, the corresponding value in enab will be ignored. The definition of
 * the bits are as given in the above defines. Eg if we want to enable STR mode on
 * scan then mask will be 0x02 and enab will be 0x02. To disable the same, make mask
 * as 0x02 and enab as 0x00.
 */
typedef struct wl_mlo_feature_en_v1 {
	uint16	version;			/* Structure version */
	uint16	length;				/* Length of structure */
	uint32	mask;				/* Feature bit Mask */
	uint32	enab;				/* Feature enable bit */
} wl_mlo_feature_en_v1_t;

/* mlo info structure per link */
typedef struct wl_mlo_link_status_v1 {
	uint8			link_id;	/* link ID - AP managed unique # */
	uint8			link_idx;	/* link index - link config idx */
	struct ether_addr	link_addr;	/* Link specific address */
	chanspec_t		chanspec;	/* Chanspec */
} wl_mlo_link_status_v1_t;

/* Note: mlo mode for an interface is "operative" means
 * - mode: value NOT 0. either STR and TDM.
 * - for sta: mlo association; more than 1 link operative
 * - for softap: more than 1 link operative for clients to join
 *	Note: 'operative' is different from active/PM or
 *	link disabled using tid2link mapping
 * mlo mode for an interface is NOT "operative" means
 * - when AP is down OR sta is down
 * - when STA is not assoced OR associated in non mlo mode
 * link_info array is present and has valid values only if mode is "operative"
 */
typedef struct wl_mlo_status_v1 {
	uint16	version;
	uint16	length;
	struct ether_addr	mld_addr;	/* if mode is STR/TDM --> mld_addr
						 * else cfg->curether_addr
						 */
	uint8	mode;				/* Present Mode of operation
						 * - MLO_STR, MLO_TDM
						 */
	uint8	num_links_operative;		/* Number of operative links. see above */
	wl_mlo_link_status_v1_t	link_status[];	/* status on operative links */
} wl_mlo_status_v1_t;

/* ================================================== */
#define WL_MLO_STATUS_VER_2	(2u)
/* peer's info for each link */
typedef struct wl_mlo_link_peer_info_v2 {
	struct ether_addr	link_addr;	/* peer Link specific address */
	struct ether_addr	mld_addr;	/* peer mld address. If this is 0, means
						 * legacy peer
						 */
} wl_mlo_link_peer_info_v2_t;

/* mlo info structure per link */
typedef struct wl_mlo_link_status_v2 {
	uint8			link_id;	/* link ID - AP managed unique # */
	uint8			link_idx;	/* link index - link config idx */
	struct ether_addr	link_addr;	/* Link specific address */
	chanspec_t		chanspec;	/* Chanspec */
	uint8			num_peers;	/* number of peers for this link. For STA, this
						 * will be 1, for AP, can be multiple
						 */
	uint8			link_pwrst;	/* power state of the link */
	wl_mlo_link_peer_info_v2_t	pi[];	/* number of peers for this link */
} wl_mlo_link_status_v2_t;

/* see comments for wl_mlo_status_v1_t */
typedef struct wl_mlo_status_v2 {
	uint16			version;
	uint16			length;
	struct ether_addr	mld_addr;		/* if mode is STR/TDM --> mld_addr
							 * else cfg->curether_addr
							 */
	uint8			mode;			/* Present Mode of operation
							 * - MLO_STR, MLO_TDM
							 */
	uint8			num_links_operative;	/* Number of operative links. see above */
	uint8			link_status[];		/* status on operative links of
							 * type wl_mlo_link_status_v2_t
							 */
} wl_mlo_status_v2_t;

#define WL_MLO_LINK_PWRST_UNKNOWN	0u		/* Uninitialized state */
#define WL_MLO_LINK_PWRST_ACTIVE	1u
#define WL_MLO_LINK_PWRST_PM		2u
#define WL_MLO_LINK_PWRST_DORMANT	3u

/* ================================================== */
/* mlo cap structure
 *
 * max_mlo_links   : value 0 means MLO is unsupported
 * max_str_links   : value 0 means STR is unsupported
 * max_emlsr_links : value 0 means EMLSR is unsupported
 *
 * The number of links is represented as the actual value similar to 'mlo config' IOVar and
 * is not like how it is in the 11be MLO spec (num links - 1).
 */
#define WL_MLO_CAP_VER_1	(1u)

typedef struct wl_mlo_cap_v1 {
	uint16	version;
	uint16  length;
	uint8	max_mlo_links;		/* Maximum number of MLO links supported */
	uint8	max_str_links;		/* Maximum number of STR links supported */
	uint8	max_emlsr_links;	/* Maximum number of EMLSR links supported */
	uint8	PAD;
} wl_mlo_cap_v1_t;

// #ifdef EHT_MAC_SW_TEST
/* IDs list/ID extensions list */
typedef struct wl_mlo_prb_ids_reqs {
	uint8	pad[3];
	/* (Extended) Request parameters (max. 2 entries with max. 14 IDs each) */
	uint8	num_req;		/* # valid reqs */
	struct {
		uint8	id_base;	/* 0 for Request IE; 255 for Extended Request IE */
		uint8	ids_num;	/* # valid IDs */
		uint8	ids_list[14];
	} req[2];
} wl_mlo_prb_ids_reqs_t;

/* Per-STA profile parameters */
typedef struct wl_mlo_prb_psta_prfs {
	uint8	pad[3];
	/* Per-STA Profile parameters (max. 1 entry) */
	uint8	num_prf;		/* # valid prfs */
	struct {
		uint8	link_id;
		uint8	cplt_sta_prf;	/* complete/partial STA profile */
		uint8	pad[2];
		wl_mlo_prb_ids_reqs_t ids;
	} prf[1];
} wl_mlo_prb_psta_prfs_t;

/* MLD probe parameters */
typedef struct wl_probe_mld_parms {
	uint8	flags;			/* see WL_MLO_PRB_FLAG_x */
	uint8	mld_id;			/* spec. MLD ID of this ML prbreq */
	uint8	pad[2];
	wl_mlo_prb_ids_reqs_t ids;	/* Request IDs in reporting STA */
	wl_mlo_prb_psta_prfs_t psta;	/* Per-STA profile for reported STA */
} wl_probe_mld_parms_t;

/* wl_probe_mld_parms_t.flags definitions */
#define WL_MLO_PRB_FLAG_MLD_ID	(1u << 0)	/* mld_id field is present */

/* current mld prb subcommand */
typedef struct wl_mlo_mld_prb_parms {
	wl_probe_params_t	probe;	/* destination parameters */
	wl_probe_mld_parms_t	mld;	/* MLD probe parameters */
} wl_mlo_mld_prb_parms_t;

/* ML Operation Update subcommand */
typedef struct wl_mlo_ml_op_upd {
	uint16	timeout;		/* timeout in millisecs */
	uint8	num_link;		/* # elements in link_id[] */
	uint8	type;			/* deprecated - to be removed */
	uint8	link_id[];		/* link ids */
} wl_mlo_ml_op_upd_t;

/* Link Reconfiguration Notify subcommand */
typedef struct wl_mlo_link_recfg_notif {
	struct ether_addr peer_addr;	/* STA/peer address */
	uint16	timeout;		/* frame tx timeout in millisecs */
	uint8	pad[3];
	uint8	num_link;		/* # elements in link[] */
	struct {
		uint8	id;		/* link id */
		uint8	op;		/* op - EHT_STA_OU_TYPE_x_LINK in 802.11be.h */
	} link[];
} wl_mlo_link_recfg_notif_t;

/* Link Reconfiguration Request subcommand */
typedef struct wl_mlo_link_recfg_req {
	uint16	timeout;		/* req/resp transaction timeout in millisecs */
	uint8	pad[1];
	uint8	num_link;		/* # elements in link[] */
	struct {
		uint8	id;		/* link id */
		uint8	op;		/* op - EHT_STA_OU_TYPE_x_LINK in 802.11be.h */
	} link[];
} wl_mlo_link_recfg_req_t;

/* EPCS Priority Access Request Enable subcommand */
typedef struct wl_mlo_epcs_enab_req {
	struct ether_addr peer_addr;	/* STA/peer address */
	uint16	timeout;		/* timeout in millisecs */
	uint8	pad[1];
	uint8	num_link;		/* # elements in link_id[] */
	uint8	link_id[];		/* link ids */
} wl_mlo_epcs_enab_req_t;

/* EPCS Priority Access Teardown subcommand */
typedef struct wl_mlo_epcs_teardn {
	struct ether_addr peer_addr;	/* STA/peer address */
	uint16	timeout;		/* timeout in millisecs */
} wl_mlo_epcs_teardn_t;
// #endif /* EHT_MAC_SW_TEST */

/* TID-to-Link Mapping negotiation */
typedef struct wl_mlo_tid_map_neg_v1 {
	uint16	version;
	uint16	length;
	uint8	action;			/* EHT_PAF_ACT_TID_MAP_x in 802.11be.h */
	uint8	pad[3];
	uint8	data[];			/* action specific data:
					 * - EHT_PAF_ACT_TID_MAP_REQ: struct ether_addr;
					 */
} wl_mlo_tid_map_neg_v1_t;

/* TID-to-Link mapping */
#define WL_MLO_TID_MAP_VER_1	(1u)

typedef struct wl_mlo_link_tid_map_v1 {
	uint8	link_id;		/* id of the link */
	uint8	tid_map;		/* tids mapped to the link (represented as bit position) */
	uint8	pad[2];
} wl_mlo_link_tid_map_v1_t;

typedef struct wl_mlo_tid_map_v1 {
	uint16	version;
	uint16	length;
	uint8	direction;		/* direction */
	uint8	num_links;		/* number of links */
	uint8	pad[2];
	wl_mlo_link_tid_map_v1_t	link_tid_map[];	/* TID to link mapping */
} wl_mlo_tid_map_v1_t;

#define TID_TO_LINK_MAP_SIZE(tid_map) \
	(OFFSETOF(wl_mlo_tid_map_v1_t, link_tid_map) + \
		(tid_map->num_links * sizeof(wl_mlo_link_tid_map_v1_t)))

#define WL_MLO_TID_MAP_VER_2	(2u)

typedef struct wl_mlo_tid_map_v2 {
	uint16	version;
	uint16	length;
	uint8	control;	/* Control bitmap as defined below:
				 * bits 0-1: direction (0-DL, 1-UL, 2-BOTH)
				 * bits 2-3: number of links
				 * bit 4: trigger TID-to-link mapping negotiation (0/1)
				 */
	uint8	pad[3];
	wl_mlo_link_tid_map_v1_t link_tid_map[];	/* TID map per link */
} wl_mlo_tid_map_v2_t;

#define WL_MLO_TID_MAP_VER_3	(3u)

typedef struct wl_mlo_tid_map_v3 {
	uint16	version;
	uint16	length;
	uint16	control;
	/* Control bitmap definition:
	 * bit 0: default mapping (0/1)
	 * - when set wl_mlo_link_tid_map_v1_t is not incliuded
	 * bits 1-2: direction (0-DL, 1-UL, 2-BOTH)
	 * bits 3-5: number of links (0-7)
	 * bit 6: negotiate(d) TID-to-link mapping
	 * bit 7: advertise(d) TID-to-link mapping
	 * - when set wl_mlo_tid_map_adv_v1_t is included after the control field
	 * bit 8:
	 * set - negotiate preferred mapping if indicated by the AP
	 * get - negotiated mapping is AP's preferred
	 * bit 9: configure TID-to-link mapping (0/1)
	 * - when set wl_mlo_tid_map_cfg_v1_t is included after the control field
	 */
	uint8	status;	/* status of the TID-to-link mapping (wl_mlo_tid_map_sts) */
	uint8	pad[1];
	/* optional params based on the control flags */
	uint8	params[];
	/* wl_mlo_tid_map_adv_v1_t	Advertised TID-to-link mapping params */
	/* wl_mlo_tid_map_cfg_v1_t	TID-to-link mapping config */
	/* wl_mlo_link_tid_map_v1_t	TID map per link[] */
} wl_mlo_tid_map_v3_t;

/* TID-to-link mapping status */
enum wl_mlo_tid_map_sts {
	WL_MLO_TID_MAP_STS_IDLE	= 0,
	WL_MLO_TID_MAP_STS_EST	= 1,
	WL_MLO_TID_MAP_STS_FAIL	= 2,
	WL_MLO_TID_MAP_STS_PREF	= 3,
	WL_MLO_TID_MAP_STS_DENY	= 4
};

/* MLO tid_map control definitions */
#define WL_MLO_TID_MAP_DEF_MASK			(0x1u)
#define WL_MLO_TID_MAP_DEF_SHIFT		(0u)
#define WL_MLO_TID_MAP_DIR_MASK			(0x06u)
#define WL_MLO_TID_MAP_DIR_SHIFT		(1u)
#define WL_MLO_TID_MAP_LINK_NUM_MASK		(0x38u)
#define WL_MLO_TID_MAP_LINK_NUM_SHIFT		(3u)
#define WL_MLO_TID_MAP_NEG_MASK			(0x40u)
#define WL_MLO_TID_MAP_NEG_SHIFT		(6u)
#define WL_MLO_TID_MAP_ADV_MASK			(0x80u)
#define WL_MLO_TID_MAP_ADV_SHIFT		(7u)
#define WL_MLO_TID_MAP_NEG_PREF_MASK		(0x100u)
#define WL_MLO_TID_MAP_NEG_PREF_SHIFT		(8u)
#define WL_MLO_TID_MAP_CFG_MASK			(0x200u)
#define WL_MLO_TID_MAP_CFG_SHIFT		(9u)

#define WL_MLO_TID_MAP_DEF_GET(tidmap) \
	((tidmap)->control & WL_MLO_TID_MAP_DEF_MASK)

#define WL_MLO_TID_MAP_DEF_SET(tidmap, val) \
	((tidmap)->control = ((tidmap)->control & ~WL_MLO_TID_MAP_DEF_MASK) | \
		((val) << WL_MLO_TID_MAP_DEF_SHIFT))

#define WL_MLO_TID_MAP_DIR_GET(tidmap) \
	(((tidmap)->control & WL_MLO_TID_MAP_DIR_MASK) >> \
		WL_MLO_TID_MAP_DIR_SHIFT)

#define WL_MLO_TID_MAP_DIR_SET(tidmap, val) \
	((tidmap)->control = ((tidmap)->control & ~WL_MLO_TID_MAP_DIR_MASK) | \
		((val) << WL_MLO_TID_MAP_DIR_SHIFT))

#define WL_MLO_TID_MAP_LINK_NUM_GET(tidmap) \
	(((tidmap)->control & WL_MLO_TID_MAP_LINK_NUM_MASK) >> \
		WL_MLO_TID_MAP_LINK_NUM_SHIFT)

#define WL_MLO_TID_MAP_LINK_NUM_SET(tidmap, val) \
	((tidmap)->control = ((tidmap)->control & ~WL_MLO_TID_MAP_LINK_NUM_MASK) | \
		((val) << WL_MLO_TID_MAP_LINK_NUM_SHIFT))

#define WL_MLO_TID_MAP_NEG_GET(tidmap) \
	(((tidmap)->control & WL_MLO_TID_MAP_NEG_MASK) >> \
		WL_MLO_TID_MAP_NEG_SHIFT)

#define WL_MLO_TID_MAP_NEG_SET(tidmap, val) \
	((tidmap)->control = ((tidmap)->control & ~WL_MLO_TID_MAP_NEG_MASK) | \
		((val) << WL_MLO_TID_MAP_NEG_SHIFT))

#define WL_MLO_TID_MAP_ADV_GET(tidmap) \
	(((tidmap)->control & WL_MLO_TID_MAP_ADV_MASK) >> \
		WL_MLO_TID_MAP_ADV_SHIFT)

#define WL_MLO_TID_MAP_ADV_SET(tidmap, val) \
	((tidmap)->control = ((tidmap)->control & ~WL_MLO_TID_MAP_ADV_MASK) | \
		((val) << WL_MLO_TID_MAP_ADV_SHIFT))

#define WL_MLO_TID_MAP_NEG_PREF_GET(tidmap) \
	(((tidmap)->control & WL_MLO_TID_MAP_NEG_PREF_MASK) >> \
		WL_MLO_TID_MAP_NEG_PREF_SHIFT)

#define WL_MLO_TID_MAP_NEG_PREF_SET(tidmap, val) \
	((tidmap)->control = ((tidmap)->control & ~WL_MLO_TID_MAP_NEG_PREF_MASK) | \
		((val) << WL_MLO_TID_MAP_NEG_PREF_SHIFT))

#define WL_MLO_TID_MAP_CFG_GET(tidmap) \
	(((tidmap)->control & WL_MLO_TID_MAP_CFG_MASK) >> \
		WL_MLO_TID_MAP_CFG_SHIFT)

#define WL_MLO_TID_MAP_CFG_SET(tidmap, val) \
	((tidmap)->control = ((tidmap)->control & ~WL_MLO_TID_MAP_CFG_MASK) | \
		((val) << WL_MLO_TID_MAP_CFG_SHIFT))

/* TID-to-link mapping configuration */
typedef struct wl_mlo_tid_map_cfg_v1 {
	uint32	flags;	/* configuration flags */
} wl_mlo_tid_map_cfg_v1_t;

/* TID-To-Link Mapping Negotiation Supported */
#define WL_MLO_TID_MAP_CFG_NEG_SUPPORT_MASK		(0x03u)

#define WL_MLO_TID_MAP_CFG_NEG_SUPPORT_GET(tidmapcfg) \
	((tidmapcfg)->flags & WL_MLO_TID_MAP_CFG_NEG_SUPPORT_MASK)

#define WL_MLO_TID_MAP_CFG_NEG_SUPPORT_SET(tidmapcfg, val) \
	((tidmapcfg)->flags = ((tidmapcfg)->flags & ~WL_MLO_TID_MAP_CFG_NEG_SUPPORT_MASK) | \
		(val))

typedef struct wl_mlo_tid_map_adv_v1 {
	uint16	bcn_cnt;	/* num of beacons until TID-to-link mapping establishment  */
	uint8	pad[2];
	uint32	duration;	/* duration in ms the established mapping will be effective */
} wl_mlo_tid_map_adv_v1_t;

#define WL_MLO_TID_MAP_SIZE_V3(tidmap) \
	(OFFSETOF(wl_mlo_tid_map_v3_t, params) + \
	(WL_MLO_TID_MAP_ADV_GET(tidmap) ? sizeof(wl_mlo_tid_map_adv_v1_t) : 0u) + \
	(WL_MLO_TID_MAP_CFG_GET(tidmap) ? sizeof(wl_mlo_tid_map_cfg_v1_t) : 0u) + \
	(sizeof(wl_mlo_link_tid_map_v1_t) * WL_MLO_TID_MAP_LINK_NUM_GET(tidmap)))

#define WL_MLO_TID_MAP_ADV_OFFSET_V3(tidmap) \
	(OFFSETOF(wl_mlo_tid_map_v3_t, params))

#define WL_MLO_TID_MAP_CFG_OFFSET_V3(tidmap) \
	(OFFSETOF(wl_mlo_tid_map_v3_t, params) + \
	(WL_MLO_TID_MAP_ADV_GET(tidmap) ? sizeof(wl_mlo_tid_map_adv_v1_t) : 0u))

#define WL_MLO_LINK_TID_MAP_OFFSET_V3(tidmap) \
	(OFFSETOF(wl_mlo_tid_map_v3_t, params) + \
	(WL_MLO_TID_MAP_ADV_GET(tidmap) ? sizeof(wl_mlo_tid_map_adv_v1_t) : 0u) + \
	(WL_MLO_TID_MAP_CFG_GET(tidmap) ? sizeof(wl_mlo_tid_map_cfg_v1_t) : 0u))

/* MLO emlsr ctrl shift definition */
#define WL_MLO_EMLSR_CTRL_FLAGS_MURTS_EN_SHIFT		(0u)
#define WL_MLO_EMLSR_CTRL_FLAGS_SWITCH_EN_SHIFT		(1u)
#define WL_MLO_EMLSR_CTRL_FLAGS_GPIO_CFG_SHIFT		(2u)
#define WL_MLO_EMLSR_CTRL_FLAGS_LINK_PRIO_CFG_SHIFT	(3u)
#define WL_MLO_EMLSR_CTRL_FLAGS_PAD_DLY_SHIFT		(4u)
#define WL_MLO_EMLSR_CTRL_FLAGS_TRANS_DLY_SHIFT		(5u)
#define WL_MLO_EMLSR_CTRL_FLAGS_EMLOMN_LINKID_SHIFT	(6u)
#define WL_MLO_EMLSR_CTRL_FLAGS_SM_PRINT_EN_SHIFT	(7u)

/* MLO emlsr ctrl Flags definition */
#define WL_MLO_EMLSR_CTRL_FLAGS_MURTS_EN	(1u << WL_MLO_EMLSR_CTRL_FLAGS_MURTS_EN_SHIFT)
#define WL_MLO_EMLSR_CTRL_FLAGS_SWITCH_EN	(1u << WL_MLO_EMLSR_CTRL_FLAGS_SWITCH_EN_SHIFT)
#define WL_MLO_EMLSR_CTRL_FLAGS_GPIO_CFG	(1u << WL_MLO_EMLSR_CTRL_FLAGS_GPIO_CFG_SHIFT)
#define WL_MLO_EMLSR_CTRL_FLAGS_LINK_PRIO_EN\
	(1u << WL_MLO_EMLSR_CTRL_FLAGS_LINK_PRIO_CFG_SHIFT)
#define WL_MLO_EMLSR_CTRL_FLAGS_PAD_DLY		(1u << WL_MLO_EMLSR_CTRL_FLAGS_PAD_DLY_SHIFT)
#define WL_MLO_EMLSR_CTRL_FLAGS_TRANS_DLY	(1u << WL_MLO_EMLSR_CTRL_FLAGS_TRANS_DLY_SHIFT)
#define WL_MLO_EMLSR_CTRL_FLAGS_EMLOMN_LINKID	(1u << WL_MLO_EMLSR_CTRL_FLAGS_EMLOMN_LINKID_SHIFT)
#define WL_MLO_EMLSR_CTRL_FLAGS_SM_PRINT_EN	(1u << WL_MLO_EMLSR_CTRL_FLAGS_SM_PRINT_EN_SHIFT)

/* setting values in params: note: reusing some of the flags */
#define WL_MLO_EMLSR_CTRL_SET_MURTS_EN(val, en)			\
	(en) ? (val |= WL_MLO_EMLSR_CTRL_FLAGS_MURTS_EN) :	\
	(val &= ~WL_MLO_EMLSR_CTRL_FLAGS_MURTS_EN)
#define WL_MLO_EMLSR_CTRL_SET_SWITCH_EN(val, en)		\
	(en) ? (val |= WL_MLO_EMLSR_CTRL_FLAGS_SWITCH_EN) :	\
	(val &= ~WL_MLO_EMLSR_CTRL_FLAGS_SWITCH_EN)
#define WL_MLO_EMLSR_CTRL_SET_EMLOMN_LINKID(val, en)		\
	(en) ? (val |= WL_MLO_EMLSR_CTRL_FLAGS_EMLOMN_LINKID) :	\
	(val &= ~WL_MLO_EMLSR_CTRL_FLAGS_EMLOMN_LINKID)

/* Note: flags reused here for checking params also. DONOT use for mac addr */
#define WL_MLO_EMLSR_CTRL_IS_SET(val, flag)	(val & flag) ? 1u : 0u

#define WL_MLO_EMLSR_CTRL_VER_1	(1u)

/* Bits for fw_status and disable/enable request */
#define EMLSR_LINK_PRIO_REQ_HOST	(1 << 0u) /* Disabled via HOST IOVAR */
#define EMLSR_LINK_PRIO_REQ_LOW_LAT	(1 << 1u) /* Disabled due to Low latency mode */

typedef struct wl_mlo_emlsr_ctrl_v1 {
	uint16	version;
	uint16	length;
	uint32	flags;		/* Flags to enable/disable */
	uint32	params;		/* params corresponding to flags */
	uint16	murts_ctrl;	/* murts_ctrl value if murts_en is 1 */
	int8	link_prio_en;	/* EMLSR link priority feature enable if 1 */
	uint8	pad_dly;	/* EMLSR mac padding delay */
	uint32	gpio_cfg;	/* GPIO pin numbers used in EMLSR emulation */
	uint8	trans_dly;	/* EMLSR transition delay */
	uint8	emlomn_linkid;	/* link to be used for EML OMN tx */
	int8	sm_print_en;	/* EMLSR SM print enable */
	uint8	PAD;
} wl_mlo_emlsr_ctrl_v1_t;

/* WL_MLO_CMD_MLD_AP_OP opcode */
#define WL_MLO_CMD_MLD_AP_OP_DEL	0u
#define WL_MLO_CMD_MLD_AP_OP_ADD	1u

/* MLD AP operation parameters */
typedef struct wl_mlo_mld_ap_op {
	uint8	op_code;		/* WL_MLO_CMD_MLD_AP_OP_ADD/DEL */
	uint8	link_id;
	uint16	del_timer;		/* delete timer in units of TBTT */
} wl_mlo_mld_ap_op_t;

#define WL_MLO_NPLINK_CONFIG_VER_1	1u

typedef struct wl_mlo_nplink_config_v1 {
	uint16	version;			/* Structure version */
	uint16	length;				/* Length of structure */
	/* rssi threshold to determine the usage of nplink during offchannel of plink */
	int8	rssi_threshold;
	/* nplink psr threshold to transmit the onward pkts on nplink */
	uint8	tx_block_threshold;
	uint8	PAD[2];
} wl_mlo_nplink_config_v1_t;

/* Current version for wlc_clm_power_limits_req_t structure and flags */
#define WLC_CLM_POWER_LIMITS_REQ_VERSION_1	 1
/* "clm_power_limits" iovar request structure */
typedef struct wlc_clm_power_limits_req {
	/* Input. Structure and flags version */
	uint32 version;
	/* Full length of buffer (includes this structure and space for TLV-encoded PPR) */
	uint32 buflen;
	/* Input. Flags (see WLC_CLM_POWER_LIMITS_INPUT_FLAG_... below) */
	uint32 input_flags;
	/* Input. CC of region whose data is being requested */
	char cc[WLC_CNTRY_BUF_SZ];
	/* Input. Channel/subchannel in chanspec_t format */
	uint32 chanspec;
	/* Subchannel encoded as clm_limits_type_t */
	uint32 clm_subchannel;
	/* Input. 0-based antenna index */
	uint32 antenna_idx;
	/* Output. General flags (see WLC_CLM_POWER_LIMITS_OUTPUT_FLAG_... below) */
	uint32 output_flags;
	/* Output. 2.4G country flags, encoded as clm_flags_t enum */
	uint32 clm_country_flags_2g;
	/* Output. 5G country flags, encoded as clm_flags_t enum */
	uint32 clm_country_flags_5g;
	/* Output. Length of TLV-encoded PPR data that follows this structure */
	uint32 ppr_tlv_size;
	/* Output. Beginning of buffer for TLV-encoded PPR data */
	uint8 ppr_tlv[BCM_FLEX_ARRAY];
} wlc_clm_power_limits_req_t;

/* Input. Do not apply SAR limits */
#define WLC_CLM_POWER_LIMITS_INPUT_FLAG_NO_SAR				0x00000001
/* Input. Do not apply board limits */
#define WLC_CLM_POWER_LIMITS_INPUT_FLAG_NO_BOARD			0x00000002
/* Output. Limits taken from product-specific country data */
#define WLC_CLM_POWER_LIMITS_OUTPUT_FLAG_PRODUCT_LIMITS			0x00000001
/* Output. Limits taken from product-specific worldwide data */
#define WLC_CLM_POWER_LIMITS_OUTPUT_FLAG_WORLDWIDE_LIMITS		0x00000002
/* Output. Limits taken from country-default (all-product) data */
#define WLC_CLM_POWER_LIMITS_OUTPUT_FLAG_DEFAULT_COUNTRY_LIMITS		0x00000004

#define WL_MBO_IOV_MAJOR_VER_SHIFT 8
#define WL_MBO_IOV_MAJOR_VER_1 1u
#define WL_MBO_IOV_MINOR_VER_1 1u

#define WL_MBO_IOV_VERSION_1_1 \
	((WL_MBO_IOV_MAJOR_VER_1 << WL_MBO_IOV_MAJOR_VER_SHIFT) | WL_MBO_IOV_MINOR_VER_1)

#define MBO_MAX_CHAN_PREF_ENTRIES  16

enum wl_mbo_cmd_ids {
	WL_MBO_CMD_ADD_CHAN_PREF = 1,
	WL_MBO_CMD_DEL_CHAN_PREF = 2,
	WL_MBO_CMD_LIST_CHAN_PREF = 3,
	WL_MBO_CMD_CELLULAR_DATA_CAP = 4,
	WL_MBO_CMD_DUMP_COUNTERS = 5,
	WL_MBO_CMD_CLEAR_COUNTERS = 6,
	WL_MBO_CMD_FORCE_ASSOC = 7,
	WL_MBO_CMD_BSSTRANS_REJECT = 8,
	WL_MBO_CMD_SEND_NOTIF = 9,
	/* Unused command, This enum no can be use
	 * for next new command
	 */
	WL_MBO_CMD_CLEAR_CHAN_PREF = 10,
	WL_MBO_CMD_NBR_INFO_CACHE = 11,
	WL_MBO_CMD_ANQPO_SUPPORT = 12,
	WL_MBO_CMD_DBG_EVENT_CHECK = 13,
	WL_MBO_CMD_EVENT_MASK = 14,
	/* Add before this !! */
	WL_MBO_CMD_LAST
};

enum wl_mbo_xtlv_id {
	WL_MBO_XTLV_OPCLASS            = 0x1,
	WL_MBO_XTLV_CHAN               = 0x2,
	WL_MBO_XTLV_PREFERENCE         = 0x3,
	WL_MBO_XTLV_REASON_CODE        = 0x4,
	WL_MBO_XTLV_CELL_DATA_CAP      = 0x5,
	WL_MBO_XTLV_COUNTERS           = 0x6,
	WL_MBO_XTLV_ENABLE             = 0x7,
	WL_MBO_XTLV_SUB_ELEM_TYPE      = 0x8,
	WL_MBO_XTLV_BTQ_TRIG_START_OFFSET = 0x9,
	WL_MBO_XTLV_BTQ_TRIG_RSSI_DELTA = 0xa,
	WL_MBO_XTLV_ANQP_CELL_SUPP      = 0xb,
	WL_MBO_XTLV_BIT_MASK		= 0xc
};

/* event bit mask flags for MBO */
#define MBO_EVT_BIT_MASK_CELLULAR_SWITCH        0x0001  /* Evt bit mask to enab cellular switch */
#define MBO_EVT_BIT_MASK_BTM_REQ_RCVD           0x0002  /* Evt bit mask to enab BTM req rcvd */

typedef struct wl_mbo_counters {
	/* No of transition req recvd */
	uint16 trans_req_rcvd;
	/* No of transition req with disassoc imminent */
	uint16 trans_req_disassoc;
	/* No of transition req with BSS Termination */
	uint16 trans_req_bss_term;
	/* No of trans req w/ unspecified reason */
	uint16 trans_resn_unspec;
	/* No of trans req w/ reason frame loss */
	uint16 trans_resn_frm_loss;
	/* No of trans req w/ reason traffic delay */
	uint16 trans_resn_traffic_delay;
	/* No of trans req w/ reason insufficient buffer */
	uint16 trans_resn_insuff_bw;
	/* No of trans req w/ reason load balance */
	uint16 trans_resn_load_bal;
	/* No of trans req w/ reason low rssi */
	uint16 trans_resn_low_rssi;
	/* No of trans req w/ reason excessive retransmission */
	uint16 trans_resn_xcess_retransmn;
	/* No of trans req w/ reason gray zone */
	uint16 trans_resn_gray_zone;
	/* No of trans req w/ reason switch to premium AP */
	uint16 trans_resn_prem_ap_sw;
	/* No of transition rejection sent */
	uint16 trans_rejn_sent;
	/* No of trans rejn reason excessive frame loss */
	uint16 trans_rejn_xcess_frm_loss;
	/* No of trans rejn reason excessive traffic delay */
	uint16 trans_rejn_xcess_traffic_delay;
	/* No of trans rejn reason insufficient QoS capability */
	uint16 trans_rejn_insuffic_qos_cap;
	/* No of trans rejn reason low RSSI */
	uint16 trans_rejn_low_rssi;
	/* No of trans rejn reason high interference */
	uint16 trans_rejn_high_interference;
	/* No of trans rejn reason service unavilable */
	uint16 trans_rejn_service_unavail;
	/* No of beacon request rcvd */
	uint16 bcn_req_rcvd;
	/* No of beacon report sent */
	uint16 bcn_rep_sent;
	/* No of null beacon report sent */
	uint16 null_bcn_rep_sent;
	/* No of wifi to cell switch */
	uint16 wifi_to_cell;
} wl_mbo_counters_t;

#define WL_FILS_IOV_MAJOR_VER_SHIFT 8u

#define WL_FILS_IOV_MAJOR_VER_1 1u
#define WL_FILS_IOV_MINOR_VER_1 1u

#define WL_FILS_IOV_VERSION_1_1 \
	((WL_FILS_IOV_MAJOR_VER_1 << WL_FILS_IOV_MAJOR_VER_SHIFT) | WL_FILS_IOV_MINOR_VER_1)

enum wl_fils_cmd_ids {
	WL_FILS_CMD_ADD_IND_IE		= 1,
	WL_FILS_CMD_ADD_AUTH_DATA	= 2,	/* Deprecated, kept to prevent ROM invalidation */
	WL_FILS_CMD_ADD_HLP_IE		= 3,
	WL_FILS_CMD_ADD_CONNECT_PARAMS	= 4,
	WL_FILS_CMD_GET_CONNECT_PARAMS	= 5,
	/* Add before this !! */
	WL_FILS_CMD_LAST
};

enum wl_fils_xtlv_id {
	WL_FILS_XTLV_IND_IE		= 0x1,
	WL_FILS_XTLV_AUTH_DATA		= 0x2,	/* Deprecated, kept to prevent ROM invalidation */
	WL_FILS_XTLV_HLP_IE		= 0x3,
	WL_FILS_XTLV_ERP_USERNAME	= 0x4,
	WL_FILS_XTLV_ERP_REALM		= 0x5,
	WL_FILS_XTLV_ERP_RRK		= 0x6,
	WL_FILS_XTLV_ERP_NEXT_SEQ_NUM	= 0x7,
	WL_FILS_XTLV_KEK		= 0x8,
	WL_FILS_XTLV_PMK		= 0x9,
	WL_FILS_XTLV_TK			= 0xa,
	WL_FILS_XTLV_PMKID		= 0xb
};

#define WL_OCE_IOV_MAJOR_VER_1 1
#define WL_OCE_IOV_MINOR_VER_1 1
#define WL_OCE_IOV_MAJOR_VER_SHIFT 8
#define WL_OCE_IOV_VERSION_1_1 \
	((WL_OCE_IOV_MAJOR_VER_1 << WL_OCE_IOV_MAJOR_VER_SHIFT)| WL_OCE_IOV_MINOR_VER_1)

enum wl_oce_cmd_ids {
	WL_OCE_CMD_VERSION		= 0u,
	WL_OCE_CMD_ENABLE		= 1u,
	WL_OCE_CMD_PROBE_DEF_TIME	= 2u,
	WL_OCE_CMD_FD_TX_PERIOD		= 3u,
	WL_OCE_CMD_FD_TX_DURATION	= 4u,
	WL_OCE_CMD_RSSI_TH		= 5u,
	WL_OCE_CMD_RWAN_LINKS		= 6u,
	WL_OCE_CMD_CU_TRIGGER		= 7u,
	WL_OCE_CMD_TXPWR_USED		= 8u,
	WL_OCE_CMD_FEATURE_FLAGS	= 9u,
	/* Add before this !! */
	WL_OCE_CMD_LAST
};

enum wl_oce_xtlv_id {
	WL_OCE_XTLV_ENABLE		= 0x1u,
	WL_OCE_XTLV_PROBE_DEF_TIME	= 0x2u,
	WL_OCE_XTLV_FD_TX_PERIOD	= 0x3u,
	WL_OCE_XTLV_FD_TX_DURATION	= 0x4u,
	WL_OCE_XTLV_RSSI_TH		= 0x5u,
	WL_OCE_XTLV_RWAN_LINKS		= 0x6u,
	WL_OCE_XTLV_CU_TRIGGER		= 0x7u,
	WL_OCE_XTLV_TXPWR_USED		= 0x8u,
	WL_OCE_XTLV_FEATURE_FLAGS	= 0x9u, /* to control OCE features on the fly */
};

/* OCE feature control flags to enable/dsiable various features. */
typedef enum oce_feature_flags_e {
	OCE_FEATURE_FLAG_UCAST_PROBE_RESP	= (1u << 0u)	/* bit 0 */
} oce_feature_flags_t;

#define WL_IGMPOE_IOV_MAJOR_VER_1 1
#define WL_IGMPOE_IOV_MINOR_VER_1 1
#define WL_IGMPOE_IOV_MAJOR_VER_SHIFT 8
#define WL_IGMPOE_IOV_VERSION_1_1	\
	((WL_IGMPOE_IOV_MAJOR_VER_1 << WL_IGMPOE_IOV_MAJOR_VER_SHIFT)| WL_IGMPOE_IOV_MINOR_VER_1)

enum wl_igmpoe_cmd_ids {
	WL_IGMPOE_CMD_VERSION		= 0u,
	WL_IGMPOE_CMD_ENABLE		= 1u,
	WL_IGMPOE_CMD_TABLE		= 2u,
	WL_IGMPOE_CMD_STATS		= 3u,
	/* Add before this !! */
	WL_IGMPOE_CMD_LAST
};

enum wl_igmpoe_xtlv_id {
	WL_IGMPOE_XTLV_VERSION	= 0x0u,
	WL_IGMPOE_XTLV_ENABLE	= 0x1u,
	WL_IGMPOE_XTLV_TABLE	= 0x2u,
	WL_IGMPOE_XTLV_STATS	= 0x3u
};

typedef struct wl_igmp_entry {
	struct ipv4_addr ipaddr;	/* multicast ip address */
	uint32 timestamp;		/* time when entry is added or deleted */
	bool valid;			/* true when entry is added and false when deleted */
	uint8 PAD[3];			/* Reserved */
} wl_igmp_entry_t;

#define WL_IGMP_TABLE_VERSION	1u	/* igmpoe table IOVA version */
typedef struct wl_igmp_table_v1 {
	uint16 version;
	uint16 len;
	wl_igmp_entry_t data[];
} wl_igmp_table_t;

#define WL_IGMP_STATS_VERSION_1	1u	/* igmpoe stats IOVAR version */
typedef struct wl_igmp_stats_v1 {
	uint16 version;
	uint16 len;

	uint32 igmp_add_success;	/* # of times igmp entry is added */
	uint32 igmp_del_success;	/* # of times igmp entry is deleted */

	uint32 igmp_add_skipped;	/* # of times igmp entry add is skipped */
	uint32 igmp_add_nospace;	/* # of times igmp entry add failed due to no space */
	uint32 igmp_del_noentry;	/* # of times igmp entry delete failed due to no etnry */

	uint32 igmp_rx_query_general;	/* # of times igmp general query received */
	uint32 igmp_rx_query_specific;	/* # of times igmp specific query received */

	uint32 igmp_tx_report;		/* # of times igmp report is txed from fw */

	uint32 igmp_nohostip;		/* # of times igmp query is fwded to host */
	uint32 igmp_del_source_succ;		/* # of times igmp source entry delete succeeded */
	uint32 igmp_del_source_fail;		/* # of times igmp source entry delete failed */
	uint32 igmp_add_source_succ;		/* # of times igmp source entry add succeeded */
	uint32 igmp_add_source_fail;		/* # of times igmp source entry add failed */
} wl_igmp_stats_t;

/* qos commands */

/* Definitions for qos iovar */
#define WL_QOS_VERSION_1	1u	/* qos IOVAR/API version */

/* qos_mgmt IOV subcommand IDs */
typedef enum wl_qos_cmd_id {
	WL_QOS_CMD_VERSION		= 0u,	/* qos_mgmt ver subcommand */
	WL_QOS_CMD_ENABLE		= 1u,	/* qos_mgmt enable subcommand */
	WL_QOS_CMD_RAV_MSCS		= 2u,	/* qos_mgmt rav_mscs subcommand */
	WL_QOS_CMD_RAV_SCS		= 3u,	/* qos_mgmt rav_scs */
	WL_QOS_CMD_RAV_SCS_DESCR_PARAMS	= 4u,	/* qos_mgmt rav_scs descr_params */
	WL_QOS_CMD_RAV_SCS_CT4		= 5u,	/* qos_mgmt rav_scs ct4 */
	WL_QOS_CMD_RAV_SCS_CT10		= 6u,	/* qos_mgmt rav_scs ct10 */
	WL_QOS_CMD_RAV_SCS_QOS_CHAR	= 7u	/* qos_mgmt rav_scs qos_char */
} wl_qos_cmd_id_e;

/* QoS enable subcommand flags */
typedef enum qos_cmd_enable_flags {
	WL_QOS_CMD_ENABLE_FLAG_RAV_MSCS			= (1u << 0u),	/* bit 0 */
	WL_QOS_CMD_ENABLE_FLAG_RAV_MSCS_NEG_IN_ASSOC	= (1u << 1u),	/* bit 1 */
	WL_QOS_CMD_ENABLE_FLAG_RAV_SCS			= (1u << 2u),	/* bit 2 */
	WL_QOS_CMD_ENABLE_FLAG_RAV_SCS_QOS_CHAR		= (1u << 3u)	/* bit 3 */
} qos_cmd_enable_flags_e;

/* MSCS activation status flags */
typedef enum wl_qos_rav_mscs_status_flags {
	/* MSCS status during the association */
	WL_QOS_RAV_MSCS_STATUS_PRE_ASSOC		= (1u << 0u),	/* bit 0 */

	/* MSCS status after association, using action frames */
	WL_QOS_RAV_MSCS_STATUS_POST_ASSOC		= (1u << 1u),	/* bit 1 */

	/* Unsolicited request from the AP */
	WL_QOS_RAV_MSCS_STATUS_UNSOLICIT_FROM_AP	= (1u << 2u),	/* bit 2 */

	/* Indicates the failure in receiving the MSCS response */
	WL_QOS_RAV_MSCS_STATUS_TIMEOUT			= (1u << 3u),	/* bit 3 */

	/* Unable to transmit the action frame */
	WL_QOS_RAV_MSCS_STATUS_TX_FAILURE		= (1u << 4u),	/* bit 4 */

	/* Indicates that the MSCS is active */
	WL_QOS_RAV_MSCS_STATUS_ACTIVE			= (1u << 5u),	/* bit 5 */

} wl_qos_rav_mscs_status_flags_e;

#define WL_QOS_RAV_MSCS_SC_VERSION_1	1u	/* rav_mscs subcommand version */
typedef struct wl_qos_rav_mscs_config_v1 {
	uint16 version;		/* version of this structure */
	uint16 length;		/* length of this structure */
	uint32 stream_timeout;	/* MSCS stream timeout */
	uint8  up_bitmap;	/* user priority bitmap */
	uint8  up_limit;	/* user priority limit */
	uint8  fc_type;		/* Frame classifier type, IPv4, IPv6, etc. */
	uint8  fc_mask;		/* Specifies the frame classifier mask */
	uint8  req_type;	/* Indicates the MSCS Request type (add/remove/change) */
	uint8  status_flags;	/* Status flags, see wl_qos_rav_mscs_status_flags_e; */
	uint16 status_code;	/* IEEE status code */
} wl_qos_rav_mscs_config_v1_t;

typedef enum wl_qos_rav_scs_flags {
	/* If bit0 is 1 which indicates to send the SCS action frame to the AP */
	WL_QOS_RAV_SCS_FLAGS_COMMIT		= (1u << 0u)	/* bit 0 */
} wl_qos_rav_scs_flags_e;

#define WL_QOS_RAV_SCS_DESCR_PARAMS_VERSION_1	1u	/* rav_scs descr subcommand version */
typedef struct wl_qos_rav_scs_descr_params_v1 {
	uint16	version;	/* version of this structure */
	uint16	length;		/* length (bytes) of this structure */
	uint16	status_code;	/* Status code from the AP */
	uint8	flags;		/* various flags, see wl_qos_rav_scs_descr_flags_e */
	uint8	scsid;		/* SCS descriptor id */
	uint8	req_type;	/* Request type (0/1/2, add/change/delete) */
	uint8	intra_ac_prio;	/* Intra AC (Access Category) priority */
	uint8	tc_processing;	/* TCLAS processing */
	uint8	pad;		/* Reserved */
} wl_qos_rav_scs_descr_params_v1_t;

/* SCS ct4 (classifier type 4) for IPv4/IPv6 */
#define WL_QOS_RAV_SCS_CT4_SC_VERSION_1	1u	/* rav_scs ct4 subcommand version */
#define SCS_CT4_IPV6_FLOW_LABEL_SIZE	3u	/* bytes */
typedef struct wl_qos_rav_scs_ct4_v1 {
	uint16	version;		/* version of this structure */
	uint16	length;			/* length (bytes) of this structure */
	uint16	src_port;		/* Src port, network byte order */
	uint16	dst_port;		/* Dst port, network byte order */
	uint8	src_ip[IPV6_ADDR_LEN];	/* Src IP v4/v6, network byte order  */
	uint8	dst_ip[IPV6_ADDR_LEN];	/* Dest IP v4/v6, network byte order */
	uint8	scsid;			/* Unique SCS descriptor id */
	uint8	up;			/* Not used, specify 255 */
	uint8	classifier_mask;	/* Each bit identifies the following */
	uint8	ip_version;		/* IP version v4/v6 */
	uint8	dscp;			/* DSCP in IP header */
	uint8	proto_or_nh;		/* IPv4: protocol, IPv6: next header */
	uint8	flow_label[SCS_CT4_IPV6_FLOW_LABEL_SIZE];	/* IPv6 flow label */
	uint8	pad[3];			/* Reserved */
} wl_qos_rav_scs_ct4_v1_t;

/* SCS ct10 (classifier type 10) for IPv4/IPv6 */
#define WL_QOS_RAV_SCS_CT10_SC_VERSION_1	1u	/* rav_scs ct10 subcommand version */
typedef struct wl_qos_rav_scsc_ct10_filter_data {
	uint8 filter_value;
	uint8 filter_mask;
	uint8 pad[2];
} wl_qos_rav_scsc_ct10_filter_data_t;

typedef struct wl_qos_rav_scs_ct10_v1 {
	uint16	version;		/* version of this structure */
	uint16	length;			/* length (bytes) of this structure */
	uint8	scsid;			/* SCS descriptor id */
	uint8	up;			/* Unused, specify 255 */
	uint8	proto_instance;		/* protocol instance */
	uint8	proto_or_nh;		/* protocol or next header */
	uint16	num_filter_items;	/* Number of entries in filter_data */
	uint8	pad[2];			/* Reserved */
	wl_qos_rav_scsc_ct10_filter_data_t filter_data[];
} wl_qos_rav_scs_ct10_v1_t;

/* SCS QoS Characteristics parameters */
/* Refer Table 9-401r in the Draft P802.11BE_D3.0 */
typedef enum wl_qos_rav_scs_qos_char_direction {
	WL_QOS_RAV_SCS_QOS_CHAR_DIR_UPLINK	= 0u,
	WL_QOS_RAV_SCS_QOS_CHAR_DIR_DOWNLINK	= 1u,
	WL_QOS_RAV_SCS_QOS_CHAR_DIR_DIRECT_LINK	= 2u
} wl_qos_rav_scs_qos_char_direction_e;

/* rav_scs qos_char subcommand version */
#define WL_QOS_RAV_SCS_QOS_CHAR_SC_VERSION_1	1u
typedef struct wl_qos_rav_scs_qos_char_v1 {
	uint16	version;		/* version of this structure */
	uint16	length;			/* length (bytes) of this structure */
	uint8	scsid;			/* SCS descriptor id */
	uint8   direction;		/* wl_qos_rav_scs_qos_char_direction_e */

	/* Same as the user priority, values 8..15 are reserved. This can be different
	 * from the user priority in the future.
	 */
	uint8   tid;

	/* Contains the user priority value (0..7). This is set to the same value of the
	 * user priority in the Intra-Access Category Priority element
	 */
	uint8   up;
	uint8   linkid;			/* Applicable only for the peer-to-peer link */
	uint8   msdu_delivery_info;	/* MSDU Delivery Info (Ratio and exponent) */
	uint16  msdu_lifetime;		/* MSDU Lifetime in milliseconds */

	/* Please see the section 9.4.2.316 in the Draft P802.11be_D3.0 for more details
	 * for the description of each field.
	 */
	uint32  min_srv_int;		/* Minimum Service Interval in microseconds */
	uint32  max_srv_int;		/* Maximum Service Interval in microseconds */
	uint32  min_data_rate;		/* Minimum Data Rate in kilobits
					 * per second (kbps), lower 3 bytes
					 */
	uint32  delay_bound;		/* Delay Bound in microseconds, lower 3 bytes */
	uint32  srv_start_time;		/* Anticipated time in microseconds */
	uint32  mean_data_rate;		/* Mean Data Rate in Kbps, lower 3 bytes */
	uint32  burst_size;		/* Burst Size in octets */
	uint16  max_msdu_size;		/* Maxmimum MSDU Size in octets */
	uint16  medium_time;		/* Medium Time in the units of 256-microseconds
					 * per second
					 */
} wl_qos_rav_scs_qos_char_v1_t;

#define WL_ESP_IOV_MAJOR_VER_1 1
#define WL_ESP_IOV_MINOR_VER_1 1
#define WL_ESP_IOV_MAJOR_VER_SHIFT 8
#define WL_ESP_IOV_VERSION_1_1 \
	((WL_ESP_IOV_MAJOR_VER_1 << WL_ESP_IOV_MAJOR_VER_SHIFT)| WL_ESP_IOV_MINOR_VER_1)

enum wl_esp_cmd_ids {
	WL_ESP_CMD_ENABLE = 1,
	WL_ESP_CMD_STATIC = 2,
	/* Add before this !! */
	WL_ESP_CMD_LAST
};

enum wl_esp_xtlv_id {
	WL_ESP_XTLV_ENABLE  = 0x1,
	WL_ESP_XTLV_STATIC_AC    = 0x2,	/* access category */
	WL_ESP_XTLV_STATIC_TYPE  = 0x3,	/* data type */
	WL_ESP_XTLV_STATIC_VAL   = 0x4
};

/* bcnprot commands */

/* Definitions for bcn_prot iovar */
#define WL_BCN_PROT_VERSION_1        1u      /* bcn_prot IOVAR/API version */

enum wl_bcn_prot_cmd_ids {
	WL_BCN_PROT_CMD_VERSION  = 0u,
	WL_BCN_PROT_CMD_ENABLE   = 1u,
	WL_BCN_PROT_CMD_COUNTERS = 2u,
	WL_BCN_PROT_CMD_STATUS	 = 3u,
	WL_BCN_PROT_CMD_CONFIG	 = 4u
};

/* Optional XTLVs included in wlc_roam_start_event_t */
enum wl_roam_start_xtlv_id {
	/* XTLV for beacon protection counters if beacon protection
	 * is active and roam started due to beacon loss
	 */
	WL_ROAM_START_XTLV_BCNPROT_STATS  = 0x1u
};

/* WL_ROAM_START_XTLV_BCNPROT_STATS */
#define BCN_PROT_COUNTERS_VERSION_0		(0u)
typedef struct wlc_bcn_prot_counters_v0 {
	uint32 no_en_bit;	/* counts beacons without bcn prot enable bit at ext cap */
	uint32 no_mme_ie;	/* counts beacons without mme ie */
	uint32 mic_fails;	/* counts beacons failed mic check */
	uint32 replay_fails;	/* counts beacons failed replay check */
} wlc_bcn_prot_counters_v0_t;

#define BCN_PROT_COUNTERS_VERSION_1		(1u)
typedef struct wlc_bcn_prot_counters_v1 {
	uint16 version;		/* version of this structure */
	uint16 length;		/* length (bytes) of this structure */
	uint32 no_en_bit;	/* counts beacons without bcn prot enable bit at ext cap */
	uint32 no_mme_ie;	/* counts beacons without mme ie */
	uint32 mic_fails;	/* counts beacons that failed mic check */
	uint32 replay_fails;	/* counts beacons that failed replay check */
	uint32 errors_since_good_bcn; /* counts failures since last good beacon */
} wlc_bcn_prot_counters_v1_t;

#define BCN_PROT_COUNTERS_VERSION_2		(2u)
typedef struct wlc_bcn_prot_counters_v2 {
	uint16 version;		/* version of this structure */
	uint16 length;		/* length (bytes) of this structure */
	uint8 link_idx;		/* link index */
	uint8 link_id;
	uint8 PAD[2];
	uint32 no_en_bit;	/* counts beacons without bcn prot enable bit at ext cap */
	uint32 no_mme_ie;	/* counts beacons without mme ie */
	uint32 mic_fails;	/* counts beacons that failed mic check */
	uint32 replay_fails;	/* counts beacons that failed replay check */
	uint32 errors_since_good_bcn; /* counts failures since last good beacon */
	uint32 no_key;		/* counts beacons where no key found for the key_id */
} wlc_bcn_prot_counters_v2_t;

#ifndef BCN_PROT_COUNTERS_TEMP_VERSION_ENABLED
#define BCN_PROT_COUNTERS_VERSION	(BCN_PROT_COUNTERS_VERSION_0)
typedef wlc_bcn_prot_counters_v0_t wlc_bcn_prot_counters_t;
#define BCN_PROT_COUNTERS_SIZE	sizeof(wlc_bcn_prot_counters_v0_t)
#endif /* BCN_PROT_COUNTERS_TEMP_VERSION_ENABLED */

#define WL_BCN_PROT_ENABLED		1u

#define WL_BCN_PROT_STATUS_ACTIVE	1u

#define WL_BCN_PROT_CONFIG_ALLOW_PROT_ONLY	0x00000001u	/* Allow join/roam to only Beacon
								* protection capable AP
								*/
#define WL_BCN_PROT_CONFIG_ALLOW_WIFI7_AP_ONLY	0x00000002u	/* Allow Beacon protection only for
								* connections with WiFi7 APs
								*/

/* otpread command */
#define WL_OTPREAD_VER 1

typedef struct {
	uint16 version;		/* cmd structure version */
	uint16 cmd_len;		/* cmd struct len */
	uint32 rdmode;		/* otp read mode */
	uint32 rdoffset;	/* byte offset into otp to start read */
	uint32 rdsize;		/* number of bytes to read */
} wl_otpread_cmd_t;

/* "otpecc_rows" command */
typedef struct {
	uint16 version;		/* version of this structure */
	uint16 len;			/* len in bytes of this structure */
	uint32 cmdtype;		/* command type : 0 : read row data, 1 : ECC lock  */
	uint32 rowoffset;	/* start row offset */
	uint32 numrows;		/* number of rows */
	uint8  rowdata[];	/* read rows data */
} wl_otpecc_rows_t;

/* "otp ecc" command */
typedef struct {
	uint16 version;		/* version of this structure */
	uint16 len;			/* len in bytes of this structure */
	uint16 cmdtype;		/* command type : 0 : read row data,
						 *   1 : ECC lock,
						 *   2 : program a row data with Parity
						 *   3 : program a row data
						 *        with Parity on locking row
						 *   4 : clear double bit error status
						 */
	uint16 rowoffset;	/* start row offset */
	uint16 numrows;		/* number of rows */
	uint16 pad;			/* for alignment */
	uint32 progdata;	/* a row data to program  */
} wl_otpecc_cmd_t;

#define WL_OTPECC_ROWS_VER  1

#define WL_OTPECC_ROWS_CMD_READ  0
#define WL_OTPECC_ROWS_CMD_LOCK  1
#define WL_OTPECC_ROWS_CMD_PROG  2
#define WL_OTPECC_ROWS_CMD_PROG_LOCK  3
#define WL_OTPECC_ROWS_CMD_CLRDBLERR  4

#define WL_OTPECC_ARGIDX_CMDTYPE		0	/* command type */
#define WL_OTPECC_ARGIDX_ROWOFFSET		1	/* start row offset */
#define WL_OTPECC_ARGIDX_NUMROWS		2	/* number of rows */
#define WL_OTPECC_ARGIDX_PROGDATA		3	/* a row data to program */

/* "otpeccrows" raw data size per row */
#define WL_ECCDUMP_ROW_SIZE_BYTE	6 /* 4 bytes row data + 2 bytes ECC status */
#define WL_ECCDUMP_ROW_SIZE_WORD	3
#define WL_ECCDUMP_ROW_DATA_SIZE_BYTE	4
#define WL_ECCDUMP_ROW_PARITY_SIZE_BYTE	2

/* otpECCstatus */
#define OTP_ECC_ENAB_SHIFT		13
#define OTP_ECC_ENAB_MASK		0x7
#define OTP_ECC_CORR_ST_SHIFT	12
#define OTP_ECC_CORR_ST_MASK	0x1
#define OTP_ECC_DBL_ERR_SHIFT	11
#define OTP_ECC_DBL_ERR_MASK	0x1
#define OTP_ECC_DED_ST_SHIFT	10
#define OTP_ECC_DED_ST_MASK		0x1
#define OTP_ECC_SEC_ST_SHIFT	9
#define OTP_ECC_SEC_ST_MASK		0x1
#define OTP_ECC_DATA_SHIFT		0
#define OTP_ECC_DATA_MASK		0x7f

/* OTP_ECC_CORR_ST field */
#define OTP_ECC_MODE		1
#define OTP_NO_ECC_MODE		0

/* OTP_ECC_ENAB field (bit15:13) :
 * When 2 or 3 bits are set,
 * it indicates that OTP ECC is enabled on the last row read.
 * Otherwise, ECC is disabled
 */
#define OTP_ECC_ENAB(val) \
	(bcm_bitcount((uint8 *)&(val), sizeof(uint8)) > 1)

/* otp command details */
#define WL_OTP_IOV_MAJOR_VER_1		1u
#define WL_OTP_IOV_MINOR_VER_1		1u
#define WL_OTP_IOV_MAJOR_VER_SHIFT	8u
#define WL_OTP_IOV_VERSION_1_1 \
		((WL_OTP_IOV_MAJOR_VER_1 << WL_OTP_IOV_MAJOR_VER_SHIFT) | WL_OTP_IOV_MINOR_VER_1)

/* OTP Regions HW/SW */
#define OTP_RGN_NONE	0u
#define OTP_RGN_HW	1u
#define OTP_RGN_SW	2u
#define OTP_RGN_BT	3u

enum wl_otp_cmd_ids {
	WL_OTP_CMD_ALL		= 0u,
	WL_OTP_CMD_RGNSTATUS	= 1u,
	WL_OTP_CMD_RGNDUMP	= 2u,
	WL_OTP_CMD_RGNWRITE	= 3u,
	WL_OTP_CMD_ECC = 4u,
	/* Add before this !!! */
	WL_OTP_CMD_LAST
};

enum wl_otp_xtlv_id {
	WL_OTP_XTLV_NONE			= 0u,	/* Not valid otp tag */
	WL_OTP_XTLV_RGN				= 1u,	/* OTP region type */
	WL_OTP_XTLV_ADDR			= 2u,	/* OTP region start address */
	WL_OTP_XTLV_SIZE			= 3u,	/* OTP region size */
	WL_OTP_XTLV_DATA			= 4u,	/* OTP dump data */
	WL_OTP_XTLV_ECC_ROW			= 5u,	/* OTP ecc row */
	WL_OTP_XTLV_ECC_ROW_DATA	= 6u,	/* OTP ecc row data */
};


#define OTP_PCIE_HDR_SZ 224u
/* HIF Header definitions */
#define OTP_HIF_HDR_SIG		"OTPH"
#define OTP_HIF_HDR_SIG_SZ	4u
#define OTP_HIF_HDR_REV_SZ	4u
#define OTP_HIF_HDR_SIG_REV_SZ	(OTP_HIF_HDR_SIG_SZ + OTP_HIF_HDR_REV_SZ)
#define OTP_HIF_HDR_REV_MAX	0xFu

#define WL_LEAKY_AP_STATS_GT_TYPE	0
#define WL_LEAKY_AP_STATS_PKT_TYPE	1
typedef struct wlc_leaked_infra_guard_marker {
	/* type field for this TLV: WL_LEAKY_AP_STATS_GT_TYPE */
	uint16  type;
	/* length field for this TLV */
	uint16  len;
	/* guard sample sequence number; Updated by 1 on every guard sample */
	uint32  seq_number;
	/* Guard time start time (tsf; PS indicated and acked) */
	uint32  start_time;
	/* tsf timestamp for the GT end event */
	uint32  gt_tsf_l;
	/* Guard time period in ms */
	uint16  guard_duration;
	/* Number PPDUs in the notification */
	uint16  num_pkts;
	/* Flags to indicate some states see below */
	uint8   flag;
	/* pad for 32-bit alignment */
	uint8   reserved[3];
} wlc_leaked_infra_guard_marker_t;

/* Flag information */
#define WL_LEAKED_GUARD_TIME_NONE	0               /* Not in any guard time */
#define WL_LEAKED_GUARD_TIME_FRTS	(0x01 << 0)     /* Normal FRTS power save */
#define WL_LEAKED_GUARD_TIME_SCAN	(0x01 << 1)     /* Channel switch due to scanning */


#define WL_LEAKED_GUARD_TIME_INFRA_STA	(0x01 << 4)	/* generic type infra sta channel switch */
#define WL_LEAKED_GUARD_TIME_TERMINATED (0x01 << 7)     /* indicate a GT is terminated early */

typedef struct wlc_leaked_infra_packet_stat {
	uint16  type;			/* type field for this TLV: WL_LEAKY_AP_STATS_PKT_TYPE */
	uint16  len;			/* length field for this TLV */
	uint16  ppdu_len_bytes;		/* PPDU packet length in bytes */
	uint16  num_mpdus;		/* number of the MPDUs in the PPDU */
	uint32  ppdu_time;		/* PPDU arrival time at the begining of the guard time */
	uint32  rate;			/* PPDU packet rate; Received packet's data rate */
	uint16  seq_number;		/* sequence number */
	int8    rssi;			/* RSSI */
	uint8   tid;			/* tid */
} wlc_leaked_infra_packet_stat_t;

/* Wake timer structure definition */
#define WAKE_TIMER_VERSION_1	1
#define WAKE_TIMER_NOLIMIT	0xFFFF

typedef struct wake_timer {
	uint16 ver;
	uint16 len;
	uint16 limit;   /* number of events to deliver
			* 0-disable, 0xffff-indefinite, num_events otherwise
			*/
	uint16 count;	/* number of events delivered since enable (get only) */
	uint16 period;	/* timeout/period in milliseconds */
} wake_timer_t;

typedef struct wl_desense_restage_gain {
	uint16 version;
	uint16 length;
	uint32 band;
	uint8 num_cores;
	uint8 desense_array[WL_TX_CHAINS_MAX];
	uint8 PAD[3];
} wl_desense_restage_gain_t;

#define MAX_UCM_CHAINS 5
#define MAX_UCM_PROFILES 10

/* UCM per chain attribute struct */
typedef struct wlc_btcx_chain_attr {
	uint16 length;			/* chain attr length, version is same as profile version */
	int8 desense_level;		/* per chain desense level */
	int8 ack_pwr_strong_rssi;	/* per chain ack power at strong rssi */
	int8 ack_pwr_weak_rssi;		/* per chain ack power at weak rssi */
	int8 tx_pwr_strong_rssi;	/* per chain tx power at strong rssi */
	int8 tx_pwr_weak_rssi;		/* per chain tx power at weak rssi */
	uint8 PAD[1];			/* additional bytes for alignment */
} wlc_btcx_chain_attr_t;

#define WL_UCM_PER_CHAIN_ATTR_VERSION_V1 1
/* UCM per chain attribute struct v1 (same structure to wlc_btcx_chain_attr_t) */
typedef struct wlc_btcx_per_chain_attr_v1 {
	uint16 length;			/* chain attr length, version is same as profile version */
	int8 desense_level;		/* per chain desense level */
	int8 ack_pwr_strong_rssi;	/* per chain ack power at strong rssi */
	int8 ack_pwr_weak_rssi;		/* per chain ack power at weak rssi */
	int8 tx_pwr_strong_rssi;	/* per chain tx power at strong rssi */
	int8 tx_pwr_weak_rssi;		/* per chain tx power at weak rssi */
	uint8 PAD[1];			/* additional bytes for alignment */
} wlc_btcx_per_chain_attr_v1_t;

#define WL_UCM_PER_CHAIN_ATTR_VERSION_V2 2
/* UCM per chain attribute struct v2 */
typedef struct wlc_btcx_per_chain_attr_v2 {
	uint16 length;			/* chain attr length, version is same as profile version */
	uint8 desense_strong_rssi;	/* per chain desense mode for strong rssi */
	uint8 desense_weak_rssi;	/* per chain desense mode for weak rssi */
	int8 ack_pwr_strong_rssi;	/* per chain ack power at strong rssi */
	int8 ack_pwr_weak_rssi;		/* per chain ack power at weak rssi */
	int8 tx_pwr_strong_rssi;	/* per chain tx power at strong rssi */
	int8 tx_pwr_weak_rssi;		/* per chain tx power at weak rssi */
} wlc_btcx_per_chain_attr_v2_t;

#define UCM_PROFILE_VERSION_1 1
typedef struct wlc_btcx_profile_v1 {
	uint16 version;			/* UCM profile version */
	uint16 length;			/* profile size */
	uint16 fixed_length;		/* size of the fixed portion of the profile */
	uint8 init;			/* profile initialized or not */
	uint8 chain_attr_count;		/* Number of elements in chain_attr array */
	uint8 profile_index;		/* profile index */
	uint8 mode_strong_wl_bt;	/* Mode under strong WLAN and BT RSSI */
	uint8 mode_weak_wl;		/* Mode under weak WLAN RSSI */
	uint8 mode_weak_bt;		/* Mode under weak BT RSSI */
	uint8 mode_weak_wl_bt;		/* Mode under weak BT and WLAN RSSI */
	int8 mode_wl_hi_lo_rssi_thresh;	/* Strong to weak WLAN RSSI threshold for mode selection */
	int8 mode_wl_lo_hi_rssi_thresh;	/* Weak to strong WLAN RSSI threshold for mode selection */
	int8 mode_bt_hi_lo_rssi_thresh;	/* Strong to weak BT RSSI threshold for mode selection */
	int8 mode_bt_lo_hi_rssi_thresh;	/* Weak to strong BT RSSI threshold for mode selection */
	int8 desense_wl_hi_lo_rssi_thresh;	/* Strong to weak RSSI threshold for desense */
	int8 desense_wl_lo_hi_rssi_thresh;	/* Weak to strong RSSI threshold for desense */
	int8 ack_pwr_wl_hi_lo_rssi_thresh;	/* Strong to weak RSSI threshold for ACK power */
	int8 ack_pwr_wl_lo_hi_rssi_thresh;	/* Weak to strong RSSI threshold for ACK power */
	int8 tx_pwr_wl_hi_lo_rssi_thresh;	/* Strong to weak RSSI threshold for Tx power */
	int8 tx_pwr_wl_lo_hi_rssi_thresh;	/* Weak to strong RSSI threshold for Tx power */
	uint8 PAD[1];				/* additional bytes for 4 byte alignment */
	wlc_btcx_chain_attr_t chain_attr[];	/* variable length array with chain attributes */
} wlc_btcx_profile_v1_t;

#define UCM_PROFILE_VERSION_2 2u
typedef struct wlc_btcx_profile_v2 {
	uint16 version;			/* UCM profile version */
	uint16 length;			/* profile size */
	uint16 fixed_length;		/* size of the fixed portion of the profile */
	uint8 init;			/* profile initialized or not */
	uint8 chain_attr_count;		/* Number of elements in chain_attr array */
	uint8 profile_index;		/* profile index */
	uint8 mode_strong_wl_bt;	/* Mode under strong WLAN and BT RSSI */
	uint8 mode_weak_wl;		/* Mode under weak WLAN RSSI */
	uint8 mode_weak_bt;		/* Mode under weak BT RSSI */
	uint8 mode_weak_wl_bt;		/* Mode under weak BT and WLAN RSSI */
	int8 mode_wl_hi_lo_rssi_thresh;	/* Strong to weak WLAN RSSI threshold for mode selection */
	int8 mode_wl_lo_hi_rssi_thresh;	/* Weak to strong WLAN RSSI threshold for mode selection */
	int8 mode_bt_hi_lo_rssi_thresh;	/* Strong to weak BT RSSI threshold for mode selection */
	int8 mode_bt_lo_hi_rssi_thresh;	/* Weak to strong BT RSSI threshold for mode selection */
	int8 desense_wl_hi_lo_rssi_thresh;	/* Strong to weak RSSI threshold for desense */
	int8 desense_wl_lo_hi_rssi_thresh;	/* Weak to strong RSSI threshold for desense */
	int8 ack_pwr_wl_hi_lo_rssi_thresh;	/* Strong to weak RSSI threshold for ACK power */
	int8 ack_pwr_wl_lo_hi_rssi_thresh;	/* Weak to strong RSSI threshold for ACK power */
	int8 tx_pwr_wl_hi_lo_rssi_thresh;	/* Strong to weak RSSI threshold for Tx power */
	int8 tx_pwr_wl_lo_hi_rssi_thresh;	/* Weak to strong RSSI threshold for Tx power */
	uint8 hybrid_ant_core_config;		/* Select antenna configuration for hybrid */
	wlc_btcx_chain_attr_t chain_attr[];	/* variable length array with chain attributes */
} wlc_btcx_profile_v2_t;

#define UCM_PROFILE_VERSION_3 3u
typedef struct wlc_btcx_profile_v3 {
	uint16 version;			/* UCM profile version */
	uint16 length;			/* profile size */
	uint16 fixed_length;		/* size of the fixed portion of the profile */
	uint8 init;			/* profile initialized or not */
	uint8 chain_attr_count;		/* Number of elements in chain_attr array */
	uint8 profile_index;		/* profile index */
	uint8 mode_strong_wl_bt;	/* Mode under strong WLAN and BT RSSI */
	uint8 mode_weak_wl;		/* Mode under weak WLAN RSSI */
	uint8 mode_weak_bt;		/* Mode under weak BT RSSI */
	uint8 mode_weak_wl_bt;		/* Mode under weak BT and WLAN RSSI */
	int8 mode_wl_hi_lo_rssi_thresh;	/* Strong to weak WLAN RSSI threshold for mode selection */
	int8 mode_wl_lo_hi_rssi_thresh;	/* Weak to strong WLAN RSSI threshold for mode selection */
	int8 mode_bt_hi_lo_rssi_thresh;	/* Strong to weak BT RSSI threshold for mode selection */
	int8 mode_bt_lo_hi_rssi_thresh;	/* Weak to strong BT RSSI threshold for mode selection */
	int8 desense_wl_hi_lo_rssi_thresh;	/* Strong to weak RSSI threshold for desense */
	int8 desense_wl_lo_hi_rssi_thresh;	/* Weak to strong RSSI threshold for desense */
	int8 ack_pwr_wl_hi_lo_rssi_thresh;	/* Strong to weak RSSI threshold for ACK power */
	int8 ack_pwr_wl_lo_hi_rssi_thresh;	/* Weak to strong RSSI threshold for ACK power */
	int8 tx_pwr_wl_hi_lo_rssi_thresh;	/* Strong to weak RSSI threshold for Tx power */
	int8 tx_pwr_wl_lo_hi_rssi_thresh;	/* Weak to strong RSSI threshold for Tx power */
	uint8 hybrid_ant_core_config;		/* Select antenna configuration for hybrid */
	wlc_btcx_per_chain_attr_v2_t chain_attr[]; /* variable length array with chain attributes */
} wlc_btcx_profile_v3_t;

#define SSSR_D11_RESET_SEQ_STEPS   5u
#define SSSR_HWA_RESET_SEQ_STEPS   8u

#define SSSR_REG_INFO_VER_0	0u
#define SSSR_REG_INFO_VER_1	1u
#define SSSR_REG_INFO_VER_2	2u
#define SSSR_REG_INFO_VER_3	3u
#define SSSR_REG_INFO_VER_4	4u
#define SSSR_REG_INFO_VER_5	5u
#define SSSR_REG_INFO_VER_6	6u


typedef struct sssr_reg_info_v0 {
	uint16 version;
	uint16 length;  /* length of the structure validated at host */
	struct {
		struct {
			uint32 pmuintmask0;
			uint32 pmuintmask1;
			uint32 resreqtimer;
			uint32 macresreqtimer;
			uint32 macresreqtimer1;
		} base_regs;
	} pmu_regs;
	struct {
		struct {
			uint32 intmask;
			uint32 powerctrl;
			uint32 clockcontrolstatus;
			uint32 powerctrl_mask;
		} base_regs;
	} chipcommon_regs;
	struct {
		struct {
			uint32 clockcontrolstatus;
			uint32 clockcontrolstatus_val;
		} base_regs;
		struct {
			uint32 resetctrl;
			uint32 itopoobb;
		} wrapper_regs;
	} arm_regs;
	struct {
		struct {
			uint32 ltrstate;
			uint32 clockcontrolstatus;
			uint32 clockcontrolstatus_val;
		} base_regs;
		struct {
			uint32 itopoobb;
		} wrapper_regs;
	} pcie_regs;
	struct {
		struct {
			uint32 ioctrl;
		} wrapper_regs;
		uint32 vasip_sr_addr;
		uint32 vasip_sr_size;
	} vasip_regs;
	struct {
		struct {
			uint32 xmtaddress;
			uint32 xmtdata;
			uint32 clockcontrolstatus;
			uint32 clockcontrolstatus_val;
		} base_regs;
		struct {
			uint32 resetctrl;
			uint32 itopoobb;
			uint32 ioctrl;
			uint32 ioctrl_resetseq_val[SSSR_D11_RESET_SEQ_STEPS];
		} wrapper_regs;
		uint32 sr_size;
	} mac_regs[MAX_NUM_D11CORES];
} sssr_reg_info_v0_t;

typedef struct sssr_reg_info_v1 {
	uint16 version;
	uint16 length;  /* length of the structure validated at host */
	struct {
		struct {
			uint32 pmuintmask0;
			uint32 pmuintmask1;
			uint32 resreqtimer;
			uint32 macresreqtimer;
			uint32 macresreqtimer1;
		} base_regs;
	} pmu_regs;
	struct {
		struct {
			uint32 intmask;
			uint32 powerctrl;
			uint32 clockcontrolstatus;
			uint32 powerctrl_mask;
		} base_regs;
	} chipcommon_regs;
	struct {
		struct {
			uint32 clockcontrolstatus;
			uint32 clockcontrolstatus_val;
		} base_regs;
		struct {
			uint32 resetctrl;
			uint32 itopoobb;
		} wrapper_regs;
	} arm_regs;
	struct {
		struct {
			uint32 ltrstate;
			uint32 clockcontrolstatus;
			uint32 clockcontrolstatus_val;
		} base_regs;
		struct {
			uint32 itopoobb;
		} wrapper_regs;
	} pcie_regs;
	struct {
		struct {
			uint32 ioctrl;
		} wrapper_regs;
		uint32 vasip_sr_addr;
		uint32 vasip_sr_size;
	} vasip_regs;
	struct {
		struct {
			uint32 xmtaddress;
			uint32 xmtdata;
			uint32 clockcontrolstatus;
			uint32 clockcontrolstatus_val;
		} base_regs;
		struct {
			uint32 resetctrl;
			uint32 itopoobb;
			uint32 ioctrl;
			uint32 ioctrl_resetseq_val[SSSR_D11_RESET_SEQ_STEPS];
		} wrapper_regs;
		uint32 sr_size;
	} mac_regs[MAX_NUM_D11CORES];
	struct {
		uint32 dig_sr_addr;
		uint32 dig_sr_size;
	} dig_mem_info;
} sssr_reg_info_v1_t;

#define MAX_NUM_D11_CORES_WITH_SCAN	3u

typedef struct sssr_reg_info_v2 {
	uint16 version;
	uint16 length;  /* length of the structure validated at host */
	struct {
		struct {
			uint32 pmuintmask0;
			uint32 pmuintmask1;
			uint32 resreqtimer;
			uint32 macresreqtimer;
			uint32 macresreqtimer1;
			uint32 macresreqtimer2;
		} base_regs;
	} pmu_regs;
	struct {
		struct {
			uint32 intmask;
			uint32 powerctrl;
			uint32 clockcontrolstatus;
			uint32 powerctrl_mask;
		} base_regs;
	} chipcommon_regs;
	struct {
		struct {
			uint32 clockcontrolstatus;
			uint32 clockcontrolstatus_val;
		} base_regs;
		struct {
			uint32 resetctrl;
			uint32 extrsrcreq;
		} wrapper_regs;
	} arm_regs;
	struct {
		struct {
			uint32 ltrstate;
			uint32 clockcontrolstatus;
			uint32 clockcontrolstatus_val;
		} base_regs;
		struct {
			uint32 extrsrcreq;
		} wrapper_regs;
	} pcie_regs;
	struct {
		struct {
			uint32 xmtaddress;
			uint32 xmtdata;
			uint32 clockcontrolstatus;
			uint32 clockcontrolstatus_val;
		} base_regs;
		struct {
			uint32 resetctrl;
			uint32 extrsrcreq;
			uint32 ioctrl;
			uint32 ioctrl_resetseq_val[SSSR_D11_RESET_SEQ_STEPS];
		} wrapper_regs;
		uint32 sr_size;
	} mac_regs[MAX_NUM_D11_CORES_WITH_SCAN];
	struct {
		uint32 dig_sr_addr;
		uint32 dig_sr_size;
	} dig_mem_info;
} sssr_reg_info_v2_t;

typedef struct sssr_reg_info_v3 {
	uint16 version;
	uint16 length;  /* length of the structure validated at host */
	struct {
		struct {
			uint32 pmuintmask0;
			uint32 pmuintmask1;
			uint32 resreqtimer;
			uint32 macresreqtimer;
			uint32 macresreqtimer1;
			uint32 macresreqtimer2;
		} base_regs;
	} pmu_regs;
	struct {
		struct {
			uint32 intmask;
			uint32 powerctrl;
			uint32 clockcontrolstatus;
			uint32 powerctrl_mask;
		} base_regs;
	} chipcommon_regs;
	struct {
		struct {
			uint32 clockcontrolstatus;
			uint32 clockcontrolstatus_val;
		} base_regs;
		struct {
			uint32 resetctrl;
			uint32 extrsrcreq;
		} wrapper_regs;
	} arm_regs;
	struct {
		struct {
			uint32 ltrstate;
			uint32 clockcontrolstatus;
			uint32 clockcontrolstatus_val;
		} base_regs;
		struct {
			uint32 extrsrcreq;
		} wrapper_regs;
	} pcie_regs;
	struct {
		struct {
			uint32 xmtaddress;
			uint32 xmtdata;
			uint32 clockcontrolstatus;
			uint32 clockcontrolstatus_val;
		} base_regs;
		struct {
			uint32 resetctrl;
			uint32 extrsrcreq;
			uint32 ioctrl;
			uint32 ioctrl_resetseq_val[SSSR_D11_RESET_SEQ_STEPS];
		} wrapper_regs;
		uint32 sr_size;
	} mac_regs[MAX_NUM_D11_CORES_WITH_SCAN];
	struct {
		uint32 dig_sr_addr;
		uint32 dig_sr_size;
	} dig_mem_info;
	uint32 fis_enab;
	struct {
		struct {
			uint32 clkenable;
			uint32 clkgatingenable;
			uint32 clkext;
			uint32 clkctlstatus;
		} base_regs;
		struct {
			uint32 ioctrl;
			uint32 resetctrl;
		} wrapper_regs;
		uint32 hwa_resetseq_val[SSSR_HWA_RESET_SEQ_STEPS];
	} hwa_regs;
} sssr_reg_info_v3_t;

typedef struct sssr_reg_info_v4 {
	uint16 version;
	uint16 length;  /* length of the structure validated at host */
	struct {
		struct {
			uint32 pmuintmask0;
			uint32 pmuintmask1;
			uint32 resreqtimer;
			uint32 macresreqtimer;
			uint32 macresreqtimer1;
			uint32 macresreqtimer2;
		} base_regs;
	} pmu_regs;
	struct {
		struct {
			uint32 intmask;
			uint32 powerctrl;
			uint32 clockcontrolstatus;
			uint32 powerctrl_mask;
		} base_regs;
	} chipcommon_regs;
	struct {
		struct {
			uint32 clockcontrolstatus;
			uint32 clockcontrolstatus_val;
		} base_regs;
		struct {
			uint32 extrsrcreq;
		} oobr_regs;
	} arm_regs;
	struct {
		struct {
			uint32 ltrstate;
			uint32 clockcontrolstatus;
			uint32 clockcontrolstatus_val;
		} base_regs;
		struct {
			uint32 extrsrcreq;
		} oobr_regs;
	} pcie_regs;
	struct {
		struct {
			uint32 xmtaddress;
			uint32 xmtdata;
			uint32 clockcontrolstatus;
			uint32 clockcontrolstatus_val;
		} base_regs;
		struct {
			uint32 extrsrcreq;
		} oobr_regs;
		uint32 sr_size;
	} mac_regs[MAX_NUM_D11_CORES_WITH_SCAN];

	struct {
		struct {
			uint32 clockcontrolstatus;
			uint32 clockcontrolstatus_val;
		} base_regs;
		struct {
			uint32 extrsrcreq;
		} oobr_regs;

		uint32 saqm_sssr_addr;
		uint32 saqm_sssr_size;

		struct {
			uint32 digsr_srcontrol1_addr;	/* DIGSR engine sr control1 register */
			uint32 digsr_srcontrol1_clrbit_val; /* clear these bits from srcontrol1 */

			uint32 digsr_srcontrol2_addr;	/* DIGSR engine sr control2 register */
			uint32 digsr_srcontrol2_setbit_val; /* Value to set in the above address */

			uint32 pmuchip_ctl_addr_reg;
			uint32 pmuchip_ctl_val;
			uint32 pmuchip_ctl_data_reg;
			uint32 pmuchip_ctl_setbit_val;
		} sssr_config_regs;
	} saqm_sssr_info;

	struct {
		uint32 dig_sssr_addr;
		uint32 dig_sssr_size;
	} dig_mem_info;

	struct {
		uint32 fis_addr;
		uint32 fis_size;
	} fis_mem_info;

	/* Start address and end address for SSSR collection by host. */
	struct {
		uint32 sysmem_sssr_addr;
		uint32 sysmem_sssr_size;
	} sssr_all_mem_info;

	uint32 fis_enab;
} sssr_reg_info_v4_t;

typedef struct sssr_reg_info_v5 {
	uint16 version;
	uint16 length;  /* length of the structure validated at host */
	struct {
		struct {
			uint32 pmuintmask0;
			uint32 pmuintmask1;
			uint32 resreqtimer;
			uint32 macresreqtimer;
			uint32 macresreqtimer1;
			uint32 macresreqtimer2;
			uint32 pmu_min_res_mask;   /* Adress of min res mask address */
			uint32 pmu_max_res_mask;   /* Adress of max res mask address */
			uint32 sssr_max_res_mask; /* Max_res_mask value to be used for SSSR */
		} base_regs;
	} pmu_regs;
	struct {
		struct {
			uint32 intmask;
			uint32 powerctrl;
			uint32 clockcontrolstatus;
			uint32 powerctrl_mask;
		} base_regs;
	} chipcommon_regs;
	struct {
		struct {
			uint32 clockcontrolstatus;
			uint32 clockcontrolstatus_val;
		} base_regs;
		struct {
			uint32 extrsrcreq;
		} oobr_regs;
		uint32 war_reg;
	} arm_regs;
	struct {
		struct {
			uint32 ltrstate;
			uint32 clockcontrolstatus;
			uint32 clockcontrolstatus_val;
		} base_regs;
		struct {
			uint32 extrsrcreq;
		} oobr_regs;
	} pcie_regs;
	struct {
		struct {
			uint32 xmtaddress;
			uint32 xmtdata;
			uint32 clockcontrolstatus;
			uint32 clockcontrolstatus_val;
		} base_regs;
		struct {
			uint32 extrsrcreq;
		} oobr_regs;
		uint32 sr_size;
		uint32 war_reg;
	} mac_regs[MAX_NUM_D11_CORES_WITH_SCAN];

	struct {
		struct {
			uint32 clockcontrolstatus;
			uint32 clockcontrolstatus_val;
		} base_regs;
		struct {
			uint32 extrsrcreq;
		} oobr_regs;

		uint32 war_reg;
		uint32 saqm_sssr_addr;
		uint32 saqm_sssr_size;

		struct {
			uint32 digsr_srcontrol1_addr;	/* DIGSR engine sr control1 register */
			uint32 digsr_srcontrol1_clrbit_val; /* clear these bits from srcontrol1 */

			uint32 digsr_srcontrol2_addr;	/* DIGSR engine sr control2 register */
			uint32 digsr_srcontrol2_setbit_val; /* Value to set in the above address */

			uint32 pmuchip_ctl_addr_reg;
			uint32 pmuchip_ctl_val;
			uint32 pmuchip_ctl_data_reg;
			uint32 pmuchip_ctl_setbit_val;
		} sssr_config_regs;
	} saqm_sssr_info;

	struct {
		uint32 dig_sssr_addr;
		uint32 dig_sssr_size;
	} dig_mem_info;

	struct {
		uint32 fis_addr;
		uint32 fis_size;
	} fis_mem_info;

	/* Start address and end address for SSSR collection by host. */
	struct {
		uint32 sysmem_sssr_addr;
		uint32 sysmem_sssr_size;
	} sssr_all_mem_info;

	uint32 fis_enab;
	uint16 sr_asm_version; /* SR ASM version to help SSSR extraction scripts */
	uint16 PAD;

	struct {
		uint32 war_reg;
		uint32 srcb_sssr_addr;
		uint32 srcb_sssr_size;
	} srcb_mem_info;
} sssr_reg_info_v5_t;

typedef struct sssr_reg_info_v6 {
	uint16 version;
	uint16 length;  /* length of the structure validated at host */
	struct {
		struct {
			uint32 pmuintmask0;
			uint32 pmuintmask1;
			uint32 resreqtimer;
			uint32 macresreqtimer;
			uint32 macresreqtimer1;
			uint32 macresreqtimer2;
			uint32 pmu_min_res_mask;   /* Adress of min res mask address */
			uint32 pmu_max_res_mask;   /* Adress of max res mask address */
			uint32 sssr_max_res_mask; /* Max_res_mask value to be used for SSSR */
		} base_regs;
	} pmu_regs;
	struct {
		struct {
			uint32 intmask;
			uint32 powerctrl;
			uint32 clockcontrolstatus;
			uint32 powerctrl_mask;
		} base_regs;
	} chipcommon_regs;
	struct {
		struct {
			uint32 clockcontrolstatus;
			uint32 clockcontrolstatus_val;
		} base_regs;
		struct {
			uint32 extrsrcreq;
		} oobr_regs;
		uint32 war_reg;
	} arm_regs;
	struct {
		struct {
			uint32 ltrstate;
			uint32 clockcontrolstatus;
			uint32 clockcontrolstatus_val;
		} base_regs;
		struct {
			uint32 extrsrcreq;
		} oobr_regs;
	} pcie_regs;
	struct {
		struct {
			uint32 xmtaddress;
			uint32 xmtdata;
			uint32 clockcontrolstatus;
			uint32 clockcontrolstatus_val;
		} base_regs;
		struct {
			uint32 extrsrcreq;
		} oobr_regs;
		uint32 sr_size;
		uint32 war_reg;
	} mac_regs[MAX_NUM_D11_CORES_WITH_SCAN];

	struct {
		struct {
			uint32 clockcontrolstatus;
			uint32 clockcontrolstatus_val;
		} base_regs;
		struct {
			uint32 extrsrcreq;
		} oobr_regs;

		uint32 war_reg;
		uint32 saqm_sssr_addr;
		uint32 saqm_sssr_size;

		struct {
			uint32 digsr_srcontrol1_addr;	/* DIGSR engine sr control1 register */
			uint32 digsr_srcontrol1_clrbit_val; /* clear these bits from srcontrol1 */

			uint32 digsr_srcontrol2_addr;	/* DIGSR engine sr control2 register */
			uint32 digsr_srcontrol2_setbit_val; /* Value to set in the above address */

			uint32 pmuchip_ctl_addr_reg;
			uint32 pmuchip_ctl_val;
			uint32 pmuchip_ctl_data_reg;
			uint32 pmuchip_ctl_setbit_val;
		} sssr_config_regs;
	} saqm_sssr_info;

	struct {
		uint32 dig_sssr_addr;
		uint32 dig_sssr_size;
	} dig_mem_info;

	struct {
		uint32 fis_addr;
		uint32 fis_size;
	} fis_mem_info;

	/* Start address and end address for SSSR collection by host. */
	struct {
		uint32 sysmem_sssr_addr;
		uint32 sysmem_sssr_size;
	} sssr_all_mem_info;

	uint32 fis_enab;
	uint16 sr_asm_version; /* SR ASM version to help SSSR extraction scripts */
	uint16 PAD;

	struct {
		uint32 war_reg;
		uint32 srcb_sssr_addr;
		uint32 srcb_sssr_size;
	} srcb_mem_info;

	struct {
		uint32 vreg_addr;
		uint32 vreg_data_addr;
		uint32 vreg_num;
		uint32 vreg_offset;
	} pmu_dbug_rst_regs;
} sssr_reg_info_v6_t;

/* A wrapper structure for all versions of SSSR register information structures */
typedef union sssr_reg_info {
	sssr_reg_info_v0_t rev0;
	sssr_reg_info_v1_t rev1;
	sssr_reg_info_v2_t rev2;
	sssr_reg_info_v3_t rev3;
	sssr_reg_info_v4_t rev4;
	sssr_reg_info_v5_t rev5;
	sssr_reg_info_v6_t rev6;
} sssr_reg_info_cmn_t;

typedef struct sssr_header {
	uint32 magic; /* should be 53535352 = 'SSSR' */
	uint16 header_version; /* version number of this SSSR header */
	uint16 sr_version; /* version of SR version. This is to differentiate changes in SR ASM. */
	/*
	 * Header length from the next field ?data_len? and upto the start of
	 * binary_data[]. This is 20 bytes for version 0
	 */
	uint32 header_len;
	uint32 data_len;  /* number of bytes in binary_data[] */
	uint16 chipid;     /* chipid */
	uint16 chiprev;    /* chiprev */
	/*
	 * For D11 MAC/sAQM cores, the coreid, coreunit &  WAR_signature in the dump belong
	 * to respective cores. For the DIG SSSR dump these fields are extracted from the ARM core.
	 */
	uint16 coreid;
	uint16 coreunit;

	uint32 war_reg; /* Value of WAR register */
	uint32 flags;	/* For future use */

	uint8  binary_data[];
} sssr_header_t;
#define SSSR_HEADER_MAGIC 0x53535352u /* SSSR */

/* ADaptive Power Save(ADPS) structure definition */
#define WL_ADPS_IOV_MAJOR_VER	1
#define WL_ADPS_IOV_MINOR_VER	0
#define WL_ADPS_IOV_MAJOR_VER_SHIFT	8
#define WL_ADPS_IOV_VER \
	((WL_ADPS_IOV_MAJOR_VER << WL_ADPS_IOV_MAJOR_VER_SHIFT) | WL_ADPS_IOV_MINOR_VER)

#define ADPS_NUM_DIR	2
#define ADPS_RX		0
#define ADPS_TX		1

#define WL_ADPS_IOV_MODE	0x0001
#define WL_ADPS_IOV_RSSI	0x0002
#define WL_ADPS_IOV_DUMP	0x0003
#define WL_ADPS_IOV_DUMP_CLEAR	0x0004
#define WL_ADPS_IOV_SUSPEND	0x0005
#define WL_ADPS_IOV_GAIN        0x0006
#define WL_ADPS_IOV_RESET_GAIN  0x0007

#define ADPS_SUMMARY_STEP_NUM   2
#define ADPS_SUMMARY_STEP_LOW	0
#define ADPS_SUMMARY_STEP_HIGH	1

#define ADPS_SUB_IOV_VERSION_1	1
#define ADPS_SUB_IOV_VERSION_2	2

/* suspend/resume ADPS by wl/private command from host */
#define ADPS_RESUME	0u
#define ADPS_SUSPEND	1u

typedef struct wl_adps_params_v1 {
	uint16 version;
	uint16 length;
	uint8 band;		/* band - 2G or 5G */
	uint8 mode;		/* operation mode, default = 0 (ADPS disable) */
	uint16 padding;
} wl_adps_params_v1_t;

typedef struct wl_adps_rssi {
	int32 thresh_hi;	/* rssi threshold to resume ADPS operation */
	int32 thresh_lo;	/* rssi threshold to suspend ADPS operation */
} wl_adps_rssi_t;

typedef struct wl_adps_rssi_params_v1 {
	uint16 version;
	uint16 length;
	uint8 band;
	uint8 padding[3];
	wl_adps_rssi_t rssi;
} wl_adps_rssi_params_v1_t;

typedef struct adps_stat_elem {
	uint32 duration;	/* each step duration time (mSec) */
	uint32 counts;		/* each step hit count number */
} adps_stat_elem_t;

typedef struct wl_adps_dump_summary_v1 {
	uint16 version;
	uint16 length;
	uint8 mode;					/* operation mode: On/Off */
	uint8 flags;					/* restrict flags */
	uint8 current_step;				/* current step */
	uint8 padding;
	adps_stat_elem_t stat[ADPS_SUMMARY_STEP_NUM];	/* statistics */
} wl_adps_dump_summary_v1_t;

typedef struct wl_adps_dump_summary_v2 {
	uint16 version;
	uint16 length;
	uint8 mode;					/* operation mode: On/Off */
	uint8 current_step;				/* current step */
	uint8 padding[2];
	uint32 flags;					/* restrict flags */
	adps_stat_elem_t stat[ADPS_SUMMARY_STEP_NUM];	/* statistics */
} wl_adps_dump_summary_v2_t;

typedef struct wl_adps_suspend_v1 {
	uint16 version;
	uint16 length;
	uint8 suspend;			/* 1: suspend 0: resume */
	uint8 padding[3];
} wl_adps_suspend_v1_t;

/* For ADPS energy gain */
typedef struct wlc_adps_energy_gain_data {
	uint32	pm_dur_gain;	/* gain of pm duration by ADPS */
	uint32	step0_dur;	/* duration of ADPS STEP0(PM MAX + PSPOLL PRD) */
	uint32	wake_up_dur;	/* duration of wake up state  */
	uint32  PAD;
	uint64	tx_tot_bytes;	/* Total Tx bytes */
} wlc_adps_energy_gain_data_t;

typedef struct wl_adps_energy_gain_v1 {
	uint16 version;
	uint16 length;
	uint32 PAD;
	wlc_adps_energy_gain_data_t gain_data[MAX_BANDS];
} wl_adps_energy_gain_v1_t;

typedef struct wlc_btc_2gchain_dis {
	uint16 ver;
	uint16 len;
	uint8 chain_dis;
	uint8 flag;
} wlc_btc_2gchain_dis_t;

/* TDLS structure definition */
#define WL_TDLS_T_VERSION_V1	1
typedef struct wl_tdls_dump_summary_v1 {
	uint16 version;
	uint16 length;		/* length of the entire structure */
	uint32 txsetupreq;      /* tdls setup req sent */
	uint32 txsetupresp;     /* tdls setup resp sent */
	uint32 txsetupcfm;      /* tdls setup confirm sent */
	uint32 txteardown;      /* tdls teardwon frames sent */
	uint32 txptireq;	/* tdls pti req frames sent */
	uint32 txptiresp;	/* tdls pti resp frames sent */
	uint32 txchswreq;	/* tdls chsw req frames sent */
	uint32 txchswresp;	/* tdls chsw resp frame sent */
	uint32 rxsetupreq;      /* tdls setup req rcvd */
	uint32 rxdsetupresp;    /* tdls setup resp rcvd */
	uint32 rxsetupcfm;      /* tdls setup confirm rcvd */
	uint32 rxteardown;      /* tdls teardown frames rcvd */
	uint32 rxptireq;	/* tdls pti req frames rcvd */
	uint32 rxptiresp;	/* tdls pti resp frames rcvd */
	uint32 rxchswreq;	/* tdls chsw req frames rcvd */
	uint32 rxchswresp;	/* tdls chsw resp frames rcvd */
	uint32 discard;         /* frames discarded due to full buffer */
	uint32 ubuffered;       /* frames buffered by TDLS txmod */
	uint32 buf_reinserted;  /* frames reinserted */
	uint32 idletime;	/* time since no traffic on tdls link */
	uint32 uptime;		/* time since  tdls link connected */
	uint32 tx_cnt;		/* frames txed over tdls link */
	uint32 rx_cnt;		/* frames rcvd over tdls link */
	uint32 blist_cnt;	/* number of tdls black list */
	uint32 scb_flags;	/* connected tdls scb flags */
	struct ether_addr peer_addr;	/* connected peer addr */
	uint8 padding[2];
} wl_tdls_dump_summary_v1_t;

#define WLC_BTC_2GCHAIN_DIS_REASSOC	0x1
#define WLC_BTC_2GCHAIN_DIS_VER1	0x1
#define WLC_BTC_2GCHAIN_DIS_VER1_LEN	6

/* --- BTCX WiFi Protection (btc_wifi_prot iovar) --- */

#define WL_BTC_WIFI_PROT_VER_1	1

typedef struct wl_btc_wifi_prot_v1 {
	uint16 ver;	/* version */
	uint16 len;	/* total length */
	uint8 data[];	/* bcm_xtlv_t payload */
} wl_btc_wifi_prot_v1_t;

/* Xtlv tags (protection type) and data */
#define WL_BTC_WIFI_PROT_M1_M4 1
typedef struct wl_btc_wifi_prot_m1_m4 {
	uint32 enable;	/* enable/disable m1-m4 protection */
	uint32 timeout;	/* maximum timeout in ms (0: default) */
} wl_btc_wifi_prot_m1_m4_t;


#define WL_BTC_WIFI_PROT_VER_2	2u

/* Enable/Disable protection and allowed tasks bitmap values */
#define WL_BTC_WIFI_PROT_ENABLE			(1 << 0u)
#define WL_BTC_WIFI_PROT_DISABLE		(0 << 0u)
#define WL_BTC_WIFI_PROT_BTTASK_ESCO		(1 << 1u)
#define WL_BTC_WIFI_PROT_BTTASK_AOS		(1 << 2u)
#define WL_BTC_WIFI_PROT_BTTASK_UHP		(1 << 3u)
#define WL_BTC_WIFI_PROT_BTTASK_A2DP		(1 << 4u)
#define WL_BTC_WIFI_PROT_BTTASK_LE_AUDIO	(1 << 5u)

/* M1M4 Tag version as part of tag structure */
#define WL_BTC_WIFI_PROT_M1M4_TAG_V1	1u
typedef struct wl_btc_wifi_prot_m1_m4_v2 {
	uint16 ver;			/* sub version of m1m4 tag */
	uint16 enable_2g;		/* BT task map and prot enable - 2G */
	uint16 enable_5g;		/* BT task map and prot enable - 5G */
	uint16 timeout_2g;		/* Max timeout in ms for 2g (0: default) */
	uint16 timeout_5g;		/* Max timeout in ms for 5g (0: default) */
	uint16 PAD;
} wl_btc_wifi_prot_m1_m4_v2_t;

/* --- End BTCX WiFi Protection --- */

/* --- BTCX ULMU disable (btc_ulmu_config iovar) --- */

/* Version number */
#define WL_BTC_ULMU_CONFIG_VER_1 1
typedef struct wl_btc_ulmu_config_v1 {
	uint16 version;			/* btc_ulmu_config version */
	uint16 len;			/* Total length */
	uint32 ulmu_bt_task_bm;		/* BT Task bimtap for ULMU disable */
	uint32 ulmu_bt_period_th;	/* BT period thresh for ULMU disable */
} wl_btc_ulmu_config_v1_t;

/* --- End BTCX ULMU config --- */

#define WL_BTC_LESCAN_TOTAL_MAX		15u

/* --- BTC ACK Counters (btc_ack_counters legacy command) --- */

#define BTC_ACK_CNTS_VER_1	1u
#define BTC_ACK_CNTS_VER_2	2u

typedef struct wl_btc_ack_cnts_v1 {
	uint16 ver;
	uint16 len;
	uint8 slice_index;
	uint8 PAD[3];
	uint32 cdd_ack_cnt;
	uint32 siso_ack_cnt;
	uint32 c0_ack_txpwr;
	uint32 c1_ack_txpwr;
} wl_btc_ack_cnts_v1_t;

typedef struct wl_btc_ack_cnts_v2 {
	uint16 ver;
	uint16 len;
	uint8 slice_index;
	uint8 PAD[3];
	uint32 cdd_ack_cnt;
	uint32 siso_ack_cnt;
	uint32 c0_ack_txpwr;
	uint32 c1_ack_txpwr;
	uint32 cdd_ack_bt_cnt;
} wl_btc_ack_cnts_v2_t;

typedef enum wl_btc_ack_cnts_command_e {
	ACK_CNTS_CMD_CLEAR = 0u, /* To clear BTC Ack counters */
} wl_btc_ack_cnts_command_t;

typedef struct wl_btc_ack_cnts_config_v1 {
	uint16 ver;	/* Command structure version */
	uint16 len;	/* Command structure length */
	uint8 cmd;	/**< 0 - clear */
	uint8 PAD[3];
} wl_btc_ack_cnts_config_v1_t;

/* --- End BTC ACK Counters --- */

/* --- BTC STATUS (coex status from both 2G and 5G slices) --- */
#define BTC_STATUS_VER_1	1u

typedef struct wlc_btc_status_v1 {
	uint16 ver;
	uint16 len;
	uint32 btc_status_5g;
	uint32 btc_status_2g;
	uint8 PAD[4]; /* For future enhancements */
} wlc_btc_status_v1_t;
/* --- End BTC STATUS --- */

#define RPSNOA_IOV_MAJOR_VER_1 1u
#define RPSNOA_IOV_MINOR_VER_1 1u
#define RPSNOA_IOV_MAJOR_VER_SHIFT 8u
#define RPSNOA_IOV_VERSION_1_1 \
	((RPSNOA_IOV_MAJOR_VER_1 << RPSNOA_IOV_MAJOR_VER_SHIFT) | RPSNOA_IOV_MINOR_VER_1)

enum wl_rpsnoa_cmd_ids {
	WL_RPSNOA_CMD_ENABLE = 1,
	WL_RPSNOA_CMD_STATUS,
	WL_RPSNOA_CMD_PARAMS,
	WL_RPSNOA_CMD_LAST
};

typedef struct rpsnoa_cmnhdr {
	uint16 ver;		/* cmd structure version */
	uint16 len;		/* cmd structure len */
	uint32 subcmd;
	uint32 cnt;
} rpsnoa_cmnhdr_t;

typedef struct rpsnoa_data {
	int16 band;
	int16 value;
} rpsnoa_data_t;

typedef struct rpsnoa_stats {
	int16 band;
	int16 state;
	uint32 sleep_dur;
	uint32 sleep_avail_dur;
	uint32 last_pps;
} rpsnoa_stats_t;

typedef struct rpsnoa_param {
	uint16 band;
	uint8 level;
	uint8 stas_assoc_check;
	uint32 pps;
	uint32 quiet_time;
} rpsnoa_param_t;

typedef struct rpsnoa_iovar {
	rpsnoa_cmnhdr_t hdr;
	rpsnoa_data_t data[BCM_FLEX_ARRAY];
} rpsnoa_iovar_t;

typedef struct rpsnoa_iovar_status {
	rpsnoa_cmnhdr_t hdr;
	rpsnoa_stats_t stats[BCM_FLEX_ARRAY];
} rpsnoa_iovar_status_t;

typedef struct rpsnoa_iovar_params {
	rpsnoa_cmnhdr_t hdr;
	rpsnoa_param_t param[BCM_FLEX_ARRAY];
} rpsnoa_iovar_params_t;

#define CSA_EVT_CSA_RXED        (1 << 0)
#define CSA_EVT_CSA_TIMEOUT     (1 << 1)
#define CSA_EVT_FROM_INFRA      (1 << 2)
typedef struct csa_event_data {
	chanspec_t chan_old;
	dot11_ext_csa_ie_t ecsa;
	dot11_mesh_csp_ie_t mcsp;
	dot11_wide_bw_chan_switch_ie_t wbcs;
	uint8 flags;
	uint8 PAD[3];
} csa_event_data_t;

/* ifdef (WL_ASSOC_BCN_RPT) */
enum wl_bcn_report_cmd_id {
	WL_BCN_RPT_CMD_VER		= 0,
	WL_BCN_RPT_CMD_CONFIG		= 1,
	WL_BCN_RPT_CMD_VENDOR_IE	= 2,
	WL_BCN_RPT_CMD_LAST
};

/* beacon report specific macros */
#define WL_BCN_RPT_CCX_IE_OVERRIDE	(1u << 0)

/* beacon report specific macros */
#define WL_BCN_RPT_ASSOC_SCAN_UNSOLICITED_MODE	(1u << 1)
#define WL_BCN_RPT_ASSOC_SCAN_SOLICITED_MODE	(1u << 2)
#define WL_BCN_RPT_ASSOC_SCAN_MODE_SHIFT	(1)
#define WL_BCN_RPT_ASSOC_SCAN_MODE_MASK		(WL_BCN_RPT_ASSOC_SCAN_UNSOLICITED_MODE |\
							WL_BCN_RPT_ASSOC_SCAN_SOLICITED_MODE)
#define WL_BCN_RPT_ASSOC_SCAN_MODE_MAX		(WL_BCN_RPT_ASSOC_SCAN_MODE_MASK >> \
							WL_BCN_RPT_ASSOC_SCAN_MODE_SHIFT)
/* beacon report mode specific macro */
#define WL_BCN_RPT_ASSOC_SCAN_MODE_DEFAULT	WL_BCN_RPT_ASSOC_SCAN_UNSOLICITED_MODE

/* beacon report timeout config specific macros */
#define WL_BCN_RPT_ASSOC_SCAN_CACHE_TIMEOUT_DEFAULT	(120000)
#define WL_BCN_RPT_ASSOC_SCAN_CACHE_TIMEOUT_MIN		(60000)
#define WL_BCN_RPT_ASSOC_SCAN_CACHE_TIMEOUT_MAX		(0xFFFFFFFF)

/* beacon report cache count specific macros */
#define WL_BCN_RPT_ASSOC_SCAN_CACHE_COUNT_MIN		(0)
#define WL_BCN_RPT_ASSOC_SCAN_CACHE_COUNT_MAX		(8)
#define WL_BCN_RPT_ASSOC_SCAN_CACHE_COUNT_DEFAULT	(WL_BCN_RPT_ASSOC_SCAN_CACHE_COUNT_MAX)

#define WL_BCN_REPORT_CMD_VERSION_V1	1

typedef struct wl_bcn_report_cfg_v1 {
	uint32	flags;			/**< Flags that defines the operation/setting information */
	uint32	scan_cache_timeout;	/**< scan cache timeout value in millisec */
	uint32	scan_cache_timer_pend;	/**< Read only pending time for timer expiry in millisec */
	uint8	scan_cache_cnt;		/**< scan cache count */
	uint8	pad[3];
} wl_bcn_report_cfg_v1_t;

/* Thermal, Voltage, and Power Mitigation */
#define TVPM_REQ_VERSION_1		1u

/* tvpm iovar data */
typedef struct {
	uint16	version;		/* TVPM request version */
	uint16	length;			/* Length of the entire structure */

	uint16	req_type;		/* Request type: wl_tvpm_req_type_t */
	uint16	req_len;		/* Length of the following value */
	uint8	value[];		/* Variable length data depending on req_type */
} wl_tvpm_req_t;

/* tvpm iovar request types */
typedef enum {
	WL_TVPM_REQ_CLTM_INDEX,	/* req_value: uint32, range 1...100 */
	WL_TVPM_REQ_PPM_INDEX,	/* req_value: uint32, range 1...100 */
	WL_TVPM_REQ_ENABLE,	/* req_value: uint32, range 0...1 */
	WL_TVPM_REQ_STATUS,	/* req_value: none */
	WL_TVPM_REQ_PERIOD,	/* req_value: int32, range {-1,1-10} */
	WL_TVPM_REQ_TXDC,	/* req_value: uint32, range 1...100  */
	WL_TVPM_REQ_VBAT,	/* req_value: uint32, range 1...48   */
	WL_TVPM_REQ_MAX
} wl_tvpm_req_type_t;

/* structure for data returned by request type WL_TVPM_REQ_STATUS */
typedef struct wl_tvpm_status {
	uint16	enable;			/* whether TVPM is enabled */
	uint16	tx_dutycycle;		/* a percentage: 1-100 */
	int16	tx_power_backoff;	/* 0...-6 */
	uint16	num_active_chains;	/* 1...3 */
	int16	temp;			/* local temperature in degrees C */
	uint8	vbat;			/* local voltage in units of 0.1V */
	uint8	PAD;
} wl_tvpm_status_t;

/* TVPM ecounters */
typedef struct wl_tvpm_ecounters_t {
	uint16	version;		/* version field */
	uint16	length;			/* byte length in wl_tvpm_ecounters_t starting at version */
	uint16	tx_dutycycle;		/* a percentage: 1-100 */
	int16	tx_power_backoff;	/* 0...-6 */
	uint16	num_active_chains;	/* 1...3 */
	int16	temp;			/* local temperature */
	uint8	vbat;			/* local voltage */
	uint8	cltm;			/* CLTM index  */
	uint8	ppm;			/* PPM index  */
	uint8	PAD;			/* pad to align to uint16 */
} wl_tvpm_ecounters_t;


#define TDMTX_ECOUNTERS_VERSION_V1      1
#define TDMTX_ECOUNTERS_VERSION_V2      2


/* TDMTX ecounters */
typedef struct wl_tdmtx_ecounters_v1 {
	uint16  version;	/* version field */
	uint16	length;	/* byte length in wl_tdmtx_ecounters_t starting at version */
	uint32	txa_on; /* TXA on requests */
	uint32	txa_tmcnt; /* Total number of TXA timeout */
	uint32	por_on; /* TXA POR requests */
	uint32	txpuen; /* Path enable requests */
	uint32	txpudis; /* Total number of times Tx path is muted on the slice */
	uint32	txpri_on; /* Total number of times Tx priority was obtained by the slice */
	uint32	txdefer; /* Total number of times Tx was deferred by the slice */
	uint32	txmute; /* Total number of times active Tx muted on the slice */
	uint32	actpwrboff; /* Total number of times TX power is backed off by the slice */
	uint32	txa_dur; /* Total time txa on */
	uint32	txpri_dur; /* Total time TXPri */
	uint32	txdefer_dur; /* Total time txdefer */
} wl_tdmtx_ecounters_v1_t;

/* TDMTX ecounters for version 2 */
typedef struct wl_tdmtx_ecounters_v2 {
	uint16  version;	/* version field */
	uint16	length;	/* byte length in wl_tdmtx_ecounters_t starting at version */
	uint32	txa_on; /* TXA on requests */
	uint32	txa_tmcnt; /* Total number of TXA timeout */
	uint32  porhi_on; /* TXA PORHI requests */
	uint32  porlo_on; /* TXA PORLO requests */
	uint32	txpuen; /* Path enable requests */
	uint32	txpudis; /* Total number of times Tx path is muted on the slice */
	uint32	txpri_on; /* Total number of times Tx priority was obtained by the slice */
	uint32	txdefer; /* Total number of times Tx was deferred by the slice */
	uint32	txmute; /* Total number of times active Tx muted on the slice */
	uint32	actpwrboff; /* Total number of times TX power is backed off by the slice */
	uint32	txa_dur; /* Total time txa on */
	uint32	txpri_dur; /* Total time TXPri */
	uint32	txdefer_dur; /* Total time txdefer */
} wl_tdmtx_ecounters_v2_t;

/* Note: if this struct is changing update wl_scb_ecounters_vX_t version,
 * as this struct is sent as payload in wl_scb_ecounters_vX_t
 */
typedef struct wlc_scb_stats_v1 {
	uint32 tx_pkts;			    /* num of packets transmitted (ucast) */
	uint32 tx_failures;		    /* num of packets failed */
	uint32 rx_ucast_pkts;		    /* num of unicast packets received */
	uint32 rx_mcast_pkts;		    /* num of multicast packets received */
	uint32 tx_rate;			    /* Rate of last successful tx frame */
	uint32 rx_rate;			    /* Rate of last successful rx frame */
	uint32 rx_decrypt_succeeds;	    /* num of packets decrypted successfully */
	uint32 rx_decrypt_failures;	    /* num of packets decrypted unsuccessfully */
	uint32 tx_mcast_pkts;		    /* num of mcast pkts txed */
	uint32 PAD;
	uint64 tx_ucast_bytes;		    /* data bytes txed (ucast) */
	uint64 tx_mcast_bytes;		    /* data bytes txed (mcast) */
	uint64 rx_ucast_bytes;		    /* data bytes recvd ucast */
	uint64 rx_mcast_bytes;		    /* data bytes recvd mcast */
	uint32 tx_pkts_retried;		    /* num of packets where a retry was necessary */
	uint32 tx_pkts_retry_exhausted;	    /* num of packets where a retry was exhausted */
	uint32 tx_rate_mgmt;		    /* Rate of last transmitted management frame */
	uint32 tx_rate_fallback;	    /* last used lowest fallback TX rate */
	uint32 rx_pkts_retried;		    /* # rx with retry bit set */
	uint32 tx_pkts_total;		    /* total num of tx pkts */
	uint32 tx_pkts_retries;		    /* total num of tx retries */
	uint32 tx_pkts_fw_total;	    /* total num of tx pkts generated from fw */
	uint32 tx_pkts_fw_retries;	    /* num of fw generated tx pkts retried */
	uint32 tx_pkts_fw_retry_exhausted;  /* num of fw generated tx pkts where retry exhausted */
} wlc_scb_stats_v1_t;

/* ecounters for scb stats
 * XTLV ID: WL_IFSTATS_XTLV_SCB_ECOUNTERS
 */

#define WL_SCB_ECOUNTERS_VERSION_1		1
#define WL_SCB_ECOUNTERS_VERSION_2		2

typedef struct wl_scb_ecounters_v1 {
	uint16	version;	/* version field */
	uint16	length;		/* struct length starting from version */
	uint32	chanspec;	/* current chanspec where scb is operating */
	struct  ether_addr  ea; /* peer ndi or sta ea */
	uint8	peer_type;	/* peer type */
	uint8	PAD;

	/* scb tx and rx stats */
	wlc_scb_stats_v1_t stats;
} wl_scb_ecounters_v1_t;

typedef struct wl_scb_ecounters_v2 {
	uint16	version;	/* version field */
	uint16	length;		/* struct length starting from version */
	uint32	chanspec;	/* current chanspec where scb is operating */
	struct  ether_addr  ea; /* peer ndi or sta ea */
	uint8	peer_type;	/* peer type */
	uint8	PAD;

	/* scb tx and rx stats */
	uint16 tx_rate;			    /* Rate(in Mbps) of last successful tx frame */
	uint16 rx_rate;			    /* Rate(in Mbps) of last successful rx frame */
	uint16 tx_rate_fallback;	    /* last used lowest fallback TX rate(in Mbps) */
	uint16 PAD;
	uint32 rx_decrypt_succeeds;	    /* num of packets decrypted successfully */
	uint32 rx_decrypt_failures;	    /* num of packets decrypted unsuccessfully */
	uint32 rx_pkts_retried;		    /* # rx with retry bit set */
	uint32 tx_pkts_retries;		    /* total num of tx retries */
	uint32 tx_failures;		    /* num of packets failed */
	uint32 tx_pkts_total;		    /* total num of tx pkts */
	int8   rssi[WL_STA_ANT_MAX];	    /* average rssi per antenna of data frames */
} wl_scb_ecounters_v2_t;

/* ecounters for nan slot stats
 * XTLV ID: WL_IFSTATS_XTLV_NAN_SLOT_STATS
 */

#define WL_NAN_SLOT_ECOUNTERS_VERSION_1		1
#define WL_NAN_SLOT_ECOUNTERS_VERSION_2		2
#define WL_NAN_SLOT_ECOUNTERS_VERSION_3		3
#define WL_NAN_SLOT_ECOUNTERS_VERSION_4		4

typedef struct wl_nan_slot_ecounters_v1 {
	uint16	version;	      /* version field */
	uint16	length;		      /* struct length starting from version */
	uint32	chan[NAN_MAX_BANDS];  /* cur nan slot chanspec of both bands */
	uint16	cur_slot_idx;	      /* cur nan slot index */
	uint16  PAD;
	nan_sched_stats_t sched;      /* sched stats */
	wl_nan_mac_stats_t mac;	      /* mac stats */
} wl_nan_slot_ecounters_v1_t;

typedef struct wl_nan_slot_ecounters_v2 {
	uint16	version;	      /* version field */
	uint16	length;		      /* struct length starting from version */
	uint32	chan[NAN_MAX_BANDS];  /* cur nan slot chanspec of both bands */
	uint16	cur_slot_idx;	      /* cur nan slot index */
	uint16  PAD;
	nan_sched_stats_t sched;      /* sched stats */
	wl_nan_mac_stats_t mac;	      /* mac stats */
	/* for v2 */
	uint16 bcn_rx_drop_rssi;      /* Beacon received but ignored due to weak rssi */
	uint16 bcn_rx_drop_rssi_5g;   /* 5G Beacon received but ignored due to weak rssi */
	uint16 cnt_rssi_close;	      /* cnt of beacon rssi > rssi_close received */
	uint16 cnt_rssi_close_5g;     /* cnt of 5G beacon rssi > rssi_close received */
	uint16 cnt_rssi_mid;	      /* cnt of beacon rssi > rssi_middle received */
	uint16 cnt_rssi_mid_5g;	      /* cnt of 5G beacon rssi > rssi_middle received */
	uint16 bcn_txfail;	      /* Beacon sending failure count */
	uint16 bcn_txfail_5g;	      /* sending 5G beacon failure count */
} wl_nan_slot_ecounters_v2_t;

typedef struct wl_nan_slot_ecounters_v3 {
	uint16	version;	      /* version field */
	uint16	length;		      /* struct length starting from version */
	uint32	chan[NAN_MAX_BANDS];  /* cur nan slot chanspec of both bands */
	uint16	cur_slot_idx;	      /* cur nan slot index */
	uint16  PAD;
	nan_sched_stats_t sched;      /* sched stats */
	/* for v3 */
	wl_nan_mac_stats_v1_t mac;      /* mac stats */
	uint16 bcn_rx_drop_rssi;      /* Beacon received but ignored due to weak rssi */
	uint16 bcn_rx_drop_rssi_5g;   /* 5G Beacon received but ignored due to weak rssi */
	uint16 cnt_rssi_close;	      /* cnt of beacon rssi > rssi_close received */
	uint16 cnt_rssi_close_5g;     /* cnt of 5G beacon rssi > rssi_close received */
	uint16 cnt_rssi_mid;	      /* cnt of beacon rssi > rssi_middle received */
	uint16 cnt_rssi_mid_5g;	      /* cnt of 5G beacon rssi > rssi_middle received */
	uint16 bcn_txfail;	      /* Beacon sending failure count */
	uint16 bcn_txfail_5g;	      /* sending 5G beacon failure count */
} wl_nan_slot_ecounters_v3_t;

typedef struct wl_nan_slot_ecounters_v4 {
	uint16	version;	      /* version field */
	uint16	length;		      /* struct length starting from version */
	uint32	chan[NAN_MAX_BANDS];  /* cur nan slot chanspec of both bands */
	uint16	cur_slot_idx;	      /* cur nan slot index */
	uint16  PAD;
	nan_sched_stats_t sched;      /* sched stats */
	/* for v4 */
	wl_nan_mac_stats_v2_t mac;      /* mac stats */
	uint16 bcn_rx_drop_rssi;      /* Beacon received but ignored due to weak rssi */
	uint16 bcn_rx_drop_rssi_5g;   /* 5G Beacon received but ignored due to weak rssi */
	uint16 cnt_rssi_close;	      /* cnt of beacon rssi > rssi_close received */
	uint16 cnt_rssi_close_5g;     /* cnt of 5G beacon rssi > rssi_close received */
	uint16 cnt_rssi_mid;	      /* cnt of beacon rssi > rssi_middle received */
	uint16 cnt_rssi_mid_5g;	      /* cnt of 5G beacon rssi > rssi_middle received */
	uint16 bcn_txfail;	      /* Beacon sending failure count */
	uint16 bcn_txfail_5g;	      /* sending 5G beacon failure count */
} wl_nan_slot_ecounters_v4_t;

/* WL_STATS_XTLV_NDP_SESSION_STATUS for ecounters */
#define WL_NAN_SESSION_STATUS_EC_VERSION_1  1
typedef struct wl_nan_ndp_session_status_v1_s {
	uint16	version;	      /* version field */
	uint16	length;		      /* struct length starting from version */
	uint8	role;		      /* Role of NAN device */
	uint8	ndp_id;		      /* local NDP ID */
	uint8	state;		      /* NDP state */
	uint8	nan_sec_csid;	      /* security csid */
	struct	ether_addr lndi_addr; /* Local NDI addr */
	struct	ether_addr pnmi_addr; /* Peer NMI addr */
	struct	ether_addr pndi_addr; /* Peer NDI addr */
	uint8	dpe_state;	      /* DPE state to know where timeout/dpend has come */
	uint8	PAD;
} wl_nan_ndp_session_status_v1_t;

/* WL_STATS_XTLV_NAN_DISC_FRM_STATUS for ecounters */
#define WL_NAN_DISC_FRM_STATUS_EC_VERSION_1  1
typedef struct wl_nan_disc_frame_status_v1_s {
	uint16	version;	      /* version field */
	uint16	length;		      /* struct length starting from version */
	uint8	type;		      /* wl_nan_frame_type_t */
	uint8	status;		      /* For TX status, success or failure */
	uint8	reason_code;	      /* to identify reason when status is failure */
	uint8	inst_id;	      /* Publish or subscribe instance id */
	uint8	req_id;		      /* Requestor instance id */
	uint8	PAD;
	uint16	token;		      /* seq num to keep track of pkts sent by host */
} wl_nan_disc_frame_status_v1_t;

/* keepalive oob ops */
typedef enum {
	NAN_KA_OOB_OPS_ADD,
	NAN_KA_OOB_OPS_DEL,
	NAN_KA_OOB_OPS_GET,
	NAN_KA_OOB_OPS_MAX
} nan_ka_oob_ops_t;

/* NAN KEEPALIVE(KA) oob flags */
typedef enum {
	NAN_KA_OOB_FLAG_LEGACY,
	NAN_KA_OOB_FLAG_PER_PEER_TX,
	NAN_KA_OOB_FLAG_PER_PEER_RX,
	NAN_KA_OOB_FLAG_MAX
} nan_ka_oob_flag_t;

/* NAN KEEPALIVE(KA) oob param */
typedef struct wl_nan_ka_oob_af_param {
	uint8 ops;			/* add/del/get */
	uint8 flags;			/* per peer tx/ rx keepalive oob or legacy oob */
} wl_nan_ka_oob_af_param_t;

typedef struct wl_nan_oob_af {
	uint64 bitmap;			/* 16 TU slots in 1024 TU window */
	struct ether_addr sa;		/* Optional SA. Default set to NMI */
	struct ether_addr da;
	struct ether_addr bssid;
	bool   secured;			/* Optional. Default set to 0 (Open) */
	uint8  map_id;			/* Host selected map id. Default 0 */
	uint16 timeout;			/* OOB AF session timeout in milliseconds */
	wl_nan_ka_oob_af_param_t koap;	/* Keep alive oob af additional param */
	uint16 PAD[2];			/* Structure padding. Can be used in future */
	uint16 token;			/* host generated. Used by FW in TX status event */
	uint16 payload_len;
	uint8  payload[];		/* AF hdr + NAN attrbutes in TLV format */
} wl_nan_oob_af_t;

typedef struct wl_nan_oob_rx_match_payload {
	uint16 offset;		/* the offset from the start of data field
				 * in nan_attr_type_t which is the part of RX pkt
				 */
	uint16 match_length;	/* Length to be used for match */
	uint8 payload[];	/* Payload to be used for match */
} wl_nan_oob_rx_match_payload_t;

/*
 * BT log definitions
 */

/* common iovar struct */
typedef struct wl_btl {
	uint16 subcmd_id;	/* subcommand id */
	uint16 len;		/* total length of data[] */
	uint8 data[2];		/* subcommand data, variable length */
} wl_btl_t;

/* subcommand ids */
#define WL_BTL_SUBCMD_ENABLE	0	/* enable/disable logging */
#define WL_BTL_SUBCMD_STATS	1	/* statistics */

/* WL_BTL_SUBCMD_ENABLE data */
typedef struct wl_blt_enable {
	uint8 enable;			/* 1 - enable, 0 - disable */
	uint8 PAD[3];			/* 4-byte struct alignment */
} wl_btl_enable_t;

/* WL_BTL_SUBCMD_STATS data */
typedef struct wl_blt_stats {
	uint32 bt_interrupt;			/* num BT interrupts */
	uint32 config_req;			/* num CONFIG_REQ */
	uint32 config_res_success;		/* num CONFIG_RES successful */
	uint32 config_res_fail;			/* num CONFIG_RES failed */
	uint32 log_req;				/* num LOG_REQ */
	uint32 log_res_success;			/* num LOG_RES successful */
	uint32 log_res_fail;			/* num LOG_RES failed */
	uint32 indirect_read_fail;		/* num indirect read fail */
	uint32 indirect_write_fail;		/* num indirect write fail */
	uint32 dma_fail;			/* num DMA failed */
	uint32 min_log_req_duration;		/* min log request duration in usec */
	uint32 max_log_req_duration;		/* max log request duration in usec */
	uint16 mem_dump_req;			/* num mem dump requests */
	uint16 mem_dump_success;		/* num mem dumps successful */
	uint16 mem_dump_fail;			/* num mem dumps failed */
	uint16 bt_wake_success;			/* num BT wakes successful */
	uint16 bt_wake_fail;			/* num BT wakes failed */
	uint16 mem_dump_req_interrupt;		/* num MEM_DUMP_REQ interrupt */
	uint16 mem_dump_res_interrupt;		/* num MEM_DUMP_RES interrupt */
	uint16 mem_dump_res_timeout;		/* num MEM_DUMP_RES timeout */
	uint16 mem_dump_proc_no_bt_ready;	/* num proceed if no BT ready */
	uint16 mem_dump_proc_no_bt_response;	/* num proceed if no BT response */
	uint16 mem_dump_proc_no_bt_clock;	/* num proceed if no BT clock */
	uint16 PAD;				/* alignment */
	uint32 last_failed_region;		/* start addr of last failed region */
	uint32 min_mem_dump_duration;		/* min mem dump duration in usec */
	uint32 max_mem_dump_duration;		/* max mem dump duration in usec */
} wl_btl_stats_t;

/* IOV AWD DATA */

/* AWD DATA structures */
typedef struct {
	uint8 version;	/* Extended trap version info */
	uint8 reserved;	/* currently unused */
	uint16 length;	/* Length of data excluding this header */
	uint8 data[];	/* this data is TLV of tags */
} awd_data_v1_t;

/* AWD TAG structure */
typedef struct {
	uint8 tagid;	/* one of AWD DATA TAGs numbers */
	uint8 length;	/* the data size represented by this field must be aligned to 32 bits */
	uint8 data[];	/* variable size, defined by length field */
} awd_tag_data_v1_t;

/* IOV ETD DATA */

/* ETD DATA structures */
typedef struct {
	uint8 version;	/* Extended trap version info */
	uint8 reserved;	/* currently unused */
	uint16 length;	/* Length of data excluding this header */
	uint8 data[];	/* this data is TLV of tags */
} etd_data_v1_t;

/* ETD TAG structure */
typedef struct {
	uint8 tagid;	/* one of ETD DATA TAGs numbers */
	uint8 length;	/* the data size represented by this field must be aligned to 32 bits */
	uint8 data[];	/* variable size, defined by length field */
} etd_tag_data_v1_t;

/* ETD information structures associated with ETD_DATA_Tags */
/* ETD_JOIN_CLASSIFICATION_INFO 10 */
typedef struct {
	uint8 assoc_type;		/* assoc type */
	uint8 assoc_state;		/* current state of assoc state machine */
	uint8 wpa_state;		/* wpa->state */
	uint8 wsec_portopen;		/* shows if security port is open */
	uint8 total_attempts_num;	/* total number of join attempts (bss_retries) */
	uint8 num_of_targets;		/* up to 3, in current design */
	uint8 reserved [2];		/* padding to get 32 bits alignment */
	uint32 wsec;			/* bsscfg->wsec */
	uint32 wpa_auth;		/* bsscfg->WPA_auth */
	uint32 time_to_join;		/* time duration to process WLC_SET_SSID request (ms) */
} join_classification_info_v1_t;

/* ETD_JOIN_TARGET_CLASSIFICATION_INFO 11 */
typedef struct {
	int8 rssi;			/* RSSI on current channel */
	uint8 cca;			/* CCA on current channel */
	uint8 channel;			/* current channel */
	uint8 num_of_attempts;		/* (bss_retries) up to 5 */
	uint8 oui[3];			/* the first three octets of the AP's address */
	uint8 reserved;			/* padding to get 32 bits alignment */
	uint32 time_duration;		/* time duration of current attempt (ms) */
} join_target_classification_info_v1_t;

/* ETD_ASSOC_STATE 12 */
typedef struct {
	uint8 assoc_state;		/* assoc type */
	uint8 reserved [3];		/* padding to get 32 bits alignment */
} join_assoc_state_v1_t;

/* ETD_CHANNEL 13 tag */
typedef struct {
	uint8 channel;			/* last attempt channel */
	uint8 reserved [3];		/* padding to get 32 bits alignment */
} join_channel_v1_t;

/* ETD_TOTAL_NUM_OF_JOIN_ATTEMPTS 14 */
typedef struct {
	uint8 total_attempts_num;	/* total number of join attempts (bss_retries) */
	uint8 reserved [3];		/* padding to get 32 bits alignment */
} join_total_attempts_num_v1_t;

/* IOV_ROAM_CACHE structures */

enum wl_rmc_report_cmd_id {
	WL_RMC_RPT_CMD_VER	= 0,
	WL_RMC_RPT_CMD_DATA	= 1,
	WL_RMC_RPT_CMD_LAST
};

enum wl_rmc_report_xtlv_id {
	WL_RMC_RPT_XTLV_VER			= 0x0,
	WL_RMC_RPT_XTLV_BSS_INFO		= 0x1,
	WL_RMC_RPT_XTLV_CANDIDATE_INFO		= 0x2,
	WL_RMC_RPT_XTLV_USER_CACHE_INFO		= 0x3,
	WL_RMC_RPT_XTLV_CANDIDATE_INFO_V2	= 0x4,
	WL_RMC_RPT_XTLV_USER_CACHE_INFO_V2	= 0x5,
	WL_RMC_RPT_XTLV_CANDIDATE_INFO_V3	= 0x6,
	WL_RMC_RPT_XTLV_USER_CACHE_INFO_V3	= 0x7
};

/* WL_RMC_RPT_XTLV_BSS_INFO */
typedef struct {
	int16 rssi;		/* current BSS RSSI */
	uint8 reason;		/* reason code for last full scan */
	uint8 status;		/* last status code for not roaming */
	uint32 fullscan_count;	/* number of full scans performed on current BSS */
	uint32 time_full_scan;	/* delta time (in ms) between cur time and full scan timestamp */
} rmc_bss_info_v1_t;

/* WL_RMC_RPT_XTLV_CANDIDATE_INFO */
typedef struct {
	int16 rssi;		/* last seen rssi */
	uint16 ctl_channel;	/* channel */
	uint32 time_last_seen;	/* delta time (in ms) between cur time and last seen timestamp */
	uint16 bss_load;	/* BSS load */
	uint8 bssid[6];		/* BSSID */
} rmc_candidate_info_v1_t;

/* WL_RMC_RPT_XTLV_CANDIDATE_INFO_V2 */
typedef struct {
	int16 rssi;             /* last seen rssi */
	chanspec_t ctl_channel; /* chanspec of primary channel */
	uint32 time_last_seen;  /* delta time (in ms) between cur time and last seen timestamp */
	uint16 bss_load;        /* BSS load */
	struct ether_addr bssid; /* BSSID */
} rmc_candidate_info_v2_t;

#define MAX_NUM_NPLINKS_V3	2u /* Max number of Non Preferred Links */
/* WL_RMC_RPT_XTLV_CANDIDATE_INFO_V3 */
typedef struct {
	int16 rssi;             /* last seen rssi */
	chanspec_t ctl_channel; /* chanspec of preferred link */
	uint32 time_last_seen;  /* delta time (in ms) between cur time and last seen timestamp */
	uint16 bss_load;        /* BSS load of the preferred link */
	struct ether_addr bssid;	/* BSSID of the preferred link */
	struct ether_addr mld_addr;	/* MLD BSSID */
	/* Account other linked AP chanspecs in case of MLO
	* Already one of the link is updated in ctl_channel.
	*/
	chanspec_t linked_chnls[MAX_NUM_NPLINKS_V3];
	uint8 reserved[2];	/* padding to get 4 bytes alignment after unpacking */
} rmc_candidate_info_v3_t;

enum wl_filter_ie_options {
	WL_FILTER_IE_CLEAR		= 0,	/* allow  element id in packet.For suboption */
	WL_FILTER_IE_SET		= 1,	/* filter element id in packet.For suboption */
	WL_FILTER_IE_LIST		= 2,	/* list  element ID's.Set as option */
	WL_FILTER_IE_CLEAR_ALL		= 3,	/* clear all the element.Set as option */
	WL_FILTER_IE_CHECK_SUB_OPTION	= 4	/* check for suboptions.Set only as option */
};

typedef struct wl_filter_ie_tlv {
	uint16	id;		/* elelment id [ + ext id ] */
	uint16	len;		/* sub option length + pattern length */
	uint8	data[];		/* sub option + pattern matching(OUI,type,sub-type) */
} wl_filter_ie_tlv_t;

#define WL_FILTER_IE_VERSION_1	1 /* the latest version */
typedef struct wl_filter_ie_iov_v1 {
	uint16	version;	/* Structure version */
	uint16	len;		/* Total length of the structure */
	uint16	fixed_length;	/* Total length of fixed fields */
	uint8	option;		/* Filter action - check for suboption */
	uint8	PAD[1];		/* Align to 4 bytes */
	uint32	pktflag;	/* frame type - FC_XXXX */
	uint8	tlvs[];		/* variable data (zero in for list ,clearall) */
} wl_filter_ie_iov_v1_t;

#define  WL_ASSOC_RESP_PARAMS_V1 (1u)
typedef struct wl_assoc_resp_params_v1 {
	uint16	version;			/* Structure version */
	uint16	len;				/* Total length of the structure */
	uint16	fixed_length;			/* Total length of fixed fields */
	uint8	mac_addr[ETHER_ADDR_LEN];	/* peer MAC address */
	uint8	resp_ie_len;			/* Assoc_resp IE's length */
	uint8	pad;				/* Pad for alignment */
	uint16	status;				/* AREQ status code from Host */
	uint8	ies[];				/* Variable data (resp_ies) */
} wl_assoc_resp_params_v1_t;

/* Event aggregation config */
#define EVENT_AGGR_CFG_VERSION		1
#define EVENT_AGGR_DISABLED		0x0
#define EVENT_AGGR_ENABLED		0x1

#define EVENT_AGGR_BUFSIZE_MAX		1512
#define EVENT_AGGR_BUFSIZE_MIN		512

#define EVENT_AGGR_FLUSH_TIMEOUT_DEFAULT	100
#define EVENT_AGGR_FLUSH_TIMEOUT_MAX		2000
#define EVENT_AGGR_NUM_EVENTS_FLUSH		5
typedef struct event_aggr_config {
	uint16 version;
	uint16 len;
	uint16 flags;			/* bit 0 to enable/disable the feature */
	uint16 bufsize;			/* Aggregate buffer size */
	uint16 flush_timeout;		/* Timeout for event flush */
	uint16 num_events_flush;	/* Number of events aggregated before flush */
} event_aggr_config_t;


/** chanctxt related statistics */
#define CHANCTXT_STATS_VERSION_1 1u
typedef struct wlc_chanctxt_stats {
	uint32  excursionq_end_miss;
	uint32	activeq_end_miss;
	uint32	no_chanctxt_count;
	uint32	txqueue_end_incomplete;
	uint32	txqueue_start_incomplete;
} wlc_chanctxt_stats_core_t;

typedef struct chanctxt_stats {
	uint16	version;
	uint16	length;
	wlc_chanctxt_stats_core_t corestats[MAX_NUM_D11CORES];
} wlc_chanctxt_stats_t;

typedef struct wl_txdc_ioc {
	uint8 ver;
	uint8 id;	/* ID of the sub-command */
	uint16 len;	/* total length of all data[] */
	uint8  data[];	/* var len payload */
} wl_txdc_ioc_t;

/*
 * iovar subcommand ids
 */
enum {
	IOV_TXDC_ENB = 1,
	IOV_TXDC_MODE = 2,
	IOV_TXDC_DUMP = 3,
	IOV_TXDC_LAST
};

/* WL_NAN_XTLV_SLOT_STATS */
/* WL_NAN_EVENT_SLOT_START, WL_NAN_EVENT_SLOT_END */
typedef struct nan_slot_event_data {
	uint32 cur_slot_idx; /* current idx in channel schedule */
	uint32 fw_time; /* target current time in microseconds */
	uint32 band; /* current band (2G/5G) for which the event is received */
} nan_slot_event_data_t;


/* Block Channel */
#define WL_BLOCK_CHANNEL_VER_1	1u

typedef struct wl_block_ch_v1 {
	uint16 version;
	uint16 len;
	uint32 band;		/* Band select */
	uint8 channel_num;	/* The number of block channels in the selected band */
	uint8 padding[3];
	uint8 channel[];	/* Channel to block, Variable Length */
} wl_block_ch_v1_t;

typedef struct dma_wl_addr_region {
	uint32 addr_low;
	uint32 addr_high;
} dma_wl_addr_region_t;


#define WL_ROAMSTATS_IOV_VERSION_1 1u

#define MAX_PREV_ROAM_EVENTS   16u

#define ROAMSTATS_UNKNOWN_CNT  0xFFFFu

/* roaming statistics counter structures */
typedef struct wlc_assoc_roamstats_event_msg_v1 {
	uint32  event_type;             /* Message (see below) */
	uint32  status;                 /* Status code (see below) */
	uint32  reason;                 /* Reason code (if applicable) */
	uint32  timestamp;              /* Timestamp of event */
} wlc_assoc_roamstats_event_msg_v1_t;

enum wl_roamstats_cmd_id {
	WL_ROAMSTATS_XTLV_CMD_VER = 0,
	WL_ROAMSTATS_XTLV_CMD_RESET = 1,
	WL_ROAMSTATS_XTLV_CMD_STATUS = 2,
	WL_ROAMSTATS_XTLV_CMD_LAST      /* Keep this at the end */
};

enum wl_roamstats_xtlv_id {
	WL_ROAMSTATS_XTLV_VER           = 0x0,
	WL_ROAMSTATS_XTLV_COUNTER_INFO          = 0x1,
	WL_ROAMSTATS_XTLV_PREV_ROAM_EVENTS      = 0x2,
	WL_ROAMSTATS_XTLV_REASON_INFO           = 0x3
};

/* WL_ROAMSTATS_XTLV_COUNTER_INFO */
typedef struct {
	uint32 initial_assoc_time;
	uint32 prev_roam_time;
	uint32 host_access_time;
	uint16 roam_success_cnt;
	uint16 roam_fail_cnt;
	uint16 roam_attempt_cnt;
	uint16 max_roam_target_cnt;
	uint16 min_roam_target_cnt;
	uint16 max_cached_ch_cnt;
	uint16 min_cached_ch_cnt;
	uint16 partial_roam_scan_cnt;
	uint16 full_roam_scan_cnt;
	uint16 PAD;
} roamstats_counter_info_v1_t;

/* WL_ROAMSTATS_XTLV_PREV_ROAM_EVENTS */
typedef struct {
	uint16 max;
	uint16 pos;
	wlc_assoc_roamstats_event_msg_v1_t roam_event[];
} roamstats_prev_roam_events_v1_t;

/* WL_ROAMSTATS_XTLV_REASON_INFO */
typedef struct {
	uint16 max;
	uint16 reason_cnt[];
} roamstats_reason_info_v1_t;

#ifdef HEALTH_CHECK_WLIOCTL
/* Health check status format:
 * reporting status size = uint32
 * 8 LSB bits are reserved for: WARN (0), ERROR (1), and other levels
 * MSB 24 bits are reserved for client to fill in its specific status
 */
#define HEALTH_CHECK_STATUS_OK			0
/* Bit positions. */
#define HEALTH_CHECK_STATUS_WARN		0x1
#define HEALTH_CHECK_STATUS_ERROR		0x2
#define HEALTH_CHECK_STATUS_TRAP		0x4
#define HEALTH_CHECK_STATUS_NOEVENT		0x8

/* Indication that required information is populated in log buffers */
#define HEALTH_CHECK_STATUS_INFO_LOG_BUF	0x80
#define HEALTH_CHECK_STATUS_MASK		(0xFF)

#define HEALTH_CHECK_STATUS_MSB_SHIFT		8
#endif /* HEALTH_CHECK_WLIOCTL */

/** receive signal reporting module interface */

#define WL_RXSIG_IOV_MAJOR_VER_1       (1u)
#define WL_RXSIG_IOV_MINOR_VER_1       (1u)
#define WL_RXSIG_IOV_MAJOR_VER_SHIFT (8u)
#define WL_RXSIG_IOV_VERSION_1_1 \
	((WL_RXSIG_IOV_MAJOR_VER_1 << WL_RXSIG_IOV_MAJOR_VER_SHIFT) | WL_RXSIG_IOV_MINOR_VER_1)
#define WL_RXSIG_IOV_GET_MAJOR(x)    (x >> WL_RXSIG_IOV_MAJOR_VER_SHIFT)
#define WL_RXSIG_IOV_GET_MINOR(x)    (x & 0xFF)

enum wl_rxsig_cmd_rssi_mode {
	WL_RXSIG_MODE_DB =   0x0,
	WL_RXSIG_MODE_QDB =  0x1,
	WL_RXSIG_MODE_LAST
};

/* structure defs for 'wl rxsig [cmd]' iovars */
enum wl_rxsig_iov_v1 {
	WL_RXSIG_CMD_RSSI		= 0x1,	/**< combined rssi moving avg */
	WL_RXSIG_CMD_SNR		= 0x2,	/**< combined snr moving avg */
	WL_RXSIG_CMD_RSSIANT		= 0x3,	/**< rssi moving avg per-ant */
	WL_RXSIG_CMD_SNRANT		= 0x4,	/**< snr moving avg per-snr */
	WL_RXSIG_CMD_SMPLWIN		= 0x5,	/**< config for sampling window size */
	WL_RXSIG_CMD_SMPLGRP		= 0x7,	/**< config for grouping of pkt type */
	WL_RXSIG_CMD_STA_MA		= 0x8,
	WL_RXSIG_CMD_MAMODE		= 0x9,
	WL_RXSIG_CMD_MADIV		= 0xa,
	WL_RXSIG_CMD_DUMP		= 0xb,
	WL_RXSIG_CMD_DUMPWIN		= 0xc,
	WL_RXSIG_CMD_RSSI_EXP		= 0xd,	/**< config for expiry time in ms */
	WL_RXSIG_CMD_RSSI_COMP		= 0xe,	/**< config for rssi compensation threshold */
	WL_RXSIG_CMD_RSSI_COMP_TBL	= 0xf,	/**< config for rssi compensation table */
	WL_RXSIG_CMD_TOTAL
};

struct wl_rxsig_cfg_v1 {
	uint16 version;
	chanspec_t chan;       /**< chanspec info for querying stats */
	uint8 pmac[ETHER_ADDR_LEN];       /**< peer(link) mac address */
};

struct wl_rxsig_iov_rssi_v1 {
	int8 rssi;
	uint8 rssi_qdb;
	uint8 PAD[2];
};

struct wl_rxsig_iov_snr_v1 {
	int16 snr;
	uint16 PAD;
};

struct wl_rxsig_iov_rssi_ant_v1 {
	int8 deci[WL_RSSI_ANT_MAX];
	uint8 frac[WL_RSSI_ANT_MAX];
	uint8 rssi_mode;       /**< MODE_DB or MODE_QDB */
	uint8 num_of_ant;      /**< total number of ants */
	uint8 PAD[2];          /**< padding for 32bit align */
};

#define WL_RXSIG_RSSI_COMP_TBL_V1       (1u)
struct wl_rxsig_rssi_comp_tbl_v1 {
	uint16 version;		/**< version */
	uint8 num_steps;	/**< number of RSSI steps for gradual compensation per BW.
				 * The total length of tbl[] must be multiple of this value
				 * Example: 3xnum_steps is needed to support 40, 80 and 160MHz
				 */
	uint8 pad;
	uint8 tbl[];		/**< compensation value tables - starting on 32-bit alignment */
};

#ifdef BCM_SDC

#define SDC_TRIGGER_CONFIG_VER_1	1
typedef struct {
	uint16  version;
	uint16	type;
	uint8	activate;
	uint8	PAD;
} sdc_trigger_cfg_t;

typedef enum sdc_trigger_types {
	SDC_TYPE_STA_ONBOARD_DEBUG = 1,
	SDC_TYPE_SCAN_DEBUG = 2,
#ifdef SDC_TEST
	/*
	 * This is for test purpose only. Don't assign specific value.
	 * Keep at the end
	 */
	SDC_TYPE_TEST1,
	SDC_TYPE_TEST2,
	SDC_TYPE_TEST3,
#endif /* SDC_TEST */
	SDC_TYPE_MAX_TRIGGER
} sdc_trigger_types_t;

/* *** SDC_TYPE_STA_ONBOARD_DEBUG specific ******* */

/* tlv IDs uniquely identifies tx and rx stats component */
enum wl_slice_hist_stats_xtlv_id {
	WL_STATE_HIST_TX_TOSS_REASONS = 0x1,
	WL_STATE_HIST_RX_TOSS_REASONS = 0x2
};

#ifndef WLC_HIST_TOSS_LEN
#define WLC_HIST_TOSS_LEN   (8u)
#endif
#define WL_HIST_COMPACT_TOSS_STATS_TX_VER_1	(1u)
#define WL_HIST_COMPACT_TOSS_STATS_RX_VER_1	(1u)

/* Format of running toss reasons with seq
 * [see  HIST_TOSS_xxxx macros]
 * bits [7..0]	: 8 bits : toss sts.
 *	[11..8]	: cfgidx
 *	[15..12]: ac
 *	[31..16]: seq
 */
#define HIST_TOSS_STS_POS	(0u)
#define HIST_TOSS_STS_MASK	(0x000000ffu)
#define HIST_TOSS_CFGIDX_POS	(8u)
#define HIST_TOSS_CFGIDX_MASK	(0x00000f00u)
#define HIST_TOSS_AC_POS	(12u)
#define HIST_TOSS_AC_MASK	(0x0000f000u)
#define HIST_TOSS_SEQ_POS	(16u)
#define HIST_TOSS_SEQ_MASK	(0xffff0000u)

/* Format of toss reasons with count
 * bits [15..0]	: 16 bits : toss reason
 * bits [31..16]: 16 bits : count
 */
#define HIST_TOSS_RC_REASON_POS		(0u)
#define HIST_TOSS_RC_REASON_MASK	(0xffffu)
#define HIST_TOSS_RC_COUNT_POS		(16u)
#define HIST_TOSS_RC_COUNT_MASK		(0xffff0000u)

typedef struct {
	uint16	version;
	uint8	hist_toss_type;		/* from wl_slice_hist_XX_stats_xtlv_id */
	uint8	hist_toss_num;		/* number of elements in hist_toss_xxx */
	uint32	hist_toss_cur_idx;	/* latest data is in this index */
	uint32	hist_toss_reasons[WLC_HIST_TOSS_LEN];	/* last 8 reasons along with seq, etc as
							 * per HIST_TOSS_xxx format
							 */
	uint32	hist_toss_counts[WLC_HIST_TOSS_LEN];	/* toss counts corr to reasons */
} wl_hist_compact_toss_stats_v1_t;

#define WL_HIST_COMPACT_TOSS_STATS_TX_VER_2	(2u)
#define WL_HIST_COMPACT_TOSS_STATS_RX_VER_2	(2u)

typedef struct {
	uint16	version;
	uint8	htr_type;	/* from wl_slice_hist_XX_stats_xtlv_id */
	uint8	htr_num;	/* number of elements in htr_running or htr_rc */
	uint16	htr_rnidx;	/* htr_running[rnidx-1] has latest data */
	uint16	htr_rcidx;	/* htr_rc[rcidx-1] has latest data */
	uint32	htr_running[WLC_HIST_TOSS_LEN];	/* last 8 reasons along with seq, etc as
						 * per WLC_SDC_COMPACT_TOSS_REASON() format
						 */
	uint32	htr_rn_ts[WLC_HIST_TOSS_LEN]; /* time stamps corr to htr_running data */
	uint32	htr_rc[WLC_HIST_TOSS_LEN];	/* last 8 toss reasons and counts in
						 * WLC_SDC_COMPACT_TOSS_RC() format
						 */
	uint32	htr_rc_ts[WLC_HIST_TOSS_LEN]; /* time stamps corr to htr_rc */
} wl_hist_compact_toss_stats_v2_t;

#define WL_HIST_COMPACT_TOSS_STATS_TX_VER_3	(3u)
#define WL_HIST_COMPACT_TOSS_STATS_RX_VER_3	(3u)

typedef struct {
	uint8	toss_reason;
	uint8	cfg_ac;
	uint16	toss_seq;
} toss_info_t;

typedef struct {
	uint16	toss_reason;
	uint16	toss_cnt;
} toss_cnt_t;

typedef struct {
	uint16		version;
	uint8		htr_type;	/* from wl_slice_hist_XX_stats_xtlv_id */
	uint8		htr_num;	/* number of elements in htr_running or htr_rc */
	uint16		htr_rnidx;	/* htr_running[rnidx-1] has latest data */
	uint16		htr_rcidx;	/* htr_rc[rcidx-1] has latest data */
	toss_info_t	htr_running[WLC_HIST_TOSS_LEN];	/* last 8 reasons along with seq, etc as
						 * per WLC_SDC_COMPACT_TOSS_REASON() format
						 */
	uint32		htr_rn_ts[WLC_HIST_TOSS_LEN]; /* time stamps corr to htr_running data */
	toss_cnt_t	htr_rc[WLC_HIST_TOSS_LEN];	/* last 8 toss reasons and counts in
						 * WLC_SDC_COMPACT_TOSS_RC() format
						 */
	uint32	htr_rc_ts[WLC_HIST_TOSS_LEN]; /* time stamps corr to htr_rc */
} wl_hist_compact_toss_stats_v3_t;

/* ***END of SDC_TYPE_STA_ONBOARD_DEBUG specific ******* */

#endif /* BCM_SDC */

typedef struct wl_avs_info_v1 {
	uint16 version;		/* Structure version */
	uint16 equ_version;	/* Equation Version */
	uint32 RO;		/* RO in OTP */
	uint32 equ_csr;		/* Equated CSR */
	uint32 read_csr;	/* Read Back CSR */
	uint32 aging;		/* aging setting in nvram */
} wl_avs_info_v1_t;

/* The value is read from AVSCapabilities.AVSHistogramDepth in 4390a0/4399b0 */
#define AVS_HIST_ENTRY_NUM	8

typedef struct wl_avs_hist_entry_s {
	uint32 time;		/* in ms */
	uint32 voltage_ndv;	/* in mV */
} wl_avs_hist_entry_t;

typedef struct wl_avs_info_v2 {
	uint16 version;		/* Structure version */
	uint16 length;		/* length of structure */
	int16 ro_margin_idx;	/* -1 when avsdump_internal=1 is not in nvram */
	uint16 voltage_ndv;	/* current NDV voltage in mV */
	uint16 voltage_ldv;	/* current LDV voltage in mV */
	uint8 dvfs_state;	/* Current DVFS state */
	uint8 hist_entry_num;	/* 0 when avsdump_internal=1 is not in nvram */
	wl_avs_hist_entry_t hist[AVS_HIST_ENTRY_NUM];
} wl_avs_info_v2_t;

#define WL_AVS_INFO_VER_1	1
#define WL_AVS_INFO_VER_2	2

#define DVFS_STATE_LDV		0u
#define DVFS_STATE_NDV		1u

/* SC (scan core) command IDs */
enum wl_sc_cmd {
	WL_SC_CMD_DBG = 0,
	WL_SC_CMD_CNX = 1,
	WL_SC_CMD_CAP = 2,
	WL_SC_CMD_CONFIG = 3,
	WL_SC_CMD_PMALERT_ADJ_FACTOR = 4,
	WL_SC_CMD_FEMASK = 5,
	WL_SC_CMD_MSCHANS = 6,
	WL_SC_CMD_LAST
};

/* WBUS sub-command IDs for unit test */
#define WL_WBUS_INA_SLOT_START                0x01u /**< Inactive slot start sub command ID. */
#define WL_WBUS_INA_SLOT_STOP                 0x02u /**< Inactive slot stop sub command ID. */

/* WBUS (WiFi BT uniform scheduler) command IDs */
enum wl_wbus_cmd {
	WL_WBUS_CMD_VER = 0,
	WL_WBUS_CMD_STATS = 1,
	WL_WBUS_CMD_UNIT_TEST = 2,
	WL_WBUS_CMD_BT_TEST = 3,
	WL_WBUS_CMD_CAP = 4,
	WL_WBUS_CMD_COEX_CFG = 5,
	WL_WBUS_CMD_LAST
};

#define WBUS_BT_SCHED_TEST_PARAMS_VER_1	1

typedef struct wbus_bt_sched_test_params_v1 {
	uint16	version;
	uint8	min_duty_cycle;
	uint8	type;
	uint32	flags;
	uint32	action;
	uint32	duration;
	uint32	interval;
} wbus_bt_sched_test_params_v1_t;

enum wl_wbus_bt_test_type {
	WBUS_TEST_BT_USER_TYPE_LE_SCAN		= 0u,
	WBUS_TEST_BT_USER_TYPE_PAGE_SCAN	= 1u,
	WBUS_TEST_BT_USER_TYPE_MAX		= 2u
};

#define WBUS_BT_SCHED_ADD	0u
#define WBUS_BT_SCHED_REMOVE	1u
#define WBUS_BT_SCHED_INVALID	0xFFu

enum wlc_btcec_iocv_subcmds {
	WL_BTCEC_SUBCMD_TEST = 0,
	WL_BTCEC_SUBCMD_STATS = 1,
	WL_BTCEC_SUBCMD_TEST_BTMC_MODE = 2,
	WL_BTCEC_SUBCMD_LAST
};

/* btcec - bt schedule id's */
typedef enum {
	BTCEC_SCHED_ID_LE_SCAN		= 1,
	BTCEC_SCHED_ID_PAGE_SCAN	= 2
} BTCEC_SCHED_IDS;
/* max schedule id must be equal to last valid schedule id */
#define BTCEC_SCHED_MAX_V1	BTCEC_SCHED_ID_PAGE_SCAN

/* meanings of flags */
/* bit0: schedule with the given sch_id is present in btcec */
#define BTCEC_SCHSTATS_FLAG_ACTIVE		(0x1)
/* bit1: schedule with the given sch_id is successfully registered with wbus */
#define BTCEC_SCHSTATS_FLAG_WBUS_REG_OK		(0x2)
/* bit2: schedule with the given sch_id is being scheduled in wbus */
#define BTCEC_SCHSTATS_FLAG_WBUS_SCHEDULED	(0x4)

/* v1 supports pmdur only */
#define WLC_BTCEC_STATS_V1	1u
typedef struct wlc_btcec_stats_v1 {
	uint16 ver;
	uint16 len;
	uint32 pmdur;	/* duration in millisec granted for bt corr to which wlsc slept */
} wlc_btcec_stats_v1_t;

/* v2 supports 1 page scan and 1 le scan */
#define WLC_BTCEC_STATS_V2	2u
/* btcec per schedule stats for general reporting */
typedef struct wlc_btcec_sch_stats_v2 {
	uint8	sch_id;		/* schedule id */
	uint8	flags;		/* flags. see BTCEC_SCHSTATS_FLAG_xxx */
	uint8	PAD[2];
	uint32	slcnt;		/* num slots granted to bt */
	uint32	skip;		/* count of btsc le/page scan, skipped */
	uint32	btdur;		/* duration in millisec granted for bt corr to which wlsc slept */
	uint32	overlap;	/* wlsc was awake and btsc le/page scan overlapped, in ms */
	uint32	txblnk;		/* wlauxtx blanked btsc le/page scan, in ms */
} wlc_btcec_sch_stats_v2_t;

/* btcec stats for general reporting */
typedef struct wlc_btcec_stats_v2 {
	uint16	ver;
	uint16	len;
	uint32	rx_msg_cnt;	/* received messages counter */
	uint32	tx_msg_cnt;	/* transmitted messages counter */
	uint32	add_msg_cnt;	/* rx add messages counter */
	uint32	del_msg_cnt;	/* rx del messages counter */
	uint32	stats_dur;	/* dur in sec for which stats is accumulated */
	uint8	PAD[3];
	uint8	num_sstats;	/* number of elements in sstats struct */
	wlc_btcec_sch_stats_v2_t sstats[];	/* sch specific stats */
} wlc_btcec_stats_v2_t;

/* v3 = v2 + ac_overlap */
#define WLC_BTCEC_STATS_V3	3u
/* btcec per schedule stats for general reporting */
typedef struct wlc_btcec_sch_stats_v3 {
	uint8	sch_id;		/* schedule id */
	uint8	flags;		/* flags. see BTCEC_SCHSTATS_FLAG_xxx */
	uint8	PAD[2];
	uint32	slcnt;		/* num slots granted to bt */
	uint32	skip;		/* count of btsc le/page scan, skipped */
	uint32	btdur;		/* duration in millisec granted for bt corr to which wlsc slept */
	uint32	overlap;	/* wlsc was awake and btsc le/page scan overlapped, in ms */
	uint32	txblnk;		/* wlauxtx blanked btsc le/page scan, in ms */
	uint32	ac_overlap;	/* wlaux overlapped btsc le/page scan, in ms */
	uint16	sched_duration;	/* duration finally scheduled in msch, in ms */
	uint16	sched_interval;	/* interval finally scheduled in msch, in ms */
	uint16	req_duration;	/* duration finally scheduled in msch, in ms */
	uint16	req_interval;	/* interval finally scheduled in msch, in ms */
	uint16	min_duty_cycle;	/* interval finally scheduled in msch, in ms */
	uint16	PAD;
} wlc_btcec_sch_stats_v3_t;

/* btcec stats for general reporting */
typedef struct wlc_btcec_stats_v3 {
	uint16	ver;
	uint16	len;
	uint32	rx_msg_cnt;	/* received messages counter */
	uint32	tx_msg_cnt;	/* transmitted messages counter */
	uint32	add_msg_cnt;	/* rx add messages counter */
	uint32	del_msg_cnt;	/* rx del messages counter */
	uint32	stats_dur;	/* dur in sec for which stats is accumulated */
	uint8	PAD[3];
	uint8	num_sstats;	/* number of elements in sstats struct */
	wlc_btcec_sch_stats_v3_t sstats[];	/* sch specific stats */
} wlc_btcec_stats_v3_t;

/* btcec periodic ecounters structs
 *  [similar to wlc_btcec_stats_vX_t, but constrained in size due to its periodicity of reporting]
 */
#define WLC_BTCEC_PERIODIC_CNTRS_V1	(1u)

/* btcec per schedule stats for periodic ecounters reporting */
typedef struct wlc_btcec_periodic_sch_stats_v1 {
	uint8	sch_id;		/* schedule id from BTCEC_SCHED_IDS */
	uint8	flags;		/* flags. see BTCEC_SCHSTATS_FLAG_xxx */
	uint8	slcnt;		/* num slots granted to bt */
	uint8	skip;		/* count of btsc le/page scan, skipped */
	uint16	btdur;		/* duration in millisec granted for bt corr to which wlsc slept */
	uint16	overlap;	/* wlsc was awake and btsc le/page scan overlapped, in ms */
	uint16	txblnk;		/* wlauxtx blanked btsc le/page scan, in ms */
	uint16	ac_overlap;	/* wlaux overlapped btsc le/page scan, in ms */
	uint16	sched_duration;	/* duration finally scheduled in msch, in ms */
	uint16	sched_interval;	/* interval finally scheduled in msch, in ms */
	uint16	req_duration;	/* duration finally scheduled in msch, in ms */
	uint16	req_interval;	/* interval finally scheduled in msch, in ms */
	uint16	min_duty_cycle;	/* interval finally scheduled in msch, in ms */
	uint16	PAD;
} wlc_btcec_periodic_sch_stats_v1_t;

/* btcec stats for periodic ecounters reporting */
typedef struct {
	uint16	ver;
	uint16	len;
	uint8	rx_msg_cnt;	/* received messages counter */
	uint8	tx_msg_cnt;	/* transmitted messages counter */
	uint8	add_msg_cnt;	/* rx add messages counter */
	uint8	del_msg_cnt;	/* rx del messages counter */
	uint8	PAD[3];
	uint8	num_sstats;	/* number of elements in sstats struct */
	wlc_btcec_periodic_sch_stats_v1_t sstats[BTCEC_SCHED_MAX_V1];	/* sch specific */
} wlc_btcec_periodic_stats_v1_t;

#define WLC_BTCEC_PERIODIC_CNTRS_V2	(2u)
typedef struct {
	uint16	ver;
	uint16	len;
	uint8	rx_msg_cnt;	/* received messages counter */
	uint8	tx_msg_cnt;	/* transmitted messages counter */
	uint8	add_msg_cnt;	/* rx add messages counter */
	uint8	del_msg_cnt;	/* rx del messages counter */
	uint8	btmc_cnt;	/* btmc state check count */
	uint8	fb_schupd_cnt;	/* fixed bit:schedule update count */
	uint8	fb_btmc_cnt;	/* fixed bit:btmc update count */
	uint8	num_sstats;	/* number of elements in sstats struct */
	uint32	fixed_bits_lsb;	/* state of gci fixed bits */
	uint32	btmc_active_ts;	/* last btmc active timestamp in ms */
	uint32	btmc_idle_ts;	/* last btmc idle timestamp in ms */
	wlc_btcec_periodic_sch_stats_v1_t sstats[BTCEC_SCHED_MAX_V1];	/* sch specific */
} wlc_btcec_periodic_stats_v2_t;

#define WBUS_OFFLOAD_STATS_V1		1u
#define WBUS_OFFLOAD_USER_STATS_V1	1u

typedef struct wbus_offload_user_stats_v1 {
	uint16	version;			/* version of this structure */
	uint16	len;				/* size of this structure */
	uint8	type;				/* Offload type */
	uint8	PAD[3];
	uint32	num_accepted;			/* num of times user got accepted */
	uint32	num_rejected;			/* num of times user got rejected */
	uint32	num_failed;			/* num of times user accept failed */
} wbus_offload_user_stats_v1_t;

typedef struct wbus_offload_stats_v1 {
	uint16	version;				/* version of this structure */
	uint16	len;					/* size of this structure */
	uint32	num_accept_ok;				/* num accept pass */
	uint32	num_accept_fail;			/* num accept fail */
	uint32	num_rejected;				/* num of rejected users so far */
	uint32	num_rejected_bt;			/* num of rejected users so far for BT */
	uint32	num_rejected_all;			/* num times all offloads are rejected */
	uint8	PAD[3];
	uint8	num_user;				/* num of users stats */
	wbus_offload_user_stats_v1_t user_stats[];	/* per user stats */
} wbus_offload_stats_v1_t;

#define KEY_UPDATE_INFO_VER_V1	1
typedef struct key_update_info_v1
{
	uint16 ver;
	uint8 pad;
	uint8 flags;
	uint32 timestamp;
	uint32 algo;
	uint32 key_flags;
	struct ether_addr ea;
	struct ether_addr sa;
} key_update_info_v1_t;

/* Key update flag bit field */
#define KEY_UPD_FLAG_ADD_KEY 0x1 /* 0 - Removal, 1 - Add key */


#define WL_OMI_CONFIG_VERSION_1	1u
#define WL_OMI_CONFIG_VERSION_2	2u

/* values for valid_bm */
#define OMI_CONFIG_VALID_BMP_RXNSS			0x0001u
#define OMI_CONFIG_VALID_BMP_BW				0x0002u
#define OMI_CONFIG_VALID_BMP_ULMU_DISABLE		0x0004u
#define OMI_CONFIG_VALID_BMP_TXNSTS			0x0008u
#define OMI_CONFIG_VALID_BMP_ERSU_DISABLE		0x0010u
#define OMI_CONFIG_VALID_BMP_DLMU_RSD_RCM		0x0020u
#define OMI_CONFIG_VALID_BMP_ULMU_DATA_DISABLE		0x0040u
#define OMI_CONFIG_VALID_BMP_BW_EXT			0x0080u
#define OMI_CONFIG_VALID_BMP_RXNSS_EXT			0x0100u
#define OMI_CONFIG_VALID_BMP_TXNSTS_EXT			0x0200u
#define OMI_CONFIG_VALID_BMP_ALL			0x0FFFu
#define OMI_CONFIG_VALID_BMP_CLEAR_ALL			0x1000u

#define OMI_CONFIG_BW_MAX				3u
#define OMI_CONFIG_BW_EXT_MAX				1u
#define OMI_CONFIG_RXNSS_EXT_MAX			1u
#define OMI_CONFIG_TXNSTS_EXT_MAX			1u

/** band_bitmap indicating bands for which OMI config is get/set */
#define WL_OMI_BAND_BITMAP_2G		0x1u  /* 2g field updated */
#define WL_OMI_BAND_BITMAP_5G		0x2u  /* 5g field updated */
#define WL_OMI_BAND_BITMAP_6G		0x4u  /* 6g field updated */
#define WL_OMI_BAND_BITMAP_ALL		(WL_OMI_BAND_BITMAP_2G |	\
					WL_OMI_BAND_BITMAP_5G | \
					WL_OMI_BAND_BITMAP_6G)

typedef struct wl_omi_config {
	uint16	valid_bm;		/* validity bitmask for each config */
	uint8	rxnss;
	uint8	bw;
	uint8   ulmu_disable;
	uint8	txnsts;
	uint8	ersu_disable;
	uint8	dlmu_resound_rec;
	uint8	ulmu_data_disable;
	uint8	bw_ext;
	uint8	rxnss_ext;
	uint8	txnsts_ext;
} wl_omi_config_t;

typedef struct wl_omi_req {
	uint16	version;
	uint16	len;
	wl_omi_config_t config;
} wl_omi_req_v1_t;

typedef struct wl_omi_req_v2 {
	uint16	version;
	uint16	len;
	uint8	band_bitmap; /* bands for which config is updated */
	uint8	PAD[3];
	wl_omi_config_t config;
} wl_omi_req_v2_t;

/* Bits for ULMU disable reason */
#define OMI_ULMU_DISABLED_HOST			0x01u   /* Host has disabled through he omi */


#define OMI_ULMU_DISABLED_NAN			0x04u   /* Disabled due to NAN enabled */
#define OMI_ULMU_DISABLED_BTCOEX		0x08u   /* Disabled while in BT Coex activity */
#define OMI_ULMU_DISABLED_LTECOEX		0x10u   /* Disabled due to LTE Coex activity */
#define OMI_ULMU_DISABLED_NON11AX_CONN		0x20u	/* Disabled due to non-11ax connection */
#define OMI_ULMU_DISABLED_THROTTLE_ENABLE	0x40u	/* Disabled due to throttle timer running */
#define OMI_ULMU_DISABLED_TXCHAIN_DOWNGRADE	0x80u	/* Disabled due to Txchain downgrade */
#define OMI_ULMU_DISABLED_TX_DUTY_CYCLE		0x100u	/* Disabled due to tx duty cycle */
#define OMI_ULMU_DISABLED_VLP			0x200u	/* Disabled due to VLP  */
#define OMI_ULMU_DISABLED_DUALSTA		0x400u	/* Disabled due to dualsta */
#define OMI_ULMU_DISABLED_UNSUPPORTED_TXPWR	0x800u	/* Disabled due to unsupported tx power */

/* Bits for DLMU Resound Recommendation reason */
#define OMI_DLMU_RSD_RCM_HOST	(0x1u << 0u)	/* Host directly set the bit */
#define OMI_DLMU_RSD_RCM_MPF	(0x1u << 1u)	/* Set on MPF state change */

#define WL_OMI_STATUS_VERSION_1	1u
typedef struct wl_omi_status {
	uint16	version;
	uint16	len;
	wl_omi_config_t	omi_pending;	/* OMI requests pending */
	uint16	omi_data;		/* current OM Control field for completed OMI requests */
	uint16	ulmu_disable_reason;	/* Bits representing UL OFDMA disable reasons */
	uint32	ulmu_disable_duration;	/* Duration (ms) for which UL OFDMA is disabled */
} wl_omi_status_v1_t;

#define WL_OMI_STATUS_VERSION_2	2u
typedef struct wl_omi_status_v2 {
	uint16	version;
	uint16	len;
	wl_omi_config_t	omi_pending;	/* OMI requests pending */
	uint16	omi_data;		/* Current OM Control field for completed OMI requests */
	uint16	ulmu_disable_reason;	/* Bits representing UL OFDMA disable reasons */
	uint32	ulmu_disable_duration;	/* Duration (ms) for which UL OFDMA is disabled */
	uint32	dlmu_rsd_rcm_duration;	/* Dur (ms) for which ResoundRecommentation is set */
	uint16	dlmu_rsd_rcm_mpf_state;	/* The MPF state value */
	uint16	dlmu_rsd_rcm_reason;	/* DL MU-MIMO recommendation reasons bitmap */
} wl_omi_status_v2_t;

#define WL_OMI_STATUS_VERSION_3	3u
typedef struct wl_omi_status_v3 {
	uint16	version;
	uint16	len;
	wl_omi_config_t	omi_pending;	/* OMI requests pending */
	uint16	omi_data;		/* Current HE OM Control field for completed OMI req */
	uint16	ulmu_disable_reason;	/* Bits representing UL OFDMA disable reasons */
	uint32	ulmu_disable_duration;	/* Duration (ms) for which UL OFDMA is disabled */
	uint32	dlmu_rsd_rcm_duration;	/* Dur (ms) for which ResoundRecommentation is set */
	uint16	dlmu_rsd_rcm_mpf_state;	/* The MPF state value */
	uint16	dlmu_rsd_rcm_reason;	/* DL MU-MIMO recommendation reasons bitmap */
	uint16	omi_data_ext;		/* Current EHT OM Control field for completed OMI req */
	uint16	PAD;
} wl_omi_status_v3_t;

#define WL_ULMU_DISABLE_STATS_VERSION_1	1u
typedef struct wl_ulmu_disable_stats {
	uint16 version;
	uint16 len;
	uint32 ulmu_disable_ts;	/* UL OFDMA disabled timestamp (ms) */
	uint16 ulmu_disable_reason;	/* Bits representing UL OFDMA disable reasons */
	uint16 ulmu_disable_count;	/* UL MU disable count during current infra association */
	uint32 last_trig_rx_ts;	/* Last trigger frame received timestamp (ms) */
	uint16 trig_rx_count;	/* No of trigger frames received after last UL OFDMA disable */
	uint16 max_latency;	/* Max latency by AP to re-act for UL OFDMA disable request (ms) */
	uint16 min_latency;	/* Min latency by AP to re-act for UL OFDMA disable request (ms) */
	uint16 avg_latency;	/* Avg latency by AP to re-act for UL OFDMA disable request (ms) */
} wl_ulmu_disable_stats_v1_t;

/* Bits for configuring the feature for setting max Rx MPDU size to 4K for 160MHz
 * association with BRCM AP
 */
#define WL_RXMPDU_MAXLEN_4K_EN		0x1u  /* Enable/disable the feature */
#define WL_RXMPDU_MAXLEN_4K_ACTIVE	0x2u  /* Status to indicate if the feature is active */

#define WL_TPE_TXPWR_CONFIG_VERSION_1	1u
#define WL_TPE_TXPWR_MAX_LEN		8u

typedef struct wl_tpe_txpwr_config {
	uint16	version;
	uint16	len;
	uint8	idx;				/* Index of profile */
	uint8	cat;				/* Transmit Power Info category */
	uint8	intr;				/* Transmit Power Info Interpretation */
	uint8	cnt;				/* Transmit Power Info count */
	uint8	regcat;				/* reg info power category */
	uint8	PAD[3];
	int8	txpwr[WL_TPE_TXPWR_MAX_LEN];	/* Maximum TX power */
} wl_tpe_txpwr_config_v1_t;

/* sub-xtlv IDs within WL_STATS_XTLV_WL_SLICE_TX_HISTOGRAMS */
enum wl_tx_histogram_id {
	WL_TX_HIST_TXQ_ID		= 1,
	WL_TX_HIST_LOW_TXQ_ID		= 2,
	WL_TX_HIST_SCBQ_ID		= 3,
	WL_TX_HIST_EXCUR_TXQ_ID		= 4,
	WL_TX_HIST_EXCUR_LOW_TXQ_ID	= 5
};

/* common tx histogram structure */
typedef struct wl_tx_hist {
	uint16 hist_bmap;	/* bit N indicates histogram follows for priority or fifo N */
	uint16 hist_count;	/* count of histograms in var len array */
	uint32 hist[BCM_FLEX_ARRAY]; /* var len array of histograms each prefix by hist length */
} wl_tx_hist_t;

#define WL_TX_HIST_FIXED_LEN	(OFFSETOF(wl_tx_hist_t, hist))
#define WL_TX_HIST_FULL_LEN(num_hist, max_hist_size)	\
	(WL_TX_HIST_FIXED_LEN + (num_hist) *		\
	(max_hist_size + 1) * sizeof(uint32))

/* structure for WL_TX_HIST_TXQ, WL_TX_HIST_EXCUR_TXQ_ID */
typedef struct wl_tx_hist_txq {
	uint32 bsscfg_bmap;      /* bitmap of bsscfg indexes associated with this queue */
	wl_tx_hist_t tx_hist;	/* tx histograms */
} wl_tx_hist_txq_t;

#define WL_TX_HIST_TXQ_FIXED_LEN	\
	(OFFSETOF(wl_tx_hist_txq_t, tx_hist) + WL_TX_HIST_FIXED_LEN)
#define WL_TX_HIST_TXQ_FULL_LEN(num_hist, max_hist_size)	\
	(OFFSETOF(wl_tx_hist_txq_t, tx_hist) +			\
	WL_TX_HIST_FULL_LEN(num_hist, max_hist_size))

/* sub-xtlv IDs within WL_STATS_XTLV_WL_SLICE_TX_HISTOGRAMS */
enum wl_txq_stop_histogram_id {
	WL_TXQ_STOP_HIST_SW		= 1,
	WL_TXQ_STOP_HIST_HW		= 2,
	WL_TXQ_STOP_HIST_PKTS_SW	= 3,
	WL_TXQ_STOP_HIST_PKTS_HW	= 4,
	WL_TXQ_STOP_HIST_MAX		= WL_TXQ_STOP_HIST_PKTS_HW
};

/* common tx histogram structure */
typedef struct wl_txq_stop_hist {
	wl_tx_hist_t tx_hist;	/* tx histograms */
} wl_txq_stop_hist_t;

#define WL_TXQ_STOP_HIST_FIXED_LEN	\
	(OFFSETOF(wl_txq_stop_hist_t, tx_hist) + WL_TX_HIST_FIXED_LEN)
#define WL_TXQ_STOP_HIST_FULL_LEN(num_hist, max_hist_size)	\
	(OFFSETOF(wl_txq_stop_hist_t, tx_hist) +		\
	WL_TX_HIST_FULL_LEN(num_hist, max_hist_size))

/* structure for WL_TX_HIST_LOW_TXQ, WL_TX_HIST_EXCUR_LOW_TXQ_ID */
typedef struct wl_tx_hist_low_txq {
	wl_tx_hist_t tx_hist;	/* tx histograms */
} wl_tx_hist_low_txq_t;

#define WL_TX_HIST_LOW_TXQ_FIXED_LEN	\
	(OFFSETOF(wl_tx_hist_low_txq_t, tx_hist) + WL_TX_HIST_FIXED_LEN)
#define WL_TX_HIST_LOW_TXQ_FULL_LEN(num_hist, max_hist_size)	\
	(OFFSETOF(wl_tx_hist_low_txq_t, tx_hist) +		\
	WL_TX_HIST_FULL_LEN(num_hist, max_hist_size))

/* structure for WL_TX_HIST_SCBQ */
typedef struct wl_tx_hist_scbq {
	struct ether_addr ea;	/* ether addr of peer */
	uint16 bsscfg_idx;	/* bsscfg index */
	wl_tx_hist_t tx_hist;	/* tx histograms */
} wl_tx_hist_scbq_t;

#define WL_TX_HIST_SCBQ_FIXED_LEN	\
	(OFFSETOF(wl_tx_hist_scbq_t, tx_hist) + WL_TX_HIST_FIXED_LEN)
#define WL_TX_HIST_SCBQ_FULL_LEN(num_hist, max_hist_size)	\
	(OFFSETOF(wl_tx_hist_scbq_t, tx_hist) +			\
	WL_TX_HIST_FULL_LEN(num_hist, max_hist_size))

/* sub-xtlv IDs within WL_STATS_XTLV_WL_SLICE_TX_QUEUE_DEPTH */
enum wl_tx_queue_depth_id {
	WL_TX_QUEUE_DEPTH_TXQ_ID		= 1,
	WL_TX_QUEUE_DEPTH_LOW_TXQ_ID		= 2,
	WL_TX_QUEUE_DEPTH_SCBQ_ID		= 3,
	WL_TX_QUEUE_DEPTH_EXCUR_TXQ_ID		= 4,
	WL_TX_QUEUE_DEPTH_EXCUR_LOW_TXQ_ID	= 5
};

/* common tx queue depth structure */
typedef struct wl_tx_queue_depth {
	uint16 queue_depth_bmap;	/* bitmap of queue depth in var len array */
	uint16 queue_depth_count;	/* count of queue depth in var len array */
	uint16 queue_depth[BCM_FLEX_ARRAY];		/* var len array of queue depth */
} wl_tx_queue_depth_t;

#define WL_TX_QUEUE_DEPTH_FIXED_LEN	(OFFSETOF(wl_tx_queue_depth_t, queue_depth))
#define WL_TX_QUEUE_DEPTH_FULL_LEN(num_queue_depth)		\
	(WL_TX_QUEUE_DEPTH_FIXED_LEN + (num_queue_depth) *	\
	sizeof(uint16))

/* structure for WL_TX_QUEUE_DEPTH_TXQ_ID, WL_TX_QUEUE_DEPTH_EXCUR_TXQ_ID */
typedef struct wl_tx_queue_depth_txq {
	uint32 bsscfg_map;      /* bitmap of bsscfg indexes associated with this queue */
	wl_tx_queue_depth_t tx_queue_depth;	/* queue depth */
} wl_tx_queue_depth_txq_t;

#define WL_TX_QUEUE_DEPTH_TXQ_FIXED_LEN	\
	(OFFSETOF(wl_tx_queue_depth_txq_t, tx_queue_depth) + WL_TX_QUEUE_DEPTH_FIXED_LEN)
#define WL_TX_QUEUE_DEPTH_TXQ_FULL_LEN(num_queue_depth)		\
	(OFFSETOF(wl_tx_queue_depth_txq_t, tx_queue_depth) +	\
	WL_TX_QUEUE_DEPTH_FULL_LEN(num_queue_depth))

/* structure for WL_TX_QUEUE_DEPTH_LOW_TXQ_ID, WL_TX_QUEUE_DEPTH_EXCUR_LOW_TXQ_ID */
typedef struct wl_tx_queue_depth_low_txq {
	wl_tx_queue_depth_t tx_queue_depth;	/* queue depth */
} wl_tx_queue_depth_low_txq_t;

#define WL_TX_QUEUE_DEPTH_LOW_TXQ_FIXED_LEN	\
	(OFFSETOF(wl_tx_queue_depth_low_txq_t, tx_queue_depth) + WL_TX_QUEUE_DEPTH_FIXED_LEN)
#define WL_TX_QUEUE_DEPTH_LOW_TXQ_FULL_LEN(num_queue_depth)		\
	(OFFSETOF(wl_tx_queue_depth_low_txq_t, tx_queue_depth) +	\
	WL_TX_QUEUE_DEPTH_FULL_LEN(num_queue_depth))

/* structure for WL_TX_QUEUE_DEPTH_SCBQ_ID */
typedef struct wl_tx_queue_depth_scbq {
	struct ether_addr ea;			/* ether addr of peer */
	uint16 bsscfg_idx;			/* bsscfg index */
	wl_tx_queue_depth_t tx_queue_depth;	/* queue depth */
} wl_tx_queue_depth_scbq_t;

#define WL_TX_QUEUE_DEPTH_SCBQ_FIXED_LEN	\
	(OFFSETOF(wl_tx_queue_depth_scbq_t, tx_queue_depth) + WL_TX_QUEUE_DEPTH_FIXED_LEN)
#define WL_TX_QUEUE_DEPTH_SCBQ_FULL_LEN(num_queue_depth)	\
	(OFFSETOF(wl_tx_queue_depth_scbq_t, tx_queue_depth) +	\
	WL_TX_QUEUE_DEPTH_FULL_LEN(num_queue_depth))


/* sub-xtlv IDs within WL_STATS_XTLV_BUS_PCIE_TX_HISTOGRAMS */
enum wl_pcie_tx_histogram_id {
	WL_PCIE_TX_HIST_ID = 1
};

/* structure for PCIE_TX_HIST_ID */
typedef struct wl_pcie_tx_hist {
	uint16 ring_id;		/* PCIe ring id */
	uint16 PAD;		/* 4-byte alignment */
	wl_tx_hist_t tx_hist;	/* hist_bmap:
				 *	0x1=tx histogram
				 *	0x2=tx status pending histogram
				 */
} wl_pcie_tx_hist_t;

#define WL_PCIE_TX_HIST_FIXED_LEN	\
	(OFFSETOF(wl_pcie_tx_hist_t, tx_hist) + WL_TX_HIST_FIXED_LEN)
#define WL_PCIE_TX_HIST_FULL_LEN(num_hist, max_hist_size)	\
	(OFFSETOF(wl_pcie_tx_hist_t, tx_hist) +			\
	WL_TX_HIST_FULL_LEN(num_hist, max_hist_size))

/* sub-xtlv IDs within WL_STATS_XTLV_BUS_PCIE_TX_QUEUE_DEPTH */
enum wl_pcie_tx_queue_depth_id {
	WL_PCIE_TX_QUEUE_DEPTH_ID = 1
};

/* structure for WL_PCIE_TX_QUEUE_DEPTH_ID */
typedef struct wl_pcie_tx_queue_depth {
	uint16 ring_id;		/* PCIe ring id */
	uint16 queue_depth;	/* queue depth of ring id */
	uint16 tx_status_pend;	/* tx status pending of ring id */
	uint16 PAD;		/* 4-byte alignment */
} wl_pcie_tx_queue_depth_t;

#define WL_PCIE_TX_QUEUE_DEPTH_FIXED_LEN	sizeof(wl_pcie_tx_queue_depth_t)

#define WL_WSEC_DEL_PMK_VER_V1 1u
/* tlv ids for del pmk */
#define WL_DEL_PMK_TLV_ID			1u
#define WL_DEL_PMKID_TLV_ID			2u
#define WL_DEL_PEER_ADDR_TLV_ID		3u
typedef struct wl_wsec_del_pmk {
	uint16 version;
	uint16 length;
	uint8 xtlvs[];
} wl_wsec_del_pmk_t;
#define WL_WSEC_DEL_PMK_FIXED_LEN_V1	OFFSETOF(wl_wsec_del_pmk_t, xtlvs)

/* WTC */
#define WLC_WTC_ROAM_VER_1	1u
#define WLC_WTC_ROAM_CONFIG_HDRLEN	4u

typedef enum wtc_band_list {
	WTC_BAND_2G = 0,
	WTC_BAND_5G = 1,
	WTC_BAND_6G = 2,
	WTC_MAX_BAND = 3
} wtc_band_list_e;

typedef struct wlc_wtcroam {
	uint16 ver;
	uint16 len;
	uint8 data[];
} wlc_wtc_args_t;

typedef struct wlc_wtcconfig_info {
	uint8	mode;				/* enable or disable wtc. static config */
	uint8	scantype;			/* type of scan for wtc roam */
	int8	rssithresh[WTC_MAX_BAND];	/* rssi value below which wtc is active */
	int8	ap_rssithresh[WTC_MAX_BAND];	/* rssi value above which candidate AP is good */
} wlc_wtcconfig_info_v1_t;

/* RCROAM */
#define WLC_RC_ROAM_VER_1	1u

typedef struct wlc_rcroam {
	uint16 ver;
	uint16 len;
	uint8 data[];
} wlc_rcroam_t;

typedef struct wlc_rcroam_info_v1 {
	uint16	inactivity_period; /* inactivty monitor period */
	uint16	roam_scan_timeout;
	uint16	periodic_roam_scan_timeout;
	uint8	roam_trig_step; /* roaming trigger step value */
	bool enab;
} wlc_rcroam_info_v1_t;

#define RCROAM_HDRLEN			4u
#define MAX_RCSCAN_TIMER		300u

/* Reasons for re-setting RC ROAM params */
#define WLC_RCROAM_RESET_JOIN	0 /* New  join */
#define WLC_RCROAM_REASON_ROAM_SUCCESS	1 /* Successful roam */
#define WLC_RCROAM_RESET_RSSI_CHANGE	2 /* RSSI of the link which was bad regained/worsened */
#define WLC_RCROAM_RESET_BCN_LOSS	3	/* Beacon loss */
#define WLC_RCROAM_RESET_DISASSOC_RX	4	/* Disassoc was received */
#define WLC_RCROAM_RESET_DEAUTH_RX	5	/* Deauth was received */
#define WLC_RCROAM_RESET_IOVAR	6	/* Iovar to disable rcroam was received from host */
#define WLC_RCROAM_RESET_WTCREQ	7	/* WTC request overriding rcroam */
#define WLC_RCROAM_RESET_RSN_ABORT      8 /* Reset RCROAM params due to roam abort */
#define WLC_RCROAM_RESET_CSA_COMP	9 /* Reset RCROAM params due to CSA */
#define WLC_RCROAM_RESET_ROAM_PROF	10 /* Reset RCROAM params due to roam prof iovar */

#define WLC_SILENT_ROAM_VER_1	1u
/* silent roam information struct */
typedef struct wlc_sroam_info_v1 {
	/* Silent roam Set/Get value */
	uint8 sroam_on;			/* sroam on/off */
	int8 sroam_min_rssi;		/* minimum rssi threshold to activate the feature */
	uint8 sroam_rssi_range;		/* rssi tolerance to determine stationary status */
	uint8 sroam_score_delta;	/* roam score delta value to prune candidate ap */
	uint8 sroam_period_time;	/* required monitoring period to trigger roaming scan */
	uint8 sroam_band;		/* band setting of roaming scan (all, 5g, 2g) */
	uint8 sroam_inact_cnt;		/* tx/rx frame count threshold for checking inactivity */
	/* Silent roam monitor value */
	int8 sroam_ref_rssi;		/* reference rssi which is picked when monitoring is
					 * started. it is updated to current rssi when it's
					 * out from rssi range
					 */
	uint8 sroam_time_since;		/* elapsed time since start monitoring */
	uint8 PAD[3];
	uint32 sroam_txfrm_prev;	/* save current tx frame counts */
	uint32 sroam_rxfrm_prev;	/* save current rx frame counts */
} wlc_sroam_info_v1_t;

typedef struct wlc_sroam {
	uint16 ver;
	uint16 len;
	uint8 data[];
} wlc_sroam_t;

#define SROAM_HDRLEN			4u

#define	DEF_SROAM_OFF			0
#define	DEF_SROAM_MIN_RSSI		-65
#define	DEF_SROAM_RSSI_RANGE		8u
#define	DEF_SROAM_SCORE_DELTA		0
#define	DEF_SROAM_PERIOD_TIME		5u
#define	DEF_SROAM_INACT_CNT		10u
#define	MAX_SROAM_RSSI			-70
#define	MAX_SROAM_RSSI_RANGE		10u
#define	MAX_SROAM_SCORE_DELTA		10u
#define	MAX_SROAM_PERIOD_TIME		250u
#define	SROAM_BAND_AUTO			3u

/* MACSMPL IOVAR parameters */
typedef enum wl_macdbg_macsmpl_iovar_id {
	WL_MACSMPL_START	= 0,
	WL_MACSMPL_STOP		= 1,
	WL_MACSMPL_DUMP		= 2,
	WL_MACSMPL_STATUS	= 3,
	WL_MACSMPL_SIZE		= 4
} wl_macdbg_macsmpl_iovar_id_t;

/* WL_MACSMPL_STATUS values */
typedef enum wl_macdbg_macsmpl_status {
	WL_MACSMPL_STATUS_IDLE		= 0,
	WL_MACSMPL_STATUS_ACTIVE	= 1,
	WL_MACSMPL_STATUS_WAIT_FOR_TRIG	= 2,
	WL_MACSMPL_STATUS_TRIGGERED	= 3
} wl_macdbg_macsmpl_status_t;

/* WL_MACSMPL_START_PARAM subcommand data */
typedef struct wl_macsmpl_start_param {
	uint32 trig_condition;	/* trigger condition */
	uint16 gpio_mux;	/* MACControl1 GPIOSel field */
	uint8 PAD[2];		/* 4-byte struct alignment */
} wl_macsmpl_param_start_t;

/* MAC SC fragment request data */
typedef struct wl_macsmpl_frag_req_param {
	uint32 offset;		/* requested MAC SC fragment offset */
	uint32 size;		/* requested MAC SC fragment size, bytes */
} wl_macsmpl_frag_req_param_t;

/* MAC SC fragment response data */
typedef struct wl_macsmpl_frag_resp_param {
	uint32 offset;		/* MAC SC response fragment offset */
	uint32 size;		/* MAC SC reponse fragment size, bytes */
	uint8 data[];		/* MAC SC response fragment data, flexible array */
} wl_macsmpl_frag_resp_param_t;

/* MAC SC status data */
typedef struct wl_macsmpl_status {
	uint32 maccontrol1;	/* MACControl1 register value */
	uint32 macsc_flags;	/* M_MACSC_FLAGS SHM register value */
	uint16 sc_play_ctrl;	/* TXE SampleCollectPlayCtrl register value */
	uint16 sc_cur_ptr;	/* TXE SampleCollectCurPtr register value */
	uint16 sc_start_ptr;	/* TXE SampleCollectStartPtr register value */
	uint16 sc_stop_ptr;	/* TXE SampleCollectStopPtr register value */
} wl_macsmpl_status_t;

/* WL_MACSMPL parameters data */
typedef struct wl_macsmpl_param {
	wl_macdbg_macsmpl_iovar_id_t subcmd_id;
	union {
		wl_macsmpl_param_start_t start;
		wl_macsmpl_frag_req_param_t frag_req;
	} u;
} wl_macsmpl_param_t;

/* High priority P2P */
#define WL_HP2P_COUNTERS_VER_V2		2u
#define WL_HP2P_COUNTERS_VER		WL_HP2P_COUNTERS_VER_V2

typedef struct hp2p_counters_v2 {
	uint32 frames_queued;		/* Number of AMPDUs processed */
	uint16 frames_exp;		/* Number of Lifetime expiries */
	uint16 edt_retry;		/* Exceed due to - retry */
	uint16 mpif_reconf;		/* MPIF Reconfigure */
	uint16 exceed_delay;		/* Exceed delay threshold */
	uint16 edt_nav_thresh;		/* Exceed due to - NAV threshold */
	uint16 edt_dc_def;		/* Exceed due to - DC based deferral */
	uint16 edt_tx_fifo_full;	/* Exceed due to - Tx FIFO full */
	uint16 edt_cts_thresh;		/* Exceed due to - CTS threshold */
	uint16 dbg1;			/* dbgX are for internal debugging */
	uint16 dbg2;
	uint16 dbg3;
	uint16 dbg4;
	uint16 dbg5;
	uint16 dbg6;
	uint16 dbg7;
	uint16 dbg8;
	uint16 dbg9;
	uint16 dbg10;
} hp2p_counters_v2_t;

typedef struct hp2p_counters_hdr {
	uint16 version;		/* version of hp2p_counters_t structure */
	uint16 len;
	uint16 slice_idx;
	uint16 PAD;
	uint8 counters[];
} hp2p_counters_hdr_t;

/* HP2P RLLW Stats */
#define WL_HP2P_RLLW_STATS_VER	1u
typedef struct hp2p_rllw_stats_hdr {
	uint16	version;	/* version of hptp_rllw_stats structure */
	uint16	len;		/* length of the payload */
	uint8	slice_idx;	/* Slice idx BAND_2G_INDEX - 0 / BAND_5G_INDEX - 1 */
	uint8	PAD[3];
	uint8 stats[];		/* rllw_stats instance */
} hp2p_rllw_stats_hdr_t;

/*
 * rllw stats common & per band
 *
 */
typedef struct hp2p_rllw_stats_v1 {
	/* Slice Specific Stats */
	uint16	n_hnav_intrs;			/* # of high nav interrupts */
	uint16	n_switches;			/* # of Switches to band n */
	/* Stats on wlc_trig */
	uint16	n_override_pkts;		/* # of pkts enqd with ovrd bit set */
	uint16	n_overrides;			/* # of overrides - this is only trig_wlc */
	uint16	n_override_pkts_acked;		/* # of ovrd pkts that got an ACK */
	uint16	n_override_pkts_not_acked;	/* # of ovrd pkts that werent ACKed */
} hp2p_rllw_stats_v1_t;

/* TX enable flags */
#define WL_HP2P_TX_AMPDU				0x0001u
#define WL_HP2P_TX_AMSDU				0x0002u
#define WL_HP2P_TX_RDG					0x0004u
#define WL_HP2P_TX_TXOP					0x0008u
#define WL_HP2P_TX_TXOP_RTS				0x0010u

/* RX enable flags */
#define WL_HP2P_RX_AMPDU				0x0001u
#define WL_HP2P_RX_AMSDU				0x0002u
#define WL_HP2P_RX_RDG					0x0004u
#define WL_HP2P_RX_AMPDU_REORDER			0x0008u
#define WL_HP2P_RX_DELETE				0x0010u

/* Max/min values for configuration parameters to check validity */
#define WL_HP2P_MAX_RETRY_MAX			14u
#define WL_HP2P_MAX_RETRY_MIN			6u
#define WL_HP2P_LATENCY_TARGET_MAX		30u
#define WL_HP2P_BURST_INTERVAL_MAX		64u
#define WL_HP2P_MAX_FIFO			5u
#define WL_HP2P_MAX_UCODE_LATENCY_THR		500u
#define WL_HP2P_MAX_UCODE_RECOV_TO		500u
#define WL_HP2P_MAX_UCODE_NAV_THR		50000u

#define WLC_HP2P_DEF_NAV_THRESH			13u
#define WLC_HP2P_MIN_NAV_THRESH			1u
#define WLC_HP2P_MAX_NAV_THRESH			35u
#define WLC_HP2P_MAX_HOF_WAIT_THRESH		65u

#define WL_HP2P_VERSION		1u
typedef struct hp2p_tx_config {
	struct ether_addr peer_addr;
	uint16 max_burst;
	uint16 txop;		/* stored in network order (ls octet first) */
	uint16 flags;	/* flags to enable/disable AMPDU, AMSDU, RDG, TXOP, TXOP_RTS */
	uint8 aci;
	uint8 ecw;
	uint8 fifo;
	uint8 tid;
	uint8 burst_interval;
	uint8 latency_target;
	uint8 max_retry;
	uint8 PAD;
} hp2p_tx_config_t;

typedef struct hp2p_rx_config {
	struct ether_addr peer_addr;
	uint16 flags;	/* flags to enable/disable AMPDU, AMSDU, RDG, AMPDU Reorder */
	uint8 tid;
	uint8 PAD;
	uint16 ba_wsize; /* AMPDU BA Window size */
} hp2p_rx_config_t;

typedef struct hp2p_udbg_config {
	uint16 recovery_timeout;	/* multiples of 256 usecs */
	uint16 latency_thresh;		/* multiples of 256 usecs */
	uint16 enable_trap;		/* trap if ucode delay exceeds latency_thresh */
	uint16 nav_thresh;		/* in usec */
} hp2p_udbg_config_t;

#define WLC_HP2P_RLLW_RETRY_LIMIT	7u
#define WLC_HP2P_MAX_PKTLIFETIME_IN_MS	2000u	/* 2 seconds */
#define WLC_HP2P_MAX_CCA_THRESHOLD	100u	/* 100 percent */

/*
 * nav_thresh:	13	: By default set to 13ms
 * retry_limit: 0	: Auto / Default retry limit to be applied
 *			Max retry limit is 7
 * pkt_lifetime: 0	: Auto / Default pkt lifetime to be applied
 *			Max value cant exceed 2000u (2seconds)
 * floor_rate: 0	: Auto / Default min rate to be applied
 * ceiling_rate: 0	: Auto / Default max rate to be applied
 * hof_wait_thr: 0	: Disabled by default
 * hof_switch_dur: 0	: Disabled by default
 */
typedef struct hp2p_rllw_cfg {
	uint8	nav_thresh;		/* NAV threshold in ms */
	uint8	retry_limit;		/* # max retries before pkt is discarded */
	uint16	pkt_lifetime;		/* Pkt lifetime in ms */
	uint16	floor_rate;		/* Min rate in Mbps */
	uint16	ceiling_rate;		/* Max rate in Mbps */
	uint16	hof_wait_thr;		/* HOF packet wait threshold in ms */
	uint16	hof_switch_dur;		/* How long to stay in the switched band in ms */
	uint16	hof_pkt_life_thr;	/* HOF remaining pkt lifetime threshold in ms */
	uint8	cca_thresh;		/* CCA threshold in percentage for HOF switch */
	uint8	PAD;
} hp2p_rllw_cfg_t;

#define WL_HP2P_CAP_MPEER	0x01u	/* Multipeer Hp2p */
#define WL_HP2P_CAP_MPEER_TXOP	0x02u	/* Same txop transmit on mpeer Hp2p */
#define WL_HP2P_CAP_RLLW	0x04u	/* Band switching and override on NAV */
#define WL_HP2P_CAP_SPLIT_TXS	0x08u	/* Spilt tx status for rllw sub feature */

typedef uint32 hp2p_cap_t;	/* HP2P capabilities */

typedef struct hp2p_cmd {
	uint16 type;
	uint16 len;
	uint8 data[];
} hp2p_cmd_t;

#define WL_HP2P_CTRL_MPEER_TXOP_ENAB	0x01u	/* Same txop transmit on mpeer Hp2p */
/*
 * Band switching on NAV. Control for band
 * switching for HPP traffic, applies only
 * for dual local/peer map availability.
 */
#define WL_HP2P_CTRL_RLLW_SWITCH_ENAB	0x02u	/* RLLW Band switching enabled */
#define WL_HP2P_CTRL_RLLW_PRIMARY_OVR	0x04u	/* RLLW Override enab on primary band. */
#define WL_HP2P_CTRL_RLLW_DENY_BT	0x08u	/* RLLW Give WiFi priority over BT */
#define WL_HP2P_CTRL_RLLW_PKTLT_ENAB	0x10u	/* RLLW pkt lifetime based decision enabled */
#define WL_HP2P_CTRL_RLLW_HOF_SW_ENAB	0x20u	/* RLLW Band switching due to HOF timeout enabled */

typedef uint32 hp2p_ctrl_t;	/* HP2P Ctrl bits */

/* TLVs for HP2P related IOVARs */
typedef enum wl_hp2p_tlv {
	HP2P_TLV_RLLW_SLICE_STATS	= 1u,
	HP2P_TLV_RLLW_PEER_STATS	= 2u
} wl_hp2p_tlv_t;

/* Top level HP2P RLLW stats header */
typedef struct wl_hp2p_stats_hdr {
	uint32	n_stats;	/* # of TLVs in the payload */
	uint32	totlen;		/* total length of TLVs following this field */
	uint8   stats_tlvs[];	/* chain of TLVs for reader to decode based on length */
} wl_hp2p_stats_hdr;

/* Per Slice RLLW Stats */
typedef struct wlc_hp2p_rllw_slice_stats {
	uint8	slice;				/* slice number */
	uint8	PAD;
	uint16	n_hnav_intrs;			/* # of high nav interrupts on slice n */
	uint16	n_overrides;			/* # of overrides */
	uint16	n_switches;			/* # of switches */
	uint32	n_switched_pkts;		/* # of packets after switching to slice n */
	uint32	n_switched_pkts_not_acked;	/* # of packets after switch & Not ACKed */
	uint32	n_override_pkts;		/* # of pkts enqd with ovrd bit set on slice n */
	uint32	n_override_pkts_not_acked;	/* # of ovrd pkts that werent ACKed on slice n */
	uint32	n_no_switch_in_pktlife;		/* # of no switch happened,pkts within lifetime */
	uint32	n_no_override_in_pktlife;	/* # of no ovrd happened,pkts within lifetime */
	uint16	n_no_hof_switch_in_pktlife;	/* # of skipped HOF switches due to pkt lifetime */
	uint16	n_no_hof_switch_null_pkt;	/* # of skipped HOF switches due to NULL HOF pkt */
	uint16	n_hof_switches;			/* # of switches triggered by HOF timeout */
	uint16	n_hof_intrs;			/* # of HOF interrupts on slice n */
} wlc_hp2p_rllw_slice_stats_t;

/* Peer specific stats */
typedef struct {
	uint8	slice;				/* slice number */
	uint8	PAD[3];
	uint16	n_overrides;			/* # of overrides */
	uint16	n_switches;			/* # of switches */
	uint32	n_switched_pkts;		/* # of packets after switching to slice n */
	uint32	n_switched_pkts_not_acked;	/* # of packets after switch & Not ACKed */
	uint32	n_override_pkts;		/* # of pkts enqd with ovrd bit set on slice n */
	uint32	n_override_pkts_not_acked;	/* # of ovrd pkts that werent ACKed on slice n */
	uint32	n_no_switch_in_pktlife;		/* # of no switch happened,pkts within lifetime */
	uint32	n_no_override_in_pktlife;	/* # of no ovrd happened,pkts within lifetime */
	uint16	n_no_hof_switch_in_pktlife;	/* # of skipped HOF switches due to pkt lifetime */
	uint16	n_hof_switches;			/* # of switches triggered by HOF timeout */
} wlc_hp2p_rllw_peer_stats_t;

/* Peer Stats Info to host */
typedef struct wlc_hp2p_peer_stats_info {
	struct ether_addr		peer_mac;	/* Mac Address of the HP2P peer */
	uint16				nslices;	/* Number of slices */
	wlc_hp2p_rllw_peer_stats_t	peer_stats[MAX_NUM_D11CORES];	/* Peer specific stats */
} wlc_hp2p_peer_stats_info_t;

typedef struct hp2p_cmd_hdr {
	uint16 version;
	uint16 slice_idx;
	uint8 cmd[];
} hp2p_cmd_hdr_t;

/* to be used in type field of hp2p_cmd_t structure while issuing HP2P commands */
typedef enum hp2p_cmd_id {
	WL_HP2P_CMD_ENABLE		= 0,	/* Enable HP2P */
	WL_HP2P_CMD_TX_CONFIG		= 1,	/* Tx Configuration */
	WL_HP2P_CMD_RX_CONFIG		= 2,	/* Rx Configuration */
	WL_HP2P_CMD_COUNTERS		= 3,	/* HP2P Counters */
	WL_HP2P_CMD_UDBG_CONFIG		= 4,	/* ucode debug config */
	WL_HP2P_CMD_RLLW_CFG		= 5,	/* HP2P RLLW config */
	WL_HP2P_CMD_RLLW_STATS		= 6,	/* HP2P RLLW Stats */
	WL_HP2P_CMD_CAP			= 7,	/* HP2P Capability */
	WL_HP2P_CMD_CTRL		= 8,	/* HP2P feature ctrl */
	WL_HP2P_CMD_RLLW_CFG_V2		= 9,	/* HP2P RLLW config v2 */
	WL_HP2P_CMD_RLLW_STATS_V2	= 10	/* HP2P RLLW Stats v2 */
} hp2p_cmd_id_t;

/**
 * TX Profile.
 *
 * TX Profile allows the host to configure frames with a specific profile. This
 * includes but is not limited to transmit rate, number of retries, whether or
 * not to use frame aggregation, whether or not to use AMSDU and RTS protection
 * threshold. The original intent is for the host to be able to specify transmit
 * "reliability".
 */

/* IOVAR */
typedef struct wlc_tx_profile_ioc {
	uint16	id;
	uint16	length;
	union {
		uint32 options;
		uint32 status;
	} u;
	uint8	data[];
} wlc_tx_profile_ioc_t;

#define TX_PROFILE_IOV_HDR_SIZE (OFFSETOF(wlc_tx_profile_ioc_t, u))
#define TX_PROFILE_IOV_OPTION_SIZE (sizeof(wlc_tx_profile_ioc_t) - TX_PROFILE_IOV_HDR_SIZE)

/* TX Profile parameter TLV */
enum tx_profile_param_id {
	WL_TX_PROFILE_PARAM_RATE	= 0,
	WL_TX_PROFILE_PARAM_RTS		= 1,
	WL_TX_PROFILE_PARAM_RETRY	= 2,
	WL_TX_PROFILE_PARAM_BW		= 3,
	WL_TX_PROFILE_PARAM_AGGRE	= 4,
	WL_TX_PROFILE_PARAM_AMSDU	= 5,
	WL_TX_PROFILE_PARAM_AMPDU	= 6,
	WL_TX_PROFILE_PARAM_LIFETIME	= 7,
	WL_TX_PROFILE_PARAM_LAST	= 8
};

typedef enum tx_profile_param_id tx_profile_param_id_t;

typedef struct tx_profile_param {
	uint16			param_id;
	uint16			length;
	uint8			param[];
} tx_profile_param_t;

/* Subcommands */
typedef enum tx_profile_cmd_id {
	/* The IOVAR to enable/disable the TX Profile in general. An integer
	 *  is passed to the firmware with value 0 or 1.
	 */
	WL_TX_PROFILE_ENABLE	= 0,
	/* Create a TX Profile with provided parameters. */
	WL_TX_PROFILE_CREATE	= 1,
	/* Update a TX profile with provided parameters. */
	WL_TX_PROFILE_UPDATE	= 2,
	/* Delete a TX Profile. */
	WL_TX_PROFILE_DELETE	= 3,
	/* Dump TX Profile related debug information. */
	WL_TX_PROFILE_DUMP	= 4,
	/* Show TX Profile stats */
	WL_TX_PROFILE_STATS	= 5,
	/* Show Tx Profile tagged packets log */
	WL_TX_PROFILE_PKT_LOGS	= 6,
	/* Set the number of logs for the profile tagged packets */
	WL_TX_PROFILE_PKT_LOG_NUM = 7
} tx_profile_cmd_id_t;

typedef struct tx_profile_index_params {
	uint16	profile_index;
	uint16	length;
	uint8	params[];
} tx_profile_index_params_t;

typedef struct tx_profile_index_stats {
	uint32	profile_index;
	uint32	tx_finish_cnt;
	uint32	tx_acked_cnt;
	uint32	tx_phy_err_cnt;
	uint32	tx_suspend_cnt;
} tx_profile_index_stats_t;

typedef struct tx_profile_pkt_log {
	uint32	rate;
	uint16	retries;
	uint16	rts;
} tx_profile_pkt_log_t;

/* End TX Profile section */

typedef enum wl_rffe_cmd_type {
	WL_RFFE_CMD_DEBUG_MODE		= 0,
	WL_RFFE_CMD_ELNABYP_MODE	= 1,
	WL_RFFE_CMD_REG			= 2,
	WL_RFFE_CMD_ELNA_VDD_MODE	= 3,
	WL_RFFE_CMD_DRV_STRENGTH        = 4,
	WL_RFFE_CMD_RX_MODE             = 5,
	WL_RFFE_CMD_LAST
} wl_rffe_cmd_type_t;

/** RFFE struct passed through ioctl */
typedef struct {
	uint32	regaddr;	/**< rFEM_RegAddr */
	uint32	antnum;		/**< rFEM AntNum */
	uint32	slaveid;	/**< rFEM SlaveID */
	uint32	value;		/**< read/write value */
} rffe_reg_t;

typedef struct {
	uint32	slaveid;	/**< rFEM SlaveID */
	uint32	ds_val;		/**< drive strength code */
} rffe_drv_strength_t;

/* subcommand ids for srcb */
enum wl_srcb_cmd_type {
	WL_SRCB_CMD_REG = 0u,	/* get/set registers */
	WL_SRCB_CMD_LAST
};

#define WL_SRCB_REG_VERSION_1	1u

typedef struct wl_srcb_reg_v1 {
	uint32 address;		/* address */
	uint32 value;		/* value */
} wl_srcb_reg_v1_t;

typedef struct wl_srcb_v1 {
	uint16  subcmd_version;		/* Version of the sub-command */
	uint16  length;			/* Length of the particular struct being used in union */
	union {
		wl_srcb_reg_v1_t reg_v1;	/* reg data */
	} u;
} wl_srcb_v1_t;

#ifndef BCMUTILS_ERR_CODES

/*
 * SOE (Security Offload Engine) status codes.
 */

/*  SOE status codes are reserved from -6144 to -7167 (1K) */

enum wl_soe_status {
	/* Invalid operational context */
	WL_SOE_E_BAD_OP_CONTEXT				= -6144,

	/* Invalid operational type */
	WL_SOE_E_BAD_OP_TYPE				= -6145,

	/* Failure to get NAF3 encoded scalar */
	WL_SOE_E_BN_GET_NAF3_ERROR			= -6146,

	/* Failure to get NAF3 params */
	WL_SOE_E_ECG_GET_NAF3_PARAMS_ERROR		= -6147,

	/* FAILURE to get Montgomery params */
	WL_SOE_E_MONT_PARAMS_GET_ERROR			= -6148,

	/* Invalid OSL handle */
	WL_SOE_E_BAD_SI_OSH				= -6149,

	/* Invalid ECG group */
	WL_SOE_E_BAD_ECG_GROUP				= -6150,

	/* Invalid BN context */
	WL_SOE_E_BAD_BN_CTX				= -6151,

	/* Invalid SOE core register base address */
	WL_SOE_E_BAD_SOE_REGBASE			= -6152,

	/* Invalid SOE context */
	WL_SOE_E_BAD_SOE_CONTXT				= -6153,

	/* Number of words are too short (i.e., not enough
	 * room to encode the PKA sequence)
	 */
	WL_SOE_E_PKA_SEQUENCE_WORDS_TOO_SHORT		= -6154,

	/* Generic bn_get error */
	WL_SOE_E_PKA_BN_GET_ERROR			= -6155,

	/* Sequence buf too short for BN */
	WL_SOE_E_PKA_BN_BUF_TOO_SHORT_BN		= -6156,

	/* Sequence buf too short for ECG prime */
	WL_SOE_E_PKA_BN_BUF_TOO_SHORT_ECG_PRIME		= -6157,

	/* Sequence buf too short for Montgomery N' */
	WL_SOE_E_PKA_SEQ_BUF_TOO_SHORT_MONT_PRIME	= -6158,

	/* Sequence buf too short for Accumulator registers */
	WL_SOE_E_PKA_SEQ_BUF_TOO_SHORT_ACCM_REG		= -6159,

	/* Sequence buf too short for the point P */
	WL_SOE_E_PKA_SEQ_BUF_TOO_SHORT_P		= -6160,

	/* Sequence buf too short for -P */
	WL_SOE_E_PKA_SEQ_BUF_TOO_SHORT_PN		= -6161,

	/* Sequence buf too short for 3P */
	WL_SOE_E_PKA_SEQ_BUF_TOO_SHORT_3P		= -6162,

	/* Sequence buf too short for -3P */
	WL_SOE_E_PKA_SEQ_BUF_TOO_SHORT_3PN		= -6163,

	/* Sequence buf too short for NAF3 scalar */
	WL_SOE_E_PKA_SEQ_BUF_TOO_SHORT_NAF3_SCALAR	= -6164,

	/* Sequence buf too short for load shift count */
	WL_SOE_E_PKA_SEQ_BUF_TOO_SHORT_PRE_JMP		= -6165,

	/* SOE engine(SHA/PKA) failed to complete the operation */
	WL_SOE_E_ENGINE_UNABLE_TO_COMPLETE		= -6166,

	/* Wrong LIR (Long Integer Register) type */
	WL_SOE_E_PKA_BAD_LIR_TYPE			= -6167,

	/* Reference count has reached maximum */
	WL_SOE_E_MAX_REF_COUNT_REACHED			= -6168,

	/* Failed to get the SOE context reference */
	WL_SOE_E_GET_REF_FAILED				= -6169,

	/* Incoming digest length is invalid */
	WL_SOE_E_SHA_WRONG_DIGEST_LEN			= -6170
};

#endif /* BCMUTILS_ERR_CODES */

#define NR5GCX_STATUS_VER_1 1
/* NR coex status structures */
typedef struct wlc_nr5gcx_status_v1 {
	uint16 version;			/* version info */
	uint16 len;			/* status length */
	uint32 mode;			/* NR coex status */
	uint32 nr_req_cnt;		/* NR req number since last read */
	uint32 nr_dur;			/* NR duration since last read, us */
	uint32 nr_duty_cycle;		/* NR duty cycle since last read */
	uint32 nr_max_dur;		/* NR max duration in a single request */
	uint32 wlan_crit_cnt;		/* aggregated # of WLAN critical events */
	uint32 wlan_crit_dur;		/* aggregated WLAN critical event duration, ms */
	uint32 wlan_crit_max_dur;	/* Duration of the WLAN critical events whose dur is max */
	uint16 wlan_crit_evt_bitmap;	/* WLAN critical event occurrence bitmap,
					* 1 event per bit.
					*/
	uint16 wlan_crit_max_evt_type;	/* The event type of the WLAN critical
					* event whose dur is max
					*/
} wlc_nr5gcx_status_v1_t;

#define NR5GCX_STATUS_VER_2 2
/* NR coex status structures */
typedef struct wlc_nr5gcx_status_v2 {
	uint16 version;			/* version info */
	uint16 len;			/* status length */
	uint32 mode;			/* NR coex status */
	uint32 nr_req_cnt;		/* NR req number since last read */
	uint32 nr_dur;			/* NR duration since last read, us */
	uint32 nr_duty_cycle;		/* NR duty cycle since last read */
	uint32 nr_max_dur;		/* NR max duration in a single request */
	uint32 wlan_crit_cnt;		/* aggregated # of WLAN critical events */
	uint32 wlan_crit_dur;		/* aggregated WLAN critical event duration, ms */
	uint32 wlan_crit_max_dur;	/* Duration of the WLAN critical events whose dur is max */
	uint16 wlan_crit_evt_bitmap;	/* WLAN critical event occurrence bitmap,
					* 1 event per bit.
					*/
	uint16 wlan_crit_max_evt_type;	/* The event type of the WLAN critical
					* event whose dur is max
					*/
	uint16 data_stall_cnt;		/* # of data stall, i.e., # of rate recovery. */
	uint16 nr_deny_cnt_data_stall;	/* # of NR deny due to data stall. */
	uint16 nr_deny_dur_data_stall;	/* Duration of NR deny due to data stall. */
	uint16 succ_rr_cnt;		/* # of successful rate recovery. */
} wlc_nr5gcx_status_v2_t;

#define NR5GCX_STATUS_VER_4 4
/* NR coex status structures */
typedef struct wlc_nr5gcx_status_v4 {
	uint16 version;			/* version info */
	uint16 len;			/* status length */
	uint32 mode;			/* NR coex status */
	uint32 nr_req_cnt;		/* NR req number since last read */
	uint32 nr_dur;			/* NR duration since last read, us */
	uint32 nr_duty_cycle;		/* NR duty cycle since last read */
	uint32 nr_max_dur;		/* NR max duration in a single request */
	uint32 wlan_crit_cnt;		/* aggregated # of WLAN critical events */
	uint32 wlan_crit_dur;		/* aggregated WLAN critical event duration, ms */
	uint32 wlan_crit_max_dur;	/* Duration of the WLAN critical events whose dur is max */
	uint16 wlan_crit_evt_bitmap;	/* WLAN critical event occurrence bitmap,
					* 1 event per bit.
					*/
	uint16 wlan_crit_max_evt_type;	/* The event type of the WLAN critical
					* event whose dur is max
					*/
	uint16 data_stall_cnt;		/* # of data stall, i.e., # of rate recovery. */
	uint16 nr_deny_cnt_data_stall;	/* # of NR deny due to data stall. */
	uint16 nr_deny_dur_data_stall;	/* Duration of NR deny due to data stall. */
	uint16 succ_rr_cnt;		/* # of successful rate recovery. */
	uint16 rc1_stuck_cnt;           /* RC1 stuck count */
	uint8 pad[2];
} wlc_nr5gcx_status_v4_t;

#define RC1CX_STATUS_VER_1 1
/* RC1 coex status structures */
typedef struct wlc_rc1cx_status_v1 {
	uint16 version;			/* version info */
	uint16 len;			/* status length */
	uint32 mode;			/* RC1 coex status */
	uint32 rc1_req_cnt;		/* RC1 req number since last read */
	uint32 rc1_dur;			/* RC1 duration since last read, us */
	uint32 rc1_duty_cycle;		/* RC1 duty cycle since last read */
	uint32 rc1_max_dur;		/* RC1 max duration in a single request */
	uint32 wlan_crit_cnt;		/* aggregated # of WLAN critical events */
	uint32 wlan_crit_dur;		/* aggregated WLAN critical event duration, ms */
	uint32 wlan_crit_max_dur;	/* Duration of the WLAN critical events whose dur is max */
	uint16 wlan_crit_evt_bitmap;	/* WLAN critical event occurrence bitmap,
					* 1 event per bit.
					*/
	uint16 wlan_crit_max_evt_type;	/* The event type of the WLAN critical
					* event whose dur is max
					*/
} wlc_rc1cx_status_v1_t;

#define RC1CX_STATUS_VER_2 2
/* RC1 coex status structures */
typedef struct wlc_rc1cx_status_v2 {
	uint16 version;			/* version info */
	uint16 len;			/* status length */
	uint32 mode;			/* RC1 coex status */
	uint32 rc1_req_cnt;		/* RC1 req number since last read */
	uint32 rc1_dur;			/* RC1 duration since last read, us */
	uint32 rc1_duty_cycle;		/* RC1 duty cycle since last read */
	uint32 rc1_max_dur;		/* RC1 max duration in a single request */
	uint32 wlan_crit_cnt;		/* aggregated # of WLAN critical events */
	uint32 wlan_crit_dur;		/* aggregated WLAN critical event duration, ms */
	uint32 wlan_crit_max_dur;	/* Duration of the WLAN critical events whose dur is max */
	uint16 wlan_crit_evt_bitmap;	/* WLAN critical event occurrence bitmap,
					* 1 event per bit.
					*/
	uint16 wlan_crit_max_evt_type;	/* The event type of the WLAN critical
					* event whose dur is max
					*/
	uint16 data_stall_cnt;		/* # of data stall, i.e., # of rate recovery. */
	uint16 rc1_deny_cnt_data_stall;	/* # of RC1 deny due to data stall. */
	uint16 rc1_deny_dur_data_stall;	/* Duration of RC1 deny due to data stall. */
	uint16 succ_rr_cnt;		/* # of successful rate recovery. */
} wlc_rc1cx_status_v2_t;

#define RC1CX_STATUS_VER_3 3
/* RC1 coex status structures */
typedef struct wlc_rc1cx_status_v3 {
	uint16 version;			/* version info */
	uint16 len;			/* status length */
	uint32 mode;			/* RC1 coex status */
	uint32 rc1_req_cnt;		/* RC1 legacy req number since last read */
	uint32 rc1_dur;			/* RC1 legacy duration since last read, us */
	uint32 rc1_duty_cycle;		/* RC1 legacy duty cycle since last read */
	uint32 rc1_max_dur;		/* RC1 legacy max duration in a single request */
	uint32 wlan_crit_cnt;		/* aggregated # of WLAN critical events */
	uint32 wlan_crit_dur;		/* aggregated WLAN critical event duration, ms */
	uint32 wlan_crit_max_dur;	/* Duration of the WLAN critical events whose dur is max */
	uint16 wlan_crit_evt_bitmap;	/* WLAN critical event occurrence bitmap,
					* 1 event per bit.
					*/
	uint16 wlan_crit_max_evt_type;	/* The event type of the WLAN critical
					* event whose dur is max
					*/
	uint16 data_stall_cnt;		/* # of data stall, i.e., # of rate recovery. */
	uint16 rc1_deny_cnt_data_stall;	/* # of RC1 deny due to data stall. */
	uint16 rc1_deny_dur_data_stall;	/* Duration of RC1 deny due to data stall. */
	uint16 succ_rr_cnt;		/* # of successful rate recovery. */
	uint32 rc1_new_req_cnt;		/* RC1_new req number since last read */
	uint32 rc1_new_dur;		/* RC1_new duration since last read, us */
	uint32 rc1_new_duty_cycle;	/* RC1_new duty cycle since last read */
	uint32 rc1_new_max_dur;		/* RC1_new max duration in a single request */
	uint16 rc1_msg73_cnt;		/* Msg73 count */
	uint8 rc1_last_msg73_pl[5];	/* the most recent Msg73 payload */
	uint8 pad;
} wlc_rc1cx_status_v3_t;

#define RC1CX_STATUS_VER_4 4
/* RC1 coex status structures */
typedef struct wlc_rc1cx_status_v4 {
	uint16 version;			/* version info */
	uint16 len;			/* status length */
	uint32 mode;			/* RC1 coex status */
	uint32 rc1_req_cnt;		/* RC1 req number since last read */
	uint32 rc1_dur;			/* RC1 duration since last read, us */
	uint32 rc1_duty_cycle;		/* RC1 duty cycle since last read */
	uint32 rc1_max_dur;		/* RC1 max duration in a single request */
	uint32 wlan_crit_cnt;		/* aggregated # of WLAN critical events */
	uint32 wlan_crit_dur;		/* aggregated WLAN critical event duration, ms */
	uint32 wlan_crit_max_dur;	/* Duration of the WLAN critical events whose dur is max */
	uint16 wlan_crit_evt_bitmap;	/* WLAN critical event occurrence bitmap,
					* 1 event per bit.
					*/
	uint16 wlan_crit_max_evt_type;	/* The event type of the WLAN critical
					* event whose dur is max
					*/
	uint16 data_stall_cnt;		/* # of data stall, i.e., # of rate recovery. */
	uint16 rc1_deny_cnt_data_stall;	/* # of RC1 deny due to data stall. */
	uint16 rc1_deny_dur_data_stall;	/* Duration of RC1 deny due to data stall. */
	uint16 succ_rr_cnt;		/* # of successful rate recovery. */
	uint16 rc1_stuck_cnt;		/* RC1 stuck count */
	uint8 pad[2];
} wlc_rc1cx_status_v4_t;

#define RC1CX_STATUS_VER_5 5
/* RC1 coex status structures */
typedef struct wlc_rc1cx_status_v5 {
	uint16 version;			/* version info */
	uint16 len;			/* status length */
	uint32 mode;			/* RC1 coex status */
	uint32 rc1_req_cnt;		/* RC1 legacy req number since last read */
	uint32 rc1_dur;			/* RC1 legacy duration since last read, us */
	uint32 rc1_duty_cycle;		/* RC1 legacy duty cycle since last read */
	uint32 rc1_max_dur;		/* RC1 legacy max duration in a single request */
	uint32 wlan_crit_cnt;		/* aggregated # of WLAN critical events */
	uint32 wlan_crit_dur;		/* aggregated WLAN critical event duration, ms */
	uint32 wlan_crit_max_dur;	/* Duration of the WLAN critical events whose dur is max */
	uint16 wlan_crit_evt_bitmap;	/* WLAN critical event occurrence bitmap,
					* 1 event per bit.
					*/
	uint16 wlan_crit_max_evt_type;	/* The event type of the WLAN critical
					* event whose dur is max
					*/
	uint16 data_stall_cnt;		/* # of data stall, i.e., # of rate recovery. */
	uint16 rc1_deny_cnt_data_stall;	/* # of RC1 deny due to data stall. */
	uint16 rc1_deny_dur_data_stall;	/* Duration of RC1 deny due to data stall. */
	uint16 succ_rr_cnt;		/* # of successful rate recovery. */
	uint32 rc1_new_req_cnt;		/* RC1_new req number since last read */
	uint32 rc1_new_dur;		/* RC1_new duration since last read, us */
	uint32 rc1_new_duty_cycle;	/* RC1_new duty cycle since last read */
	uint32 rc1_new_max_dur;		/* RC1_new max duration in a single request */
	uint16 rc1_msg73_cnt;		/* Msg73 count */
	uint8 rc1_last_msg73_pl[5];	/* the most recent Msg73 payload */
	uint8 pad;
	uint16 rc1_stuck_cnt;		/* RC1 stuck count */
	uint8 pad2[2];
} wlc_rc1cx_status_v5_t;


/* ifdef (WLC_OBSS_HW) */
/* OBSS HW specific Macros */
#define WLC_OBSS_HW_CMD_VERSION_1	1u
#define WLC_OBSS_HW_CMD_VERSION_2	2u
#define WLC_OBSS_HW_CMD_VERSION_3	3u

/* OBSS HW config sub command identification flag */
#define OBSS_HW_CFG_SUB_CMD_ENABLE		(1u << 0u)
#define OBSS_HW_CFG_SUB_CMD_SW_CACHE_INTVL	(1u << 1u)
#define OBSS_HW_CFG_SUB_CMD_PHY_SENSE_DUR	(1u << 2u)
#define OBSS_HW_CFG_SUB_CMD_MASK		(OBSS_HW_CFG_SUB_CMD_ENABLE | \
						OBSS_HW_CFG_SUB_CMD_SW_CACHE_INTVL | \
						OBSS_HW_CFG_SUB_CMD_PHY_SENSE_DUR)

#define WLC_OBSS_HW_DEF_SW_CACHE_INTVL		1u /* 1 sec */
#define WLC_OBSS_HW_MAX_SW_CACHE_INTVL		5u /* 5 sec */
#define WLC_OBSS_HW_DEF_PHY_SENSE_DUR		30 /* 30 msec */
#define WLC_OBSS_HW_MAX_PHY_SENSE_DUR		500 /* 500 msec */

/* OBSS HW test sub command identification flag */
#define WLC_OBSS_HW_TEST_SUB_CMD_TEST_MODE	(1u << 0u)
#define WLC_OBSS_HW_TEST_SUB_CMD_MITI_MODE	(1u << 1u)
#define WLC_OBSS_HW_TEST_SUB_CMD_MASK		(WLC_OBSS_HW_TEST_SUB_CMD_TEST_MODE |\
							WLC_OBSS_HW_TEST_SUB_CMD_MITI_MODE)

/* Test mode values */
#define WLC_OBSS_HW_TEST_MODE_STOP		0u /* Stop the test */
#define WLC_OBSS_HW_TEST_MODE_TRIGGER		1u /* Trigger mode */
#define WLC_OBSS_HW_TEST_MODE_FREE_RUNNING	2u /* Free running stats collection */

#define WLC_OBSS_HW_TEST_MITI_TX_RX_DISABLE	0u /* Both Tx and Rx mitigation disabled */
#define WLC_OBSS_HW_TEST_MITI_RX_FILT_PKTENG	1u /* Rx mitigation via filter override enabled */
							/* All Tx miti disabled; Only pkteng */
#define WLC_OBSS_HW_TEST_MITI_TX_ONLY		2u /* Rx mitigation disabled, Tx mitigation */
#define WLC_OBSS_HW_TEST_MITI_TX_RX_FILT	3u /* Rx Tx mitigation enabled */
#define WLC_OBSS_HW_TEST_MITI_CHAN_CHANGE	4u /* Mitigation by chanspec change */
#define WLC_OBSS_HW_TEST_MITI_RX_ONLY		5u /* Tx mitigation disabled, Rx mitigation */
#define WLC_OBSS_HW_TEST_MITI_TX_RX_NO_FILT	6u /* Rx Tx mitigation enabled, no filter */
#define WLC_OBSS_HW_TEST_MITI_LAST		WLC_OBSS_HW_TEST_MITI_TX_RX_NO_FILT

#define WL_OBSS_ANT_MAX			2u	/* Max Antennas */
#define ACPHY_OBSS_STATS_BIN_CNT	8u	/* min 1 for default */

enum wlc_obss_hw_cmd_id {
	WLC_OBSS_HW_CMD_VER		= 1u,
	WLC_OBSS_HW_CMD_CONFIG		= 2u,
	WLC_OBSS_HW_CMD_DUMP_STATS	= 3u,
	WLC_OBSS_HW_CMD_TEST		= 4u,
	WLC_OBSS_HW_CMD_LAST
};

/* OBSS HW specific structures */
typedef struct wlc_obss_hw_cfg_v1 {
	uint16 sub_cmd_flags;		/* Flag bits to Identify configuring sub command */
	uint8 is_enable;		/* Feature is enabled or not */
	uint8 sw_cache_interval;	/* SW cache interval to cache OBSS stats in sec */
	uint16 phy_sensing_duration;	/* PHY OBSS sensing duration in msec */
} wlc_obss_hw_cfg_v1_t;

typedef struct  wlc_obss_hw_stats_v1 {
	uint16 avg_obss_stats[WL_OBSS_ANT_MAX][ACPHY_OBSS_SUBBAND_CNT][ACPHY_OBSS_STATS_BIN_CNT];
	uint16 obss_det_stats[ACPHY_OBSS_SUBBAND_CNT];
	uint16 stats_cnt;		/* Stats count */
	uint8 obss_mit_status;		/* OBSS mitigation status */
	uint8 mit_bw;			/* Mitigation BW that got selected */
} wlc_obss_hw_stats_v1_t;

typedef struct wlc_obss_hw_test_v1 {
	uint16 sub_cmd_flags;		/* Flag bits to Identify configuring sub command */
	uint8 test_mode;		/* To stop/start respective test mode */
	uint8 mitigation_mode;		/* mitigation enabling/disabling options */
} wlc_obss_hw_test_v1_t;

#define OBSS_HW_CFG_FEATURE_DISABLED						0u
#define OBSS_HW_CFG_STATS_AND_MITIGATION_ENABLED				1u
#define OBSS_HW_CFG_STATS_ONLY_ENABLED						2u
#define OBSS_HW_CFG_INVALID							255u

typedef struct wlc_obss_hw_cfg_v2 {
	uint16 sub_cmd_flags;		/* Flag bits to Identify configuring sub command */
	uint8 is_enable;		/* Feature  and mode enable flag */
	uint8 sw_cache_interval;	/* SW cache interval to cache OBSS stats in sec */
	uint16 phy_sensing_duration;	/* PHY OBSS sensing duration in msec */
} wlc_obss_hw_cfg_v2_t;

typedef struct  wlc_obss_hw_stats_v2 {
	uint16 avg_obss_stats[WL_OBSS_ANT_MAX][ACPHY_OBSS_SUBBAND_CNT][ACPHY_OBSS_STATS_BIN_CNT];
	uint16 obss_det_stats[ACPHY_OBSS_SUBBAND_CNT];
	uint16 stats_cnt;		/* Stats count */
	uint8 obss_mit_status;		/* OBSS mitigation status */
	uint8 mit_bw;			/* Mitigation BW that got selected */
	uint32  lifetime_expiry_cnt;    /* count of lifetime expired pkts */
	uint32  medium_access_delay;     /* sum of medium access delay */
	uint32  pkt_cnt;                /* number of pkts for which Tx status stats are logged */
	uint32  rts_pkts_cnt;           /* number of packets sent with RTS */
} wlc_obss_hw_stats_v2_t;

typedef struct wlc_obss_hw_test_v2 {
	uint16 sub_cmd_flags;		/* Flag bits to Identify configuring sub command */
	uint8 test_mode;		/* To stop/start respective test mode */
	uint8 mitigation_mode;		/* mitigation enabling/disabling options */
} wlc_obss_hw_test_v2_t;

typedef wlc_obss_hw_test_v2_t wlc_obss_hw_test_v3_t;
typedef wlc_obss_hw_cfg_v2_t wlc_obss_hw_cfg_v3_t;

/* obss_pwrest is normally a negative value in dBm,
 * when it is not negative, the following enum is
 * used to indicate exceptional cases as listed below.
 * The below structures and onwards will follow this format
 * - wlc_obss_hw_stats_v3_t.obss_pwrest[][]
 * - phy_ecounter_v5.phy_ecounter_log_core_v4_t.obss_pwrest[]
 * - phy_periodic_obss_stats_v4.obss_pwrest[]
 */
typedef enum obss_pwr_dbm_status {
	OBSS_PWRSTS_UNDEFINED		= 0,	/* Undefined */
	OBSS_PWRSTS_PRIM_SB		= 1,	/* Primary sub-band */
	OBSS_PWRSTS_SATURATED		= 2,	/* Measurement Saturated */
	OBSS_PWRSTS_NO_VALID_CNT	= 3,	/* No valid measure count */
	OBSS_PWRSTS_MAX
} obss_pwr_dbm_status_t;

typedef struct wlc_obss_hw_stats_v3 {
	uint16	avg_obss_stats[WL_OBSS_ANT_MAX][ACPHY_OBSS_SUBBAND_CNT][ACPHY_OBSS_STATS_BIN_CNT];
						/* average bin counts collected per bin across
						 * multiple sensing durations
						 */
	uint16	obss_det_stats[ACPHY_OBSS_SUBBAND_CNT];
						/* OBSS detection count for each sub-band */
	uint16	stats_cnt;			/* stats count */
	uint8	obss_mit_status;		/* OBSS mitigation status */
	uint8	mit_bw;				/* mitigation BW that got selected */
	uint32	lifetime_expiry_cnt;		/* count of lifetime expired pkts */
	uint32	medium_access_delay;		/* sum of medium access delay */
	uint32	pkt_cnt;			/* number of pkts for which Tx status stats are
						 * logged
						 */
	uint32	rts_pkts_cnt;			/* number of packets sent with RTS */
	int8	obss_pwrest[WL_OBSS_ANT_MAX][ACPHY_OBSS_SUBBAND_CNT];
						/* estimated OBSS signal power in dBm,
						 * please refer to obss_pwr_dbm_status_t
						 * for exceptional cases
						 */
	int8	soi_rssi;			/* rssi of SOI */
	uint8	deferred_enable_config;		/* Feature enable config if deferred */
	uint8	obss_last_rec_bw;		/* Last stats block recommended BW */
	uint8	current_enable_config;		/* Current active feature enable config */
} wlc_obss_hw_stats_v3_t;

#define STA_PM_SC_OFLD_CFG_VER_V1                1u
#define STA_PM_SC_OFLD_ENAB_FLAG                 (1u << 0u)

#define STA_PM_SC_FLAG_MASK                      (1u << 0u)
#define STA_PM_SC_CONS_EXP_BCN_RX_THR_MASK       (1u << 1u)
#define STA_PM_SC_MISSED_BCN_THR_MASK            (1u << 2u)
#define STA_PM_SC_OFLD_RSSI_THR_MASK             (1u << 3u)
#define STA_PM_SC_OFLD_RSSI_HYST_MASK            (1u << 4u)
#define STA_PM_SC_OFLD_TIM_EXIT_DLY_MASK         (1u << 5u)
#define STA_PM_SC_OFLD_FIELD_MASK_ALL    (STA_PM_SC_FLAG_MASK | \
				STA_PM_SC_CONS_EXP_BCN_RX_THR_MASK | \
				STA_PM_SC_MISSED_BCN_THR_MASK | \
				STA_PM_SC_OFLD_RSSI_THR_MASK | \
				STA_PM_SC_OFLD_RSSI_HYST_MASK | \
				STA_PM_SC_OFLD_TIM_EXIT_DLY_MASK)

#define STA_PM_SC_OFLD_RSSI_HYST_MAX              80u

typedef struct wlc_sta_pm_sc_ofld_cfg_v1 {
	uint16 version;
	uint16 len;
	uint16 field_mask;           /* Provides info on which of the following fields are valid */
	uint16 flags;                /* enable 0x1 */
	uint8  cons_exp_bcn_rx_thr;  /* Consecutive expected bcn in true slice to offload to SC */
	uint8  sc_missed_bcn_thr;    /* Missed beacons threshold in SC to exit offload */
	int8   sc_offload_rssi_thr;  /* Bcn RSSI threshold to exit offload, must be negative */
	uint8  sc_offload_rssi_hyst; /* Hysteresis for the RSSI threshold, 0 - RSSI_HYST_MAX */
	uint8  tim_exit_delay_ms;    /* TIM exit delay when DTIM AID=0 is also set */
	uint8  PAD[3];
} wlc_sta_pm_sc_ofld_cfg_v1_t;

typedef enum wlc_sta_pm_sc_ofld_fail_reason {
	STA_PM_SC_OFLD_FAIL_RSSI =			(1u << 0u),  /* Low rssi */
	STA_PM_SC_OFLD_FAIL_UNSUPPORTED =		(1u << 1u),  /* Unsupported by WBUS */
	STA_PM_SC_OFLD_FAIL_MISSED_BCN =		(1u << 2u),  /* missed bcns on true slice */
	STA_PM_SC_OFLD_FAIL_NO_PS =			(1u << 3u),  /* not in PM */
	STA_PM_SC_OFLD_FAIL_TX_ACTIVE =			(1u << 4u),  /* Active TX in true slice */
	STA_PM_SC_OFLD_FAIL_CSA =			(1u << 5u),  /* CSA */
	STA_PM_SC_OFLD_FAIL_MRC =			(1u << 6u),  /* in MRC */
	STA_PM_SC_OFLD_FAIL_AS_STATE =			(1u << 7u),  /* Assoc state not IDLE */
	STA_PM_SC_OFLD_FAIL_NO_BASIC_RATESET =		(1u << 8u),  /* AP bcn has no basic rate */
	STA_PM_SC_OFLD_FAIL_UNSUP_BCN_RATE_RX =		(1u << 8u),  /* AP bcn at SC unsup rate */
	STA_PM_SC_OFLD_FAIL_DISABLED =			(1u << 9u),  /* Offload disabled */
	STA_PM_SC_OFLD_FAIL_CAL =			(1u << 10u), /* Cal on main/aux core */
	STA_PM_SC_OFLD_FAIL_UNSUP_BCMC_RATE_RX =	(1u << 11u), /* unsupported bcmc rate rx */
	STA_PM_SC_OFLD_FAIL_TWT_ACTIVE =		(1u << 12u), /* TWT is active */
	STA_PM_SC_OFLD_FAIL_AP_ENAB =			(1u << 13u), /* AP cfg is enabled */
	STA_PM_SC_OFLD_FAIL_SLOTTED_BSS_ENAB =		(1u << 14u), /* Slotted BSS is enabled */
	STA_PM_SC_OFLD_FAIL_BTMC_ACTIVE =		(1u << 15u), /* BT Main Core is active */
	STA_PM_SC_OFLD_FAIL_UNSUP_BASIC_RATE =		(1u << 16u), /* SC Unsupported basic rate */
	STA_PM_SC_OFLD_FAIL_UNSUP_CHANSPEC =		(1u << 17u), /* SC Unsupported chanspec */
	STA_PM_SC_OFLD_FAIL_MLO_LINK_ACTIVE_IN_SC =	(1u << 18u), /* One of the MLO link is
								      * already offloaded to sc
								      */
	STA_PM_SC_OFLD_FAIL_UATBTT =			(1u << 19u), /* UATBTT active */
	STA_PM_SC_OFLD_FAIL_TDLS_ACTIVE =		(1u << 20u), /* TDLS active */
	STA_PM_SC_OFLD_FAIL_EMLSR_ACTIVE =		(1u << 21u)  /* EMLSR is Active */
} wlc_sta_pm_sc_ofld_fail_reason_t;

typedef enum wlc_sta_pm_sc_ofld_exit_reason {
	STA_PM_SC_OFLD_EXIT_AS_STATE		= 1u,	/* Exit due to assoc state not IDLE */
	STA_PM_SC_OFLD_EXIT_BCN_LOSS		= 2u,	/* Exit due to beacon loss */
	STA_PM_SC_OFLD_EXIT_TIM			= 3u,	/* Exit due to TIM bit set */
	STA_PM_SC_OFLD_EXIT_PM			= 4u,	/* Exit due to exit out of PM mode */
	STA_PM_SC_OFLD_EXIT_TX			= 5u,	/* Exit due to tx on true slice */
	STA_PM_SC_OFLD_EXIT_CSA			= 6u,	/* Exit due to CSA */
	STA_PM_SC_OFLD_EXIT_LOW_RSSI		= 7u,	/* Exit due to rssi lower than threshold */
	STA_PM_SC_OFLD_EXIT_MRC			= 8u,	/* Exit due to MRC being active */
	STA_PM_SC_OFLD_EXIT_BSSCFG_DOWN		= 9u,	/* Exit due to bsscfg down */
	STA_PM_SC_OFLD_EXIT_WLC_DOWN		= 10u,	/* Exit due to wlc down */
	STA_PM_SC_OFLD_EXIT_WBUS_REJECT		= 11u,	/* Exit due to wbus reject */
	STA_PM_SC_OFLD_EXIT_HOST_DISABLE	= 12u,	/* Exit due to disable by host */
	STA_PM_SC_OFLD_EXIT_CAL_TVPM		= 13u,	/* Exit due to Cal/TVPM on main/aux core */
	STA_PM_SC_OFLD_EXIT_DISASSOC		= 14u,	/* Exit due to disassoc */
	STA_PM_SC_OFLD_EXIT_CAC			= 15u,	/* Exit due to CAC admit */
	STA_PM_SC_OFLD_EXIT_CHN_OVERLAP		= 16u,	/* Exit due to true slice chan overlap */
	STA_PM_SC_OFLD_EXIT_UNSUP_BCMC_RATE_RX	= 17u,	/* Exit due to unsupported bcmc rate rx */
	STA_PM_SC_OFLD_EXIT_BCMC_LOSS		= 18u,	/* Exit due to bcmc pkt loss */
	STA_PM_SC_OFLD_EXIT_TWT			= 19u,	/* Exit due to TWT active */
	STA_PM_SC_OFLD_EXIT_SLOTTED_BSS		= 20u,	/* Exit due to Slotted BSS active */
	STA_PM_SC_OFLD_EXIT_AP_BSS		= 21u,	/* Exit due to AP BSS active */
	STA_PM_SC_OFLD_EXIT_MLO			= 22u,	/* Exit due to high priority MLO link */
	STA_PM_SC_OFLD_EXIT_TDLS		= 23u,	/* Exit due to TDLS active */
	STA_PM_SC_OFLD_EXIT_EMLSR_ML_MODE_CHANGE = 24u,	/* Exit due to EMLSR ML mode change */
	STA_PM_SC_OFLD_EXIT_MAX			= 255u	/* Max, uint8 for now */
} wlc_sta_pm_sc_ofld_exit_reason_t;

#define STA_PM_SC_OFLD_STATUS_VER_V1                1u

#define STA_PM_SC_OFLD_STATUS_CLEAR          (1u << 0u)
#define STA_PM_SC_OFLD_STATUS_OFFLOADED      (1u << 1u)
#define STA_PM_SC_OFLD_STATUS_ASSOCIATED     (1u << 2u)

typedef struct wlc_sta_pm_sc_status_v1 {
	uint16 version;
	uint16 len;
	uint32 flags;
	uint32 sc_offload_enter_cnt;           /* Offload enter cnt */
	uint32 sc_offload_exit_cnt;            /* Offload exit cnt */
	uint32 sc_offload_wbus_reject_cnt;     /* WBUS reject reg cnt */
	uint32 sc_offload_exp_bcn_cnt;         /* SC ofld expected bcn cnt */
	uint32 sc_offload_exp_bcn_missed_cnt;  /* SC ofld missed bcn cnt */
	uint32 sc_offload_last_exit_reason;    /* Previous exit reason */
	uint32 sc_offload_enter_fail_reason;   /* SC ofld enter fail reason  */
	uint32 sc_offload_total_dur_ms;        /* Cumulative duration in offload (ms) */
	uint32 sc_offload_tim_exit_dly_cnt;    /* TIM exit delay cnt */
} wlc_sta_pm_sc_status_v1_t;

#define WL_SDTC_IOV_VERSION		1
#define SDTC_SUB_IOV_VERSION_1		1
#define SDTC_SUB_IOV_VERSION_1_1	1
#define MAX_ATID_CORES			5u
#define SDTC_ID_ALL			0xFFu

/* SDTC IOV ID's. SW assigned IDs to each of the SDTC units. */
enum wl_sdtc_iov_id {
	SDTC_ID_INVALID		= 0x0, /* Invalid SDTC id. This is used for ETM internally */
	SDTC_ID_CB		= 0x1, /* Common Bridge AXL SDTC */
	SDTC_ID_PCIE		= 0x2, /* AXL SDTC between common bridge and PCIE core. */
	SDTC_ID_SYSMEM		= 0x3, /* AXL between WLAN backplane and CA7 subsystem */
	SDTC_ID_AUXMAC		= 0x4, /* HWL inside AUX MAC */
	SDTC_ID_MAINMAC		= 0x5, /* HWL inside MAIN MAC */
	SDTC_ID_LP		= 0x6, /* HWL connected to Async APB bridge */
	SDTC_ID_AUXMAC_AXL	= 0x7, /* AXL between booker interconnect and AUX MAC */
	SDTC_ID_MAINMAC_AXL	= 0x8, /* AXL between booker interconnect and MAIN MAC */
	SDTC_ID_SCANMAC_HWL	= 0x9, /* HWL inside SCAN MAC */
	SDTC_ID_SCANMAC_AXL	= 0xa, /* AXL between booker interconnect and SCAN MAC */
	SDTC_ID_SAQM_AXL	= 0xb, /* AXL between booker interconnect and  SAQM */
	SDTC_ID_SAQM_HWL	= 0xc, /* HWL inside SAQM */
	SDTC_ID_WL_AXL		= 0xd, /* AXL for WL_SDTC */
	SDTC_ID_LAST		= 0xe
};

/* SDTC Iovars */
enum wl_sdtc_cmd_ids {
	WL_SDTC_CMD_EN			= 0x1,
	WL_SDTC_CMD_CONFIG		= 0x2,
	WL_SDTC_CMD_CORE_ENAB		= 0x3,
	WL_SDTC_CMD_ETB_INFO		= 0x4,
	WL_SDTC_CMD_LAST
};

enum wl_sdtc_xtlv_version {
	WL_SDTC_ENABLE_V1  = 0x1
};

typedef struct sdtc_regs {
	uint32 addr;
	uint32 val;
} sdtc_regs_t;

typedef struct sdtc_config {
	uint16		version;
	uint16		len;

	uint8		sdtc_id;	/* 0xFF indicates, all core id's enable */
	uint8		PAD;		/* 32 bit alignment */
	uint16		num_of_registers; /* if no of reg is "0",it will use default SDTC config */
	sdtc_regs_t	regs[BCM_FLEX_ARRAY];	/* Array of register list */
} sdtc_config_t;

typedef struct sdtc_enab_atid_list {
	uint16	version;
	uint16	len;

	uint32	sdtc_id_bmp;
	uint32	sdtc_id_mask;
} sdtc_enab_atid_list_t;

typedef struct etb_addr_info {
	uint16	version;
	uint16	len;
	uint32	etbinfo_addr;	/* etb_info address */
} etb_addr_info_t;

typedef struct etb_info {
	uint16	version;
	uint16	len;
	uint32	read_write_p;	/* read write pointer address */
	uint8	etb_full;	/* status bit */
	uint8	PAD[3];		/* 32bit alignment */
	uint32	addr;		/* H/W Address */
	uint32	read_bytes;	/* Size of ETB(Embedded Trace Buffer) Memory */
} etb_info_t;

/* This ROAM RSSI limit value is used in order to prune LOW RSSI candidate
 * for Priority roam and Beacon lost
 * WLC_E_REASON_DEAUTH, WLC_E_REASON_DISASSOC, WLC_E_REASON_BCNS_LOST
 */
#define WLC_ROAM_RSSI_LMT_VER_1	1
typedef struct wlc_roam_rssi_lmt_info_v1 {
	int16 rssi_limit_2g;
	int16 rssi_limit_5g;
} wlc_roam_rssi_lmt_info_v1_t;

typedef struct wlc_wlc_roam_rssi_limit {
	uint16 ver;
	uint16 len;
	uint8 data[];
} wlc_roam_rssi_limit_t;


#define WLC_ROAM_5G_BANDBOOST_VER_1	1
typedef struct wlc_roam_5g_band_boost_info_v1 {
	uint16 ver;
	uint16 len;
	int8 tgt_thresh;
	int8 cur_thresh;
	uint8 PAD[2];
} wlc_roam_5g_band_boost_info_v1_t;

#define ROAMRSSI_HDRLEN		4u
#define ROAMRSSI_2G_DEFAULT	-128
#define ROAMRSSI_2G_MAX		-70
#define ROAMRSSI_2G_MIN		-128
#define ROAMRSSI_5G_DEFAULT	-128
#define ROAMRSSI_5G_MAX		-70
#define ROAMRSSI_5G_MIN		-128

#define WLC_ROAM_MIN_DELTA_VER_1  1
typedef struct wlc_roam_min_score_delta_info_v1 {
	uint32 roam_min_delta_2g;
	uint32 roam_min_delta_5g;
} wlc_roam_min_delta_info_v1_t;

typedef struct wlc_roam_min_delta {
	uint16 ver;
	uint16 len;
	uint8 data[];
} wlc_roam_min_delta_t;

#define ROAM_MIN_DELTA_HDRLEN	4u
#define ROAM_MIN_DELTA_DEFAULT    0u
/* MAX score is (RSSI Factor MAX * RSSI weight) + (CU factor MAX * CU weight)
 * 10000 = (100 * 65) + (100 * 35)
 */
#define ROAM_MIN_DELTA_MAX        10000u

/* Beacon mute mitigation specific Macros */

/* Macro flags to identify the specific config commands in IOVAR set operation */
#define WLC_BCN_MUTE_MITI_CFG_CMD_ENABLE		(1u << 0u)
#define WLC_BCN_MUTE_MITI_CFG_CMD_PM0_PERIOD		(1u << 1u)
#define WLC_BCN_MUTE_MITI_CFG_CMD_MAX_MITI_TIMEOUT	(1u << 2u)
#define WLC_BCN_MUTE_MITI_CFG_CMD_LOW_RSSI_THRESH	(1u << 3u)
#define WLC_BCN_MUTE_MITI_CFG_CMD_ML_SUPPORT_ENAB	(1u << 4u)

/* Define the config command mask */
#define WLC_BCN_MUTE_MITI_CFG_CMD_MASK		(WLC_BCN_MUTE_MITI_CFG_CMD_ENABLE |\
						WLC_BCN_MUTE_MITI_CFG_CMD_PM0_PERIOD |\
						WLC_BCN_MUTE_MITI_CFG_CMD_MAX_MITI_TIMEOUT |\
						WLC_BCN_MUTE_MITI_CFG_CMD_LOW_RSSI_THRESH |\
						WLC_BCN_MUTE_MITI_CFG_CMD_ML_SUPPORT_ENAB)

/* Configurable PM0 period range in sec */
#define WLC_BCN_MUTE_MITI_CFG_PM0_PERIOD_MIN		(0u)	/* Minimum PM0 periodicity */
#define WLC_BCN_MUTE_MITI_CFG_PM0_PERIOD_DEFAULT	(1u)	/* Default PM0 periodicity */
#define WLC_BCN_MUTE_MITI_CFG_PM0_PERIOD_MAX		(10u)	/* Maximum PM0 priodicity */

/* Configurable mitigation Timeout range */
#define WLC_BCN_MUTE_MITI_CFG_TIMEOUT_MIN		(30u)	/* Minimum Timeout in sec */
#define WLC_BCN_MUTE_MITI_CFG_TIMEOUT_DEFAULT		(60u)	/* Default Timeout in sec */
#define WLC_BCN_MUTE_MITI_CFG_TIMEOUT_MAX		(120u)	/* Maximum Timeout in sec */

/* Configurable RSSI Threshold ranges */
#define WLC_BCN_MUTE_MITI_CFG_RSSI_THRESH_MAX		(0)	/* Maximum RSSI threshold */
#define WLC_BCN_MUTE_MITI_CFG_RSSI_THRESH_DEFAULT	(WLC_BCN_MUTE_MITI_CFG_RSSI_THRESH_MAX)
#define WLC_BCN_MUTE_MITI_CFG_RSSI_THRESH_MIN		(-127)	/* Min RSSI threshold */

#define WLC_BCN_MUTE_MITI_CMD_VER_1	1u /* Version number for wlc_bcn_mute_miti_cfg_v1 struct */
#define WLC_BCN_MUTE_MITI_CMD_VER_2	2u /* Version number for wlc_bcn_mute_miti_cfg_v2 struct */
#define WLC_BCN_MUTE_MITI_CMD_VER_3	3u /* Version number for wlc_bcn_mute_miti_cfg_v3 struct */
typedef struct wlc_bcn_mute_miti_cfg_v1 {
	uint16	version;		/* Structure version */
	uint16	length;			/* Length of whole struct */
	uint32	cmd_flags;		/* Flag bits to Identify configuring command */
	uint8	is_enabled;		/* Feature is enabled or not */
	uint8	pm0_periodicity;	/* Interval between consecutive PM0 during mitigation
					 * period (in sec)
					 */
	uint16	max_miti_timeout;	/* Maximum mitigation timeout in sec to send
					 * Mitigation Timeout event.
					 */
} wlc_bcn_mute_miti_cfg_v1_t;

typedef struct wlc_bcn_mute_miti_cfg_v2 {
	uint16	version;		/* Structure version */
	uint16	length;			/* Length of whole struct */
	uint32	cmd_flags;		/* Flag bits to Identify configuring command */
	uint8	is_enabled;		/* Feature is enabled or not */
	uint8	pm0_periodicity;	/* Interval between consecutive PM0 during mitigation
					 * period (in sec)
					 */
	uint16	max_miti_timeout;	/* Maximum mitigation timeout in sec to send
					 * Mitigation Timeout event.
					 */
	int8	low_rssi_threshold;	/* Low RSSI threshold value */
	uint8	PAD[3];			/* Pad bytes */
} wlc_bcn_mute_miti_cfg_v2_t;

typedef struct wlc_bcn_mute_miti_cfg_v3 {
	uint16	version;		/* Structure version */
	uint16	length;			/* Length of whole struct */
	uint32	cmd_flags;		/* Flag bits to Identify configuring command */
	uint8	is_enabled;		/* Feature is enabled or not */
	uint8	pm0_periodicity;	/* Interval between consecutive PM0 during mitigation
					 * period (in sec)
					 */
	uint16	max_miti_timeout;	/* Maximum mitigation timeout in sec to send
					 * Mitigation Timeout event.
					 */
	int8	low_rssi_threshold;	/* Low RSSI threshold value */
	uint8	is_mlo_miti_enabled;	/* Enable or disable Beacon mute mitigation handling for MLD
					 * Association
					 */
	uint8	PAD[2];			/* Pad bytes */
} wlc_bcn_mute_miti_cfg_v3_t;

#define WLC_BCN_TBTT_CMD_VER_1	(1u)	/* Ver num for wlc_bcn_tbtt_cfg struct */
/* Configurable mitigation Timeout range */
#define WLC_BCN_TBTT_CFG_EVENT_PERIOD	(3u)	/* Send event after this value of drift */
#define WLC_BCN_TBTT_CFG_DRIFT_THRESH	(3)		/* in ms */
#define WLC_BCN_TBTT_CFG_JITTER_THRESH	(1)		/* in ms */

/* Macro flags to identify the specific config commands in IOVAR set operation */
#define WLC_BCN_DRIFT_BCN_OFFSET	(1u << 0u)
#define WLC_BCN_DRIFT_EVENT_PERIOD	(1u << 1u)
#define WLC_BCN_DRIFT_DRIFT_THRESHOLD	(1u << 2u)
#define WLC_BCN_DRIFT_JITTER_THRESHOLD	(1u << 3u)

/* configurable range */
#define WLC_BCN_DRIFT_BCN_OFFSET_MAX	(15u)
#define WLC_BCN_DRIFT_BCN_OFFSET_MIN	(5u)

#define WLC_BCN_DRIFT_DRIFT_THRESHOLD_MAX	(10)
#define WLC_BCN_DRIFT_DRIFT_THRESHOLD_MIN	(-4)

#define WLC_BCN_DRIFT_JITTER_THRESHOLD_MAX	(2)
#define WLC_BCN_DRIFT_JITTER_THRESHOLD_MIN	(-2)

typedef struct wlc_bcn_tbtt_cfg_v1 {
	uint16	version;		/* Structure version */
	uint16	length;			/* Length of whole struct */
	uint32	cmd;			/* type of cmd */
	uint8	tbtt_bcn_off;	/* num btw (5-15u) for uatbtt */
	uint8	event_period;	/* continous / periodic event */
	int8	drift_thres;	/* drift threshold for event in ms */
	int8	jitter_thres;	/* jitter threshold for event in ms */
} wlc_bcn_tbtt_cfg_v1_t;


/* SC scan retry config params */
#define SC_SCAN_RETRY_CFG_VERSION_1	1u
#define SC_SCAN_RETRY_CFG_VERSION_2	2u
#define SC_SCAN_RETRY_CFG_VERSION_3	3u
#define SC_SCAN_RETRY_CFG_VERSION_4	4u

/* Bits indicating which are the valid params in the set command. */
#define SC_SCAN_RETRY_CFG_PARAMS_THRESHOLD		(1u << 0u)
#define SC_SCAN_RETRY_CFG_PARAMS_MODE			(1u << 1u)
#define SC_SCAN_RETRY_CFG_PARAMS_BTMCRX_WEIGHT		(1u << 2u)
#define SC_SCAN_RETRY_CFG_PARAMS_BCN_DUR_2G		(1u << 3u)
#define SC_SCAN_RETRY_CFG_PARAMS_BCN_DUR_5G		(1u << 4u)
#define SC_SCAN_RETRY_CFG_PARAMS_BCN_DUR_6G		(1u << 5u)
#define SC_SCAN_RETRY_CFG_PARAMS_THRESHOLD_5G		(1u << 6u)
#define SC_SCAN_RETRY_CFG_PARAMS_BTMCRX_WEIGHT_5G		(1u << 7u)


/* Input structure for sc_scan_retry_cfg IOVAR */
typedef struct sc_scan_retry_cfg_params_v1 {
	uint16 version;		/* config version. */
	uint16 len;		/* Length of this struct. */
	uint32 set_flag;	/* Flag bits to Identify valid param type to be set */
	uint8 threshold;	/* Amount of Tx-Blanking in percentage considered as failed scan. */
	uint8 scan_mode;	/* Scan mode in which scan need to be re-scheduled. */
	uint8 PAD[2];		/* 32bit alignment */
} sc_scan_retry_cfg_params_v1_t;

/* Input structure for sc_scan_retry_cfg v2 IOVAR */
typedef struct sc_scan_retry_cfg_params_v2 {
	uint16 version;				/* config version. */
	uint16 len;				/* Length of this struct. */
	uint32 set_flag;			/* Flag bits to Identify valid param type to
						 * be set.
						 */
	uint8 threshold;			/* Amount of Tx-Blanking + the weighted
						 * BTMC Rx overlap.
						 * in percentage considered as failed scan.
						*/
	uint8 scan_mode;			/* Scan mode in which scan need to be
						 *re-scheduled..
						 */
	uint8 btmc_rx_overlap_weightage;	/* weightage for btmc_rx_overlap duration in % */
	uint8 PAD;				/* 32bit alignment */
} sc_scan_retry_cfg_params_v2_t;

#define SC_SCAN_RETRY_CFG_BCN_DUR_MAX	50u		/* max dur is 5ms(50 * 0.1ms) */
#define SC_SCAN_RETRY_CFG_BCN_DUR_MIN	1u		/* max dur is 0.1ms(1 * 0.1ms) */
#define SC_SCAN_RETRY_CFG_BANDS_MAX	3u		/* max bands supported(2G, 5G and 6G) */

/* Input structure for sc_scan_retry_cfg v3 IOVAR */
typedef struct sc_scan_retry_cfg_params_v3 {
	uint16 version;					/* config version. */
	uint16 len;					/* Length of this struct. */
	uint32 set_flag;				/* Flag bits to Identify valid param type to
							 * be set.
							 */
	uint8 threshold;				/* Amount of Tx-Blanking + the weighted
							 * BTMC Rx overlap.
							 * in percentage considered as failed scan.
							 */
	uint8 scan_mode;				/* Scan mode in which scan need to be
							 * re-scheduled..
							 */
	uint8 btmc_rx_overlap_weightage;		/* weightage for btmc_rx_overlap
							 * duration in %
							 */
	uint8 bcn_dur[SC_SCAN_RETRY_CFG_BANDS_MAX];	/* Beacon reception time in 0.1ms
							 * Byte 0:2G Beacon Duration
							 * Byte 1: 5G Beacon Duration
							 * Byte 2: 6G Beacon Duration
							 */
	uint8 PAD[2];					/* pad for 32 bit alignment */
} sc_scan_retry_cfg_params_v3_t;

/* Input structure for sc_scan_retry_cfg v4 IOVAR */
typedef struct sc_scan_retry_cfg_params_v4 {
	uint16 version;					/* config version. */
	uint16 len;					/* Length of this struct. */
	uint32 set_flag;				/* Flag bits to Identify valid param type to
							 * be set.
							 */
	uint8 threshold;				/* Amount of Tx-Blanking + the weighted
							 * BTMC Rx overlap for 2g.
							 * in percentage considered as failed scan.
							 */
	uint8 scan_mode;				/* Scan mode in which scan need to be
							 * re-scheduled..
							 */
	uint8 btmc_rx_overlap_weightage;		/* 2g - weightage for btmc_rx_overlap
							 * duration in %
							 */
	uint8 bcn_dur[SC_SCAN_RETRY_CFG_BANDS_MAX];	/* Beacon reception time in 0.1ms
							 * Byte 0:2G Beacon Duration
							 * Byte 1: 5G Beacon Duration
							 * Byte 2: 6G Beacon Duration
							 */
	uint8 threshold_5g;				 /* Amount of Tx-Blanking + the weighted
							 * BTMC Rx overlap for 5g.
							 * in percentage considered as failed scan.
							 */
	uint8 btmc_rx_overlap_weightage_5g ;		 /* 5g - weightage for btmc_rx_overlap
							 * duration in %
							 */
} sc_scan_retry_cfg_params_v4_t;


/* host queries RNG version from 'wl cap' iovar */
#define BCM_RNG_VERSION_1	1u /* for initial "reseed" version */
/* Supported entropy size, bytes */
#define BCM_RNG_ENTROPY_SIZE_BYTES	64u

/* RNG top level command IDs */
typedef enum wl_rng_cmd_id {
	WL_RNG_CMD_RESEED = 0u,
	WL_RNG_CMD_LAST = 1u
} wl_rng_cmd_id_t;

/* RNG reseed host entropy */
typedef struct bcm_rng_reseed {
	uint16		entropy_size;		/* host entropy size, bytes */
	uint8		entropy[];		/* host entropy, flexible array */
} bcm_rng_reseed_t;

/* RNG IOVAR data */
typedef struct wl_rng_iovar {
	uint16	version;			/* Version of this structure */
	uint16	subcmd_id;			/* RNG subcmd ID */
	uint16	PAD;				/* padding for 32-bit struct alignment */
	uint16	length;				/* Length of data following this length field */
	union {
		bcm_rng_reseed_t reseed;	/* RNG reseed data */
	} u;
} wl_rng_iovar_t;


#ifdef WL_UWBCOEX
/* Definitions for UWB coex iovar */
#define WL_UWBCX_VERSION	1

/* payload for the WL_UWBCX_COEX_BITMAP command */
typedef struct uwbcx_coex_bitmap {
	uint16 low_bitmap;	/* Lower 16 bits for channels 61 - 1 */
	uint16 mid_low_bitmap;	/* Mid lower 16 bits for channels 125 - 65 */
	uint16 mid_high_bitmap;	/* Mid upper 16 bits for channels 189 - 129 */
	uint16 high_bitmap;    /* Upper 16 bits for channels 233 - 193 */
} uwbcx_coex_bitmap_t;

/* Bands select for WL_UWBCX_COEX_BITMAP command */
typedef enum uwbcx_bands_e {
	UWBCX_BAND_5G = 0u, /* UWBCX coex for 5G */
	UWBCX_BAND_6G = 1u, /* UWBCX coex for 6G */
	UWBCX_MAX_BAND
} uwbcx_bands_t;

/* Version of uwbcx_coex_bitmap_t structure.
 * Increment whenever a change is made to uwbcx_coex_bitmap_t
 */
#define UWBCX_COEX_BITMAP_VERSION_V2 2u

/* payload for the WL_UWBCX_COEX_BITMAP command to configure bitmap for 5G/6G */
typedef struct uwbcx_coex_bitmap_v2 {
	uint16  version;			/* version info */
	uint8   len;				/* uwb coex bitmap length */
	uint8   pad;
	uwbcx_bands_t band;			/* Band configuration info */
	uwbcx_coex_bitmap_t coex_bitmap;	/* Coex bitmap configuration for the band */
} uwbcx_coex_bitmap_v2_t;

typedef struct uwbcx_test_params {
	uint16 duration;	/* Duration of the UWB high signal in ms */
	uint16 interval;	/* Period or Interval for making UWB high in ms */
	uint16 gpio;		/* GPIO for UWB signal out */
} uwbcx_test_params_t;

/* Version of uwbcx_dbg_stats_t structure.
 * Increment whenever a change is made to uwbcx_dbg_stats_t
 */
#define UWBCX_DBG_STATS_VER_1 1
/* UWB coex status structures */
typedef struct uwbcx_dbg_stats_v1 {
	uint16  version;             /* version info */
	uint8   len;                 /* stats length */
	uint8   PAD;
	uint32  uwb_req_cnt;         /* Total number of GPIO requests  by UWB */
	uint32  uwb_grant_cnt;       /* Total number of grants by WLAN to UWB */
	uint32  uwb_wl_preempt_cnt;  /* Total number of grants by WLAN to UWB
		* without protection (WL preempt)
		*/
} uwbcx_dbg_stats_v1_t;

/* UWB coex IOV sub command IDs */
typedef enum uwbcx_cmd_id {
	WL_UWBCX_CMD_VER	= 0,	/* UWBCX version sub command */
	WL_UWBCX_CMD_MODE	= 1,
	WL_UWBCX_CMD_PM_PROT	= 2,
	WL_UWBCX_CMD_GPIO_OUT_EN = 3,
	WL_UWBCX_CMD_GPIO_STATUS = 3,
	WL_UWBCX_CMD_ADV_REQ_TIMER = 4,
	WL_UWBCX_CMD_MAX_GRANT_TIMER = 5,
	WL_UWBCX_CMD_COEX_BITMAP = 6,
	WL_UWBCX_CMD_TEST_MODE = 7,
	WL_UWBCX_CMD_TEST_PARAMS = 8,
	WL_UWBCX_CMD_GET_STATS = 9,
	WL_UWBCX_CMD_CLEAR_STATS = 10,
	WL_UWBCX_CMD_MIN_IDLE_TIMER = 11 /* uint8 param range <0 - 250> */
} uwbcx_cmd_id_t;

#define UWBCX_GPIO_OUT_ENABLED		0x01u
#define UWBCX_GPIO_OUT_DISABLED		0x00u

#define UWBCX_DISABLED		0x00u
#define UWBCX_ENABLED		0x01u

#define UWBCX_MODE_DISABLED		0x00u
#define UWBCX_MODE_TDD		0x01u

/* first byte of bcm_iov_batch_subcmd.data for the WL_UWBCX_CMD_PM_PROT command */
#define UWBCX_PM_PROT_ENABLED	0x01u
#define UWBCX_PM_PROT_DISABLED	0x00u

#define UWBCX_ADV_REQ_TIMER_DEFAULT		(1u)
#define UWBCX_ADV_REQ_TIMER_MAX			(30u)

#define UWBCX_MAX_GRANT_TIMER_DEFAULT	(30u)
#define UWBCX_MIN_IDLE_TIMER_DEFAULT	(10u)

/* Version of wlc_uwbcx_stats_t structure.
 * Increment whenever a change is made to wlc_uwbcx_stats_t
 */
#define UWBCX_STATS_VER_1 1
/* UWB coex status structures */
typedef struct wlc_uwbcx_stats_v1 {
	uint16	version;		/* version info */
	uint8	len;			/* stats length */
	uint8	mode;			/* UWB coex mode */
	uint32	uwb_req_cnt;		/* UWB req count since last read */
	uint32	uwb_grant_cnt;		/* UWB grant count since last read */
	uint32	uwb_grant_dur;		/* UWB grant duration since last read (us) */
	uint16	uwb_succ_pm_prot_cnt;	/* UWBCX number of successfully acked PM. */
	uint16	uwb_succ_cts_prot_cnt;	/* UWBCX number of successfully TXed CTS2A. */
	uint16	uwb_rx_preempt_cnt;	/* UWBCX RX preempt (grant delay count) */
	uint16	uwb_grant_rstr_cnt;	/* UWBCX grant restore count - timeout */
	uint16	uwb_tx_preempt_cnt;	/* UWBCX Tx preemt count */
	uint16	PAD;
} wlc_uwbcx_stats_v1_t;
#endif /* WL_UWBCOEX */

#define WL_BTC_VER_V1	1u
#define BTCX_ROAM_PROF_VERSION_V1 1u
#define BTCX_ROAM_BW_VERSION_V1 1u
#define BTCX_TEST_MODE_VERSION_V1	1u
#define BTCX_TEST_MODE_INVALID		0u
#define	BTCX_TEST_MODE_ACK_CNCL		1u

/* BT coex IOV sub command IDs */
typedef enum btc_cmd_id {
	WL_BTC_CMD_VER			= 0, /* BTCX version sub command */
	WL_BTC_CMD_ROAM_PROFILE		= 1,
	WL_BTC_CMD_ROAM_BW		= 2,
	WL_BTC_CMD_ROAM_ON		= 3,
	WL_BTC_CMD_ROAM_BAND_FILTER	= 4,
	WL_BTC_CMD_ROAM_DELTA		= 5,
	WL_BTC_CMD_ROAM_RSSI		= 6,
	WL_BTC_CMD_ROAM_MODE		= 7,
	WL_BTC_CMD_ACK_CNTS		= 8,
	WL_BTC_CMD_RR_ENABLE		= 9,
	WL_BTC_CMD_HP_OVR_LESCAN	= 10,
	WL_BTC_CMD_TEST_MODE		= 11,
	WL_BTC_CMD_LR154CX_STATUS	= 12,
	WL_BTC_CMD_SWDIV_ANT_SWITCH	= 13
} btc_cmd_id_t;

typedef struct btcx_roam_bandwidth {
	uint16  version;            /* version info */
	uint8   len;                /* btc roam profile length */
	uint8   pad;
	uint8 low_offset;
	uint8 mid_offset;
	uint8 high_offset;
	uint8 pad1;
} btcx_roam_bw_v1_t;

typedef struct btcx_roam_profile {
	uint16  version;            /* version info */
	uint8   len;                /* btc roam profile length */
	uint8   pad;
	uint8 prof_id;
	uint8 initialized;
	uint16 offset;
	uint32 task_bm;
} btcx_roam_profile_v1_t;

typedef struct btcx_test_mode1 {
	uint16	count;	/* Number of Packets with Desired rate */
	uint8	rate;	/* Desired rate */
	uint8	ratio;	/* (1 out of X) packets will be canceled */
} btcx_test_mode1_v1_t;

typedef struct btcx_test_mode {
	union {
		btcx_test_mode1_v1_t mode1;
	} data;
	uint16	version;	/* version info */
	uint16	len;		/* btc test_mode_info length */
	uint16	pad;		/* padding */
	uint8	mode;		/* Mode to identify which features needs to debug */
	bool	enable;		/* enable and disable mode */
} btcx_test_mode_v1_t;

/* WLAN Rate Recovery Configuration */
/* Version 1 is the IOVAR itself. */
#define BTC_RR_CONFIG_VER_1 1

#define BTC_RR_CONFIG_VER_2 2
typedef struct wlc_btc_rr_config_v2 {
	uint8 version;			/* version */
	uint8 len;			/* length */
	bool enable;			/* enable/disable */
	uint8 nss;			/* NSS and MCS from the host define the cutoff rate. */
	uint8 mcs;
	uint8 cutoff_mcs_nss1_11n;	/* Cutoff MCS when NSS=1. */
	uint8 cutoff_mcs_nss2_11n;	/* Cutoff MCS when NSS=2. */
	uint8 pad;
} wlc_btc_rr_config_v2_t;

enum phy_rxgcrs_ed_enhncd_cmd_id {
	PHY_RXGCRS_ED_ENHNCD_CMD_EN		= 1u,
	PHY_RXGCRS_ED_ENHNCD_CMD_STATUS		= 2u,
	PHY_RXGCRS_ED_ENHNCD_CMD_COREMASK	= 3u,
	PHY_RXGCRS_ED_ENHNCD_CMD_LAST
};

/* SAE command
 * Only IOV batching support - see bcmiov.h
 */

/* SAE command version */
#define WL_SAE_CMD_VERSION	1

/* SAE sub-commands */
typedef enum sae_cmd_id {
	WL_SAE_CMD_AP_SESSION_HOLD_TIME	= 1, /* AP rate limit session hold time
										 * Data: uint32,
										 * miliseconds
										 */
	WL_SAE_CMD_AP_MAX_ACTIVE_SESSIONS	= 2, /* AP max sessions
											* Data:
											* uint32
											*/
	WL_SAE_CMD_STATUS = 3, /* RSNXE Caps
											* Data:
											* uint32
											*/
} sae_cmd_id_t;

/* Frameburst COT IOVAR data */
#define	WL_FRAMEBURST_COT_VERSION	1u /* current version of frameburst_cot structure */
#define WL_FRAMEBURST_MAX_AC		4u
typedef struct frameburst_cot {
	uint16 version;		/* version of frameburst_cot_t structure */
	uint16 length;
	chanspec_t chspec;
	uint16	PAD;	/* padding */
	uint16 fbcot[WL_FRAMEBURST_MAX_AC];	/* per AC (BK, BE, VI, and VO) in us units */
} frameburst_cot_t;

typedef enum {
	BCM_TRACE_VER		= 1,
	BCM_TRACE_ENAB		= 2,
	BCM_TRACE_EVENT_ENAB	= 3
} bcm_trace_sub_cmds_t;

#define BCM_TRACE_VERSION_1	1u
#define BCM_TRACE_VERSION_2	2u

typedef struct bcm_trace_event_enab_v1 {
	uint8 version;
	uint8 event;
	uint8 enab;
	uint8 PAD[1];
} bcm_trace_event_enab_v1_t;


/* BCMTRACE even log type bit definitions */
#define BCM_TRACE_TYPE_NORMAL	0x1
#define BCM_TRACE_TYPE_CHATTY	0x2

#define BCM_TRACE_TYPE_MASK	0x3

/* Enum value for BCMTRACE array index */
enum bcmtrace_array_val {
	BCM_TRACE_NORMAL	= 0,
	BCM_TRACE_CHATTY,
	BCM_TRACE_LAST
};

typedef struct bcm_trace_event_enab_v2 {
	uint8 version;
	uint8 event;	/* event operated */
	uint8 type;	/* bit map of event type */
	uint8 val;	/* values corresponding to event type */
} bcm_trace_event_enab_v2_t;

/* rate_info command version */
#define WL_RATE_INFO_VERSION_1	 1
typedef struct wl_rate_info {
	uint16	version;		/**< structure version */
	uint16  length;			/**< length of this struct */
	uint32	mode_tx_rate;	/**< the most used tx rate in tx_rate_histo */
	uint32	mode_rx_rate;	/**< the most used rx rate in rx_rate_histo */
} wl_rate_info_t;

/* "rng_test" IOVAR param */
typedef struct {
	uint32 rounds_no;	/* IN number of generate cycles */
	uint32 gen_no;		/* IN number of buffers per cycle */
	uint32 time_max;	/* OUT max time of one cycle, us */
	uint32 time_min;	/* OUT min time of one cycle, us */
	uint32 time_aver;	/* OUT time of all cycles, us */
} rng_test_t;

/* latency_critical_data mode to reduce a latency */
typedef enum {
	LATENCY_CRT_DATA_MODE_OFF	= 0,	/* Turn off */
	LATENCY_CRT_DATA_MODE_1		= 1u,	/* Remap BLE scan window size */
	LATENCY_CRT_DATA_MODE_2		= 2u,	/* Remap BLE scan window size +
						 * Prevent full roam scan
						 */
	LATENCY_CRT_DATA_MODE_LAST
} latency_crt_mode_t;

#define WL_AUTH_START_EVT_V1 1u
typedef struct wl_auth_start_evt {
	uint16 version;
	uint16 len;
	wlc_ssid_t ssid;
	struct ether_addr bssid;
	uint8 PAD[2];
	uint32 key_mgmt_suite;
	uint8 opt_tlvs[];
} wl_auth_start_evt_t;

/* Additional header pushed to the logged mgmt frame in aml(assoc mgmt frame logger) */
typedef struct wl_aml_header_v1 {
	uint16 version;		/* Header version, now 1 */
	uint16 length;		/* frame length excluding this header */
	uint16 flags;		/* flag to indicate tx/rx, acked/noack and others */
	uint8 PAD[2];		/* pad bytes for align */
} wl_aml_header_v1_t;

/* Version for aml header */
#define WL_AML_HEADER_VERSION	1u

/* Flags for aml header */
#define WL_AML_F_DIRECTION 0x0001	/* This flag indicates this is Tx mgmt, otherwise Rx */
#define WL_AML_F_ACKED     0x0002	/* This flag indicates the frame is acked */
#define WL_AML_F_EAPOL     0x0004	/* This flag indicates the frame is wpa supplicant */

/* Version for IOVAR 'aml' */
#define WL_AML_IOV_MAJOR_VER_1 1u
#define WL_AML_IOV_MINOR_VER_0 0u
#define WL_AML_IOV_MAJOR_VER_SHIFT 8u
#define WL_AML_IOV_VERSION_1_0 \
	((WL_AML_IOV_MAJOR_VER_1 << WL_AML_IOV_MAJOR_VER_SHIFT) | WL_AML_IOV_MINOR_VER_0)

/* Common header for IOVAR 'aml' */
typedef struct wl_aml_iov_cmnhdr {
	uint16 ver;	/* IOVAR cmd structure version */
	uint16 len;	/* IOVAR cmd structure total len (cmnhdr + following subcmd) */
	uint16 subcmd;	/* IOVAR subcmd */
	uint8 PAD[2];
} wl_aml_iov_cmnhdr_t;

/* IOVAR 'aml' data structure, cmn header is followed by subcmd structure */
typedef struct wl_aml_iovar {
	wl_aml_iov_cmnhdr_t hdr;
	uint32 data[];
} wl_aml_iovar_t;

/* IOVAR 'aml' subcmd list */
enum wl_aml_subcmd_ids {
	WL_AML_SUBCMD_ENABLE = 1,	/* Enable/disable aml feature */
	WL_AML_SUBCMD_LAST
};

/* IOVAR 'aml' subcmd structure for uint32 data
 * Subcmd 'enable' uses this structure
 */
typedef struct wl_aml_iov_uint_data {
	uint32 val;
} wl_aml_iov_uint_data_t;

/* IOVAR 'aml enable' subcmd bit numbers
 *  wl_aml_iov_uint_data.val is popluated with following bits
 */
enum wl_aml_cmd_enable_bitnums {
	WL_AML_ASSOC_ENABLE = 0,	/* Logging for association */
	WL_AML_ROAM_ENABLE = 1,		/* Logging for roaming */
	WL_AML_ENABLE_LAST
};

/* Snapshot of various timestamps */
#define WL_TIMESTAMP_VERSION_1 1
typedef struct wl_timestamps_v1 {
	uint16	version;	/**< structure version */
	uint16	length;		/**< length of this struct */
	uint32  PAD;
	uint64	cpu_cycles;	/**< CPU Cycle count */
	uint64	ptm;		/**< Local PTM master timestamp in nano seconds */
	uint64	pmu_timer;	/**< Legacy PMU timer in micsoseconds */
	uint32	tsf_main;	/**< Main slice TSF */
	uint32	tsf_aux;	/**< Aux slice TSF */
	uint32	tsf_scan;	/**< Scan core TSF */
	uint32  PAD;
} wl_timestamps_v1_t;

#define WL_TIMESTAMP_VERSION_2 2
typedef struct wl_timestamps_v2 {
	uint16	version;	  /**< structure version */
	uint16	length;		  /**< length of this struct */
	uint32  PAD;
	uint64	cpu_cycles;	  /**< cpu cycle count */
	uint64	pgt;		  /**< local pgt master timestamp in nano seconds */
	uint64	pmu_timer;	  /**< legacy pmu timer in microseconds */
	uint32	tsf_main;	  /**< main slice tsf */
	uint32	tsf_aux;	  /**< aux slice tsf */
	uint32	tsf_scan;	  /**< scan core tsf */
	uint32  PAD;
	uint64	sysuptime_ns;	  /**< sysup time in nanoseconds */
} wl_timestamps_v2_t;

#define WL_LOW_LATENCY_CONFIG_V1	1u
#define WL_LOW_LATENCY_V1		1u

/* low latency config */
typedef struct wl_low_latency_config_v1 {
	uint16 version;
	uint16 length;
	uint8 periodicity;	/* periodicity in sec of low latency PNO scan */
	uint8 PAD[3];
} wl_low_latency_config_v1_t;

/* Interface for low latency */
typedef struct wl_low_latency_v1 {
	uint16 version;
	uint16 length;
	uint8 mode;				/* latency mode */
	uint8 PAD[3];
	wl_low_latency_config_v1_t config;	/* config for latency mode */
} wl_low_latency_v1_t;

#define WL_PASN_VERSION_1	 0x0001u

/* PASN subcommands ID. */
enum {
	WL_PASN_CMD_NONE		= 0,
	/* get PASN IOVAR version. */
	WL_PASN_CMD_GET_VERSION		= 1,
	/* set, enable PASN. */
	WL_PASN_CMD_ENABLE		= 2,
	/* set, disable PASN. */
	WL_PASN_CMD_DISABLE		= 3,
	/* get/set, PASN finite cyclic group id. */
	WL_PASN_CMD_CONFIG_GROUP	= 4,
	/* get/set, PASN authentication policy. */
	WL_PASN_CMD_CONFIG_POLICY	= 5,
	/* get/set, The key lifetime in minutes. */
	WL_PASN_CMD_CONFIG_KEY_LIFETIME_MIN	= 6,
	/* get/set, peer mac address of PASN session. */
	WL_PASN_CMD_CONFIG_PEER_ADDR	= 7,
	/* get/set, local mac address of PASN session. */
	WL_PASN_CMD_CONFIG_LOCAL_ADDR	= 8,
	/* get/set, chanspec of PASN session. */
	WL_PASN_CMD_CONFIG_CHANSPEC	= 9,
	/* set, start PASN session. */
	WL_PASN_CMD_START		= 10,
	/* set, stop PASN session. */
	WL_PASN_CMD_STOP		= 11,
	/* set, delete PASN session. */
	WL_PASN_CMD_DELETE		= 12,
	/* get info for PASN session with given id - wl_pasn_session_info_t. */
	WL_PASN_CMD_GET_INFO		= 13,
	/* Note: for debug purpose or future event - wl_pasn_key_info_t. */
	WL_PASN_CMD_GET_KEY_INFO	= 14,
	/* get/set. uint32. PASN event mask. */
	WL_PASN_CMD_EVENT_MASK		= 15,
	/* get. List id of active sessions. */
	WL_PASN_CMD_LIST_SESSIONS	= 16,
	/* get/set, flags of PASN session. */
	WL_PASN_CMD_CONFIG_FLAGS	= 17,
	/* get/set. uint32. PASN key idle time. */
	WL_PASN_CMD_KEY_IDLE_TIME	= 18,
	/* get/set. for testing purpose. */
	WL_PASN_CMD_TEST		= 0xfffe
};
typedef uint16 wl_pasn_cmd_t;

enum {
	/* No test action */
	WL_PASN_CMD_TEST_NONE		= 0,
	/* DUT to send auth frame with invalid MIC */
	WL_PASN_CMD_TEST_INVALID_MIC	= 1,
	/* DUT to send deauth frame to peer */
	WL_PASN_CMD_TEST_DEAUTH		= 2
};
typedef uint8 wl_pasn_cmd_test_id_t;

typedef uint16 wl_pasn_session_id_t;
/* Session id ranges are allocated to various users of PASN -
 * including internal use (e.g. automatically allocated IDs on an AP).
 * Other ranges can be defined in the future.
 */
enum {
	PASN_SID_TYPE_HOST	= 0,
	PASN_SID_TYPE_FTM	= 1,
	PASN_SID_TYPE_INTERNAL	= 2
};

#define PASN_SID_GLOBAL	0u

#define PASN_SID_HOST_START	1u
#define PASN_SID_HOST_END	1023u

#define PASN_SID_FTM_START	1024u
#define PASN_SID_FTM_END	2047u

#define PASN_SID_NAN_START	2048u
#define PASN_SID_NAN_END	3071u

#define PASN_SID_INTERNAL_START	3072u
#define PASN_SID_INTERNAL_END	32767u


typedef struct wl_pasn_iov {
	uint16			version; /* structure version will be incremented
					* when header is changed.
					*/
	uint16			len;	/* includes the id field and following data. */
	wl_pasn_cmd_t		cmd;	/* sub-command id. */
	wl_pasn_session_id_t	id;	/* session id is the input for session specific commands
					* and a reserved value for global commands.
					*/
	uint8			data[];	/* variable */
} wl_pasn_iov_t;

/* The policy used in the PASN exchange. */
/*
 * ALLOW_NO_PMKSA may be set independent of USE_PMKSA or SETUP_PMKSA.
 * SETUP_PMKSA implies USE_PMKSA i.e. attempt to use existing PMKSA before
 * attempting setup. Ignore USE_PMKSA when SETUP_PMKSA is set.
 * if USE_PMKSA or SETUP_PMKSA are not set, attempt w/o PMKSA - OWE like
 * USE_PMKSA does not imply SETUP_PMKSA - i.e. we wont attempt setting up PASN via tunneling.
 */
enum {
	WL_PASN_POLICY_NONE		= 0, /* Not used. */
	WL_PASN_POLICY_ALLOW_NO_PMKSA	= 1, /* allow no pmksa, fixed pmk */
	WL_PASN_POLICY_USE_PMKSA	= 2, /* use existing pmksa for base akm */
	/* 3 is reserved for bit "OR" of ALLOW_NO_PMKSA and USE_PMKSA. */
	WL_PASN_POLICY_SETUP_PMKSA	= 4, /* setup PMKSA using base akm */
	WL_PASN_POLICY_MAX		= WL_PASN_POLICY_SETUP_PMKSA
};

enum {
	/* Initial state of PASN state machine.
	 * Initialization is allowed if PASN is enabled via configuration.
	 */
	WL_PASN_STATE_IDLE		= 0,
	/* For STA, Initiate an instance requested by user, ex. FTM.
	 * For AP, Receive PASN authentication frame with sequence 1.
	 */
	WL_PASN_STATE_INIT		= 1,
	/* STA only. Auth frame is constructed and ready for
	 * transmission. STA request for channel scheduling.
	 * It is not necessary to AP because AP will not be off channel.
	 */
	WL_PASN_STATE_SCHED_WAIT	= 2,
	/* STA only. PASN is waiting for desired security
	 * methods(FILS/SAE/FBT) t obuild tunneled protocol data for authentication
	 * frame with sequence 1.
	 */
	WL_PASN_STATE_DATA_WAIT_1	= 3,
	/* STA only. PASN authentication frame with sequence
	 * 1 is constructed and pushed to FIFO, waiting for ACK.
	 */
	WL_PASN_STATE_WAIT_ACK_1	= 4,
	/* STA only. STA is waiting for authentication frame with sequence 2. */
	WL_PASN_STATE_WAIT_AUTH_2	= 5,
	/* STA: PASN is waiting for desired security
	 * methods(FILS/SAE/FBT) to build tunneled protocol data for authentication
	 * frame with sequence 3.
	 * AP: PASN is waiting for desired security module to validate the tunneled
	 * protocol data in received authentication frame with sequence 3.
	 */
	WL_PASN_STATE_DATA_WAIT_3	= 6,
	/* STA only. PASN authentication frame with sequence 3
	 * is constructed and pushed to FIFO, waiting for ACK.
	 */
	WL_PASN_STATE_WAIT_ACK_3	= 7,
	/* STA only. The received PASN authentication frame
	 * with sequence 2 has status code as REFUSED_TEMPORARILY,
	 * and the comeback info field in PASN parameters element indicates
	 * the time for STA to retry the authentication later.
	 */
	WL_PASN_STATE_COME_BACK		= 8,
	/* AP only. PASN is waiting for desired security
	 * methods(FILS/SAE/FBT) to build tunneled protocol data for
	 * authentication frame with sequence 2.
	 */
	WL_PASN_STATE_DATA_WAIT_2	= 9,
	/* AP only. PASN authentication frame with
	 * sequence 2 is constructed and pushed to FIFO, waiting for ACK.
	 */
	WL_PASN_STATE_WAIT_ACK_2	= 10,
	/* AP only. AP is waiting for authentication frame with sequence 3. */
	WL_PASN_STATE_WAIT_AUTH_3	= 11,
	/* PASN exchange is done. */
	WL_PASN_STATE_DONE			= 12
};

typedef uint8 wl_pasn_session_state_t;
typedef int32 wl_pasn_status_t;

/* Global and bsscfg flags */
enum {
	/* PASN is enabled on bsscfg */
	WL_PASN_FLAG_BSSCFG_ENABLED	= 0x0001u
};
typedef uint16 wl_pasn_flags_t;

enum {
	/* PASN exchange will use PMKSA to derive PTKSA */
	WL_PASN_SESSION_FLAG_CACHED_PMK = 0x0001u,
	/* PASN exchange will setup PMKSA by tunneling protocol data */
	WL_PASN_SESSION_FLAG_TUNNELED_AKM = 0x0002u,
	/* PASN session will be deleted if error occurs */
	WL_PASN_SESSION_FLAG_DELETE_ON_ERR = 0x0004u,
	/* PASN session will issue scan with randmac */
	WL_PASN_SESSION_FLAG_RANDMAC = 0x0008u
};
typedef uint16 wl_pasn_session_flags_t;

/* PASN session state information. */
typedef struct wl_pasn_session_info {
	wl_pasn_session_id_t	id;	/* The id of pasn auth exchange. */
	wl_pasn_session_state_t	state;	/* The state of pasn auth exchange. */
	uint8			PAD[1];
	wl_pasn_status_t	status;	/* The status code of pasn auth exchange. bcmerror.h */
	struct ether_addr	local_addr; /* The source mac address of pasn auth exchange. */
	struct ether_addr	peer_addr; /* The peer mac address of pasn auth exchange. */
	uint8	akm;			/* The AKM used in pasn auth exchange.
					* see rsn_akm_t in bcmwpa.h
					*/
	uint8	pairwise_cipher;	/* The pairwise cipher used in pasn auth exchange. */
	uint16	comeback_after_ms;	/* time in miliseconds after which the non-AP STA is
					* requested to retry the PASN authentication.
					*/
	uint16	cyclic_group_id;	/* Finite cyclic group id, IANA id, see bcm_ec.h */
	wl_pasn_session_flags_t	flags;	/* session flags. */
	uint32	key_lifetime_ms;	/* key lifetime negotiated in authentication. */
} wl_pasn_session_info_t;

#define WL_PASN_MAX_CC_GROUP 6

/* PASN global information. */
typedef struct wl_pasn_info {
	wl_pasn_flags_t	flags;		/* flags. */
	wl_pasn_policy_t policy;	/* authentication policy supported. */
	uint8	auth_num_retries;	/* authentication frame retry limit. */
	uint8	num_groups;		/* number of cyclic groups. */
	uint8	max_auth_session;	/* Maximum number of sessions. */
	uint8	active_auth_session;	/* number of active sessions. */
	uint8	PAD[1];
	uint32	key_lifetime_min;	/* key lifetime in minutes. */
	uint32	event_mask;
	uint16	auth_timeout_ms;	/* timeout waiting for auth frame. */
	uint16	group_offset;		/* the offset of cyclic_group_id. */
	uint32	key_idle_sec;		/* Idle time of PASN keys in seconds. */
	/* uint16 cyclic_group_id[];
	 * Finite cyclic group id list, IANA id, see bcm_ec.h.
	 */
} wl_pasn_info_t;

/* Note: for debug/internal purpose or future event.
 * PTKSA derived from PASN exchange.
 */
typedef struct wl_pasn_key_info {
	wl_pasn_session_id_t id; /* The id of pasn auth exchange. */
	uint16 len;		/* The length includes the structure and data. */
	struct ether_addr local; /* The source mac address of pasn auth exchange. */
	struct ether_addr peer;	/* The peer mac address of pasn auth exchange. */
	bcm_xlo_t kdk;		/* KDK length and data offset. */
	bcm_xlo_t kck;		/* KCK length and data offset. */
	bcm_xlo_t tk;		/* TK length and data offset. */
	/* data follows */
} wl_pasn_key_info_t;

/* WL_PASN_CMD_LIST_SESSIONS */
typedef struct wl_pasn_active_session_list {
	uint16	num_sids;
	wl_pasn_session_id_t	sids[];
} wl_pasn_active_session_list_t;

/* WL_PASN_CMD_CONFIG_GROUP */
typedef struct wl_pasn_group_id_list {
	uint16	num_groups;		/* number of cyclic groups. */
	uint16	cyclic_group_id[];	/* Finite cyclic group id list, IANA id, see bcm_ec.h. */
} wl_pasn_group_id_list_t;

/* pasn sub-event type. */
enum {
	WL_PASN_EVENT_NONE		= 0, /* reserved */
	WL_PASN_EVENT_CREATED		= 1, /* create a new session */
	WL_PASN_EVENT_STARTED		= 2, /* session is started */
	WL_PASN_EVENT_SCHED_WAIT	= 3, /* wait for channel scheduling . */
	WL_PASN_EVENT_DATA_WAIT		= 4, /* wait for wrapped data to be ready. */
	WL_PASN_EVENT_ACK_WAIT		= 5, /* wait for ACK */
	WL_PASN_EVENT_AUTH_WAIT		= 6, /* wait for next authentication frame from peer. */
	WL_PASN_EVENT_COME_BACK		= 7, /* wait for certain time to start auth again */
	WL_PASN_EVENT_DESTROYED		= 8, /* session is deleted. ie. PTKSA is deleted. */
	WL_PASN_EVENT_DONE		= 9, /* session is done. */
	WL_PASN_EVENT_REQUEST		= 10 /* Request PASN session setup. */
};
typedef uint16 wl_pasn_event_type_t;

/* pasn event */
typedef struct wl_pasn_event {
	uint16	version;
	uint16	len;			/* includes the entire structure and following data. */
	wl_pasn_session_id_t	sid;	/* session id */
	wl_pasn_event_type_t	type;	/* sub event type */
	wl_pasn_session_state_t	state;	/* current pasn authentication state.  */
	wl_pasn_session_state_t	prev_state; /* previous pasn authentication state.  */
	uint8	PAD[2];
	uint8	data[];			/* variable */
} wl_pasn_event_t;

/* amt command details */
/* non-batched command version = major|minor w/ major <= 127 */
#define WL_AMT_IOV_MAJOR_VER_1		1u
#define WL_AMT_IOV_MINOR_VER_2		2u
#define WL_AMT_IOV_MAJOR_VER_SHIFT	8u
#define WL_AMT_IOV_VERSION_1_2 \
		((WL_AMT_IOV_MAJOR_VER_1 << WL_AMT_IOV_MAJOR_VER_SHIFT) | WL_AMT_IOV_MINOR_VER_2)

enum wl_amt_cmd_ids {
	WL_AMT_CMD_VER			= 1u,	/* Get AMT API VER */
	WL_AMT_CMD_HW_ENTRY		= 2u,	/* Get/Set HW AMT only */
	WL_AMT_CMD_HW_DUMP		= 4u,	/* Dump HW AMT only */
	WL_AMT_CMD_HW_ENTRY_EX		= 5u,	/* Get/Set HW AMT, AFT and FFT */
	WL_AMT_CMD_HW_DUMP_EX		= 7u	/* Dump HW AMT, AFT and FFT */
};

enum wl_amt_xtlv_id {
	WL_AMT_XTLV_NONE		= 0u,	/* Not valid AMT tag */
	WL_AMT_XTLV_VER			= 1u,	/* Version */
	WL_AMT_XTLV_IDX			= 2u,	/* AMT index */
	WL_AMT_XTLV_AMT			= 3u,	/* AMT table entry */
	WL_AMT_XTLV_AFT			= 4u,	/* AFT table entry */
	WL_AMT_XTLV_FFT			= 5u,	/* FFT table entry */
	WL_AMT_XTLV_AMTINFO_FLAGS	= 6u	/* M_AMT_INFO SHM flags */
};

/* struct for WL_AMT_XTLV_AMT xtlv */
typedef struct wlc_amt_entry_info_v1 {
	uint16 attr;
	struct ether_addr ea;
} wlc_amt_entry_info_v1_t;

/* struct for WL_AMT_XTLV_AFT xtlv */
typedef struct wlc_aft_entry_info_v1 {
	uint8 ds_bits;
	uint8 if_id;
	uint16 fft_idx;
} wlc_aft_entry_info_v1_t;

/* struct for WL_AMT_XTLV_FFT xtlv */
typedef struct wlc_fft_entry_info_v1 {
	uint16 fft_mask_mng;
	uint16 fft_mask_ctl;
	uint16 fft_mask_data;
	uint16 fft_mask_ext;
} wlc_fft_entry_info_v1_t;

/* struct for WL_AMT_XTLV_AFT xtlv version 2 */
typedef struct wlc_aft_entry_info_v2 {
	uint8 ds_bits;
	uint8 fft_idx;
	uint16 if_id;
} wlc_aft_entry_info_v2_t;



/* subcommand ids for phy_tpc */
#define WL_TPC_CMD_INFO_BUF_SZ	4

enum wl_tpc_subcmd_ids {
	WL_TPC_SUBCMD_AV = 0u,			/* AV */
	WL_TPC_SUBCMD_VMID = 1u,		/* VMID */
	WL_TPC_SUBCMD_VER_CHECK = 2u,		/* VER CHECK */
	WL_TPC_SUBCMD_PACAL = 3u,		/* VER CHECK */
	WL_TPC_SUBCMD_TXPWRINDEX = 4u	/* PHY TX BASEINDEX */
};

typedef struct wl_tpc_avvmid_info_v1 {
	uint16  version;	/* structure version */
	uint16  length;		/* structure length */
	uint8   core;		/* core number */
	uint8   sub_band_idx;	/* sub band index */
	uint8   pa_mode;	/* PA mode */
	uint8   is_6g;		/* 0: 2G/5G or 1: 6G */
	uint8   avvmid_val;	/* AV or Vmid value */
	uint8	PAD[3];		/* explicit padding */
} wl_tpc_avvmid_info_v1_t;

typedef struct wl_tpc_cmd_v1 {
	uint16  version;			/* structure version */
	uint16  length;				/* structure length */
	uint16	sub_cmd;			/* sub_cmd type */
	uint8	PAD[2];				/* explicit padding */
	wl_tpc_avvmid_info_v1_t avvmid_info;	/* avvmid info */
} wl_tpc_cmd_v1_t;

typedef struct wl_tpc_cmd_info_v2 {
	uint16  version;			/* structure version */
	uint16  length;				/* structure length */
	uint8   core;				/* core number */
	uint8   sub_band_idx;			/* sub band index */
	uint8   pa_mode;			/* PA mode */
	uint8   is_6g;				/* 0: 2G/5G or 1: 6G */
	uint8   buf[WL_TPC_CMD_INFO_BUF_SZ];	/* value buffer */
} wl_tpc_cmd_info_v2_t;

typedef struct wl_tpc_cmd_v2 {
	uint16  version;			/* structure version */
	uint16  length;				/* structure length */
	uint16	sub_cmd;			/* sub_cmd type */
	uint8	PAD[2];				/* explicit padding */
	wl_tpc_cmd_info_v2_t info;
} wl_tpc_cmd_v2_t;

#define WL_TPC_CMD_IOV_VER_1		1u
#define WL_TPC_CMD_IOV_VER_2		2u
#define WL_TPC_AVVMID_IOV_VER_1		1u
#define WL_TPC_PACAL_IOV_VER_1		1u
#define WL_TPC_TXPWRINDEX_IOV_VER_1	1u


/* PTM */
#define WL_PTM_IOV_MAJOR_VER_1		1u
#define WL_PTM_IOV_MINOR_VER_1		1u
#define WL_PTM_IOV_MAJOR_VER_SHIFT	8u
#define WL_PTM_IOV_VERSION_1_1 \
	((WL_PTM_IOV_MAJOR_VER_1 << WL_PTM_IOV_MAJOR_VER_SHIFT)| WL_PTM_IOV_MINOR_VER_1)

/* subcommand ids */
enum wl_ptm_cmd_ids {
	WL_PTM_SUBCMD_VER =			0u,	/* PTM IOVAR version */
	WL_PTM_SUBCMD_ENABLE =			1u,	/* PTM disable:0, interrupt:1, polling:2 */
	WL_PTM_SUBCMD_OFFSET_PERIOD =		2u,	/* PTM offset period in ms */
	WL_PTM_SUBCMD_DRIFT_LIMIT =		3u,	/* PTM drift limit in ns */
	WL_PTM_SUBCMD_LAST_DRIFT =		4u,	/* PTM last drift in ns */
	WL_PTM_SUBCMD_LAST_OFFSET =		5u,	/* PTM last offset in ns */
	WL_PTM_SUBCMD_CURRENT_TIME =		6u,	/* PTM master and local time in ns */
	WL_PTM_SUBCMD_FAIL_LIMIT =		7u,	/* PTM fail count limit */
	WL_PTM_SUBCMD_LAST
};

typedef struct wl_ptm_offset {
	uint32 offset_high;
	uint32 offset_low;
	uint32 master_ahead;
} wl_ptm_offset_t;

typedef struct wl_ptm_time {
	uint32 offset_high;
	uint32 offset_low;
	uint32 local_high;
	uint32 local_low;
	uint32 master_high;
	uint32 master_low;
} wl_ptm_time_t;

typedef struct wl_ptm_master_time {
	uint32 master_high;
	uint32 master_low;
} wl_ptm_master_time_t;

#define WL_TX_PER_TID_WME_ECNT_VER	1u
typedef struct tx_wme_eCounters
{
	uint16	version;
	uint16	length;
	uint32  tx[NUMPRIO];
} tx_wme_eCounters_t;

#define WL_RX_PER_TID_WME_ECNT_VER	1u
typedef struct rx_wme_eCounters
{
	uint16	version;
	uint16	length;
	uint32  rx[NUMPRIO];
} rx_wme_eCounters_t;

/* Interface for pm2 peek */
#define WL_PM2_PEEK_VERSION_1	1u
typedef struct wl_pm2_peek_v1 {
	uint16 version;		/* structure version */
	uint16 length;		/* length of this structure */
	uint16 enable;		/* enable or disable the feature */
	uint16 offset;		/* time since last tbtt when FW sends null data frame with PM0 */
	uint16 awake_time;	/* After FW wake up, time to detect traffic */
} wl_pm2_peek_v1_t;

/* Triggered log events iovar structure */
#define TRIG_LOG_EVENTS_CFG_REQ_VERSION_1	(1u)
enum trig_log_events_cmd {
	TRIG_LOG_EVENTS_CMD_DISABLE_ALL = 0,	/* Disable all active TLEs */
	TRIG_LOG_EVENTS_CMD_ENABLE = 1,		/* Enable a list of TLEs */
	TRIG_LOG_EVENTS_CMD_DISABLE = 2,	/* Disable a list of TLEs */
	TRIG_LOG_EVENTS_CMD_MAX
};

typedef struct trig_log_events_config_request_v1 {
	uint16 version;		/* config version */
	uint16 len;		/* Length of this struct including variable len */
	uint8 cmd;		/* Command to execute on list of TLEs sent in this struct */
	uint8 rsvd;
	uint16 tles[];		/* List of triggered log event (TLE) ids */
} trig_log_events_config_request_v1_t;

/** Dynamic Tx Power Control common interfaces */
#define WL_DTPC_IOV_VERSION_1   (1u)

typedef enum wl_dtpc_iov {
	WL_DTPC_CMD_EN			=	0x0,	/**< top control switch */
	WL_DTPC_CMD_TXS_THRES		=	0x1,	/**< txs PER threshold */
	WL_DTPC_CMD_PROBSTEP		=	0x2,	/**< step size */
	WL_DTPC_CMD_ALPHA		=	0x3,	/**< mv alpha */
	WL_DTPC_CMD_TRIGINT		=	0x4,	/**< pwr prob trigger interval */
	WL_DTPC_CMD_HEADROOM		=	0x5,	/**< power headroom */
	WL_DTPC_CMD_CAP			=	0x6	/**< DTPC Tx/Rx capability */
} wl_dtpc_iov_t;

typedef struct rate_headroom {
	ratespec_t rspec;
	uint32	headroom;
} rate_headroom_t;

typedef struct wl_dtpc_cfg_headroom {
	uint8	max_bw; /* Max BW supported */
	uint8	max_nss; /* Max NSS supported */
	uint16  len;	/* count of rspec/headroom entries that follow */
	rate_headroom_t headrooms[];
} wl_dtpc_cfg_headroom_t;

#define WL_SBSS_RSPEC_VERSION_1	1

typedef struct wl_scb_rspec_info {
	struct ether_addr	ea;
	uint8			num_of_rspecs;
	uint8			chspecs_and_rspecs[];
} wl_scb_rspec_info_v1_t;

typedef struct wl_bsscfg_rspec_info {
	struct ether_addr	BSSID;
	uint8			num_scb_rspecs;
	uint8			PAD[1];
	bsscfg_subtype_t	subtype;
	ratespec_t		rspec;
	uint8			scb_rspecs_info[];
} wl_bsscfg_rspec_info_v1_t;

typedef struct wl_sbss_rspec_info {
	uint16	version;
	uint16	len;
	uint8	PAD[1];
	uint8	num_bsscfg_rspecs;
	uint8	bsscfg_rspecs_info[];
} wl_sbss_rspec_info_v1_t;

/* Version for IOVAR 'cellavoid' */
#define WL_CELL_AVOID_IOV_VERSION_1		1u
/* Version for IOVAR 'cellavoid' subcmd */
#define WL_CELL_AVOID_SUB_IOV_VERSION_1		1u
/* Max chanspec number for IOVAR 'cellavoid' */
#define WL_CELL_AVOID_MAX_NUM_CHANSPEC		128u

/* IOVAR 'cellavoid' subcmd list */
enum wl_cell_avoid_subcmd_ids {
	WL_CELL_AVOID_SUBCMD_VER = 0u,		/* Get version */
	WL_CELL_AVOID_SUBCMD_CH_INFO = 1u,	/* Set/Get/Remove cellular channel list */
};

typedef struct wl_cell_pwrcap_chanspec_v1 {
	int8 pwrcap;			/* pwr cap applied to the cellular channel */
	int8 padding;			/* padding */
	chanspec_t chanspec;		/* chanspec for the clellular channel */
} wl_cell_pwrcap_chanspec_v1_t;

/* IOVAR 'cellavoid' subcmd structure
 * Subcmd 'chinfo' uses this structure
 */
typedef struct wl_cell_avoid_ch_info_v1 {
	uint16 version;				/* version of this structure */
	uint16 length;				/* length of this structure */
	uint16 flags;				/* Mandatory flags */
	uint16 cnt;				/* Count of pwrcap + chanspec info */
	wl_cell_pwrcap_chanspec_v1_t list[];	/* list for pwrcap+chanspec info */
} wl_cell_avoid_ch_info_v1_t;

/* flags in wl_cell_avoid_ch_info_v1_t is populated with following values
 * If any of these are set, then the driver should completely avoid
 * using any of cellular channels for any of these modes
 */
#define WL_CELL_AVOID_WIFI_DIRECT	0x0001u
#define WL_CELL_AVOID_SOFTAP		0x0002u
#define WL_CELL_AVOID_NAN		0x0004u
/* This flag is used to remove channel info list in chinfo subcmd */
#define WL_CELL_AVOID_REMOVE_CH_INFO	0x8000u


#ifdef COEX_CPU
enum wl_coex_cpu_sub_cmd_xtlv_id {
	WL_COEX_CPU_CMD_FW_VERSION	= 0u, /* Get firmware version string */
	WL_COEX_CPU_CMD_STATUS		= 1u, /* Get current coex cpu status */
};
#endif /* COEX_CPU */

#ifdef CHRE
#define WL_CHRE_IOV_MAJOR_VER_SHIFT	8u

#define WL_CHRE_IOV_MAJOR_VER_1		1u
#define WL_CHRE_IOV_MINOR_VER_1		1u

#define WL_CHRE_IOV_VERSION_1_1 \
	((WL_CHRE_IOV_MAJOR_VER_1 << WL_CHRE_IOV_MAJOR_VER_SHIFT) | WL_CHRE_IOV_MINOR_VER_1)

enum wl_chre_cmd_ids {
	WL_CHRE_CMD_VERSION		= 0u,	/* Get firmware version string */
	WL_CHRE_CMD_ENABLE		= 1u	/* Set/Get CHRE feature enable */
};

enum wl_chre_xtlv_id {
	WL_CHRE_XTLV_VERSION		= 0x0u,
	WL_CHRE_XTLV_ENABLE		= 0x1u
};
#endif /* CHRE */

#define WLC_DYN_BW_CONFIG_VER_1 (1u)
typedef struct wlc_dyn_bw_cfg_v1 {
	uint16	ver;		/* Version */
	uint16	len;		/* Length of the structure */
	uint8	is_enabled;	/* Enable/disable dynamic BW */
	uint8	tx_dyn_bw;	/* Tx dynamic BW mode of operation */
	uint8	backoff_sb;	/* Dynamic BW Backoff sub-band */
	uint8	pad;	/* Pad bytes */
	uint32	pricrs_mask;	/* PRI_40_80_CRS_MASK in LSB
				 * IFS_CTL_SEL_PRICRS in MSB
				 */
} wlc_dyn_bw_cfg_v1_t;

enum wlc_dyn_bw_cmd_id {
	WLC_DYN_BW_XTLV_VER_ID		= 1u,
	WLC_DYN_BW_XTLV_ENABLE_ID	= 2u,
	WLC_DYN_BW_XTLV_TX_ID		= 3u,
	WLC_DYN_BW_XTLV_BACKOFF_SB_ID	= 4u,
	WLC_DYN_BW_XTLV_PRICRS_MASK_ID	= 5u,
	WLC_DYN_BW_XTLV_SCRMBLR_WAR_ID	= 6u,
	WLC_DYN_BW_XTLV_LAST_ID
};

#define WL_ROAM_PARAMS_IOV_MAJOR_VER_SHIFT 8u

#define WL_ROAM_PARAMS_IOV_MAJOR_VER_1 1u
#define WL_ROAM_PARAMS_IOV_MINOR_VER_1 1u

#define WL_ROAM_PARAMS_IOV_VERSION_1_1 \
	((WL_ROAM_PARAMS_IOV_MAJOR_VER_1 << WL_ROAM_PARAMS_IOV_MAJOR_VER_SHIFT) | \
	WL_ROAM_PARAMS_IOV_MINOR_VER_1)

/* roam_params sub-command types */
typedef enum {
	WL_ROAM_PARAMS_NONE = 0,
	WL_ROAM_PARAMS_ALLOWED_BAND = 1,
	WL_ROAM_PARAMS_MAX
} wl_roam_params_type_t;

/* roam allowed band index */
#define	WLC_ROAM_ALLOW_BAND_AUTO	WLC_BAND_AUTO	/* auto-select */
#define	WLC_ROAM_ALLOW_BAND_2G		(1<<0)		/* 2.4 Ghz */
#define	WLC_ROAM_ALLOW_BAND_5G		(1<<1)		/* 5 Ghz */
#if defined(WL_BAND6G) || defined(WL_6G_BAND)
#define	WLC_ROAM_ALLOW_BAND_6G		(1<<2)		/* 6 Ghz */
#define	WLC_ROAM_ALLOW_BAND_MAX		(WLC_ROAM_ALLOW_BAND_2G | WLC_ROAM_ALLOW_BAND_5G | \
	WLC_ROAM_ALLOW_BAND_6G)
#else
#define	WLC_ROAM_ALLOW_BAND_MAX		(WLC_ROAM_ALLOW_BAND_2G | WLC_ROAM_ALLOW_BAND_5G)
#endif	/* WL_BAND6G || WL_6G_BAND */

/*
 * Channel State Information (CSI) definitions
 *
 * IOVAR encoded in bcm_xtlv_t format with the subcommand as the TYPE field,
 * length of subcommand data as the LENGTH field, followed by subcommand data
 * structure as the DATA field.
 */

/* subcommand enum */
typedef enum wl_csi_subcommand {
	WL_CSI_SUBCMD_VERSION	= 0u,		/* CSI version */
	WL_CSI_SUBCMD_DISABLE	= 1u,		/* disable CSI data */
	WL_CSI_SUBCMD_ENABLE	= 2u,		/* enable CSI data */
	WL_CSI_SUBCMD_DATA_INFO	= 3u,		/* get CSI data info */
	WL_CSI_SUBCMD_GET_DATA	= 4u,		/* get CSI data fragment */
} wl_csi_subcommand_t;

/* current version */
#define WL_CSI_VERSION_V2		2u	/* adds CSI data available event */
#define WL_CSI_VERSION_V1		1u

/* WL_CSI_SUBCMD_VERSION GET subcommand data */
typedef struct wl_csi_version {
	uint16 version;				/* CSI version */
	uint8 pad[2u];				/* 4-byte struct alignment */
} wl_csi_version_t;

/* enable control bits */
#define WL_CSI_ENABLE_CNTRL_TA		(1u << 0u)	/* match TA addr */
#define WL_CSI_ENABLE_CNTRL_FRAME	(1u << 1u)	/* match frame type/subtype */
#define WL_CSI_ENABLE_CNTRL_SUBCHANNEL	(1u << 2u)	/* enable subchannel index/width */
#define WL_CSI_ENABLE_CNTRL_NSTREAM	(1u << 3u)	/* enable number of streams */

/* WL_CSI_SUBCMD_ENABLE SET subcommand data */
typedef struct wl_csi_enable {
	uint16 enable_control;			/* control bits */
	uint8 subchannel_index;			/* 0=1st, 1=2nd, etc */
	uint8 subchannel_width;			/* 0=20mhz, 1=40mhz, 2=80mhz, etc */
	uint8 num_streams;			/* 0=1 stream, 1=2 streams, etc */
	uint8 frame;				/* frame type (bits=0x0c) and subtype (bits=0xf0) */
	chanspec_t chanspec;			/* not currently used, reserved for future */
	struct ether_addr ta;			/* transmit address to match */
	uint16 mode_timer;			/* not currently used, reserved for future */
} wl_csi_enable_t;

/* WL_CSI_SUBCMD_DATA_INFO GET subcommand data info */
typedef struct wl_csi_data_info {
	struct ether_addr ta;			/* transmit address */
	struct ether_addr ra;			/* receive address */
	struct ether_addr bssid;		/* bssid address */
	uint16 num_tones;			/* number of tones in data */
	uint16 bw;				/* bandwidth of frame */
	uint8 frame;				/* frame type (bits=0x0c) and subtype (bits=0xf0) */
	uint8 nsts;				/* num streams of frame */
	uint8 slice;				/* 0=main, 1=aux */
	uint8 num_rx;				/* number of rx cores in data */
	uint8 num_streams;			/* number of streams in data */
	uint8 num_rssi;				/* number of rssi in following array */
	int8 rssi[BCM_FLEX_ARRAY];		/* variable length array of rssi of each core */
} wl_csi_data_info_t;

/* WL_CSI_SUBCMD_GET_DATA GET subcommand data fragment request */
typedef struct wl_csi_get_data_req {
	uint32 offset;				/* CSI data offset to read */
	uint32 length;				/* request length */
} wl_csi_get_data_req_t;

/* WL_CSI_SUBCMD_GET_DATA GET subcommand data fragment response */
typedef struct wl_csi_get_data_resp {
	uint32 total_length;			/* total length of CSI data */
	uint32 offset;				/* CSI data offset read */
	uint32 length;				/* length of CSI data in following array */
	uint32 csi_data[BCM_FLEX_ARRAY];	/* variable length array of CSI data */
} wl_csi_get_data_resp_t;

/* User roam cache size updated 10 so that
 * 4 slots are provided for 6G.
 */
#define	USER_ROAM_CACHE_MAX_COUNT	10u

#define WL_OCT_IOV_VERSION_1	1u


enum wl_oct_iov_sub_cmd_v1 {
	IOV_OCT_SUB_CMD_NONE =				0u,
	IOV_OCT_SUB_CMD_ENAB =				1u,
	IOV_OCT_SUB_CMD_STALE_RESET_TIMEOUT =		2u,
	IOV_OCT_SUB_CMD_NO_DECISION_TIMEOUT =		3u,
	IOV_OCT_SUB_CMD_CUR_CORE_MIN_DWELL_TIME =	4u,
	IOV_OCT_SUB_CMD_EMA_WEIGHT_FACTOR =		5u,
	IOV_OCT_SUB_CMD_MIN_NUM_VALID_CNT =		6u,
	IOV_OCT_SUB_CMD_RSSI_DELTA_THRESH =		7u,
	IOV_OCT_SUB_CMD_MIN_SAMPLING_FREQ =		8u,
	IOV_OCT_SUB_CMD_VERSION =			9u,
	IOV_OCT_SUB_CMD_STATUS =			10u,
	IOV_OCT_SUB_CMD_RESET_STATUS =			11u,
	IOV_OCT_SUB_CMD_TOTAL
};

enum wl_oct_xtlv_id_v1 {
	WL_OCT_XTLV_NONE =				0x0u,
	WL_OCT_XTLV_ENABLE =				0x1u,
	WL_OCT_XTLV_STALE_RESET_TIMEOUT =		0x2u,
	WL_OCT_XTLV_NO_DECISION_TIMEOUT	=		0x3u,
	WL_OCT_XTLV_CUR_CORE_MIN_DWELL_TIME =		0x4u,
	WL_OCT_XTLV_EMA_WEIGHT_FACTOR =			0x5u,
	WL_OCT_XTLV_MIN_NUM_VALID_CNT =			0x6u,
	WL_OCT_XTLV_RSSI_DELTA_THRESH =			0x7u,
	WL_OCT_XTLV_MIN_SAMPLING_FREQ =			0x8u,
	WL_OCT_XTLV_VERSION =				0x9u,
	WL_OCT_XTLV_STATUS =				0xau,
	WL_OCT_XTLV_RESET_STATUS =			0xbu,
	WL_OCT_XTLV_TOTAL
};

typedef enum wl_oct_status_xtlv_id {
	WL_OCT_STATUS_CMN_ID =				0u,
	WL_OCT_STATUS_SLICE_ID =			1u,
	WL_OCT_STATUS_XTLV_MAX =			2u
} wl_oct_status_xtlv_id_t;

#define WL_OCT_STATUS_CMN_VER_1				1u /* status_cmn version */
#define	WL_OCT_STATUS_SLICE_VER_1			1u /* status_slice version */

/* WL_OCT_STATUS_CMN_ID */
typedef struct wl_oct_status_cmn_v1 {
	uint8	version;
	uint8	infra_assoced;			/* primary infra assoced */
	uint8	infra_assoc_slice;		/* slice of primary infra assoced */
	uint8	selected_txcm;			/* currently selected txcm */
} wl_oct_status_cmn_v1_t;

/*
 * Definitions for disable bits in the per slice status
 */
#define OCT_DISABLED_HOST			(1u << 0u) /* Host disabled */
#define OCT_DISABLED_NOINFRA			(1u << 1u) /* Disabled due to
		                                            * no-infra or AS not-idle
		                                            */
#define OCT_DISABLED_SCAN			(1u << 2u) /* Disable due to Scan active */

#define OCT_CORES_MAX				2u
/* WL_OCT_STATUS_SLICE_ID */
typedef struct wl_oct_status_slice_v1 {
	uint8	version;
	uint8	slice_id;			/* slice identifier */
	uint8	PAD[2];				/* 32-bit align */
	uint32	disable_bits;			/* current disable bits */
	uint32	disable_dur;			/* total disable duration (ms) */
	uint32	enable_dur;			/* total enable duration (ms) */
	uint32  txcm_upd_cnt;			/* total count of txcm decision updates
	                                         * due RSSI, TxPER, etc. blocks,
	                                         * currently RSSI block only
	                                         */
	uint32	txcm_upd_cnt_rssi;		/* total count of txcore updates
		                                 * due to RSSI delta
	                                         */
	uint32	txcm_upd_cnt_rssi_aged;		/* total count of txcore updates due to rssi aged */
	uint32	override_dur;			/* total dur (ms) oct decision is overriden */
	uint32	txcm_dur[OCT_CORES_MAX];	/* per core txcm duration (ms) */
	uint32	txcm_cnt[OCT_CORES_MAX];	/* per core txcm count */
} wl_oct_status_slice_v1_t;

/* ====== C2C definitions ===== */
/* subcommand IDs */
enum wl_c2c_cmd_id {
	WL_C2C_CMD_PRE_EXPIRY,
	WL_C2C_CMD_REG_RULES,
	WL_C2C_CMD_STATUS,
	WL_C2C_CMD_STATS,
	WL_C2C_CMD_AP_CACHE,
	WL_C2C_CMD_CTRL,
	WL_C2C_CMD_EVTMASK,
	WL_C2C_CMD_LAST
};

/* xtlvs */
enum wl_c2c_tlv_id {
	WL_C2C_XTLV_AP_ENTRY,
	WL_C2C_XTLV_AP_INFO,
	WL_C2C_XTLV_LAST
};

/* WL_C2C_XTLV_AP_ENTRY */
typedef struct wl_c2c_ap_entry {
	struct ether_addr bssid;		/* ap bssid */
	chanspec_t	chanspec;		/* ap chanspec */
	uint32		last_rx_time;		/* last esig rx time (GET only) */
	uint32		num_esig_rx;		/* esigs received from AP (GET only) */
	int8		rssi;			/* rssi (GET only) */
	uint8		num_tlvs;		/* number of tlvs included */
	uint8		pad[2];
	uint8		tlvs[];
} wl_c2c_ap_entry_t;

/* internal c2c states
 *
 * |----------ON----------|--ON(Pre-expiry)--|
 * |                                         |
 * |<--------Enabling signal duration------->|
 */
#define C2C_STATE_OFF		1u	/* c2c is inactive */
#define C2C_STATE_ON		2u	/* c2c active */
#define C2C_STATE_ON_PRE_EXP	3u	/* c2c active, in pre-expiry region */

#define C2C_SCAN_STATE_OFF	1u	/* no c2c-initiated scan */
#define C2C_SCAN_STATE_IN_PROG	2u	/* c2c-initiated scan in progress */

#define C2C_SCAN_TYPE_DIRECT	1u	/* direct scan to 6GHz AP */
#define C2C_SCAN_TYPE_FULL	2u	/* full scan */

/* WL_C2C_CMD_PRE_EXPIRY */
typedef uint32 wl_c2c_pre_exp_t;	/* pre expiry time */

/* WL_C2C_CMD_REG_RULES */
#define WL_C2C_REG_RULES_VLP_ALLOWED	1u
#define WL_C2C_REG_RULES_INV_TIMEOUT	0xFFFFFFFFu

typedef struct wl_c2c_reg_rules {
	uint32	esig_timeout;		/* enabling signal timeout(ms) */
	int8	min_rssi;		/* enabling signal min rssi */
	uint8	pad[3];			/* available for use */
} wl_c2c_reg_rules_t;

/* WL_C2C_CMD_STATUS */
typedef struct wl_c2c_status {
	uint8	cur_state;		/* c2c state */
	uint8	scan_state;		/* scan state */
	struct ether_addr esig_bssid;	/* current enabling signal bssid */
	chanspec_t esig_chanspec;	/* current enabling signal chanspec */
	int8	esig_rssi;		/* current enabling signal rssi */
	uint8	scan_type;		/* scan type */
	uint32	last_dscan;		/* last direct scan start (FW) time */
	uint32	last_fscan;		/* last full scan start time */
} wl_c2c_status_t;

/* WL_C2C_CMD_CTRL */
/* if set, enable scan requests from c2c module */
#define	WL_C2C_CTRL_C2C_SCAN		0x0001u
/* if set, enable c2c functionality
 * if clear, c2c functionality - enabling signal processing, scanning, etc is disabled
 */
#define	WL_C2C_CTRL_ENAB		0x0002u
#define WL_C2C_CTRL_ALLOWED_MASK	(WL_C2C_CTRL_C2C_SCAN | WL_C2C_CTRL_ENAB)

#define WL_C2C_CTRL_IS_C2C_SCAN(_ctrl)	(((_ctrl) & WL_C2C_CTRL_C2C_SCAN) != 0)
#define WL_C2C_CTRL_IS_ENAB(_ctrl)	(((_ctrl) & WL_C2C_CTRL_ENAB) != 0)

typedef uint32 wl_c2c_ctrl_t;		/* c2c control bit masks */

/* WL_C2C_CMD_AP_CACHE */
/* WL_C2C_CMD_AP_CACHE operations */
#define WL_C2C_CACHE_OP_GET	0u
#define WL_C2C_CACHE_OP_ADD	1u
#define WL_C2C_CACHE_OP_DEL	2u
#define WL_C2C_CACHE_OP_CLR	3u

typedef struct wl_c2c_ap_cache {
	uint8		op;		/* WL_C2C_CACHE_OP */
	uint8		num_tlvs;	/* number of tlvs */
	uint8		pad[2];
	uint8		tlvs[];
} wl_c2c_ap_cache_t;

/* WL_C2C_CMD_STATS */
/* WL_C2C_CMD_STATS operations */
#define WL_C2C_STATS_OP_CLEAR	0u

typedef struct wl_c2c_stats {
	uint32		esig_rx;	/* total received enabling signals */
	uint32		c2c_on_off;	/* count of c2c on to off */
	uint32		c2c_off_on;	/* count of c2c off to on */
	uint32		c2c_extn;	/* c2c extension (due to new esig) */
	uint32		dscan;		/* number of directed scans */
	uint32		fscan;		/* number of full scans */
	uint8		op;		/* WL_C2C_STATS_OP */
	uint8		num_tlvs;	/* number of tlvs included */
	uint8		pad[2];		/* available for use */
	uint8		tlvs[];
} wl_c2c_stats_t;

/* WL_C2C_CMD_EVTMASK */
typedef uint32 wl_c2c_evt_mask_t;	/* event mask */

/* events */
#define WL_C2C_EVENT_AP_INFO_VER_1	1u
/* WL_C2C_XTLV_AP_INFO */
typedef struct wl_c2c_event_ap_info {
	uint16		version;	/* version */
	uint16		len;		/* total len */
	struct ether_addr bssid;	/* AP bssid */
	chanspec_t	chanspec;	/* AP chanspec */
	uint32		time_ms;	/* esig receive time (FW time in ms) */
	int8		rssi;		/* rssi */
	uint8		num_tlvs;	/* number of tlvs */
	uint8		pad[2];
	uint8		tlvs[];		/* optional TLVs */
} wl_c2c_event_ap_info_t;

#define WL_CH6GPROF_VER_1 1
enum wl_ch6gprof_cmd_id {
	WL_CH6GPROF_CMD_VER		= 0u,
	WL_CH6GPROF_CMD_EXPIRY_TO	= 1u,
	WL_CH6GPROF_CMD_RESET_CACHE	= 2u
};

/* scbrate iovar: txminrate  */
#define WL_SCBRATE_TXMINRATE_VER_1	1u
typedef struct wl_scbrate_txminrate {
	uint16		version;
	uint16		len;
	struct ether_addr	peer_mac;	/* mac addrss of peer */
	uint16		txminrate;	/* Tx min rate in 0.5mbps */
} wl_scbrate_txminrate_t;

/* Test seeding top level command IDs */
enum {
	WL_SEED_TEST_CMD_ROAM_CONFIG			= 0u,
	WL_SEED_TEST_CMD_ROAM_CLEAR_TARGETS		= 1u,
	WL_SEED_TEST_CMD_ROAM_ADD_TARGETS		= 2u,
	WL_SEED_TEST_CMD_LAST
};

#define SEED_TEST_ROAM_CFG_VERSION_1 1u
/* set roam test seeding rule:
 * replace(1) or append(0) host's target list to FW's scan results
 */
#define SEED_TEST_ROAM_CFGFLAG_REPLACE 0x1u

struct wl_seed_test_roam_config_v1 {
	uint16 version;
	uint16 length;
	uint16 flags;
	uint8  pad[2];
};

#define SEED_TEST_ROAM_MODE_CAP_N	0x1u
#define SEED_TEST_ROAM_MODE_CAP_VHT	0x2u
#define SEED_TEST_ROAM_MODE_CAP_EHT	0x4u
#define SEED_TEST_ROAM_MODE_CAP_HE	0x8u

#define SEED_TEST_ROAM_ADD_TARGETS_VERSION_1 1u

struct wl_seed_test_roam_add_targets_v1 {
	uint16		version;		/* version field */
	/* byte length of data in this data structure,
	 * starting at version and including IEs appended afterwards
	 */
	uint16		length;
	struct ether_addr BSSID;
	uint8		mode_cap;	/* n_cap, he_cap, vht_cap, eht_cap */
	uint8		SSID_len;
	/* values can be short ssid or ssid, indicated in flags */
	uint8		SSID[DOT11_MAX_SSID_LEN];
	chanspec_t	chanspec;	/* chanspec for bss */
	int16		RSSI;		/* receive signal strength (in dBm) */
	uint16		capability;	/* capability filed in beacon */
	uint16		flags;		/* flags w.r.t. beacon: TBD */
	struct {
		uint32	count;		/* number of rates in this set */
		uint8	rates[WLC_NUMRATES];	/* rates in 500kbps units w/hi bit set if basic */
	} rateset;			/* supported rates */
	uint8		basic_mcs[MCSSET_LEN];		/* 802.11N BSS required MCS set */
	uint16		vht_mcsmap;		/* vhtmcsmap */
	uint16		vht_mcsmap_prop;	/* prop vhtmcsmap */
	uint32		he_mcsmap;		/* same as he_mcs_nss_set */
	wl_eht_mcsmap_t eht_mcsmap;	/* EHT-MCS Map for the BSS operating chan width */
	uint8		pad1;
	/* qbss load available admission capacity, from qbss_load_ie */
	uint16		qbss_load_aac;
	/* indicates how free the channel is, (0xff - channel_utilization of qbss_load_ie) */
	uint8		qbss_load_chan_free;
	/* last set max txpwr, from VHT TPE IE, for 6G AP */
	int8		max_tx_pwr_used;
	/* from HE Operation IE with HE 6G Operation info, for 6G AP */
	uint8		reg_info_6g_bss;
	uint8		pad2;
	uint16		ie_length;	/* length of following ie data, can be 0 */
	uint8		ie_data[];	/* ie data indicated by length above */
};

/* subcommand ids for phy_dbg */
enum wl_phy_dbg_cmd_type {
	WL_PHY_DBG_CMD_GCI = 0u,	/* GCI Dump */
	WL_PHY_DBG_CMD_SRA = 1u,	/* SRA Info */
	WL_PHY_DBG_CMD_LAST
};

#define WL_PHY_DBG_GCI_VERSION_1	1u

typedef struct wl_phy_dbg_gci_data_v1 {
	uint16 gci_lst_inv_ctr;
	uint16 gci_lst_rst_ctr;
	uint16 gci_lst_sem_fail;
	uint16 gci_lst_rb_state;
	uint16 gci_lst_state_mask;
	uint16 gci_inv_tx;
	uint16 gci_inv_rx;
	uint16 gci_rst_tx;
	uint16 gci_rst_rx;
	uint16 gci_sem_fail;
	uint16 gci_invstate;
	uint16 gci_phyctl2;
	uint16 gci_chan;
	uint16 gci_cm;
	uint16 gci_intr;
	uint16 gci_rst_intr;
	uint16 gci_rst_prdc_rx;
	uint16 gci_rst_wk_rx;
	uint16 gci_rst_rmac_rx;
	uint16 gci_rst_tx_rx;
	uint16 gci_dbg01;
	uint16 gci_dbg02;
	uint16 gci_dbg03;
	uint16 gci_dbg04;
	uint16 gci_dbg05;
	uint16 gci_dbg06;
} wl_phy_dbg_gci_data_v1_t;

#define WL_PHY_DBG_SRA_INFO_VERSION_1	1u
/* Info struct */
typedef struct wl_phy_dbg_sra_info_v1 {
	uint32	srmc_init_status_bt;	/* SRCB init_status reg for bt */
	uint32	srmc_init_status_wl;	/* SRCB init_status reg for wl */
	uint32	sr_crash_counter_bt;	/* BT FW trap count */
	uint32	sr_crash_counter_wl;	/* WL FW trap count */
	uint16	sr_crash_reason_bt;	/* BT FW trap reason */
	uint16	sr_crash_reason_wl;	/* WL FW trap reason */
	uint8	sr_boot_count;		/* critical + non-critical region boot count */
	uint8	sr_softrecovery_count;	/* critical region recoverable boot count */
	uint16	sr_dbg01;
	uint32	sr_dbg02;
	uint32	sr_dbg03;
	uint32	sr_dbg04;
} wl_phy_dbg_sra_info_v1_t;

typedef struct wl_phy_dbg_v1 {
	uint16	subcmd_version;		/* Version of the sub-command */
	uint16  length;			/* Length of the particular struct being used in union */
	union {
		wl_phy_dbg_gci_data_v1_t gcivals_v1;	/* GCI data */
		wl_phy_dbg_sra_info_v1_t srainfo_v1;	/* SRA info */
	} u;
} wl_phy_dbg_v1_t;

/* subcommand ids for phy_ed */
enum wl_phy_ed_cmd_type {
	WL_PHY_ED_CMD_MLED = 0u,	/* get mled profile */
	WL_PHY_ED_CMD_LAST
};

#define WL_PHY_ED_MLED_VERSION_1	1u

typedef struct wl_phy_ed_mled_v1 {
	chanspec_t chanspec;		/* chanspec */
	uint8 mled_profile;		/* mled profile */
	uint8 region_group;		/* region group - EU or other */
	int8 edthresh_val;		/* ed threshold - primary */
	int8 edthresh_sec_val;		/* ed threshold - secondary */
} wl_phy_ed_mled_v1_t;

#define WL_PHY_ED_MLED_VERSION_2	2u

typedef struct wl_phy_ed_mled_v2 {
	chanspec_t chanspec;		/* chanspec */
	uint8 mled_profile;		/* mled profile */
	uint8 eu_edcrs;			/* EU or other */
	int8 edthresh_val;		/* ed threshold - primary */
	int8 edthresh_sec_val;		/* ed threshold - secondary */
} wl_phy_ed_mled_v2_t;

typedef struct wl_phy_ed_v1 {
	uint16  subcmd_version;		/* Version of the sub-command */
	uint16  length;			/* Length of the particular struct being used in union */
	union {
		wl_phy_ed_mled_v1_t mled_v1;	/* mled profile data */
		wl_phy_ed_mled_v2_t mled_v2;	/* mled profile data */
	} u;
} wl_phy_ed_v1_t;

/* subcommand ids for phy_noise */
enum wl_phy_noise_cmd_type {
	WL_PHY_NOISE_CMD_LONG = 0u,	/* get noise profile */
	WL_PHY_NOISE_CMD_LAST
};

#define WL_PHY_NOISE_LONG_VERSION_1	1u

typedef struct wl_phy_noise_long_v1 {
	uint8 long_accum;		/* long accum */
	uint8   PAD[3];
} wl_phy_noise_long_v1_t;

typedef struct wl_phy_noise_v1 {
	uint16  subcmd_version;		/* Version of the sub-command */
	uint16  length;			/* Length of the particular struct being used in union */
	union {
		wl_phy_noise_long_v1_t long_v1;
	} u;
} wl_phy_noise_v1_t;

#ifndef CHSPEC2WLC_BAND
#ifdef WL_BAND6G
#define CHSPEC2WLC_BAND(chspec) (CHSPEC_IS2G(chspec) ? WLC_BAND_2G : \
				 CHSPEC_IS5G(chspec) ? WLC_BAND_5G : WLC_BAND_6G)
#else
#define CHSPEC2WLC_BAND(chspec) (CHSPEC_IS2G(chspec) ? WLC_BAND_2G : WLC_BAND_5G)
#endif
#endif /* CHSPEC2WLC_BAND */

#define BUS_TX_RELEASE_LOG_VERSION_1	(1u)
typedef struct _bus_tx_release_log_entry_v1 {
	uint32	ts;		/* timestamp */
	uint32  flowid;		/* flowing id */
	uint32  pkts_cnt;	/* # of tx pkts released to wl layer */
	uint32  times;		/* same flowid repeats how many times */
} bus_tx_release_log_entry_v1_t;

#define BUS_TX_RELEASE_LOG_FLAG_TRUNCATED	(0x1u)
struct bus_tx_release_log_v1 {
	uint16				version;	/**< version field */
	uint16				flags;		/**< flags */
	uint16				idx;		/**< current index inside log */
	uint16				count;		/**< total log count */
	bus_tx_release_log_entry_v1_t	log[];		/**< count # of tx release log entries */
};

#define BUS_FETCH_HISTOGRAM_VERSION_1	(1u)
typedef struct  flow_fetch_histogram_v1 {
	uint32  flowid;				 /* flowing id */
	uint32	fetch_histogram[BCM_FLEX_ARRAY]; /* per flow fetch histogram of all stop reasons */
} flow_fetch_histogram_v1_t;

#define BUS_TX_RELEASE_LOG_FLAG_TRUNCATED	(0x1u)
struct bus_fetch_historgram_v1 {
	uint16				version;		/**< version field */
	uint16				unused;			/**< unused */
	uint16				bucket_interval;	/* bucket interval */
	uint16				buckets_max;		/* max # of buckets */
	uint16				reason_max;		/* max # of fetch stop reasons */
	uint16				histogram_count;	/**< count of fetch histograms */
	flow_fetch_histogram_v1_t fetch_histograms[BCM_FLEX_ARRAY]; /**< flow fetch histograms */
};

/* Scancache command IDs */
enum wl_scan_sub_cmd {
	WL_SCAN_CACHE_EXT_CMD_CLEAR = 0u,
	WL_SCAN_CACHE_EXT_CMD_GET = 1u,
	WL_SCAN_CMD_LAST
};

#define SCAN_CACHE_VERSION_1	(1u)
typedef struct wlc_scan_cache_data_v1 {
	wlc_ssid_t		SSID;		/* SSID of cached result */
	struct ether_addr	BSSID;		/* BSSID of cached result */
	bool			valid;		/* is cache entry valid */
	uint8			PAD[1];
	chanspec_t		chanspec;	/* chanspec of cached result */
	int16			RSSI;		/* RSSI of cached result */
	uint32			timestamp;	/* time when scancache is updated */
	uint32			capability;	/* capability of cached entry */
	uint32			flags;		/* Flags to indicate HT/VHT/HE/EHT etc */
	uint32			age_ms;		/* Time difference of scan cache update
						 * and scan cache query
						 */
} wlc_scan_cache_data_v1_t;

typedef struct wlc_scan_cache_v1 {
	uint16				version;
	uint16				len;
	wlc_scan_cache_data_v1_t	scan_data[];
} wlc_scan_cache_v1_t;

#define SCAN_CACHE_FIXED_SIZE	(OFFSETOF(wlc_scan_cache_v1_t, scan_data))

/* definition for uint16 iovar wifi_bt5g_policy */
#define WIFI_BT5G_POLICY_ASSOC		0x0001u	/**< to be obsoleted */
#define WIFI_BT5G_POLICY_ASSOCMODE	0xFu	/**< reserved after this */

enum {
	WIFI_BT5G_BT_SWITCH_ASSOC_WAIT		= 0u, /* BT switch band, wait for BT switch */
	WIFI_BT5G_BT_SWITCH_ASSOC_NOWAIT	= 1u, /* BT switch band, associate immediately */
	WIFI_BT5G_NO_BT_SWITCH			= 2u, /* No BT switch the band, BT coexist in 5G */
};

/* Event throttle configuration */
#define WL_EVT_THROT_CFG_VERSION_1	1u
typedef struct wl_evt_throt_cfg_v1 {
	uint16 version;
	uint16 payload_len;
	uint32 mem_thresh;	/* global memory threshold in bytes */
	uint32 flags;		/* future use */
	uint8 payload[];	/* list of TLVs */
} wl_evt_throt_cfg_v1_t;

/* XTLV type */
#define WL_EVT_THROT_IOV_MOD_CFG	1u

/* XTLV type 1 */
typedef struct wl_evt_throt_cfg_mod_payload {
	uint16 event_id;
	uint16 nevents;		/* max pending bursty events allowed */
	uint32 ageout_ms;	/* time threshold in ms to throttle old pkts */
	uint32 flags;		/* generic enable/disable (1|0) */
} wl_evt_throt_cfg_mod_payload_t;

#define PHY_SCCA_JAMMER_HOSTPARAMS_SET_V1 1

/* Host 'Set' params */
typedef struct  phy_scca_jammer_host_params_v1 {
	uint32	version;
	uint32	length;
	int8	asym_intf_jammer_pwr;
	uint8	asym_intf_jammer_cm;
	int16	pad1;
} phy_scca_jammer_host_params_v1_t;

#define PHY_SCCA_JAMMER_CONFIG_PARAMS_V1 1

/* Host 'Get' params */
typedef struct phy_scca_jammer_config_params_v1 {
	uint32	version;
	uint32	length;
	bool	asym_intf_jammer_en;
	uint8	asym_intf_jammer_cm;
	int8	asym_intf_jammer_pwr[2];
} phy_scca_jammer_config_params_v1_t;

#define WLC_SUP_OPER_CLASS_IE_IOV_VER_1		0x1u
typedef struct wlc_sup_oper_class_cfg_v1 {
	uint16 version;		/* Structure version */
	uint16 length;		/* Length of whole Structure */
	uint32 cmd_flags;	/* Flag bits to Identify configuring command */
	uint32 config_flags;	/* Configuration and conditional flags that shall be set by the
				 * host for including the Supported Operating Class IE
				 */
} wlc_sup_oper_class_cfg_v1_t;

/* Following MACROS are used to individually identifies the config flags which is provided in
 * set operation
 */
#define WLC_SUP_OPR_CLS_IE_CFG_ENABLE_CMD		(1u << 0u)
#define WLC_SUP_OPR_CLS_IE_CFG_RNR_PRES_CMD		(1u << 1u)
#define WLC_SUP_OPR_CLS_IE_CFG_SUP_OPR_CLS_PRES_CMD	(1u << 2u)

/* Define the config command mask */
#define WLC_SUP_OPR_CLS_IE_CFG_CMD_MASK	(WLC_SUP_OPR_CLS_IE_CFG_ENABLE_CMD |\
						WLC_SUP_OPR_CLS_IE_CFG_RNR_PRES_CMD |\
						WLC_SUP_OPR_CLS_IE_CFG_SUP_OPR_CLS_PRES_CMD)

/* Following MACROS are used to set the respective flags which is provided through subcommand
 * set operation
 */

/* Enable flag for Supported Operating Class IE */
#define WLC_SUP_OPR_CLS_IE_ENAB_VAL			(1u << 0u)

/* Send Supported Operating Class IE when RNR has 6G channels */
#define WLC_SUP_OPR_CLS_IE_RNR_PRES_VAL			(1u << 1u)

/* Send Supported Operating Class IE when AP advertises its Supported
 * Operating Class IE in Beacon/Probe response
 */
#define WLC_SUP_OPR_CLS_IE_SUP_OPR_CLS_PRES_VAL		(1u << 2u)

#define WLC_SUP_OPR_CLS_IE_DEFAULT_CFG_VAL	(WLC_SUP_OPR_CLS_IE_ENAB_VAL |\
							WLC_SUP_OPR_CLS_IE_RNR_PRES_VAL |\
							WLC_SUP_OPR_CLS_IE_SUP_OPR_CLS_PRES_VAL)


#ifdef PHY_FCBS_CHSW
/* for FW FCBS_HW control version */
#define WL_FCBS_CHSW_IOV_VERSION_1 1u

/* FW FCBS_CHSW control iovar subcmd ids */
enum {
	IOV_FCBS_CHSW_ENAB = 1,
	IOV_FCBS_CHSW_STATUS = 2,
	IOV_FCBS_CHSW_LAST
};
#endif /* PHY_FCBS_CHSW */

#define BSS_STA_INFO_PARAM_VER_1	1u

/** Input structure for IOV_BSS_PEER_INFO */
typedef struct bss_sta_info_param {
	uint16			version;
	struct ether_addr	ea;		/**< peer MAC address */
}  bss_sta_info_param_t;

#define BSS_STA_INFO_VER_1	1u

typedef struct bss_sta_info {
	uint16			version;
	uint16			aid;		/* Association ID */
	struct ether_addr	ea;		/* Peer MAC ADDR */
	uint16			pad;		/* For 32bit alignment */
	uint32			idle;		/**< time since data pkt rx'd from sta */
	uint32			listen_int;	/* Min Listen interval in ms for this STA */
	int32			rssi;		/* RSSI AVG */
	uint32			tx_rate;	/**< current tx rate */
	uint32			rx_rate;	/**< current rx rate */
	uint32			in_network;	/**< in network seconds */
	uint32			rx_pkts;	/* rx data pkts */
	uint64			rx_bytes;	/* rx data bytes */
	uint32			rx_retries;	/* rx pkts with retry bit set */
	uint32			rx_error;	/* rx error */
	uint64			tx_bytes;	/* tx total bytes */
	uint32			tx_pkts;	/* tx total pkts */
	uint32			tx_retries;	/* tx pkts retries */
	uint32			tx_fail;	/* tx failures */
	uint32			pad1;		/* For 64bits alignment */
} bss_sta_info_t;

#define BSS_STA_LIST_INFO_VER_1	1u

typedef struct bss_sta_list_info {
	uint16			version;
	uint16			bss_sta_info_len;	/**< length of bss_sta_info_t */
	uint32			count;			/**< number of peer info */
	bss_sta_info_t		peer_sta_info[];	/**< peer info */
} bss_sta_list_info_t;

#define BSS_STA_LIST_INFO_FIXED_LEN OFFSETOF(bss_sta_list_info_t, peer_sta_info)

#define CAPEXT_INFO_VERSION_1	(1u)

/* TODO: This would be moved to src once component depedency is addressed */
#ifndef BCM_CAPEXT
#define CAPEXT_INFO_VERSION	CAPEXT_INFO_VERSION_1
#endif /* BCM_CAPEXT */

/* Top structure of capext reporting. For reporting, feature ids are used as types in XTLVs */
typedef struct {
	uint16	version;	/**< see definition of CAPEXT_INFO_VERSION */
	uint16	datalen;	/**< length of data including all paddings. */
	uint8   data [];	/**< variable length payload:
				 * 1 or more bcm_xtlv_t type of tuples.
				 * each tuple is padded to multiple of 4 bytes.
				 * 'datalen' field of this structure includes all paddings.
				 */
} capext_info_t;

/* Each feature reported in capext has a feature id. Feature id is a 16-bit value.
 * The feature id namespace is split into 3 partitions. One for BUS, the second for RTE,
 * and the third for WL. All partitions are contiguous and fixed in size
 */
#define CAPEXT_FEATURE_ID_NUM_PARTITIONS	(3u)
#define CAPEXT_FEATURE_ID_PARTITION_SIZE	(1024u)
/* Feature IDs from 3072 for capext are reserved */
#define CAPEXT_RSVD_FEATURE_ID_BASE		(3072u)

/* Bus partition */
/* The features listed in the enumeration below have subfeatures.
 * If a new feature is added/updated and that feature has sub-features that need to be reported,
 * add that feature here
 */
#define CAPEXT_BUS_FEATURE_ID_BASE		(0)
enum capext_bus_feature_ids {
	CAPEXT_BUS_FEATURE_RSVD		= (CAPEXT_BUS_FEATURE_ID_BASE + 0),
	/* BUS top level feature id to hold and report bitmaps of features with and
	 * without sub-features.
	 */
	CAPEXT_BUS_FEATURE_BUS_FEATURES	= (CAPEXT_BUS_FEATURE_ID_BASE + 1),
	/* BUS feature ids below hold and report sub-feature bitmaps of some features
	 * mentioned in top level feature id bitmap
	 */
	CAPEXT_BUS_FEATURE_PKTLAT	= (CAPEXT_BUS_FEATURE_ID_BASE + 2),
	CAPEXT_BUS_FEATURE_MAX
};

/* BUS features bit positions in top level rte feature id. Features mentioned below are reported */
enum capext_bus_feature_bitpos {
	CAPEXT_BUS_FEATURE_BITPOS_HP2P		= 0,
	CAPEXT_BUS_FEATURE_BITPOS_PTM		= 1,
	CAPEXT_BUS_FEATURE_BITPOS_PKTLAT	= 2,
	CAPEXT_BUS_FEATURE_BITPOS_BUSTPUT	= 3,	/* feature with sub-features */
	CAPEXT_BUS_FEATURE_BITPOS_MAX
};

/* Packet latency sub-feature bit positions. These sub-features need to be reported */
enum capext_pktlat_subfeature_bitpos {
	CAPEXT_PKTLAT_BITPOS_META	= 0,
	CAPEXT_PKTLAT_BITPOS_IPC	= 1,
	CAPEXT_PKTLAT_BITPOS_MAX
};

/* RTE partition */
/* The features listed in the enumeration below have subfeatures.
 * If a new feature is added and that feature has sub-features that need to be reported,
 * add that feature here
 */
#define CAPEXT_RTE_FEATURE_ID_BASE		(1024u)
enum capext_rte_feature_ids {
	CAPEXT_RTE_FEATURE_RSVD		= (CAPEXT_RTE_FEATURE_ID_BASE + 0),
	/* RTE top level feature id to hold and report bitmaps of features with and
	 * without sub-features.
	 */
	CAPEXT_RTE_FEATURE_RTE_FEATURES	= (CAPEXT_RTE_FEATURE_ID_BASE + 1),
	/* RTE feature ids below hold and report sub-feature bitmaps of some features
	 * mentioned in top level feature id bitmap
	 */
	CAPEXT_RTE_FEATURE_ECOUNTERS	= (CAPEXT_RTE_FEATURE_ID_BASE + 2),
	CAPEXT_RTE_FEATURE_MAX
};

/* Ecounters sub-feature bit positions. These sub-features need to be reported */
enum capext_ecounters_subfeature_bitpos {
	CAPEXT_ECOUNTERS_BITPOS_TXHIST		= 0,
	CAPEXT_ECOUNTERS_BITPOS_ADV		= 1,
	CAPEXT_ECOUNTERS_BITPOS_PHY		= 2,
	CAPEXT_ECOUNTERS_BITPOS_PHY_CAL		= 3,
	CAPEXT_ECOUNTERS_BITPOS_CHSTATS		= 4,
	CAPEXT_ECOUNTERS_BITPOS_PEERSTATS	= 5,
	CAPEXT_ECOUNTERS_BITPOS_DTIM_MISS	= 6,
	CAPEXT_ECOUNTERS_BITPOS_SOFTAPSTATS	= 7,
	CAPEXT_ECOUNTERS_BITPOS_MAX
};

/* RTE features bit positions in top level rte feature id. Features mentioned below are reported */
enum capext_rte_feature_bitpos {
	CAPEXT_RTE_FEATURE_BITPOS_H2D_LOG_TIME_SYNC	= 0,
	CAPEXT_RTE_FEATURE_BITPOS_HWRNG			= 1,
	CAPEXT_RTE_FEATURE_BITPOS_SPMI			= 2,
	CAPEXT_RTE_FEATURE_BITPOS_ECOUNTERS		= 3,	/* feature with sub-features */
	CAPEXT_RTE_FEATURE_BITPOS_EVENT_LOG		= 4,

	CAPEXT_RTE_FEATURE_BITPOS_LOGTRACE		= 5,
	CAPEXT_RTE_FEATURE_BITPOS_HCHK			= 6,
	CAPEXT_RTE_FEATURE_BITPOS_SMD			= 7,
	CAPEXT_RTE_FEATURE_BITPOS_ETD			= 8,
	CAPEXT_RTE_FEATURE_BITPOS_CST                   = 9,
	CAPEXT_RTE_FEATURE_BITPOS_MAX
};

#if defined(WL_EDS)
#define WL_EDS_VERSION_V1	1u
/* WL_EDS commands TLVIDs */
enum wl_eds_iov_xtlv {
	WL_EDS_IOV_XTLV_RSVD			= 0u,

	/* Per filter command */
	WL_EDS_IOV_XTLV_FILTER_ADD		= 1u,
	WL_EDS_IOV_XTLV_FILTER_GET		= 2u,
	WL_EDS_IOV_XTLV_FILTER_DELETE		= 3u,
	WL_EDS_IOV_XTLV_FILTER_ABORT		= 4u,

	/* Per radio commands */
	WL_EDS_IOV_XTLV_START			= 20u,

	/* Output XTLV */
	/* Contains a pair of WL_EDS_IOV_XTLV_FILTER + WL_EDS_IOV_XTLV_FILTER_STATUS */
	/* iovar "wl eds cap" */
	WL_EDS_IOV_XTLV_CAPABILITY		= 30u,
	/* iovar "wl eds get" */
	/* wrapper around multiple radio filter status */
	WL_EDS_IOV_XTLV_RADIO_STATUS_COLLECTION	= 31u,
	/* wrapper around filters for a radio */
	WL_EDS_IOV_XTLV_FILTER_STATUS_CONTAINER = 32u,
	/* the filter specification */
	WL_EDS_IOV_XTLV_FILTER			= 33u,
	/* filter status */
	WL_EDS_IOV_XTLV_FILTER_STATUS		= 34u,
	WL_EDS_IOV_XTLV_MAX
};
typedef uint16 wl_eds_iov_xtlv_t;

enum wl_eds_capability {
	WL_EDS_CAP_BITPOS_FILTER_FRAMETYPESUBTYPE	= 0u,
	WL_EDS_CAP_BITPOS_FILTER_PKT_FILTER		= 1u,
	WL_EDS_CAP_BITPOS_MAX
};
typedef uint16 wl_eds_capablity_t;
typedef struct wl_eds_iov_capability_v1 {
	uint8			version;
	uint8			PAD;
	wl_eds_capablity_t	capability[WL_EDS_RADIO_ENC_MAX];
	uint8			max_filters[WL_EDS_RADIO_ENC_MAX];
	uint8			PAD;
} wl_eds_iov_capability_v1_t;

#define WL_EDS_FILTER_ID_MAX		7u
#define WL_EDS_FILTER_FTYPE_WILDCARD    0xFFu
typedef struct wl_eds_filter_v1 {
	struct ether_addr		a1;		/* NULL address indicates wildcard */
	struct ether_addr		a2;		/* same as above. */
	struct ether_addr		a3;		/* same as above. */
	uint16				PAD;
	uint16				num_rpts;	/* max. num of reports (stop condition) */
	wl_eds_frame_type_subtype_t	ftype;		/* frame type/subtyupe */
	uint8				filter_id;	/* specified by host */
} wl_eds_filter_v1_t;

/* Input payload of WL_EDS_IOV_XTLV_FILTER_ADD.
 */
typedef struct wl_eds_filter_add_v1 {
	wl_eds_radio_enc_t	radio_enc;
	uint8			PAD[3];
	wl_eds_filter_v1_t	new_filter;
} wl_eds_iov_filter_add_v1_t;

/* Input payload of WL_EDS_IOV_XTLV_FILTER_GET/DELETE/ABORT.  */
typedef struct wl_eds_iov_filter_select_v1 {
	wl_eds_radio_enc_t	radio_enc;
	uint8			filter_id;
	uint8			PAD[2];
} wl_eds_iov_filter_select_v1_t;

/* Input payload of WL_EDS_IOV_XTLV_START */
enum wl_eds_iov_start_radio_bmap {
	/* When the stealth mode is specified, start command's chanspec will be ignored and
	 * the filters will be programed to the respective slices and no additional
	 * operation will be performed to take the slices explicilty to certain channel.
	 * The filtering will be happening as per the existing system's activities.
	 */
	WL_EDS_IOV_START_STEALTH_MODE			= 0u,
	WL_EDS_IOV_START_RADIO_TRUE_SLICE		= (1u << 0),
	WL_EDS_IOV_START_RADIO_LISTEN_SLICE		= (1u << 1),
	WL_EDS_IOV_START_RADIO_MAX			= 0xFFu
};
typedef uint8 wl_eds_iov_start_radio_bmp_t;
typedef struct wl_eds_iov_start_v1 {
	wl_eds_iov_start_radio_bmp_t	start_radio_bmap;
	uint8				PAD;
	chanspec_t			chanspec;	/* single channel */
	/* global_timeout in ms. A 0 value means indefinite timeout in stealth mode only.
	 * Otherwise, an invalid value.
	 */
	uint32				global_timeout;
} wl_eds_iov_start_v1_t;

/* Can be set in filter_id filed in wl_eds_filter_select_v1_t
 * When WL_EDS_IOV_FILTER_SELECT_RADIO_ENC_ALL is used, it is assumed
 * to be WL_EDS_IOV_FILTER_SELECT_ID_ALL in the filter_id field.
 */
#define WL_EDS_IOV_FILTER_SELECT_RADIO_ENC_ALL		0xFFu
#define WL_EDS_IOV_FILTER_SELECT_ID_ALL			0xFFu

/* Output payload xtlv WL_EDS_IOV_XTLV_FILTER_STATUS */
typedef wl_eds_filter_status_v1_t wl_eds_iov_filter_status_v1_t;
typedef wl_eds_filter_v1_t wl_eds_iov_filter_v1_t;
#endif /* WL_EDS */

/* ====== platform configuration (platcfg) interface definitions ===== */
/* subcommand IDs */
enum wl_platcfg_cmd_id {
	WL_PLATCFG_CMD_VER		= 0u,
	WL_PLATCFG_CMD_DLOAD		= 1u,
	WL_PLATCFG_CMD_DLOAD_STATUS	= 2u,
	WL_PLATCFG_CMD_BLOB_VER		= 3u,
	WL_PLATCFG_CMD_DUMP_DATA	= 4u,
	WL_PLATCFG_CMD_BLOB_VER_V2	= 5u,
	WL_PLATCFG_CMD_DUMP_DATA_V2	= 6u,
	WL_PLATCFG_CMD_LAST
};

/* xtlvs */
enum wl_platcfg_tlv_id {
	WL_PLATCFG_XTLV_SEG_TITLE	= 0u,
	WL_PLATCFG_XTLV_SEG_CREATION	= 1u,
	WL_PLATCFG_XTLV_DUMP_DATA	= 2u,
	WL_PLATCFG_XTLV_SEG_METADATA	= 3u,

	/* blob data dump tlvs */
	WL_PLATCFG_XTLV_DUMP_ANTGAIN    = 40u,
	WL_PLATCFG_XTLV_LAST
};

/* WL_PLATCFG_CMD_VER */
typedef struct wl_platcfg_ver {
	uint16	ver;		/* WL_PLATCFG_CMD_VER */
	uint8	pad[2];
} wl_platcfg_ver_t;

/* tracks current module command version */
#define WL_PLATCFG_CMD_VER_V2	2u
#define WL_PLATCFG_CMD_VER_V1	1u

/* WL_PLATCFG_CMD_DLOAD */
typedef struct wl_platcfg_dload {
	uint16	flags;		/* DL flags (DL_BEGIN, DL_END) */
	uint16	data_len;	/* blob data len */
	uint8	data[];		/* blob data */
} wl_platcfg_dload_t;

/* WL_PLATCFG_CMD_DLOAD_STATUS */
typedef struct wl_platcfg_seg_status {
	uint8	type;		/* segment type */
	uint8	pad[3];
	uint32	status;		/* status */
} wl_platcfg_seg_status_t;

typedef struct wl_platcfg_dload_status {
	uint8	count;				/* 1 per segment type */
	uint8	pad[3];
	uint32	blob_status;			/* blob downlod status */
	wl_platcfg_seg_status_t status[];	/* list of segment status */
} wl_platcfg_dload_status_t;

/* segment type */
enum {
	WL_PLATCFG_SEGTYPE_NONE		= 0u,
	WL_PLATCFG_SEGTYPE_CHIPID	= 1u,
	WL_PLATCFG_SEGTYPE_ANTGAIN	= 2u,
	WL_PLATCFG_SEGTYPE_LAST
};

/* WL_PLATCFG_CMD_BLOB_VER_V2 */
typedef struct wl_platcfg_blobver_v2 {
	uint16	flags;		/* flags */
	uint8	n_metadata;	/* number of segment metadata tlvs */
	uint8	pad;
	uint8	metadata[];	/* segment metadata tlvs */
} wl_platcfg_blobver_v2_t;

typedef struct wl_platcfg_seg_metadata_v2 {
	uint8	type;		/* platcfg segment type */
	uint8	format;		/* segment bin format version */
	uint8	n_tlv;		/* number of metadata tlvs */
	uint8	pad;
	uint8	tlv[];		/* metadata tlvs */
} wl_platcfg_seg_metadata_v2_t;

/* WL_PLATCFG_CMD_BLOB_VER */
typedef struct wl_platcfg_seg_ver {
	uint8	type;		/* platcfg segment type */
	uint8	num_tlvs;	/* number of tlvs */
	uint8	pad[2];
	uint8	tlvs[];		/* bcm_xtlv_t */
} wl_platcfg_seg_ver_t;

typedef struct wl_platcfg_blob_ver {
	uint16	flags;		/* flags */
	uint8	count;		/* 1 per segment type */
	uint8	pad;
	uint8	ver[];		/* list of wl_platcfg_seg_ver_t */
} wl_platcfg_blob_ver_t;

/* WL_PLATCFG_CMD_DUMP_DATA_V2 */
typedef struct wl_platcfg_dump_data_v2 {
	uint16	flags;		/* flags */
	uint8	n_seg;		/* number of segment dumps */
	uint8	pad;
	uint8	seg[];		/* segment dumps in bcm_xtlv_t */
} wl_platcfg_dump_v2_t;

typedef struct wl_antgain6g_datalist_v2 {
	uint8	n_antg;		/* number of antenna gain sets */
	uint8	pad[3];
	uint8	antg[];		/* list of wl_antgain6g_data_t */
} wl_antgain6g_datalist_v2_t;

#define WL_ANTG_NUM_ANTS	2u
typedef struct wl_antgain6g_data_v2 {
	uint16  start_freq;	/* in MHz */
	uint16  end_freq;	/* in MHz */
	int16   corr_gain;	/* directional gain of correlated signals used by FW,
				 * scaled to 100x to avoid float
				 */
	int16	uncorr_gain;	/* same definition as corr_gain, but for
				 * non-correlated signals
				 */
	int16	ag_val[WL_ANTG_NUM_ANTS]; /* individual antenna gains */
} wl_antgain6g_data_v2_t;

/* WL_PLATCFG_XTLV_DUMP_DATA */
typedef struct wl_platcfg_dump_data {
	uint8	type;		/* segment type */
	uint8	pad;
	uint16	data_len;	/* length of data field */
	uint8	data[];		/* segment type specific data */
} wl_platcfg_dump_data_t;

/* dump data for antenna gain segment type (WL_PLATCFG_SEGTYPE_ANTGAIN) */
typedef struct wl_antgain6g {
	uint16	start_freq;	/* in MHz */
	uint16	end_freq;	/* in MHz */
	int16	corr_gain;	/* directional gain of correlated signals used by FW,
				 * scaled to 100x to avoid float
				 */
	int16	uncorr_gain;	/* same definition as corr_gain, but for
				 * non-correlated signals
				 */
} wl_antgain6g_t;

typedef struct wl_antgain6g_list {
	uint8	num_sets;		/* number of antenna gain sets */
	uint8	pad[3];
	wl_antgain6g_t antgain[];	/* list of antenna gain values */
} wl_antgain6g_list_t;

/* Debug Crash types */
#define WL_DBG_CRSH_TYPE_RD_RANDOM			0x00
#define WL_DBG_CRSH_TYPE_RD_INV_CORE			0x01
#define WL_DBG_CRSH_TYPE_WR_INV_CORE			0x02
#define WL_DBG_CRSH_TYPE_RD_INV_WRAP			0x03
#define WL_DBG_CRSH_TYPE_WR_INV_WRAP			0x04
#define WL_DBG_CRSH_TYPE_RD_RES_CORE			0x05
#define WL_DBG_CRSH_TYPE_WR_RES_CORE			0x06
#define WL_DBG_CRSH_TYPE_RD_RES_WRAP			0x07
#define WL_DBG_CRSH_TYPE_WR_RES_WRAP			0x08
#define WL_DBG_CRSH_TYPE_RD_CORE_NO_CLK			0x09
#define WL_DBG_CRSH_TYPE_WR_CORE_NO_CLK			0x0A
#define WL_DBG_CRSH_TYPE_RD_CORE_NO_PWR			0x0B
#define WL_DBG_CRSH_TYPE_WR_CORE_NO_PWR			0x0C
#define WL_DBG_CRSH_TYPE_PCIe_AER			0x0D
#define WL_DBG_CRSH_TYPE_POWERCYCLE			0x0E
#define WL_DBG_CRSH_TYPE_TRAP				0x0E
#define WL_DBG_CRSH_TYPE_HANG				0x0F
#define WL_DBG_CRSH_TYPE_PHYTXERR			0x10

#define WL_DBG_CRSH_TYPE_PHYREAD			0x11
#define WL_DBG_CRSH_TYPE_PHYWRITE			0x12
#define WL_DBG_CRSH_TYPE_INV_PHYREAD			0x13
#define WL_DBG_CRSH_TYPE_INV_PHYWRITE			0x14
#define WL_DBG_CRSH_TYPE_DUMP_STATE			0x15


/* Radio/PHY health check crash scenarios - reserved 0x16 to 0x30 */
#define WL_DBG_CRSH_TYPE_RADIO_HEALTHCHECK_START	0x16
#define WL_DBG_CRSH_TYPE_DESENSE_LIMITS			0x17
#define WL_DBG_CRSH_TYPE_BASEINDEX_LIMITS		0x18
#define WL_DBG_CRSH_TYPE_TXCHAIN_INVALID		0x19
#define WL_DBG_CRSH_TYPE_CRITICAL_MALLOC_FAIL		0x1a
#define WL_DBG_CRSH_TYPE_TEMPSENSE_LIMITS		0x20
#define WL_DBG_CRSH_TYPE_TXPOWER_LIMITS			0x21
#define WL_DBG_CRSH_TYPE_VCOCAL_FAILED			0x22
#define WL_DBG_CRSH_TYPE_PLL_NOTLOCKED			0x23
#define WL_DBG_CRSH_TYPE_RADIO_HEALTHCHECK_LAST		0x30
#define WL_DBG_CRSH_TYPE_BTCOEX_RFACTIVE		0x31
#define WL_DBG_CRSH_TYPE_BTCOEX_TXCONF_DELAY		0x32
#define WL_DBG_CRSH_TYPE_BTCOEX_ANT_DELAY		0x33
#define WL_DBG_CRSH_TYPE_BTCOEX_INVLD_TASKID		0x34
#define WL_DBG_CRSH_TYPE_STACK_OVERRUN			0x35
#define WL_DBG_CRSH_TYPE_MPU_HCHK			0x36
#define WL_DBG_CRSH_TYPE_STACK_CORRUPTION		0x38
/* To maintain uniformity with other branches, leaving 0x37 & 0x38 unused */
#define WL_DBG_CRSH_TYPE_ARM_DBG_REG_ACCESS		0x39
#define WL_DBG_CRSH_TYPE_WLREGON			0x3a

#define WL_DBG_CRSH_TYPE_SR_DCCAL0			0x40
#define WL_DBG_CRSH_TYPE_SR_DCCAL1			0x41
#define WL_DBG_CRSH_TYPE_SR_RXIQCAL0			0x50
#define WL_DBG_CRSH_TYPE_SR_RXIQCAL1			0x51
#define WL_DBG_CRSH_TYPE_SR_TEMP0			0x60
#define WL_DBG_CRSH_TYPE_SR_TEMP1			0x61
#define WL_DBG_CRSH_TYPE_SR_FULLCAL0			0x70
#define WL_DBG_CRSH_TYPE_SR_FULLCAL1			0x71
#define WL_DBG_CRSH_TYPE_SR_MPCAL0			0x80
#define WL_DBG_CRSH_TYPE_SR_MPCAL1			0x81
#define WL_DBG_CRSH_TYPE_SR_SEM				0x90
#define WL_DBG_CRSH_TYPE_SR_AXI				0x91
#define WL_DBG_CRSH_TYPE_LAST				0x92

#endif /* _wlioctl_h_ */
