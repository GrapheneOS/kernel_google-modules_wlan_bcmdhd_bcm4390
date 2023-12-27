/*
 * Linux DHD Bus Module for PCIE
 *
 * Copyright (C) 2023, Broadcom.
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


#ifndef dhd_pcie_h
#define dhd_pcie_h

#include <bcmpcie.h>
#include <hnd_cons.h>

#ifdef DHD_PCIE_RUNTIMEPM
#include <linux/mutex.h>
#include <linux/wait.h>
#endif /* DHD_PCIE_RUNTIMEPM */
#include <fwpkg_utils.h>

#include <dngl_rtlv.h>
#include <ewp.h>

/* defines */
#define PCIE_SHARED_VERSION		PCIE_SHARED_VERSION_9

#define PCMSGBUF_HDRLEN 0
#define DONGLE_REG_MAP_SIZE (32 * 1024)
#define DONGLE_TCM_MAP_SIZE (4096 * 1024)
#define DONGLE_BAR2_MAP_SIZE (1024 * 1024)
#define DONGLE_MIN_MEMSIZE (128 *1024)
#ifdef DHD_DEBUG
#define DHD_PCIE_SUCCESS 0
#define DHD_PCIE_FAILURE 1
#endif /* DHD_DEBUG */
#define	REMAP_ENAB(bus)			((bus)->remap)
#define	REMAP_ISADDR(bus, a)		(((a) >= ((bus)->orig_ramsize)) && ((a) < ((bus)->ramsize)))

#define MAX_DHD_TX_FLOWS	320

/* user defined data structures */
/* Device console log buffer state */
#define CONSOLE_LINE_MAX	192u
#define CONSOLE_BUFFER_MAX	(64 * 1024)

#ifdef IDLE_TX_FLOW_MGMT
#define IDLE_FLOW_LIST_TIMEOUT 5000
#define IDLE_FLOW_RING_TIMEOUT 5000
#endif /* IDLE_TX_FLOW_MGMT */

#ifdef DEVICE_TX_STUCK_DETECT
#define DEVICE_TX_STUCK_CKECK_TIMEOUT	1000 /* 1 sec */
#define DEVICE_TX_STUCK_TIMEOUT		10000 /* 10 secs */
#define DEVICE_TX_STUCK_WARN_DURATION (DEVICE_TX_STUCK_TIMEOUT / DEVICE_TX_STUCK_CKECK_TIMEOUT)
#define DEVICE_TX_STUCK_DURATION (DEVICE_TX_STUCK_WARN_DURATION * 2)
#endif /* DEVICE_TX_STUCK_DETECT */

/* implicit DMA for h2d wr and d2h rd indice from Host memory to TCM */
#define IDMA_ENAB(dhd)		((dhd) && (dhd)->idma_enable)
#define IDMA_ACTIVE(dhd)	((dhd) && ((dhd)->idma_enable) && ((dhd)->idma_inited))

#define IDMA_CAPABLE(bus)	(((bus)->sih->buscorerev == 19) || ((bus)->sih->buscorerev >= 23))

/* IFRM (Implicit Flow Ring Manager enable and inited */
#define IFRM_ENAB(dhd)		((dhd) && (dhd)->ifrm_enable)
#define IFRM_ACTIVE(dhd)	((dhd) && ((dhd)->ifrm_enable) && ((dhd)->ifrm_inited))

/* DAR registers use for h2d doorbell */
#define DAR_ENAB(dhd)		((dhd) && (dhd)->dar_enable)
#define DAR_ACTIVE(dhd)		((dhd) && ((dhd)->dar_enable) && ((dhd)->dar_inited))

/* DAR WAR for revs < 64 */
#define DAR_PWRREQ(bus)		(((bus)->_dar_war) && DAR_ACTIVE((bus)->dhd))

/* PCIE CTO Prevention and Recovery */
#define PCIECTO_ENAB(bus)	((bus)->cto_enable)

/* Implicit DMA index usage :
 * Index 0 for h2d write index transfer
 * Index 1 for d2h read index transfer
 */
#define IDMA_IDX0 0
#define IDMA_IDX1 1
#define IDMA_IDX2 2
#define IDMA_IDX3 3
#define DMA_TYPE_SHIFT	4
#define DMA_TYPE_IDMA	1

#define DHDPCIE_CONFIG_HDR_SIZE 16
#define DHDPCIE_CONFIG_CHECK_DELAY_MS 10 /* 10ms */
#define DHDPCIE_CONFIG_CHECK_RETRY_COUNT 20
#define DHDPCIE_DONGLE_PWR_TOGGLE_DELAY 1000 /* 1ms in units of us */
#define DHDPCIE_PM_D3_DELAY 200000 /* 200ms in units of us */
#define DHDPCIE_PM_D2_DELAY 200 /* 200us */

#ifdef TX_CSO
#define TXCSO_ENAB(dhd)		((dhd)->dongle_txcso_enabled)
#define TXCSO_ACTIVE(dhd)	(((dhd)->dongle_txcso_enabled) && ((dhd)->host_txcso_enabled) && \
				((dhd)->dongle_txpost_ext_enabled))
#endif /* TX_CSO */

typedef struct dhd_console {
	 uint		count;	/* Poll interval msec counter */
	 uint		log_addr;		 /* Log struct address (fixed) */
	 hnd_log_t	 log;			 /* Log struct (host copy) */
	 uint		 bufsize;		 /* Size of log buffer */
	 uint8		 *buf;			 /* Log buffer (host copy) */
	 uint		 last;			 /* Last buffer read index */
} dhd_console_t;

typedef struct ring_sh_info {
	uint32 ring_mem_addr;
	uint32 ring_state_w;
	uint32 ring_state_r;
} ring_sh_info_t;

#define MAX_DS_TRACE_SIZE	50

#ifdef DHD_MMIO_TRACE
#define MAX_MMIO_TRACE_SIZE    256
/* Minimum of 250us should be elapsed to add new entry */
#define MIN_MMIO_TRACE_TIME 250
#define DHD_RING_IDX 0x00FF0000


typedef struct _dhd_mmio_trace_t {
	uint64  timestamp;
	uint32	addr;
	uint32	value;
	bool	set;
} dhd_mmio_trace_t;
#endif /* defined(DHD_MMIO_TRACE) */

#define DHD_MAX_CONTEXT_LEN 256

typedef struct _dhd_ds_trace_t {
	uint64  timestamp;
	bool	d2h;
	uint32	dsval;
#ifdef PCIE_INB_DW
	enum dhd_bus_ds_state inbstate;
#endif /* PCIE_INB_DW */
	char context[DHD_MAX_CONTEXT_LEN];
} dhd_ds_trace_t;

#define DEVICE_WAKE_NONE	0
#define DEVICE_WAKE_OOB		1
#define DEVICE_WAKE_INB		2

#define INBAND_DW_ENAB(bus)		((bus)->dw_option == DEVICE_WAKE_INB)
#define OOB_DW_ENAB(bus)		((bus)->dw_option == DEVICE_WAKE_OOB)
#define NO_DW_ENAB(bus)			((bus)->dw_option == DEVICE_WAKE_NONE)

#define PCIE_PWR_REQ_RELOAD_WAR_ENAB(buscorerev) \
	((buscorerev == 66) || (buscorerev == 67) || (buscorerev == 68) || \
	(buscorerev == 70) || (buscorerev == 72))

#define PCIE_FASTLPO_ENABLED(buscorerev) \
	((buscorerev == 66) || (buscorerev == 67) || (buscorerev == 68) || \
	(buscorerev == 70) || (buscorerev == 72) || (buscorerev == 76) || (buscorerev == 77))

/*
 * HW JIRA - CRWLPCIEGEN2-672
 * Producer Index Feature which is used by F1 gets reset on F0 FLR
 * fixed in REV68
 */
#define PCIE_ENUM_RESET_WAR_ENAB(buscorerev) \
	((buscorerev == 66) || (buscorerev == 67))

struct dhd_bus;

struct dhd_pcie_rev {
	uint8	fw_rev;
	void (*handle_mb_data)(struct dhd_bus *);
};

typedef struct dhdpcie_config_save
{
	uint32 header[DHDPCIE_CONFIG_HDR_SIZE];
	/* pmcsr save */
	uint32 pmcsr;
	/* express save */
	uint32 exp_dev_ctrl_stat;
	uint32 exp_link_ctrl_stat;
	uint32 exp_dev_ctrl_stat2;
	uint32 exp_link_ctrl_stat2;
	/* msi save */
	uint32 msi_cap;
	uint32 msi_addr0;
	uint32 msi_addr1;
	uint32 msi_data;
	/* l1pm save */
	uint32 l1pm0;
	uint32 l1pm1;
	/* ltr save */
	uint32 ltr;
	/* aer save */
	uint32 aer_caps_ctrl; /* 0x18 */
	uint32 aer_severity;  /* 0x0C */
	uint32 aer_umask;     /* 0x08 */
	uint32 aer_cmask;     /* 0x14 */
	uint32 aer_root_cmd;  /* 0x2c */
	/* BAR0 and BAR1 windows */
	uint32 bar0_win;	/* 0x80 */
	uint32 bar1_win;	/* 0x84 */
	/* BAR0 wrapper base */
	uint32 bar0_win2;	/* 0x70 */
	/* Secondoary BAR0 */
	uint32 bar0_core2_win;	/* 0x74 */
	/* Secondoary BAR0 wrapper base */
	uint32 bar0_core2_win2;	/* 0x78 */
} dhdpcie_config_save_t;

/* The level of bus communication with the dongle */
enum dhd_bus_low_power_state {
	DHD_BUS_NO_LOW_POWER_STATE,	/* Not in low power state */
	DHD_BUS_D3_INFORM_SENT,		/* D3 INFORM sent */
	DHD_BUS_D3_ACK_RECEIVED,	/* D3 ACK received */
};

#ifdef DHD_FLOW_RING_STATUS_TRACE
#define FRS_TRACE_SIZE 32 /* frs - flow_ring_status */
typedef struct _dhd_flow_ring_status_trace_t {
	uint64  timestamp;
	uint16 h2d_ctrl_post_drd;
	uint16 h2d_ctrl_post_dwr;
	uint16 d2h_ctrl_cpln_drd;
	uint16 d2h_ctrl_cpln_dwr;
	uint16 h2d_rx_post_drd;
	uint16 h2d_rx_post_dwr;
	uint16 d2h_rx_cpln_drd;
	uint16 d2h_rx_cpln_dwr;
	uint16 d2h_tx_cpln_drd;
	uint16 d2h_tx_cpln_dwr;
	uint16 h2d_info_post_drd;
	uint16 h2d_info_post_dwr;
	uint16 d2h_info_cpln_drd;
	uint16 d2h_info_cpln_dwr;
	uint16 d2h_ring_edl_drd;
	uint16 d2h_ring_edl_dwr;
} dhd_frs_trace_t;
#endif /* DHD_FLOW_RING_STATUS_TRACE */

typedef enum dhd_pcie_link_state {
	DHD_PCIE_ALL_GOOD = 0,
	DHD_PCIE_LINK_DOWN = 1,
	DHD_PCIE_COMMON_BP_DOWN = 2,
	DHD_PCIE_WLAN_BP_DOWN = 3,
	DHD_PCIE_LINK_RESET = 4
} dhd_pcie_link_state_type_t;

/* PCIe bus memory mapped regions for device memory accees */
typedef enum dhd_pcie_mem_region {
	DHD_PCIE_MEM_BAR1 = 1,
	DHD_PCIE_MEM_BAR2 = 2
} dhd_pcie_mem_region_t;

#ifdef DHD_AGGR_WI
#define DHD_AGGR_TXPOST_MAX	2u
#define DHD_AGGR_RXPOST_MAX	2u
#define DHD_AGGR_TXPOST_ENAB(bus) (bus->d2h_aggr_wi_enab & \
	bus->dhd_aggr_wi_enab & PCIE_AGGR_WI_TXPOST)
#define DHD_AGGR_RXPOST_ENAB(bus) (bus->d2h_aggr_wi_enab & \
	bus->dhd_aggr_wi_enab & PCIE_AGGR_WI_RXPOST)
#define DHD_AGGR_TXCPL_ENAB(bus)  (bus->d2h_aggr_wi_enab & \
	bus->dhd_aggr_wi_enab & PCIE_AGGR_WI_TXCPL)
#define DHD_AGGR_RXCPL_ENAB(bus)  (bus->d2h_aggr_wi_enab & \
	bus->dhd_aggr_wi_enab & PCIE_AGGR_WI_RXCPL)
#define DHD_PROT_PROCESS_MSGBUF_RXCPL_AGGR_WI(rxcpl_aggr_cnt, msg_aggr) { \
	for (j = 0; (j < rxcpl_aggr_cnt) && (pending_pkt > 0); j++) { \
		pkt = dhd_prot_get_msgbuf_rxcpl_pkt(dhd, &msg_aggr->item[j]); \
		if (!pkt) \
			break; \
		pkt_cnt++; \
		pending_pkt--; \
		if (!pktqhead) { \
			pktqhead = prevpkt = pkt; \
		} else { \
			PKTSETNEXT(dhd->osh, prevpkt, pkt); \
			prevpkt = pkt; \
		} \
		if (RXCSO_ENAB(dhd)) { \
			dhd_rx_set_csum_status(dhd, pkt, msg_aggr->item[j].flags); \
		} \
	} \
	msg_len -= item_len; \
	msg_addr += item_len; \
	}
typedef enum aggr_state {
	PCIE_AGGR_WI = 0,
	PCIE_AGGR_WI_EXT = 1
} aggr_state_t;
#endif /* DHD_AGGR_WI */

/* Max length of filename in IOVAR or in module parameter */
#define DHD_MAX_PATH	2048u

/* PTM related internal defines */
#define PTM_VALIDATE_TS_RX		0x00000001u
#define PTM_VALIDATE_TS_TX		0x00010000u
#define PTM_BAD_RXTS_CNT_MASK		0x0000FF00u
#define PTM_BAD_RXTS_CNT_SHF		8u
#define PTM_BAD_TXTS_CNT_MASK		0xFF000000u
#define PTM_BAD_TXTS_CNT_SHF		24u

#define PTM_CLKINVALID_RX_TRAP_TH	0u
#define PTM_CLKINVALID_TX_TRAP_TH	0u


/** Instantiated once for each hardware (dongle) instance that this DHD manages */
typedef struct dhd_bus {
	dhd_pub_t	*dhd;	/**< pointer to per hardware (dongle) unique instance */
#if !defined(NDIS)
	struct pci_dev  *rc_dev;	/* pci RC device handle */
	struct pci_dev  *dev;		/* pci device handle */
#ifdef BCMINTERNAL
	uint32 aspm_enab_during_suspend;	/* aspm enab flag during suspend */
	uint32 l1ss_enab_during_suspend;	/* l1ss enab flag during suspend */
#endif /* BCMINTERNAL */
#endif /* !defined(NDIS) */
#ifdef DHD_EFI
	void *pcie_dev;
#endif
	dll_t		flowring_active_list; /* constructed list of tx flowring queues */
#ifdef IDLE_TX_FLOW_MGMT
	uint64		active_list_last_process_ts;
						/* stores the timestamp of active list processing */
#endif /* IDLE_TX_FLOW_MGMT */

#ifdef DEVICE_TX_STUCK_DETECT
	/* Flag to enable/disable device tx stuck monitor by DHD IOVAR dev_tx_stuck_monitor */
	uint32 dev_tx_stuck_monitor;
	/* Stores the timestamp (msec) of the last device Tx stuck check */
	uint32	device_tx_stuck_check;
#endif /* DEVICE_TX_STUCK_DETECT */

	si_t		*sih;			/* Handle for SI calls */
	char		*vars;			/* Variables (from CIS and/or other) */
	uint		varsz;			/* Size of variables buffer */
	uint32		sbaddr;			/* Current SB window pointer (-1, invalid) */
	sbpcieregs_t	*reg;			/* Registers for PCIE core */

	uint		armrev;			/* CPU core revision */
	uint		coreid;			/* CPU core id */
	uint		ramrev;			/* SOCRAM core revision */
	uint32		ramsize;		/* Size of RAM in SOCRAM (bytes) */
	uint32		orig_ramsize;		/* Size of RAM in SOCRAM (bytes) */
	uint32		srmemsize;		/* Size of SRMEM */

	uint32		bus;			/* gSPI or SDIO bus */
	uint32		intstatus;		/* Intstatus bits (events) pending */
	bool		dpc_sched;		/* Indicates DPC schedule (intrpt rcvd) */
	bool		fcstate;		/* State of dongle flow-control */

	uint16		cl_devid;		/* cached devid for dhdsdio_probe_attach() */
	char		*fw_path;		/* module_param: path to firmware image */
	char		*nv_path;		/* module_param: path to nvram vars file */
#ifdef CACHE_FW_IMAGES
	int			processed_nvram_params_len;	/* Modified len of NVRAM info */
#endif

#ifdef BCM_ROUTER_DHD
	char		*nvram_params;		/* user specified nvram params. */
	int			nvram_params_len;
#endif /* BCM_ROUTER_DHD */

	struct pktq	txq;			/* Queue length used for flow-control */

	bool		intr;			/* Use interrupts */
	bool		ipend;			/* Device interrupt is pending */
	bool		intdis;			/* Interrupts disabled by isr */
	uint		intrcount;		/* Count of device interrupt callbacks */
	uint		lastintrs;		/* Count as of last watchdog timer */

	dhd_console_t	console;		/* Console output polling support */
	uint		console_addr;		/* Console address from shared struct */

	bool		alp_only;		/* Don't use HT clock (ALP only) */

	bool		remap;		/* Contiguous 1MB RAM: 512K socram + 512K devram
					 * Available with socram rev 16
					 * Remap region not DMA-able
					 */
	uint32		resetinstr;
	uint32		dongle_ram_base;
#ifdef COEX_CPU
	uint32		coex_itcm_base;
	uint32		coex_dtcm_base;
	uint32		coex_itcm_size;
	uint32		coex_dtcm_size;
#endif /* COEX_CPU */
	ulong		shared_addr;
	pciedev_shared_t	*pcie_sh;
	uint32		dma_rxoffset;
	volatile char	*regs;		/* pci device memory pointed by BAR0 va */
	volatile char	*tcm;		/* pci device memory pointed by BAR1 va */
	volatile char	*bar2;		/* pci device memory pointed by BAR2 va */
	uint32		bar1_size;	/* pci device memory size mapped by BAR1 */
	uint32		bar2_size;	/* pci device memory size mapped by BAR2 */
	uint32		curr_bar1_win;	/* current PCIEBar1Window setting */
	uint32		curr_bar2_win;	/* current PCIEBar2Window setting */
	osl_t		*osh;
	uint32		nvram_csm;	/* Nvram checksum */


	uint16		pollrate;
	uint16  polltick;

	volatile uint32  *pcie_mb_intr_addr;
	volatile uint32  *pcie_mb_intr_2_addr;
	void    *pcie_mb_intr_osh;
	bool	sleep_allowed;

	wake_counts_t	wake_counts;

	/* version 3 shared struct related info start */
	ring_sh_info_t	ring_sh[BCMPCIE_COMMON_MSGRINGS + MAX_DHD_TX_FLOWS];

	uint8	h2d_ring_count;
	uint8	d2h_ring_count;
	uint32  ringmem_ptr;
	uint32  ring_state_ptr;

	uint32 d2h_dma_scratch_buffer_mem_addr;

	uint32 h2d_mb_data_ptr_addr;
	uint32 d2h_mb_data_ptr_addr;
	/* version 3 shared struct related info end */

	uint32 def_intmask;
	uint32 d2h_mb_mask;
	uint32 pcie_mailbox_mask;
	uint32 pcie_mailbox_int;
	bool	ltrsleep_on_unload;
	uint	wait_for_d3_ack;
	uint16	max_tx_flowrings;
	uint16	max_submission_rings;
	uint16	max_completion_rings;
	uint16	last_dynamic_ringid;
	uint16	max_cmn_rings;
	uint32	rw_index_sz;
	bool	db1_for_mb;

	dhd_timeout_t doorbell_timer;
	bool	device_wake_state;
#ifdef PCIE_OOB
	bool	oob_enabled;
#endif /* PCIE_OOB */
	bool	irq_registered;
	bool	d2h_intr_method;
	bool	d2h_intr_control;
#ifdef SUPPORT_LINKDOWN_RECOVERY
	uint8 no_cfg_restore;
	bool read_shm_fail;
#endif /* SUPPORT_LINKDOWN_RECOVERY */
	int32 idletime;                 /* Control for activity timeout */
	bool rpm_enabled;
#ifdef DHD_PCIE_RUNTIMEPM
	int32 idlecount;                /* Activity timeout counter */
	int32 bus_wake;                 /* For wake up the bus */
	bool runtime_resume_done;       /* For check runtime suspend end */
	struct mutex pm_lock;            /* Synchronize for system PM & runtime PM */
	wait_queue_head_t rpm_queue;    /* wait-queue for bus wake up */
#endif /* DHD_PCIE_RUNTIMEPM */
	uint32 d3_inform_cnt;
	uint32 d0_inform_cnt;
	uint32 d0_inform_in_use_cnt;
#ifdef DHD_TREAT_D3ACKTO_AS_LINKDWN
	uint32 d3ackto_as_linkdwn_cnt;
	uint32 iovarto_as_linkdwn_cnt;
#endif
	uint8 force_suspend;
	uint8 is_linkdown;
	uint8 no_bus_init;
	bool enumeration_fail;
#ifdef IDLE_TX_FLOW_MGMT
	bool enable_idle_flowring_mgmt;
#endif /* IDLE_TX_FLOW_MGMT */
	struct	dhd_pcie_rev api;
	bool use_mailbox;
	bool    use_d0_inform;
	void	*bus_lp_state_lock;
	void	*pwr_req_lock;
	bool	dongle_in_deepsleep;
	void	*dongle_ds_lock;
	bool	bar1_switch_enab;
	void	*bar1_switch_lock;
	bool    *bar2_switch_lock;
	void *backplane_access_lock;
	enum dhd_bus_low_power_state bus_low_power_state;
#ifdef DHD_FLOW_RING_STATUS_TRACE
	dhd_frs_trace_t frs_isr_trace[FRS_TRACE_SIZE]; /* frs - flow_ring_status */
	dhd_frs_trace_t frs_dpc_trace[FRS_TRACE_SIZE]; /* frs - flow_ring_status */
	uint32	frs_isr_count;
	uint32	frs_dpc_count;
#endif /* DHD_FLOW_RING_STATUS_TRACE */
#ifdef DHD_MMIO_TRACE
	dhd_mmio_trace_t   mmio_trace[MAX_MMIO_TRACE_SIZE];
	uint32	mmio_trace_count;
#endif /* defined(DHD_MMIO_TRACE) */
	dhd_ds_trace_t   ds_trace[MAX_DS_TRACE_SIZE];
	uint32	ds_trace_count;

#ifdef DHD_SSSR_DUMP
	bool sssr_in_progress;	/* flag to indicate sssr ops in progress */
#endif /* DHD_SSSR_DUMP */

	uint32  hostready_count; /* Number of hostready issued */
#if defined(PCIE_OOB) || defined (BCMPCIE_OOB_HOST_WAKE)
	bool	oob_presuspend;
#endif /* PCIE_OOB || BCMPCIE_OOB_HOST_WAKE */
	dhdpcie_config_save_t saved_config;
	ulong host_irq_enable_count;
	ulong host_irq_disable_count;
	ulong dngl_intmask_disable_count;
	ulong dngl_intmask_enable_count;
	ulong dpc_return_busdown_count;
	ulong non_ours_irq_count;
#ifdef BCMPCIE_OOB_HOST_WAKE
	ulong oob_intr_count;
	ulong oob_intr_enable_count;
	ulong oob_intr_disable_count;
	uint64 last_oob_irq_isr_time;
	uint64 last_oob_irq_thr_time;
	uint64 last_oob_irq_enable_time;
	uint64 last_oob_irq_disable_time;
#endif /* BCMPCIE_OOB_HOST_WAKE */
	uint64 isr_entry_time;
	uint64 isr_exit_time;
	uint64 isr_sched_dpc_time;
	uint64 rpm_sched_dpc_time;
	uint64 dpc_entry_time;
	uint64 dpc_exit_time;
	uint64 resched_dpc_time;
	uint64 last_d3_inform_time;
	uint64 last_d3_ack_time;
	uint64 last_process_ctrlbuf_time;
	uint64 last_process_flowring_time;
	uint64 last_process_txcpl_time;
	uint64 last_process_rxcpl_time;
	uint64 last_process_infocpl_time;
	uint64 last_process_edl_time;
	uint64 last_suspend_start_time;
	uint64 last_suspend_end_time;
	uint64 last_resume_start_time;
	uint64 last_resume_end_time;
	uint64 last_non_ours_irq_time;
	uint64 dpc_time_usec;
	uint64  *dpc_time_histo;
	uint64 ctrl_cpl_post_time_usec;
	uint64  *ctrl_cpl_post_time_histo;
	uint64 tx_post_time_usec;
	uint64  *tx_post_time_histo;
	uint64 tx_cpl_time_usec;
	uint64  *tx_cpl_time_histo;
	uint64 rx_cpl_post_time_usec;
	uint64  *rx_cpl_post_time_histo;
	bool  idma_enabled;
	bool  ifrm_enabled;
	bool  dar_enabled;
	uint32 dmaxfer_complete;
	uint8	dw_option;
#ifdef PCIE_INB_DW
	bool	inb_enabled;
	uint32	ds_exit_timeout;
	uint32	host_sleep_exit_timeout;
	uint	wait_for_ds_exit;
	uint32	inband_dw_assert_cnt; /* # of inband device_wake assert */
	uint32	inband_dw_deassert_cnt; /* # of inband device_wake deassert */
	uint32	inband_ds_exit_host_cnt; /* # of DS-EXIT , host initiated */
	uint32	inband_ds_exit_device_cnt; /* # of DS-EXIT , device initiated */
	uint32	inband_ds_exit_to_cnt; /* # of DS-EXIT timeout */
	uint32	inband_host_sleep_exit_to_cnt; /* # of Host_Sleep exit timeout */
	void	*inb_lock;	/* Lock to serialize in band device wake activity */
	/* # of contexts in the host which currently want a FW transaction */
	uint32  host_active_cnt;
	bool	skip_ds_ack; /* Skip DS-ACK during suspend in progress */
#endif /* PCIE_INB_DW */
#if defined(PCIE_OOB) || defined(PCIE_INB_DW)
	bool  ds_enabled;
#endif
#ifdef OEM_ANDROID
	bool chk_pm;	/* To avoid counting of wake up from Runtime PM */
#endif /* OEM_ANDROID */
#if defined(PCIE_INB_DW)
	bool calc_ds_exit_latency;
	bool deep_sleep; /* Indicates deep_sleep set or unset by the DHD IOVAR deep_sleep */
	uint64 ds_exit_latency;
	uint64 ds_exit_ts1;
	uint64 ds_exit_ts2;
#endif /* PCIE_INB_DW */
	bool _dar_war;
#ifdef GDB_PROXY
	/* True if firmware loaded and backplane accessible */
	bool gdb_proxy_access_enabled;
	/* ID set by last "gdb_proxy_probe" iovar */
	uint32 gdb_proxy_last_id;
	/* True if firmware was started in bootloader mode */
	bool gdb_proxy_bootloader_mode;
	/* Counter incremented at each generated memory dump */
	uint32 gdb_proxy_mem_dump_count;
#endif /* GDB_PROXY */
	uint8  dma_chan;

	bool    cto_enable;     /* enable PCIE CTO Prevention and recovery */
	uint32  cto_threshold;  /* PCIE CTO timeout threshold */
	bool	cto_triggered;	/* CTO is triggered */
	/* enable PCIE CTO recovery when PCIE CTO Prevention is enabled */
	bool    cto_recovery_enable;
	bool	init_done;		/* ready to receive interrupts from dongle */
	int	pwr_req_ref;
	bool flr_force_fail;	/* user intends to simulate flr force fail */

	/* Information used to compose the memory map and to write the memory map,
	 * FW, and FW signature to dongle RAM.
	 * This information is used by the bootloader.
	 */
	uint32 ramtop_addr;		/* Dongle address of unused space at top of RAM */
	uint32 fw_download_addr;	/* Dongle address of FW download */
	uint32 fw_download_len;		/* Length in bytes of FW download */
	uint32 fwsig_download_addr;	/* Dongle address of FW signature download */
	uint32 fwsig_download_len;	/* Length in bytes of FW signature download */
	uint32 fwstat_download_addr;	/* Dongle address of FWS status download */
	uint32 fwstat_download_len;	/* Length in bytes of FWS status download */
	uint32 fw_memmap_download_addr;	/* Dongle address of FWS memory-info download */
	uint32 fw_memmap_download_len;	/* Length in bytes of FWS memory-info download */

	char fwsig_filename[DHD_MAX_PATH];		/* Name of FW signature file */
	char bootloader_filename[DHD_FILENAME_MAX];	/* Name of bootloader image file */
	uint32 bootloader_addr;		/* Dongle address of bootloader download */
	bool force_bt_quiesce; /* send bt_quiesce command to BT driver. */
	bool rc_ep_aspm_cap; /* RC and EP ASPM capable */
	bool rc_ep_l1ss_cap; /* EC and EP L1SS capable */
#if defined(DHD_H2D_LOG_TIME_SYNC)
	ulong dhd_rte_time_sync_count; /* OSL_SYSUPTIME_US() */
#endif /* DHD_H2D_LOG_TIME_SYNC */
#ifdef D2H_MINIDUMP
	bool d2h_minidump; /* This flag will be set if Host and FW handshake to collect minidump */
	bool d2h_minidump_override; /* Force disable minidump through dhd IOVAR */
#endif /* D2H_MINIDUMP */
#ifdef BCMSLTGT
	int xtalfreq;			/* Xtal frequency used for htclkratio calculation */
	uint32 ilp_tick;		/* ILP ticks per second read from pmutimer */
	uint32 alp_to_ilp_ratio;	/* ALP ticks per ILP ticks read from pmu_xtalfreq */
	uint32 xtal_to_alp_ratio;	/* xtal to ALP ratio which can change from chip to chip */
#endif /* BCMSLTGT */
#ifdef BT_OVER_PCIE
	/* whether the chip is in BT over PCIE mode or not */
	bool btop_mode;
#endif /* BT_OVER_PCIE */
	uint16 hp2p_txcpl_max_items;
	uint16 hp2p_rxcpl_max_items;
	/* PCIE coherent status */
	uint32 coherent_state;
	uint64 arm_oor_time;
	uint64 rd_shared_pass_time;
	uint32 inb_dw_deassert_cnt;
	uint16 mesh_rxcpl_max_items;
	uint32 fw_boot_intr; /* interrupt from FW upon boot complete */
	dhd_pcie_link_state_type_t link_state;

	uint32 rot_dpc_sched_count;

	bool lpm_mode;	/* lpm enabled */
	bool lpm_keep_in_reset; /* during LPM keep in FLR, if FLR force is enabled */
	bool lpm_mem_kill; /* kill WLAN memories in LPM */
	bool lpm_force_flr; /* Force F0 FLR on WLAN  when in LPM */
	ewp_info_t ewp_info;
#ifdef EWP_DACS
	ewp_hw_info_t ewp_hw_info;
#endif /* EWP_DACS */
	etb_config_info_t *etb_config_info;
	uint etb_config_size;
	bool etb_validity[ETB_USER_MAX];
#ifdef DHD_AGGR_WI
	/* Aggregation bits : 0 = TXPOST | 1 = RXPOST | 2 = TXCPL | 3 = RXCPL */
	uint8 dhd_aggr_wi_enab; /* By default set to DHD_AGGR_WI_EN, override via sysfs/iovar */
	uint8 d2h_aggr_wi_enab;  /* FW capability */
#endif /* DHD_AGGR_WI */
	bool dar_err_set;
	uint32 ptm_ctrl_reg;
	bool ptm_cfg_enabled; /* status of PTM enable in pcie config space */
	bool ptm_sync_periodic_h2d ; /* PTM sync periodic advertised to Dongle */
	fwpkg_info_t fwpkg;	/* combined fw package info structure */
	/* the max no. of unprocessed work items in a flowring seen till now */
	uint32 *flowring_high_watermark;
	/* current no. of unprocessed work items present in a flowring - snapshot
	 * taken everytime new work items are posted to the flowring, so value
	 * keeps changing frequently
	 */
	uint32 *flowring_cur_items;
	uint32 lpbk_xfer_data_pattern_type; /*  data Pattern type DMA lpbk */
	bool ltr_active_set_during_init;
	uint32 etb_config_addr;

	/* PTM validating related fields */

	/* PTM txs counter/status */
	ts_timestamp_t last_tx_ptm_ts;
	uint32 txs_clkid_bad_ts; /* timestamp low = 0, high = 0 */
	uint32 txs_clkid_invalid_clkid; /* clock ID is 0xF */
	uint32 ptm_tx_ts_good_adopted_pkt_cnt; /* PTM clock, good pkts */
	uint32 ptm_tx_ts_good_not_adopted_pkt_cnt; /* PTM clock, good pkts, ts in past */
	uint32 ptm_tx_ts_not_adopted_pkt_cnt; /* PTM clock pkts with bad TS */
	uint32 txs_fail_clkid_bad_ts; /* failed tx, clock ID not invalid */
	uint32 txs_fail_clkid_inv; /* failed tx, clock ID invalid */
	uint32 ptm_bad_txts_cont_cnt; /* count of successive pkts with bad txts */
	uint32 ptm_bad_txts_cont_cnt_max; /* count of successive pkts with bad txts */
	uint32 tot_txcpl_last; /* total rxcpl value at counter reset */

	/* PTM rxs counter/status */
	ts_timestamp_t last_rx_ptm_ts;
	uint32 rxs_clkid_bad_ts;
	uint32 rxs_clkid_invalid_clkid;
	uint32 ptm_rx_ts_good_adopted_pkt_cnt;
	uint32 ptm_rx_ts_good_not_adopted_pkt_cnt;
	uint32 ptm_rx_ts_not_adopted_pkt_cnt;
	uint32 ptm_bad_rxts_cont_cnt; /* count of successive pkts with bad txts */
	uint32 ptm_bad_rxts_cont_cnt_max; /* max count of successive pkts with bad txts */
	uint32 tot_rxcpl_last; /* total rxcpl value at counter reset */

	/* ptm ts validation */
	uint32	ptm_ts_validate;
	bool ptm_rxts_validate;
	bool ptm_txts_validate;
	uint32 ptm_bad_rxts_trap_th;
	uint32 ptm_bad_txts_trap_th;
	bool ptm_host_ready_adopt_rx; /* rx: some systems host PTM gets reset on host sleep/wake */
	bool ptm_host_ready_adopt_tx; /* tx: some systems host PTM gets reset on host sleep/wake */

} dhd_bus_t;

#ifdef DHD_PCIE_WRAPPER_DUMP
typedef struct pcie_wrapper {
	char *core;
	uint32 base;
} pcie_wrapper_t;

typedef struct pcie_wrapper_offset {
	uint32 offset;
	uint32 len;
} pcie_wrapper_offset_t;
#endif /* DHD_PCIE_WRAPPER_DUMP */

#define LPBK_DMA_XFER_DTPTRN_DEFAULT	0
#define LPBK_DMA_XFER_DTPTRN_0x00	1
#define LPBK_DMA_XFER_DTPTRN_0xFF	2
#define LPBK_DMA_XFER_DTPTRN_0x55	3
#define LPBK_DMA_XFER_DTPTRN_0xAA	4

#define LPM_MODE_LPM_ALL	0x1
#define LPM_MODE_NO_MEMKILL	0x010
#define LPM_MODE_NO_FLR		0x100

#ifdef DHD_MSI_SUPPORT
extern uint enable_msi;
#endif /* DHD_MSI_SUPPORT */

enum {
	PCIE_INTX = 0,
	PCIE_MSI = 1
};

enum {
	PCIE_D2H_INTMASK_CTRL = 0,
	PCIE_HOST_IRQ_CTRL = 1
};

static INLINE bool
__dhd_check_bus_in_lps(dhd_bus_t *bus)
{
	bool ret = (bus->bus_low_power_state == DHD_BUS_D3_INFORM_SENT) ||
		(bus->bus_low_power_state == DHD_BUS_D3_ACK_RECEIVED);
	return ret;
}

static INLINE bool
dhd_check_bus_in_lps(dhd_bus_t *bus)
{
	unsigned long flags_bus;
	bool ret;
	DHD_BUS_LP_STATE_LOCK(bus->bus_lp_state_lock, flags_bus);
	ret = __dhd_check_bus_in_lps(bus);
	DHD_BUS_LP_STATE_UNLOCK(bus->bus_lp_state_lock, flags_bus);
	return ret;
}

static INLINE bool
__dhd_check_bus_lps_d3_acked(dhd_bus_t *bus)
{
	bool ret = (bus->bus_low_power_state == DHD_BUS_D3_ACK_RECEIVED);
	return ret;
}

static INLINE bool
dhd_check_bus_lps_d3_acked(dhd_bus_t *bus)
{
	unsigned long flags_bus;
	bool ret;
	DHD_BUS_LP_STATE_LOCK(bus->bus_lp_state_lock, flags_bus);
	ret = __dhd_check_bus_lps_d3_acked(bus);
	DHD_BUS_LP_STATE_UNLOCK(bus->bus_lp_state_lock, flags_bus);
	return ret;
}

static INLINE void
__dhd_set_bus_not_in_lps(dhd_bus_t *bus)
{
	bus->bus_low_power_state = DHD_BUS_NO_LOW_POWER_STATE;
	return;
}

static INLINE void
dhd_set_bus_not_in_lps(dhd_bus_t *bus)
{
	unsigned long flags_bus;
	DHD_BUS_LP_STATE_LOCK(bus->bus_lp_state_lock, flags_bus);
	__dhd_set_bus_not_in_lps(bus);
	DHD_BUS_LP_STATE_UNLOCK(bus->bus_lp_state_lock, flags_bus);
	return;
}

static INLINE void
__dhd_set_bus_lps_d3_informed(dhd_bus_t *bus)
{
	bus->bus_low_power_state = DHD_BUS_D3_INFORM_SENT;
	return;
}

static INLINE void
dhd_set_bus_lps_d3_informed(dhd_bus_t *bus)
{
	unsigned long flags_bus;
	DHD_BUS_LP_STATE_LOCK(bus->bus_lp_state_lock, flags_bus);
	__dhd_set_bus_lps_d3_informed(bus);
	DHD_BUS_LP_STATE_UNLOCK(bus->bus_lp_state_lock, flags_bus);
	return;
}

static INLINE void
__dhd_set_bus_lps_d3_acked(dhd_bus_t *bus)
{
	bus->bus_low_power_state = DHD_BUS_D3_ACK_RECEIVED;
	return;
}

static INLINE void
dhd_set_bus_lps_d3_acked(dhd_bus_t *bus)
{
	unsigned long flags_bus;
	DHD_BUS_LP_STATE_LOCK(bus->bus_lp_state_lock, flags_bus);
	__dhd_set_bus_lps_d3_acked(bus);
	DHD_BUS_LP_STATE_UNLOCK(bus->bus_lp_state_lock, flags_bus);
	return;
}

/* check routines */
#define DHD_CHK_BUS_IN_LPS(bus)			dhd_check_bus_in_lps(bus)
#define __DHD_CHK_BUS_IN_LPS(bus)		__dhd_check_bus_in_lps(bus)

#define DHD_CHK_BUS_NOT_IN_LPS(bus)		!(DHD_CHK_BUS_IN_LPS(bus))
#define __DHD_CHK_BUS_NOT_IN_LPS(bus)		!(__DHD_CHK_BUS_IN_LPS(bus))

#define DHD_CHK_BUS_LPS_D3_INFORMED(bus)	DHD_CHK_BUS_IN_LPS(bus)
#define __DHD_CHK_BUS_LPS_D3_INFORMED(bus)	__DHD_CHK_BUS_IN_LPS(bus)

#define DHD_CHK_BUS_LPS_D3_ACKED(bus)		dhd_check_bus_lps_d3_acked(bus)
#define __DHD_CHK_BUS_LPS_D3_ACKED(bus)		__dhd_check_bus_lps_d3_acked(bus)

/* set routines */
#define DHD_SET_BUS_NOT_IN_LPS(bus)		dhd_set_bus_not_in_lps(bus)
#define __DHD_SET_BUS_NOT_IN_LPS(bus)		__dhd_set_bus_not_in_lps(bus)

#define DHD_SET_BUS_LPS_D3_INFORMED(bus)	dhd_set_bus_lps_d3_informed(bus)
#define __DHD_SET_BUS_LPS_D3_INFORMED(bus)	__dhd_set_bus_lps_d3_informed(bus)

#define DHD_SET_BUS_LPS_D3_ACKED(bus)		dhd_set_bus_lps_d3_acked(bus)
#define __DHD_SET_BUS_LPS_D3_ACKED(bus)		__dhd_set_bus_lps_d3_acked(bus)

/* function declarations */

extern uint32* dhdpcie_bus_reg_map(osl_t *osh, ulong addr, int size);
extern int dhdpcie_bus_register(void);
extern void dhdpcie_bus_unregister(void);
extern bool dhdpcie_chipmatch(uint16 vendor, uint16 device);

extern int dhdpcie_bus_attach(osl_t *osh, dhd_bus_t **bus_ptr,
	volatile char *regs, volatile char *tcm, volatile char *bar2, void *pci_dev);
extern uint32 dhdpcie_bus_cfg_read_dword(struct dhd_bus *bus, uint32 addr, uint32 size);
extern void dhdpcie_bus_cfg_write_dword(struct dhd_bus *bus, uint32 addr, uint32 size, uint32 data);
extern void dhdpcie_bus_intr_enable(struct dhd_bus *bus);
extern void dhdpcie_bus_intr_disable(struct dhd_bus *bus);
extern int dhpcie_bus_mask_interrupt(dhd_bus_t *bus);
extern void dhdpcie_bus_release(struct dhd_bus *bus);
extern int32 dhdpcie_bus_isr(struct dhd_bus *bus);
extern void dhdpcie_free_irq(dhd_bus_t *bus);
extern void dhdpcie_bus_ringbell_fast(struct dhd_bus *bus, uint32 value);
extern void dhdpcie_bus_ringbell_2_fast(struct dhd_bus *bus, uint32 value, bool devwake);
extern void dhdpcie_dongle_reset(dhd_bus_t *bus);
extern int dhd_bus_cfg_sprom_ctrl_bp_reset(struct dhd_bus *bus);
extern int dhd_bus_cfg_ss_ctrl_bp_reset(struct dhd_bus *bus);
#ifdef DHD_PCIE_NATIVE_RUNTIMEPM
extern int dhdpcie_bus_suspend(struct  dhd_bus *bus, bool state, bool byint);
#else
extern int dhdpcie_bus_suspend(struct  dhd_bus *bus, bool state);
#endif /* DHD_PCIE_NATIVE_RUNTIMEPM */
extern int dhdpcie_pci_suspend_resume(struct  dhd_bus *bus, bool state);
extern uint32 dhdpcie_force_alp(struct dhd_bus *bus, bool enable);
extern uint32 dhdpcie_set_l1_entry_time(struct dhd_bus *bus, int force_l1_entry_time);
extern bool dhdpcie_tcm_valid(dhd_bus_t *bus);
extern void dhdpcie_pme_active(osl_t *osh, bool enable);
extern bool dhdpcie_pme_cap(osl_t *osh);
extern uint32 dhdpcie_lcreg(osl_t *osh, uint32 mask, uint32 val);
extern uint8 dhdpcie_clkreq(osl_t *osh, uint32 mask, uint32 val);
extern int dhdpcie_disable_irq(dhd_bus_t *bus);
extern int dhdpcie_disable_irq_nosync(dhd_bus_t *bus);
extern int dhdpcie_enable_irq(dhd_bus_t *bus);

extern void dhd_bus_dump_dar_registers(struct dhd_bus *bus);
extern void dhd_bus_dump_imp_cfg_registers(struct dhd_bus *bus);

#if defined(__linux__)
extern uint32 dhdpcie_rc_config_read(dhd_bus_t *bus, uint offset);
extern uint32 dhdpcie_rc_access_cap(dhd_bus_t *bus, int cap, uint offset, bool is_ext,
		bool is_write, uint32 writeval);
extern uint32 dhdpcie_ep_access_cap(dhd_bus_t *bus, int cap, uint offset, bool is_ext,
		bool is_write, uint32 writeval);
extern uint32 dhd_debug_get_rc_linkcap(dhd_bus_t *bus);
extern void dhdpcie_enable_irq_loop(dhd_bus_t *bus);
#else
static INLINE uint32 dhdpcie_rc_config_read(dhd_bus_t *bus, uint offset) { return 0;}
static INLINE uint32 dhdpcie_rc_access_cap(dhd_bus_t *bus, int cap, uint offset, bool is_ext,
		bool is_write, uint32 writeval) { return -1;}
static INLINE uint32 dhdpcie_ep_access_cap(dhd_bus_t *bus, int cap, uint offset, bool is_ext,
		bool is_write, uint32 writeval) { return -1;}
static INLINE uint32 dhd_debug_get_rc_linkcap(dhd_bus_t *bus) { return -1;}
static INLINE void dhdpcie_enable_irq_loop(dhd_bus_t *bus) { return; }
#endif /* LINUX || linux */
#if defined(__linux__)
extern int dhdpcie_start_host_dev(dhd_bus_t *bus);
extern int dhdpcie_stop_host_dev(dhd_bus_t *bus);
extern int dhdpcie_disable_device(dhd_bus_t *bus);
extern int dhdpcie_alloc_resource(dhd_bus_t *bus);
extern void dhdpcie_free_resource(dhd_bus_t *bus);
extern void dhdpcie_dump_resource(dhd_bus_t *bus);
extern int dhdpcie_bus_request_irq(struct dhd_bus *bus);
void dhdpcie_os_setbar1win(dhd_bus_t *bus, uint32 addr);
void dhdpcie_os_wtcm8(dhd_bus_t *bus, ulong offset, uint8 data);
uint8 dhdpcie_os_rtcm8(dhd_bus_t *bus, ulong offset);
void dhdpcie_os_wtcm16(dhd_bus_t *bus, ulong offset, uint16 data);
uint16 dhdpcie_os_rtcm16(dhd_bus_t *bus, ulong offset);
void dhdpcie_os_wtcm32(dhd_bus_t *bus, ulong offset, uint32 data);
uint32 dhdpcie_os_rtcm32(dhd_bus_t *bus, ulong offset);
#ifdef DHD_SUPPORT_64BIT
void dhdpcie_os_wtcm64(dhd_bus_t *bus, ulong offset, uint64 data);
uint64 dhdpcie_os_rtcm64(dhd_bus_t *bus, ulong offset);
#endif
extern void dhd_dpc_kill(dhd_pub_t *dhdp);
#endif /* __linux__ */

#if defined(__linux__) || defined(DHD_EFI)
extern int dhdpcie_enable_device(dhd_bus_t *bus);
#endif

#ifdef BCMPCIE_OOB_HOST_WAKE
extern int dhdpcie_oob_intr_register(dhd_bus_t *bus);
extern void dhdpcie_oob_intr_unregister(dhd_bus_t *bus);
extern void dhdpcie_oob_intr_set(dhd_bus_t *bus, bool enable);
extern int dhdpcie_get_oob_irq_num(struct dhd_bus *bus);
extern int dhdpcie_get_oob_irq_status(struct dhd_bus *bus);
extern int dhdpcie_get_oob_irq_level(void);
#ifdef PRINT_WAKEUP_GPIO_STATUS
extern int dhdpcie_get_oob_gpio_number(void);
#endif /* PRINT_WAKEUP_GPIO_STATUS */
#endif /* BCMPCIE_OOB_HOST_WAKE */
#ifdef PCIE_OOB
extern void dhd_oob_set_bt_reg_on(struct dhd_bus *bus, bool val);
extern int dhd_oob_get_bt_reg_on(struct dhd_bus *bus);
extern void dhdpcie_oob_init(dhd_bus_t *bus);
extern int dhd_os_oob_set_device_wake(struct dhd_bus *bus, bool val);
extern void dhd_os_ib_set_device_wake(struct dhd_bus *bus, bool val);
#endif /* PCIE_OOB */
#if defined(PCIE_OOB) || defined(PCIE_INB_DW)
extern void dhd_bus_doorbell_timeout_reset(struct dhd_bus *bus);
#endif /* defined(PCIE_OOB) || defined(PCIE_INB_DW) */

#define DHD_REGULAR_RING    0
#define DHD_HP2P_RING    1
#define DHD_MESH_RING    2

#ifdef DHD_SET_PCIE_DMA_MASK_FOR_GS101
/* This is only for GS101 platform. Others is done on RC side */
#define DHD_PCIE_DMA_MASK_FOR_GS101 36
#endif /* DHD_SET_PCIE_DMA_MASK_FOR_GS101 */

#ifdef CONFIG_ARCH_TEGRA
extern int tegra_pcie_pm_suspend(void);
extern int tegra_pcie_pm_resume(void);
#endif /* CONFIG_ARCH_TEGRA */

extern int dhd_buzzz_dump_dngl(dhd_bus_t *bus);
extern int dhdpcie_send_mb_data(dhd_bus_t *bus, uint32 h2d_mb_data, const char *context);

#ifdef DHD_WAKE_STATUS
int bcmpcie_get_total_wake(struct dhd_bus *bus);
int bcmpcie_set_get_wake(struct dhd_bus *bus, int flag);
int bcmpcie_get_wake(struct dhd_bus *bus);
int bcmpcie_set_get_wake_pkt_dump(struct dhd_bus *bus, int wake_pkt_dump);
#endif /* DHD_WAKE_STATUS */
#ifdef DHD_MMIO_TRACE
extern void dhd_dump_bus_mmio_trace(dhd_bus_t *bus, struct bcmstrbuf *strbuf);
#endif /* defined(DHD_MMIO_TRACE) */
extern void dhd_dump_bus_ds_trace(dhd_bus_t *bus, struct bcmstrbuf *strbuf);
void dhd_dump_ds_trace_console(dhd_pub_t *dhdp);
extern bool dhdpcie_bus_get_pcie_hostready_supported(dhd_bus_t *bus);
extern void dhd_bus_hostready(struct  dhd_bus *bus);
#ifdef PCIE_OOB
extern bool dhdpcie_bus_get_pcie_oob_dw_supported(dhd_bus_t *bus);
#endif /* PCIE_OOB */
#ifdef PCIE_INB_DW
extern bool dhdpcie_bus_get_pcie_inband_dw_supported(dhd_bus_t *bus);
extern void dhdpcie_bus_set_pcie_inband_dw_state(dhd_bus_t *bus,
	enum dhd_bus_ds_state state);
extern enum dhd_bus_ds_state dhdpcie_bus_get_pcie_inband_dw_state(dhd_bus_t *bus);
extern const char * dhd_convert_inb_state_names(enum dhd_bus_ds_state inbstate);
extern const char * dhd_convert_dsval(uint32 val, bool d2h);
extern int dhd_bus_inb_set_device_wake(struct dhd_bus *bus, bool val, const char *context);
extern void dhd_bus_inb_ack_pending_ds_req(dhd_bus_t *bus, const char *context);
#endif /* PCIE_INB_DW */
extern void dhdpcie_bus_enab_pcie_dw(dhd_bus_t *bus, uint8 dw_option);
#if defined(__linux__)
extern int dhdpcie_irq_disabled(struct dhd_bus *bus);
extern int dhdpcie_set_master_and_d0_pwrstate(struct dhd_bus *bus);
#else
static INLINE bool dhdpcie_irq_disabled(struct dhd_bus *bus) { return BCME_ERROR;}
static INLINE int dhdpcie_set_master_and_d0_pwrstate(struct dhd_bus *bus)
{ return BCME_ERROR;}
#endif /* defined(__linux__) */

#ifdef DHD_EFI
extern bool dhdpcie_is_arm_halted(struct dhd_bus *bus);
extern int dhd_os_wifi_platform_set_power(uint32 value);
extern void dhdpcie_dongle_pwr_toggle(dhd_bus_t *bus);
void dhdpcie_dongle_flr_or_pwr_toggle(dhd_bus_t *bus);
int dhd_control_signal(dhd_bus_t *bus, char *arg, int len, int set);
extern int dhd_wifi_properties(struct dhd_bus *bus, char *arg, int len);
extern int dhd_otp_dump(dhd_bus_t *bus, char *arg, int len);
extern int dhdpcie_deinit_phase1(dhd_bus_t *bus);
int dhdpcie_disable_intr_poll(dhd_bus_t *bus);
int dhdpcie_enable_intr_poll(dhd_bus_t *bus);
#ifdef BT_OVER_PCIE
int dhd_btop_test(dhd_bus_t *bus, char *arg, int len);
#endif /* BT_OVER_PCIE */
#else
static INLINE bool dhdpcie_is_arm_halted(struct dhd_bus *bus) {return TRUE;}
static INLINE int dhd_os_wifi_platform_set_power(uint32 value) {return BCME_OK; }
static INLINE void
dhdpcie_dongle_flr_or_pwr_toggle(dhd_bus_t *bus)
{ return; }
#endif /* DHD_EFI */

int dhdpcie_config_check(dhd_bus_t *bus);
int dhdpcie_config_restore(dhd_bus_t *bus, bool restore_pmcsr);
int dhdpcie_config_save(dhd_bus_t *bus);
int dhdpcie_set_pwr_state(dhd_bus_t *bus, uint state);

extern bool dhdpcie_bus_get_pcie_idma_supported(dhd_bus_t *bus);
extern bool dhdpcie_bus_get_pcie_ifrm_supported(dhd_bus_t *bus);
extern bool dhdpcie_bus_get_pcie_dar_supported(dhd_bus_t *bus);
extern bool dhdpcie_bus_get_hp2p_supported(dhd_bus_t *bus);

static INLINE uint32
dhd_pcie_config_read(dhd_bus_t *bus, uint offset, uint size)
{
	/* For 4375 or prior chips to 4375 */
	if (bus->sih && bus->sih->buscorerev <= 64) {
		OSL_DELAY(100);
	}
	return OSL_PCI_READ_CONFIG(bus->osh, offset, size);
}

static INLINE void
dhd_pcie_config_write(dhd_bus_t *bus, uint offset, uint size, uint data)
{
	/* For 4375 or prior chips to 4375 */
	if (bus->sih && bus->sih->buscorerev <= 64) {
		OSL_DELAY(100);
	}
	OSL_PCI_WRITE_CONFIG(bus->osh, offset, size, data);
}

extern int dhdpcie_get_fwpath_otp(dhd_bus_t *bus, char *fw_path, char *nv_path,
		char *clm_path, char *txcap_path);

extern int dhd_pcie_debug_info_dump(dhd_pub_t *dhd);
extern void dhd_pcie_intr_count_dump(dhd_pub_t *dhd);
extern void dhdpcie_bus_clear_intstatus(dhd_bus_t *bus);
#ifdef DHD_HP2P
extern uint16 dhd_bus_get_hp2p_ring_max_size(dhd_bus_t *bus, bool tx);
#endif
#ifdef DHD_MESH
extern uint16 dhd_bus_get_mesh_ring_max_size(dhd_bus_t *bus, bool tx);
#endif

#if defined(DHD_EFI)
extern wifi_properties_t *dhd_get_props(dhd_bus_t *bus);
#endif

#if defined(DHD_EFI) || defined(NDIS)
extern int dhd_get_platform(dhd_pub_t* dhd, char *progname);
extern bool dhdpcie_is_chip_supported(uint32 chipid, int *idx);
#endif

extern int dhd_get_pcie_linkspeed(dhd_pub_t *dhd);
extern void dhdpcie_bar1_window_switch_enab(dhd_bus_t *bus);

#ifdef PCIE_INB_DW
extern void dhdpcie_set_dongle_deepsleep(dhd_bus_t *bus, bool val);
extern void dhd_init_dongle_ds_lock(dhd_bus_t *bus);
extern void dhd_deinit_dongle_ds_lock(dhd_bus_t *bus);
#endif /* PCIE_INB_DW */
extern void dhdpcie_sbtopcie_translation_config(struct dhd_bus *bus,
	int bp_window, dmaaddr_t addr);
extern int dhdpcie_bus_membytes(dhd_bus_t *bus, bool write, dhd_pcie_mem_region_t region,
	ulong address, uint8 *data, uint size);
extern int dhdpcie_download_rtlv(dhd_bus_t *bus, dngl_rtlv_type_t type,
	dngl_rtlv_len_t len, uint8 *value);
extern void dhd_update_chip_specific_tunables(dhd_pub_t *dhd);
bool dhd_ptm_sync_prerequisite(dhd_pub_t *dhd);
#if defined(DEVICE_TX_STUCK_DETECT) && defined(ASSOC_CHECK_SR)
void dhd_assoc_check_sr(dhd_pub_t *dhd, bool state);
#endif /* DEVICE_TX_STUCK_DETECT && ASSOC_CHECK_SR */

#define DHD_BUS_BAR2_SWITCH_LOCK(bus, flags) \
	DHD_BAR2_SWITCH_LOCK((bus)->bar2_switch_lock, flags)

#define DHD_BUS_BAR2_SWITCH_UNLOCK(bus, flags) \
	DHD_BAR2_SWITCH_UNLOCK((bus)->bar2_switch_lock, flags)

void dhdpcie_setbar2win(dhd_bus_t *bus, uint32 addr);
void dhd_init_bar1_switch_lock(dhd_bus_t *bus);
int dhd_pcie_nci_wrapper_dump(dhd_pub_t *dhd, bool dump_to_dmesg);
int dhd_bus_get_armca7_pc(struct dhd_bus *bus, bool loop_print);
void dhd_bt_dwnld_pwr_req(dhd_bus_t *bus);
void dhd_bt_dwnld_pwr_req_clear(dhd_bus_t *bus);

void dhd_validate_pcie_link_cbp_wlbp(dhd_bus_t *bus);

uint32 dhdpcie_cfg_indirect_bpaccess(struct dhd_bus *bus, uint32 addr, bool read, uint value);
int dhdpcie_get_cbaon_coredumps(struct dhd_bus *bus);
void dhd_dump_intr_counters(dhd_pub_t *dhd, struct bcmstrbuf *strbuf);

/* Host Platform quirk callbacks */
extern void dhdpcie_set_pmu_min_res_mask(void *bus, uint min_res_mask);
extern int dhdpcie_skip_xorcsum_request(void *dhd_bus_p);

void dhd_sbreg_op(dhd_pub_t *dhd, uint addr, uint *val, bool read);
void dhd_sbreg_op_silent(dhd_pub_t *dhd, uint addr, uint *val, bool read);
uint serialized_backplane_access(dhd_bus_t *bus, uint addr, uint size, uint *val, bool read);
dhd_pcie_link_state_type_t dhdpcie_get_link_state(dhd_bus_t *bus);
void dhd_bus_pcie_pwr_req_wl_domain(struct dhd_bus *bus, uint offset, bool enable);
#ifdef DHD_FW_COREDUMP
int dhdpcie_get_mem_dump(dhd_bus_t *bus);
#endif /* DHD_FW_COREDUMP */
void dhd_dump_pcie_slave_wrapper_regs(dhd_bus_t *bus);
void dhd_init_dpc_histos(dhd_pub_t *dhd);
void dhd_deinit_dpc_histos(dhd_pub_t *dhd);
void dhd_dump_dpc_histos(dhd_pub_t *dhd, struct bcmstrbuf *strbuf);
void dhd_clear_dpc_histos(dhd_pub_t *dhd);
void dhdpcie_schedule_log_dump(dhd_bus_t *bus);
#if defined(FW_SIGNATURE)
int dhd_bus_dump_fws(dhd_bus_t *bus, struct bcmstrbuf *strbuf);
#endif /* FW_SIGNATURE */
void dhd_bus_dump_txcpl_info(dhd_pub_t *dhdp, struct bcmstrbuf *strbuf);
void dhd_bus_dump_mdring_info(dhd_pub_t *dhdp, struct bcmstrbuf *strbuf);
void dhd_bus_dump_rxlat_info(dhd_pub_t *dhdp, struct bcmstrbuf *strbuf);
void dhd_bus_dump_rxlat_histo(dhd_pub_t *dhdp, struct bcmstrbuf *strbuf);
#ifdef DNGL_AXI_ERROR_LOGGING
void dhdpcie_dump_axi_error(uint8 *axi_err);
#endif /* DNGL_AXI_ERROR_LOGGING */
#ifdef DHD_MMIO_TRACE
void dhd_bus_mmio_trace(dhd_bus_t *bus, uint32 addr, uint32 value, bool set);
#endif /* DHD_MMIO_TRACE */
#ifdef PCIE_INB_DW
void dhd_bus_ds_trace(dhd_bus_t *bus, uint32 dsval,
	bool d2h, enum dhd_bus_ds_state inbstate, const char *context);
#else
void dhd_bus_ds_trace(dhd_bus_t *bus, uint32 dsval, bool d2h);
#endif /* PCIE_INB_DW */
#ifdef DHD_PCIE_WRAPPER_DUMP
void dhd_pcie_get_wrapper_regs(dhd_pub_t *dhd);
#endif /* DHD_PCIE_WRAPPER_DUMP */
int dhd_pcie_dump_wrapper_regs(dhd_pub_t *dhd);
void dhdpcie_hw_war_regdump(dhd_bus_t *bus);
#if defined(EWP_DACS) || defined(DHD_SDTC_ETB_DUMP)
int dhd_bus_get_etb_dump_cmn(dhd_bus_t *bus, uint8 *buf, uint bufsize,
	uint32 etb_config_info_addr);
#endif /* EWP_DACS || DHD_SDTC_ETB_DUMP */
#ifdef EWP_DACS
int dhdpcie_ewphw_get_initdumps(dhd_bus_t *bus);
#endif /* EWP_DACS */
void dhd_bus_pcie_pwr_req(struct dhd_bus *bus);
void dhd_bus_pcie_pwr_req_clear(struct dhd_bus *bus);

#endif /* dhd_pcie_h */
