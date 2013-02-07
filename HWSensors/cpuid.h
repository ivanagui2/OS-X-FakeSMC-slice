/* *  cpuid.h * */#ifndef M_CPUID_H#define M_CPUID_H#include <machine/machine_routines.h>#include <pexpert/pexpert.h>#include <i386/proc_reg.h>#include <string.h>#define	CPUID_VID_INTEL		"GenuineIntel"#define	CPUID_VID_AMD		"AuthenticAMD"#define CPUID_STRING_UNKNOWN    "Unknown CPU Typ"/* Known MSR registers */ //exclude redefined but remain as comments//#define MSR_IA32_PLATFORM_ID        0x0017	 //#define MSR_CORE_THREAD_COUNT       0x0035	 /* limited use - not for Penryn or older	*///#define MSR_IA32_BIOS_SIGN_ID       0x008B   /* microcode version */#define MSR_FSB_FREQ                0x00CD	 /* limited use - not for i7						*///#define	MSR_PLATFORM_INFO           0x00CE   /* limited use - MinRatio for i7 but Max for Yonah	*//* turbo for penryn */#define MSR_IA32_EXT_CONFIG         0x00EE	 /* limited use - not for i7						*///#define MSR_FLEX_RATIO              0x0194	 /* limited use - not for Penryn or older			*///see no value on most CPUs//#define	MSR_IA32_PERF_STATUS        0x0198#define MSR_IA32_PERF_CONTROL       0x0199//#define MSR_IA32_CLOCK_MODULATION   0x019A#define MSR_THERMAL_STATUS          0x019C//#define MSR_IA32_MISC_ENABLE        0x01A0#define MSR_THERMAL_TARGET          0x01A2	 /* limited use - not for Penryn or older			*/#define MSR_TURBO_RATIO_LIMIT       0x01AD	 /* limited use - not for Penryn or older			*///AMD#define K8_FIDVID_STATUS        0xC0010042#define K10_COFVID_STATUS       0xC0010071#define DEFAULT_FSB             100000          /* for now, hardcoding 100MHz for old CPUs */#define _Bit(n)			(1ULL << n)#define _HBit(n)		(1ULL << ((n)+32))/* * The CPUID_FEATURE_XXX values define 64-bit values * returned in %ecx:%edx to a CPUID request with %eax of 1:  */#define	CPUID_FEATURE_FPU     _Bit(0)	/* Floating point unit on-chip */#define	CPUID_FEATURE_VME     _Bit(1)	/* Virtual Mode Extension */#define	CPUID_FEATURE_DE      _Bit(2)	/* Debugging Extension */#define	CPUID_FEATURE_PSE     _Bit(3)	/* Page Size Extension */#define	CPUID_FEATURE_TSC     _Bit(4)	/* Time Stamp Counter */#define	CPUID_FEATURE_MSR     _Bit(5)	/* Model Specific Registers */#define CPUID_FEATURE_PAE     _Bit(6)	/* Physical Address Extension */#define	CPUID_FEATURE_MCE     _Bit(7)	/* Machine Check Exception */#define	CPUID_FEATURE_CX8     _Bit(8)	/* CMPXCHG8B */#define	CPUID_FEATURE_APIC    _Bit(9)	/* On-chip APIC */#define CPUID_FEATURE_SEP     _Bit(11)	/* Fast System Call */#define	CPUID_FEATURE_MTRR    _Bit(12)	/* Memory Type Range Register */#define	CPUID_FEATURE_PGE     _Bit(13)	/* Page Global Enable */#define	CPUID_FEATURE_MCA     _Bit(14)	/* Machine Check Architecture */#define	CPUID_FEATURE_CMOV    _Bit(15)	/* Conditional Move Instruction */#define CPUID_FEATURE_PAT     _Bit(16)	/* Page Attribute Table */#define CPUID_FEATURE_PSE36   _Bit(17)	/* 36-bit Page Size Extension */#define CPUID_FEATURE_PSN     _Bit(18)	/* Processor Serial Number */#define CPUID_FEATURE_CLFSH   _Bit(19)	/* CLFLUSH Instruction supported */#define CPUID_FEATURE_DS      _Bit(21)	/* Debug Store */#define CPUID_FEATURE_ACPI    _Bit(22)	/* Thermal monitor and Clock Ctrl */#define CPUID_FEATURE_MMX     _Bit(23)	/* MMX supported */#define CPUID_FEATURE_FXSR    _Bit(24)	/* Fast floating pt save/restore */#define CPUID_FEATURE_SSE     _Bit(25)	/* Streaming SIMD extensions */#define CPUID_FEATURE_SSE2    _Bit(26)	/* Streaming SIMD extensions 2 */#define CPUID_FEATURE_SS      _Bit(27)	/* Self-Snoop */#define CPUID_FEATURE_HTT     _Bit(28)	/* Hyper-Threading Technology */#define CPUID_FEATURE_TM      _Bit(29)	/* Thermal Monitor (TM1) */#define CPUID_FEATURE_PBE     _Bit(31)	/* Pend Break Enable */#define CPUID_FEATURE_SSE3    _HBit(0)	/* Streaming SIMD extensions 3 */#define CPUID_FEATURE_PCLMULQDQ _HBit(1) /* PCLMULQDQ Instruction */#define CPUID_FEATURE_MONITOR _HBit(3)	/* Monitor/mwait */#define CPUID_FEATURE_DSCPL   _HBit(4)	/* Debug Store CPL */#define CPUID_FEATURE_VMX     _HBit(5)	/* VMX */#define CPUID_FEATURE_SMX     _HBit(6)	/* SMX */#define CPUID_FEATURE_EST     _HBit(7)	/* Enhanced SpeedsTep (GV3) */#define CPUID_FEATURE_TM2     _HBit(8)	/* Thermal Monitor 2 */#define CPUID_FEATURE_SSSE3   _HBit(9)	/* Supplemental SSE3 instructions */#define CPUID_FEATURE_CID     _HBit(10)	/* L1 Context ID */#define CPUID_FEATURE_CX16    _HBit(13)	/* CmpXchg16b instruction */#define CPUID_FEATURE_xTPR    _HBit(14)	/* Send Task PRiority msgs */#define CPUID_FEATURE_PDCM    _HBit(15)	/* Perf/Debug Capability MSR */#define CPUID_FEATURE_DCA     _HBit(18)	/* Direct Cache Access */#define CPUID_FEATURE_SSE4_1  _HBit(19)	/* Streaming SIMD extensions 4.1 */#define CPUID_FEATURE_SSE4_2  _HBit(20)	/* Streaming SIMD extensions 4.2 */#define CPUID_FEATURE_xAPIC   _HBit(21)	/* Extended APIC Mode */#define CPUID_FEATURE_POPCNT  _HBit(23)	/* POPCNT instruction */#define CPUID_FEATURE_AES     _HBit(25)	/* AES instructions */#define CPUID_FEATURE_VMM     _HBit(31)	/* VMM (Hypervisor) present *//* * The CPUID_EXTFEATURE_XXX values define 64-bit values * returned in %ecx:%edx to a CPUID request with %eax of 0x80000001:  */#define CPUID_EXTFEATURE_SYSCALL   _Bit(11)	/* SYSCALL/sysret */#define CPUID_EXTFEATURE_XD		   _Bit(20)	/* eXecute Disable */#define CPUID_EXTFEATURE_1GBPAGE   _Bit(26)     /* 1G-Byte Page support */#define CPUID_EXTFEATURE_RDTSCP	   _Bit(27)	/* RDTSCP */#define CPUID_EXTFEATURE_EM64T	   _Bit(29)	/* Extended Mem 64 Technology *///#define CPUID_EXTFEATURE_LAHF	   _HBit(20)	/* LAFH/SAHF instructions */// New definition with Snow kernel#define CPUID_EXTFEATURE_LAHF	   _HBit(0)	/* LAHF/SAHF instructions *//* * The CPUID_EXTFEATURE_XXX values define 64-bit values * returned in %ecx:%edx to a CPUID request with %eax of 0x80000007:  */#define CPUID_EXTFEATURE_TSCI      _Bit(8)	/* TSC Invariant */#define	CPUID_CACHE_SIZE	16	/* Number of descriptor values */#define CPUID_MWAIT_EXTENSION	_Bit(0)	/* enumeration of WMAIT extensions */#define CPUID_MWAIT_BREAK	_Bit(1)	/* interrupts are break events	   */#define CPU_MODEL_PENTIUM_M			0x0D#define CPU_MODEL_YONAH         0x0E#define CPU_MODEL_MEROM         0x0F#define CPU_MODEL_PENRYN        0x17#define CPU_MODEL_NEHALEM       0x1A#define CPU_MODEL_ATOM          0x1C#define CPU_MODEL_XEON_MP       0x1D  /* MP 7400 */#define CPU_MODEL_FIELDS        0x1E	/* Lynnfield, Clarksfield, Jasper */#define CPU_MODEL_DALES         0x1F	/* Havendale, Auburndale */#define CPU_MODEL_DALES_32NM		0x25	/* Clarkdale, Arrandale */#define CPU_MODEL_ATOM_SAN      0x26#define CPU_MODEL_LINCROFT      0x27#define CPU_MODEL_SANDY_BRIDGE	0x2A#define CPU_MODEL_WESTMERE			0x2C	/* Gulftown, Westmere-EP, Westmere-WS */#define CPU_MODEL_JAKETOWN      0x2D#define CPU_MODEL_NEHALEM_EX		0x2E#define CPU_MODEL_WESTMERE_EX		0x2F#define CPU_MODEL_ATOM_2000     0x36#define CPU_MODEL_IVY_BRIDGE    0x3A#define CPU_MODEL_HASWELL       0x3C#define CPU_MODEL_IVY_BRIDGE_E5 0x3E#include <stdint.h>#include <mach/mach_types.h>#include <kern/kern_types.h>#include <mach/machine.h>typedef enum { eax, ebx, ecx, edx } cpuid_register_t;static inline voidcpuid(uint32_t *data){	__asm__ volatile("cpuid"					 : "=a" (data[eax]),					 "=b" (data[ebx]),					 "=c" (data[ecx]),					 "=d" (data[edx])					 : "a"  (data[eax]),					 "b"  (data[ebx]),					 "c"  (data[ecx]),					 "d"  (data[edx]));}static inline voiddo_cpuid(uint32_t selector, uint32_t *data){	__asm__ volatile("cpuid"					 : "=a" (data[0]),					 "=b" (data[1]),					 "=c" (data[2]),					 "=d" (data[3])					 : "a"(selector));}/* * Cache ID descriptor structure, used to parse CPUID leaf 2. * Note: not used in kernel. */typedef enum { Lnone, L1I, L1D, L2U, L3U, LCACHE_MAX } cache_type_t ; typedef struct {	unsigned char	value;          /* Descriptor value */	cache_type_t 	type;           /* Cache type */	unsigned int 	size;           /* Cache size */	unsigned int 	linesize;       /* Cache line size */	const char	*description;   /* Cache description */} cpuid_cache_desc_t;  #define CACHE_DESC(value,type,size,linesize,text) \{ value, type, size, linesize, text }#define _Bit(n)         (1ULL << n)//#define _HBit(n)        (1ULL << ((n) + 32))#define min(a,b)        ((a) < (b) ? (a) : (b))#define quad(hi,lo)     (((uint64_t)(hi)) << 32 | (lo))#define bit(n)          (1UL << (n))#define bitmask(h,l)    ((bit(h) | (bit(h) - 1UL)) & ~(bit(l) - 1UL))#define bitfield(x,h,l) (((x) & bitmask(h, l)) >> l)/* Physical CPU info - this is exported out of the kernel (kexts), so be wary of changes */typedef struct {	char		cpuid_vendor[16];	char		cpuid_brand_string[48];	const char	*cpuid_model_string;		cpu_type_t	cpuid_type;					/* this is *not* a cpu_type_t in our <mach/machine.h> */	uint8_t		cpuid_family;	uint8_t		cpuid_model;	uint8_t		cpuid_extmodel;	uint8_t		cpuid_extfamily;	uint8_t		cpuid_stepping;	uint64_t	cpuid_features;	uint64_t	cpuid_extfeatures;	uint32_t	cpuid_signature;	uint8_t   	cpuid_brand;   uint8_t		cpuid_processor_flag;		uint32_t	cache_size[LCACHE_MAX];	uint32_t	cache_linesize;		uint8_t		cache_info[64];    /* list of cache descriptors */		uint32_t	cpuid_cores_per_package;	uint32_t	cpuid_logical_per_package;	uint32_t	cache_sharing[LCACHE_MAX];	uint32_t	cache_partitions[LCACHE_MAX];		cpu_type_t	cpuid_cpu_type;			/* <mach/machine.h> */	cpu_subtype_t	cpuid_cpu_subtype;		/* <mach/machine.h> */			/* Monitor/mwait Leaf: */	uint32_t	cpuid_mwait_linesize_min;	uint32_t	cpuid_mwait_linesize_max;	uint32_t	cpuid_mwait_extensions;	uint32_t	cpuid_mwait_sub_Cstates;		/* Thermal and Power Management Leaf: */	boolean_t	cpuid_thermal_sensor;	boolean_t	cpuid_thermal_dynamic_acceleration;	uint32_t	cpuid_thermal_thresholds;	boolean_t	cpuid_thermal_ACNT_MCNT;		/* Architectural Performance Monitoring Leaf: */	uint8_t		cpuid_arch_perf_version;	uint8_t		cpuid_arch_perf_number;	uint8_t		cpuid_arch_perf_width;	uint8_t		cpuid_arch_perf_events_number;	uint32_t	cpuid_arch_perf_events;	uint8_t		cpuid_arch_perf_fixed_number;	uint8_t		cpuid_arch_perf_fixed_width;		/* Cache details: */	uint32_t	cpuid_cache_linesize;	uint32_t	cpuid_cache_L2_associativity;	uint32_t	cpuid_cache_size;		/* Virtual and physical address aize: */	uint32_t	cpuid_address_bits_physical;	uint32_t	cpuid_address_bits_virtual;		uint32_t	cpuid_microcode_version;		/* Numbers of tlbs per processor [i|d, small|large, level0|level1] */	uint32_t	cpuid_tlb[2][2][2];#define	TLB_INST	0#define	TLB_DATA	1#define	TLB_SMALL	0#define	TLB_LARGE	1	uint32_t	cpuid_stlb;		uint32_t	core_count;	uint32_t	thread_count;		/* Max leaf ids available from CPUID */	uint32_t	cpuid_max_basic;	uint32_t	cpuid_max_ext;} i386_cpu_info_t;static i386_cpu_info_t cpuid_cpu_info;static i386_cpu_info_t	*cpuid_info(void){	return 	&cpuid_cpu_info;}static void cpuid_update_generic_info(){  uint32_t cpuid_reg[4];  uint32_t max_extid;  char     str[128];  char*    p;	i386_cpu_info_t* info_p = cpuid_info();	  /* Get vendor */  do_cpuid(0, cpuid_reg);  bcopy((char *)&cpuid_reg[ebx], &info_p->cpuid_vendor[0], 4); /* ug */  bcopy((char *)&cpuid_reg[ecx], &info_p->cpuid_vendor[8], 4);  bcopy((char *)&cpuid_reg[edx], &info_p->cpuid_vendor[4], 4);  info_p->cpuid_vendor[12] = 0;	  /* Get extended CPUID results */  do_cpuid(0x80000000, cpuid_reg);  max_extid = cpuid_reg[eax];	  /* Check to see if we can get the brand string */  if (max_extid >= 0x80000004) {    /*     * The brand string is up to 48 bytes and is guaranteed to be     * NUL terminated.     */    do_cpuid(0x80000002, cpuid_reg);    bcopy((char *)cpuid_reg, &str[0], 16);    do_cpuid(0x80000003, cpuid_reg);    bcopy((char *)cpuid_reg, &str[16], 16);    do_cpuid(0x80000004, cpuid_reg);    bcopy((char *)cpuid_reg, &str[32], 16);    for (p = str; *p != '\0'; p++) {      if (*p != ' ') break;    }    strncpy(info_p->cpuid_brand_string, p,            sizeof(info_p->cpuid_brand_string));		    if (!strncmp(info_p->cpuid_brand_string, CPUID_STRING_UNKNOWN,                 min(sizeof(info_p->cpuid_brand_string),                     strlen(CPUID_STRING_UNKNOWN) + 1))) {                   /*                    * This string means we have a firmware-programmable brand string,                    * and the firmware couldn't figure out what sort of CPU we have.                    */                   info_p->cpuid_brand_string[0] = '\0';                 }  }    /* Get cache and addressing info */  if (max_extid >= 0x80000006) {    do_cpuid(0x80000006, cpuid_reg);    info_p->cpuid_cache_linesize = bitfield(cpuid_reg[ecx], 7, 0);    info_p->cpuid_cache_L2_associativity = bitfield(cpuid_reg[ecx], 15, 12);    info_p->cpuid_cache_size = bitfield(cpuid_reg[ecx], 31, 16);    do_cpuid(0x80000008, cpuid_reg);    info_p->cpuid_address_bits_physical = bitfield(cpuid_reg[eax], 7, 0);    info_p->cpuid_address_bits_virtual = bitfield(cpuid_reg[eax], 15, 8);  }	/*	 * Get processor signature and decode	 * and bracket this with the approved procedure for reading the	 * the microcode version number a.k.a. signature a.k.a. BIOS ID	 */	wrmsr64(MSR_IA32_BIOS_SIGN_ID, 0);	do_cpuid(1, cpuid_reg);	info_p->cpuid_microcode_version =  (uint32_t) (rdmsr64(MSR_IA32_BIOS_SIGN_ID) >> 32);	  /* Get processor signature and decode */    info_p->cpuid_signature = cpuid_reg[eax];  info_p->cpuid_stepping  = bitfield(cpuid_reg[eax],  3,  0);  info_p->cpuid_model     = bitfield(cpuid_reg[eax],  7,  4);  info_p->cpuid_family    = bitfield(cpuid_reg[eax], 11,  8);  info_p->cpuid_type      = bitfield(cpuid_reg[eax], 13, 12);  info_p->cpuid_extmodel  = bitfield(cpuid_reg[eax], 19, 16);  info_p->cpuid_extfamily = bitfield(cpuid_reg[eax], 27, 20);  info_p->cpuid_brand     = bitfield(cpuid_reg[ebx],  7,  0);  info_p->cpuid_features  = quad(cpuid_reg[ecx], cpuid_reg[edx]);  	/* Get "processor flag"; necessary for microcode update matching */	info_p->cpuid_processor_flag = (rdmsr64(MSR_IA32_PLATFORM_ID)>> 50) & 3;	  /* Fold extensions into family/model */  if (info_p->cpuid_family == 0x0f) {    info_p->cpuid_family += info_p->cpuid_extfamily;  }  if (info_p->cpuid_family == 0x0f || info_p->cpuid_family== 0x06) {    info_p->cpuid_model += (info_p->cpuid_extmodel << 4);  }	  if (info_p->cpuid_features & CPUID_FEATURE_HTT) {    info_p->cpuid_logical_per_package = bitfield(cpuid_reg[ebx], 23, 16);  } else {    info_p->cpuid_logical_per_package = 1;  }	  if (max_extid >= 0x80000001) {    do_cpuid(0x80000001, cpuid_reg);    info_p->cpuid_extfeatures = quad(cpuid_reg[ecx], cpuid_reg[edx]);  }    /* Fold in the Invariant TSC feature bit, if present */	if (info_p->cpuid_max_ext >= 0x80000007) {		do_cpuid(0x80000007, cpuid_reg);  		info_p->cpuid_extfeatures |=    cpuid_reg[edx] & (uint32_t)CPUID_EXTFEATURE_TSCI;	}    if (info_p->cpuid_extfeatures & CPUID_FEATURE_MONITOR) {    if (info_p->cpuid_max_ext >= 5) {      do_cpuid(5, cpuid_reg);      info_p->cpuid_mwait_linesize_min = cpuid_reg[eax];      info_p->cpuid_mwait_linesize_max = cpuid_reg[ebx];      info_p->cpuid_mwait_extensions   = cpuid_reg[ecx];      info_p->cpuid_mwait_sub_Cstates  = cpuid_reg[edx];    }    if (info_p->cpuid_max_ext >= 6) {      do_cpuid(6, cpuid_reg);      info_p->cpuid_thermal_sensor = bitfield((uint32_t)cpuid_reg[eax], 0, 0);      info_p->cpuid_thermal_dynamic_acceleration =      bitfield((uint32_t)cpuid_reg[eax], 1, 1);      info_p->cpuid_thermal_thresholds = bitfield((uint32_t)cpuid_reg[ebx], 3, 0);      info_p->cpuid_thermal_ACNT_MCNT = bitfield((uint32_t)cpuid_reg[ecx], 0, 0);    }    if (info_p->cpuid_max_ext >= 0xa) {      do_cpuid(0xa, cpuid_reg);      info_p->cpuid_arch_perf_version = bitfield(cpuid_reg[eax], 7, 0);      info_p->cpuid_arch_perf_number = bitfield(cpuid_reg[eax],15, 8);      info_p->cpuid_arch_perf_width = bitfield(cpuid_reg[eax],23,16);      info_p->cpuid_arch_perf_events_number = bitfield(cpuid_reg[eax],31,24);      info_p->cpuid_arch_perf_events = cpuid_reg[ebx];      info_p->cpuid_arch_perf_fixed_number = bitfield(cpuid_reg[edx], 4, 0);      info_p->cpuid_arch_perf_fixed_width = bitfield(cpuid_reg[edx],12, 5);    }		  }		do_cpuid(4, cpuid_reg);	info_p->cpuid_cores_per_package = bitfield((uint32_t)cpuid_reg[eax], 31, 26) + 1;  	if (info_p->cpuid_cores_per_package == 0) {		info_p->cpuid_cores_per_package = 1;			}				switch (info_p->cpuid_model)	{		case CPU_MODEL_NEHALEM: // Intel Core i7 LGA1366 (45nm)		case CPU_MODEL_FIELDS: // Intel Core i5, i7 LGA1156 (45nm)		case CPU_MODEL_DALES_32NM: // Intel Core i3, i5, i7 LGA1156 (32nm)    case CPU_MODEL_SANDY_BRIDGE:    case CPU_MODEL_JAKETOWN:    case CPU_MODEL_NEHALEM_EX:      case CPU_MODEL_IVY_BRIDGE:    case CPU_MODEL_HASWELL:    case CPU_MODEL_IVY_BRIDGE_E5:		{			uint64_t msr = rdmsr64(MSR_CORE_THREAD_COUNT);			info_p->core_count   = bitfield((uint32_t)msr, 31, 16);			info_p->thread_count = bitfield((uint32_t)msr, 15,  0);		} break;		case CPU_MODEL_WESTMERE: // Intel Core i7 LGA1366 (32nm) 6 Core		case CPU_MODEL_DALES:		case CPU_MODEL_WESTMERE_EX:		{			uint64_t msr = rdmsr64(MSR_CORE_THREAD_COUNT);			info_p->core_count   = bitfield((uint32_t)msr, 19, 16);			info_p->thread_count = bitfield((uint32_t)msr, 15,  0);			break;		}					default:		{			do_cpuid(1, cpuid_reg);			info_p->core_count = bitfield(cpuid_reg[1], 23, 16);			info_p->thread_count = info_p->cpuid_logical_per_package;      //workaround for N270. I don't know why it detected wrong      if ((info_p->cpuid_model == CPU_MODEL_ATOM) &&           (info_p->cpuid_stepping == 2)) {        info_p->core_count = 1;      }      		} break;	}	if (info_p->core_count == 0) {		info_p->core_count   = info_p->cpuid_cores_per_package;		info_p->thread_count = info_p->cpuid_logical_per_package;	}	}#endif /*M_CPUID_H*/