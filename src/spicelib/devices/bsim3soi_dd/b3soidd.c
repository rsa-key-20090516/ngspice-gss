/**********
Copyright 1999 Regents of the University of California.  All rights reserved.
Author: Weidong Liu and Pin Su         Feb 1999
Author: 1998 Samuel Fung, Dennis Sinitsky and Stephen Tang
File: b3soidd.c          98/5/01
Modified by Wei Jin 99/9/27
Modified by Paolo Nenzi 2001
**********/

/*
 * Revision 2.1  99/9/27 Pin Su 
 * BSIMDD2.1 release
 */
 
#include "ngspice.h"
#include "devdefs.h"
#include "b3soidddef.h"
#include "suffix.h"

IFparm B3SOIDDpTable[] = { /* parameters */
IOP( "l",   B3SOIDD_L,      IF_REAL   , "Length"),
IOP( "w",   B3SOIDD_W,      IF_REAL   , "Width"),
IOP( "m",   B3SOIDD_M,      IF_REAL   , "Parallel Multiplier"),
IOP( "ad",  B3SOIDD_AD,     IF_REAL   , "Drain area"),
IOP( "as",  B3SOIDD_AS,     IF_REAL   , "Source area"),
IOP( "pd",  B3SOIDD_PD,     IF_REAL   , "Drain perimeter"),
IOP( "ps",  B3SOIDD_PS,     IF_REAL   , "Source perimeter"),
IOP( "nrd", B3SOIDD_NRD,    IF_REAL   , "Number of squares in drain"),
IOP( "nrs", B3SOIDD_NRS,    IF_REAL   , "Number of squares in source"),
IOP( "off", B3SOIDD_OFF,    IF_FLAG   , "Device is initially off"),
IP( "ic",  B3SOIDD_IC,     IF_REALVEC , "Vector of DS,GS,BS initial voltages"),
OP( "gmbs",         B3SOIDD_GMBS,       IF_REAL,    "Gmb"),
OP( "gm",           B3SOIDD_GM,         IF_REAL,    "Gm"),
OP( "gm/ids",       B3SOIDD_GMID,       IF_REAL,    "Gm/Ids"),
OP( "gds",          B3SOIDD_GDS,        IF_REAL,    "Gds"),
OP( "vdsat",        B3SOIDD_VDSAT,      IF_REAL,    "Vdsat"),
OP( "vth",          B3SOIDD_VON,        IF_REAL,    "Vth"),
OP( "ids",          B3SOIDD_CD,         IF_REAL,    "Ids"),
OP( "vbs",          B3SOIDD_VBS,        IF_REAL,    "Vbs"),
OP( "vgs",          B3SOIDD_VGS,        IF_REAL,    "Vgs"),
OP( "vds",          B3SOIDD_VDS,        IF_REAL,    "Vds"),
OP( "ves",	    B3SOIDD_VES,	      IF_REAL,    "Ves"),
IOP( "bjtoff", B3SOIDD_BJTOFF,	      IF_INTEGER, "BJT on/off flag"),
IOP( "debug", B3SOIDD_DEBUG,	      IF_INTEGER, "BJT on/off flag"),
IOP( "rth0", B3SOIDD_RTH0,  IF_REAL,    "Instance Thermal Resistance"),
IOP( "cth0", B3SOIDD_CTH0,  IF_REAL,    "Instance Thermal Capacitance"),
IOP( "nrb", B3SOIDD_NRB,    IF_REAL,    "Number of squares in body"),
};

IFparm B3SOIDDmPTable[] = { /* model parameters */
IOP( "capmod", B3SOIDD_MOD_CAPMOD, IF_INTEGER, "Capacitance model selector"),
IOP( "mobmod", B3SOIDD_MOD_MOBMOD, IF_INTEGER, "Mobility model selector"),
IOP( "noimod", B3SOIDD_MOD_NOIMOD, IF_INTEGER, "Noise model selector"),
IOP( "paramchk", B3SOIDD_MOD_PARAMCHK, IF_INTEGER, "Model parameter checking selector"),
IOP( "binunit", B3SOIDD_MOD_BINUNIT, IF_INTEGER, "Bin  unit  selector"),
IOP( "version", B3SOIDD_MOD_VERSION, IF_REAL, " parameter for model version"),
IOP( "tox", B3SOIDD_MOD_TOX, IF_REAL, "Gate oxide thickness in meters"),

IOP( "cdsc", B3SOIDD_MOD_CDSC, IF_REAL, "Drain/Source and channel coupling capacitance"),
IOP( "cdscb", B3SOIDD_MOD_CDSCB, IF_REAL, "Body-bias dependence of cdsc"), 
IOP( "cdscd", B3SOIDD_MOD_CDSCD, IF_REAL, "Drain-bias dependence of cdsc"), 
IOP( "cit", B3SOIDD_MOD_CIT, IF_REAL, "Interface state capacitance"),
IOP( "nfactor", B3SOIDD_MOD_NFACTOR, IF_REAL, "Subthreshold swing Coefficient"),
IOP( "vsat", B3SOIDD_MOD_VSAT, IF_REAL, "Saturation velocity at tnom"),
IOP( "at", B3SOIDD_MOD_AT, IF_REAL, "Temperature coefficient of vsat"),
IOP( "a0", B3SOIDD_MOD_A0, IF_REAL, "Non-uniform depletion width effect coefficient."), 
IOP( "ags", B3SOIDD_MOD_AGS, IF_REAL, "Gate bias  coefficient of Abulk."), 
IOP( "a1", B3SOIDD_MOD_A1, IF_REAL, "Non-saturation effect coefficient"),
IOP( "a2", B3SOIDD_MOD_A2, IF_REAL, "Non-saturation effect coefficient"),
IOP( "keta", B3SOIDD_MOD_KETA, IF_REAL, "Body-bias coefficient of non-uniform depletion width effect."),
IOP( "nsub", B3SOIDD_MOD_NSUB, IF_REAL, "Substrate doping concentration with polarity"),
IOP( "nch", B3SOIDD_MOD_NPEAK, IF_REAL, "Channel doping concentration"),
IOP( "ngate", B3SOIDD_MOD_NGATE, IF_REAL, "Poly-gate doping concentration"),
IOP( "gamma1", B3SOIDD_MOD_GAMMA1, IF_REAL, "Vth body coefficient"),
IOP( "gamma2", B3SOIDD_MOD_GAMMA2, IF_REAL, "Vth body coefficient"),
IOP( "vbx", B3SOIDD_MOD_VBX, IF_REAL, "Vth transition body Voltage"),
IOP( "vbm", B3SOIDD_MOD_VBM, IF_REAL, "Maximum body voltage"),

IOP( "xt", B3SOIDD_MOD_XT, IF_REAL, "Doping depth"),
IOP( "k1", B3SOIDD_MOD_K1, IF_REAL, "Bulk effect coefficient 1"),
IOP( "kt1", B3SOIDD_MOD_KT1, IF_REAL, "Temperature coefficient of Vth"),
IOP( "kt1l", B3SOIDD_MOD_KT1L, IF_REAL, "Temperature coefficient of Vth"),
IOP( "kt2",  B3SOIDD_MOD_KT2, IF_REAL, "Body-coefficient of kt1"),
IOP( "k2",   B3SOIDD_MOD_K2,  IF_REAL, "Bulk effect coefficient 2"),
IOP( "k3",   B3SOIDD_MOD_K3,  IF_REAL, "Narrow width effect coefficient"),
IOP( "k3b",  B3SOIDD_MOD_K3B, IF_REAL, "Body effect coefficient of k3"),
IOP( "w0",   B3SOIDD_MOD_W0,  IF_REAL, "Narrow width effect parameter"),
IOP( "nlx",  B3SOIDD_MOD_NLX, IF_REAL, "Lateral non-uniform doping effect"),
IOP( "dvt0", B3SOIDD_MOD_DVT0, IF_REAL, "Short channel effect coeff. 0"),
IOP( "dvt1", B3SOIDD_MOD_DVT1, IF_REAL, "Short channel effect coeff. 1"),
IOP( "dvt2", B3SOIDD_MOD_DVT2, IF_REAL, "Short channel effect coeff. 2"),
IOP( "dvt0w", B3SOIDD_MOD_DVT0W, IF_REAL, "Narrow Width coeff. 0"),
IOP( "dvt1w", B3SOIDD_MOD_DVT1W, IF_REAL, "Narrow Width effect coeff. 1"),
IOP( "dvt2w", B3SOIDD_MOD_DVT2W, IF_REAL, "Narrow Width effect coeff. 2"),
IOP( "drout", B3SOIDD_MOD_DROUT, IF_REAL, "DIBL coefficient of output resistance"),
IOP( "dsub", B3SOIDD_MOD_DSUB, IF_REAL, "DIBL coefficient in the subthreshold region"),
IOP( "vth0", B3SOIDD_MOD_VTH0, IF_REAL,"Threshold voltage"),
IOP( "vtho", B3SOIDD_MOD_VTH0, IF_REAL,"Threshold voltage"),
IOP( "ua", B3SOIDD_MOD_UA, IF_REAL, "Linear gate dependence of mobility"),
IOP( "ua1", B3SOIDD_MOD_UA1, IF_REAL, "Temperature coefficient of ua"),
IOP( "ub", B3SOIDD_MOD_UB, IF_REAL, "Quadratic gate dependence of mobility"),
IOP( "ub1", B3SOIDD_MOD_UB1, IF_REAL, "Temperature coefficient of ub"),
IOP( "uc", B3SOIDD_MOD_UC, IF_REAL, "Body-bias dependence of mobility"),
IOP( "uc1", B3SOIDD_MOD_UC1, IF_REAL, "Temperature coefficient of uc"),
IOP( "u0", B3SOIDD_MOD_U0, IF_REAL, "Low-field mobility at Tnom"),
IOP( "ute", B3SOIDD_MOD_UTE, IF_REAL, "Temperature coefficient of mobility"),
IOP( "voff", B3SOIDD_MOD_VOFF, IF_REAL, "Threshold voltage offset"),
IOP( "tnom", B3SOIDD_MOD_TNOM, IF_REAL, "Parameter measurement temperature"),
IOP( "cgso", B3SOIDD_MOD_CGSO, IF_REAL, "Gate-source overlap capacitance per width"),
IOP( "cgdo", B3SOIDD_MOD_CGDO, IF_REAL, "Gate-drain overlap capacitance per width"),
IOP( "cgeo", B3SOIDD_MOD_CGEO, IF_REAL, "Gate-substrate overlap capacitance"),
IOP( "xpart", B3SOIDD_MOD_XPART, IF_REAL, "Channel charge partitioning"),
IOP( "delta", B3SOIDD_MOD_DELTA, IF_REAL, "Effective Vds parameter"),
IOP( "rsh", B3SOIDD_MOD_RSH, IF_REAL, "Source-drain sheet resistance"),
IOP( "rdsw", B3SOIDD_MOD_RDSW, IF_REAL, "Source-drain resistance per width"),    

IOP( "prwg", B3SOIDD_MOD_PRWG, IF_REAL, "Gate-bias effect on parasitic resistance "),    
IOP( "prwb", B3SOIDD_MOD_PRWB, IF_REAL, "Body-effect on parasitic resistance "),    

IOP( "prt", B3SOIDD_MOD_PRT, IF_REAL, "Temperature coefficient of parasitic resistance "),    
IOP( "eta0", B3SOIDD_MOD_ETA0, IF_REAL, "Subthreshold region DIBL coefficient"),
IOP( "etab", B3SOIDD_MOD_ETAB, IF_REAL, "Subthreshold region DIBL coefficient"),
IOP( "pclm", B3SOIDD_MOD_PCLM, IF_REAL, "Channel length modulation Coefficient"),
IOP( "pdiblc1", B3SOIDD_MOD_PDIBL1, IF_REAL, "Drain-induced barrier lowering coefficient"),   
IOP( "pdiblc2", B3SOIDD_MOD_PDIBL2, IF_REAL, "Drain-induced barrier lowering coefficient"),   
IOP( "pdiblcb", B3SOIDD_MOD_PDIBLB, IF_REAL, "Body-effect on drain-induced barrier lowering"),   

IOP( "pvag", B3SOIDD_MOD_PVAG, IF_REAL, "Gate dependence of output resistance parameter"),   

IOP( "shmod", B3SOIDD_MOD_SHMOD, IF_INTEGER, "Self heating mode selector"),
IOP( "tbox", B3SOIDD_MOD_TBOX, IF_REAL, "Back gate oxide thickness in meters"),   
IOP( "tsi", B3SOIDD_MOD_TSI, IF_REAL, "Silicon-on-insulator thickness in meters"),   
IOP( "xj", B3SOIDD_MOD_XJ, IF_REAL, "Junction Depth"),   
IOP( "kb1", B3SOIDD_MOD_KB1, IF_REAL, "Backgate coupling coefficient at strong inversion"),   
IOP( "kb3", B3SOIDD_MOD_KB3, IF_REAL, "Backgate coupling coefficient at subthreshold"),   
IOP( "dvbd0", B3SOIDD_MOD_DVBD0, IF_REAL, "First coefficient of short-channel effect on Vbs0t"),   
IOP( "dvbd1", B3SOIDD_MOD_DVBD1, IF_REAL, "Second coefficient of short-channel effect on Vbs0t"),   
IOP( "vbsa", B3SOIDD_MOD_VBSA, IF_REAL, "Vbs0t offset voltage"),
IOP( "delp", B3SOIDD_MOD_DELP, IF_REAL, "Offset constant for limiting Vbseff to Phis"),
IOP( "rbody", B3SOIDD_MOD_RBODY, IF_REAL, "Intrinsic body contact sheet resistance"),   
IOP( "rbsh",  B3SOIDD_MOD_RBSH,  IF_REAL, "Extrinsic body contact sheet resistance"),
IOP( "adice0", B3SOIDD_MOD_ADICE0, IF_REAL, "DICE constant for bulk charge effect"),   
IOP( "abp", B3SOIDD_MOD_ABP, IF_REAL, "Gate bias coefficient for Xcsat calculation"),   
IOP( "mxc", B3SOIDD_MOD_MXC, IF_REAL, "A smoothing parameter for Xcsat calculation"),   
IOP( "rth0", B3SOIDD_MOD_RTH0, IF_REAL, "Self-heating thermal resistance"),   
IOP( "cth0", B3SOIDD_MOD_CTH0, IF_REAL, "Self-heating thermal capacitance"),
IOP( "aii", B3SOIDD_MOD_AII, IF_REAL, "1st Vdsatii parameter"),   
IOP( "bii", B3SOIDD_MOD_BII, IF_REAL, "2nd Vdsatii parameter"),   
IOP( "cii", B3SOIDD_MOD_CII, IF_REAL, "3rd Vdsatii parameter"),   
IOP( "dii", B3SOIDD_MOD_DII, IF_REAL, "4th Vdsatii parameter"),   
IOP( "ngidl", B3SOIDD_MOD_NGIDL, IF_REAL, "GIDL first parameter"),   
IOP( "agidl", B3SOIDD_MOD_AGIDL, IF_REAL, "GIDL second parameter"),   
IOP( "bgidl", B3SOIDD_MOD_BGIDL, IF_REAL, "GIDL third parameter"),   
IOP( "ndiode", B3SOIDD_MOD_NDIODE, IF_REAL, "Diode non-ideality factor"),   
IOP( "ntun", B3SOIDD_MOD_NTUN, IF_REAL, "Reverse tunneling non-ideality factor"),   
IOP( "isbjt", B3SOIDD_MOD_ISBJT, IF_REAL, "BJT emitter injection constant"),   
IOP( "isdif", B3SOIDD_MOD_ISDIF, IF_REAL, "Body to S/D injection constant"),   
IOP( "isrec", B3SOIDD_MOD_ISREC, IF_REAL, "Recombination in depletion constant"),   
IOP( "istun", B3SOIDD_MOD_ISTUN, IF_REAL, "Tunneling diode constant"),   
IOP( "xbjt", B3SOIDD_MOD_XBJT, IF_REAL, "Temperature coefficient for Isbjt"),   
IOP( "xdif", B3SOIDD_MOD_XBJT, IF_REAL, "Temperature coefficient for Isdif"),   
IOP( "xrec", B3SOIDD_MOD_XREC, IF_REAL, "Temperature coefficient for Isrec"),   
IOP( "xtun", B3SOIDD_MOD_XTUN, IF_REAL, "Temperature coefficient for Istun"),   
IOP( "edl", B3SOIDD_MOD_EDL, IF_REAL, "Electron diffusion length"),   
IOP( "kbjt1", B3SOIDD_MOD_KBJT1, IF_REAL, "Vds dependency on BJT base width"),   
IOP( "tt", B3SOIDD_MOD_TT, IF_REAL, "Diffusion capacitance transit time coefficient"),
IOP( "vsdth", B3SOIDD_MOD_VSDTH, IF_REAL, "Source/Drain diffusion threshold voltage"),
IOP( "vsdfb", B3SOIDD_MOD_VSDFB, IF_REAL, "Source/Drain diffusion flatband voltage"),
IOP( "csdmin", B3SOIDD_MOD_CSDMIN, IF_REAL, "Source/Drain diffusion bottom minimum capacitance"),
IOP( "asd", B3SOIDD_MOD_ASD, IF_REAL, "Source/Drain diffusion smoothing parameter"),

IOP( "pbswg", B3SOIDD_MOD_PBSWG, IF_REAL, "Source/drain (gate side) sidewall junction capacitance built in potential"),
IOP( "mjswg", B3SOIDD_MOD_MJSWG, IF_REAL, "Source/drain (gate side) sidewall junction capacitance grading coefficient"),

IOP( "cjswg", B3SOIDD_MOD_CJSWG, IF_REAL, "Source/drain (gate side) sidewall junction capacitance per unit width"),
IOP( "csdesw", B3SOIDD_MOD_CSDESW, IF_REAL, "Source/drain sidewall fringing constant"),
IOP( "lint", B3SOIDD_MOD_LINT, IF_REAL, "Length reduction parameter"),
IOP( "ll",   B3SOIDD_MOD_LL, IF_REAL, "Length reduction parameter"),
IOP( "lln",  B3SOIDD_MOD_LLN, IF_REAL, "Length reduction parameter"),
IOP( "lw",   B3SOIDD_MOD_LW,  IF_REAL, "Length reduction parameter"),
IOP( "lwn",  B3SOIDD_MOD_LWN, IF_REAL, "Length reduction parameter"),
IOP( "lwl",  B3SOIDD_MOD_LWL, IF_REAL, "Length reduction parameter"),

IOP( "wr",   B3SOIDD_MOD_WR, IF_REAL, "Width dependence of rds"),
IOP( "wint", B3SOIDD_MOD_WINT, IF_REAL, "Width reduction parameter"),
IOP( "dwg",  B3SOIDD_MOD_DWG, IF_REAL, "Width reduction parameter"),
IOP( "dwb",  B3SOIDD_MOD_DWB, IF_REAL, "Width reduction parameter"),

IOP( "wl",   B3SOIDD_MOD_WL, IF_REAL, "Width reduction parameter"),
IOP( "wln",  B3SOIDD_MOD_WLN, IF_REAL, "Width reduction parameter"),
IOP( "ww",   B3SOIDD_MOD_WW, IF_REAL, "Width reduction parameter"),
IOP( "wwn",  B3SOIDD_MOD_WWN, IF_REAL, "Width reduction parameter"),
IOP( "wwl",  B3SOIDD_MOD_WWL, IF_REAL, "Width reduction parameter"),

IOP( "b0",  B3SOIDD_MOD_B0, IF_REAL, "Abulk narrow width parameter"),
IOP( "b1",  B3SOIDD_MOD_B1, IF_REAL, "Abulk narrow width parameter"),

IOP( "cgsl", B3SOIDD_MOD_CGSL, IF_REAL, "New C-V model parameter"),
IOP( "cgdl", B3SOIDD_MOD_CGDL, IF_REAL, "New C-V model parameter"),
IOP( "ckappa", B3SOIDD_MOD_CKAPPA, IF_REAL, "New C-V model parameter"),
IOP( "cf",  B3SOIDD_MOD_CF, IF_REAL, "Fringe capacitance parameter"),
IOP( "clc", B3SOIDD_MOD_CLC, IF_REAL, "Vdsat parameter for C-V model"),
IOP( "cle", B3SOIDD_MOD_CLE, IF_REAL, "Vdsat parameter for C-V model"),
IOP( "dwc", B3SOIDD_MOD_DWC, IF_REAL, "Delta W for C-V model"),
IOP( "dlc", B3SOIDD_MOD_DLC, IF_REAL, "Delta L for C-V model"),

IOP( "alpha0", B3SOIDD_MOD_ALPHA0, IF_REAL, "substrate current model parameter"),
IOP( "alpha1", B3SOIDD_MOD_ALPHA1, IF_REAL, "substrate current model parameter"),
IOP( "beta0", B3SOIDD_MOD_BETA0, IF_REAL, "substrate current model parameter"),

IOP( "noia", B3SOIDD_MOD_NOIA, IF_REAL, "Flicker noise parameter"),
IOP( "noib", B3SOIDD_MOD_NOIB, IF_REAL, "Flicker noise parameter"),
IOP( "noic", B3SOIDD_MOD_NOIC, IF_REAL, "Flicker noise parameter"),
IOP( "em", B3SOIDD_MOD_EM, IF_REAL, "Flicker noise parameter"),
IOP( "ef", B3SOIDD_MOD_EF, IF_REAL, "Flicker noise frequency exponent"),
IOP( "af", B3SOIDD_MOD_AF, IF_REAL, "Flicker noise exponent"),
IOP( "kf", B3SOIDD_MOD_KF, IF_REAL, "Flicker noise coefficient"),
IOP( "noif", B3SOIDD_MOD_NOIF, IF_REAL, "Floating body excess noise ideality factor"),

/* Added for binning - START */
/* Length Dependence */
IOP( "lnch",  B3SOIDD_MOD_LNPEAK, IF_REAL, "Length dependence of nch"),
IOP( "lnsub", B3SOIDD_MOD_LNSUB, IF_REAL, "Length dependence of nsub"),
IOP( "lngate", B3SOIDD_MOD_LNGATE, IF_REAL, "Length dependence of ngate"),
IOP( "lvth0", B3SOIDD_MOD_LVTH0, IF_REAL,"Length dependence of vto"),
IOP( "lk1",   B3SOIDD_MOD_LK1,  IF_REAL, "Length dependence of k1"),
IOP( "lk2",   B3SOIDD_MOD_LK2,  IF_REAL, "Length dependence of k2"),
IOP( "lk3",   B3SOIDD_MOD_LK3,  IF_REAL, "Length dependence of k3"),
IOP( "lk3b",  B3SOIDD_MOD_LK3B, IF_REAL, "Length dependence of k3b"),
IOP( "lvbsa",  B3SOIDD_MOD_LVBSA, IF_REAL, "Length dependence of vbsa"),
IOP( "ldelp",  B3SOIDD_MOD_LDELP, IF_REAL, "Length dependence of delp"),
IOP( "lkb1",  B3SOIDD_MOD_LKB1, IF_REAL, "Length dependence of kb1"),
IOP( "lkb3",  B3SOIDD_MOD_LKB3, IF_REAL, "Length dependence of kb3"),
IOP( "ldvbd0",  B3SOIDD_MOD_LDVBD0, IF_REAL, "Length dependence of dvbd0"),
IOP( "ldvbd1",  B3SOIDD_MOD_LDVBD1, IF_REAL, "Length dependence of dvbd1"),
IOP( "lw0",   B3SOIDD_MOD_LW0,  IF_REAL, "Length dependence of w0"),
IOP( "lnlx",  B3SOIDD_MOD_LNLX, IF_REAL, "Length dependence of nlx"),
IOP( "ldvt0", B3SOIDD_MOD_LDVT0, IF_REAL, "Length dependence of dvt0"),
IOP( "ldvt1", B3SOIDD_MOD_LDVT1, IF_REAL, "Length dependence of dvt1"),
IOP( "ldvt2", B3SOIDD_MOD_LDVT2, IF_REAL, "Length dependence of dvt2"),
IOP( "ldvt0w", B3SOIDD_MOD_LDVT0W, IF_REAL, "Length dependence of dvt0w"),
IOP( "ldvt1w", B3SOIDD_MOD_LDVT1W, IF_REAL, "Length dependence of dvt1w"),
IOP( "ldvt2w", B3SOIDD_MOD_LDVT2W, IF_REAL, "Length dependence of dvt2w"),
IOP( "lu0",  B3SOIDD_MOD_LU0, IF_REAL, "Length dependence of u0"),
IOP( "lua",  B3SOIDD_MOD_LUA, IF_REAL, "Length dependence of ua"),
IOP( "lub", B3SOIDD_MOD_LUB, IF_REAL, "Length dependence of ub"),
IOP( "luc",  B3SOIDD_MOD_LUC, IF_REAL, "Length dependence of uc"),
IOP( "lvsat", B3SOIDD_MOD_LVSAT, IF_REAL, "Length dependence of vsat"),
IOP( "la0", B3SOIDD_MOD_LA0, IF_REAL, "Length dependence of a0"),
IOP( "lags", B3SOIDD_MOD_LAGS, IF_REAL, "Length dependence of ags"),
IOP( "lb0",  B3SOIDD_MOD_LB0, IF_REAL, "Length dependence of b0"),
IOP( "lb1",  B3SOIDD_MOD_LB1, IF_REAL, "Length dependence of b1"),
IOP( "lketa", B3SOIDD_MOD_LKETA, IF_REAL, "Length dependence of keta"),
IOP( "labp", B3SOIDD_MOD_LABP, IF_REAL, "Length dependence of abp"),
IOP( "lmxc", B3SOIDD_MOD_LMXC, IF_REAL, "Length dependence of mxc"),
IOP( "ladice0", B3SOIDD_MOD_LADICE0, IF_REAL, "Length dependence of adice0"),
IOP( "la1", B3SOIDD_MOD_LA1, IF_REAL, "Length dependence of a1"),
IOP( "la2", B3SOIDD_MOD_LA2, IF_REAL, "Length dependence of a2"),
IOP( "lrdsw", B3SOIDD_MOD_LRDSW,  IF_REAL, "Length dependence of rdsw "),
IOP( "lprwb", B3SOIDD_MOD_LPRWB,  IF_REAL, "Length dependence of prwb "),
IOP( "lprwg", B3SOIDD_MOD_LPRWG,  IF_REAL, "Length dependence of prwg "),
IOP( "lwr",  B3SOIDD_MOD_LWR, IF_REAL, "Length dependence of wr"),
IOP( "lnfactor", B3SOIDD_MOD_LNFACTOR, IF_REAL, "Length dependence of nfactor"),
IOP( "ldwg", B3SOIDD_MOD_LDWG, IF_REAL, "Length dependence of dwg"),
IOP( "ldwb", B3SOIDD_MOD_LDWB, IF_REAL, "Length dependence of dwb"),
IOP( "lvoff", B3SOIDD_MOD_LVOFF, IF_REAL, "Length dependence of voff"),
IOP( "leta0", B3SOIDD_MOD_LETA0, IF_REAL, "Length dependence of eta0"),
IOP( "letab", B3SOIDD_MOD_LETAB, IF_REAL, "Length dependence of etab"),
IOP( "ldsub", B3SOIDD_MOD_LDSUB, IF_REAL, "Length dependence of dsub"),
IOP( "lcit",   B3SOIDD_MOD_LCIT,  IF_REAL, "Length dependence of cit"),
IOP( "lcdsc",  B3SOIDD_MOD_LCDSC, IF_REAL, "Length dependence of cdsc"),
IOP( "lcdscb", B3SOIDD_MOD_LCDSCB, IF_REAL, "Length dependence of cdscb"),
IOP( "lcdscd", B3SOIDD_MOD_LCDSCD, IF_REAL, "Length dependence of cdscd"),
IOP( "lpclm", B3SOIDD_MOD_LPCLM, IF_REAL, "Length dependence of pclm"),
IOP( "lpdiblc1", B3SOIDD_MOD_LPDIBL1, IF_REAL, "Length dependence of pdiblc1"),
IOP( "lpdiblc2", B3SOIDD_MOD_LPDIBL2, IF_REAL, "Length dependence of pdiblc2"),
IOP( "lpdiblcb", B3SOIDD_MOD_LPDIBLB, IF_REAL, "Length dependence of pdiblcb"),
IOP( "ldrout", B3SOIDD_MOD_LDROUT, IF_REAL, "Length dependence of drout"),
IOP( "lpvag", B3SOIDD_MOD_LPVAG, IF_REAL, "Length dependence of pvag"),
IOP( "ldelta", B3SOIDD_MOD_LDELTA, IF_REAL, "Length dependence of delta"),
IOP( "laii", B3SOIDD_MOD_LAII, IF_REAL, "Length dependence of aii"),
IOP( "lbii", B3SOIDD_MOD_LBII, IF_REAL, "Length dependence of bii"),
IOP( "lcii", B3SOIDD_MOD_LCII, IF_REAL, "Length dependence of cii"),
IOP( "ldii", B3SOIDD_MOD_LDII, IF_REAL, "Length dependence of dii"),
IOP( "lalpha0", B3SOIDD_MOD_LALPHA0, IF_REAL, "Length dependence of alpha0"),
IOP( "lalpha1", B3SOIDD_MOD_LALPHA1, IF_REAL, "Length dependence of alpha1"),
IOP( "lbeta0", B3SOIDD_MOD_LBETA0, IF_REAL, "Length dependence of beta0"),
IOP( "lagidl", B3SOIDD_MOD_LAGIDL, IF_REAL, "Length dependence of agidl"),
IOP( "lbgidl", B3SOIDD_MOD_LBGIDL, IF_REAL, "Length dependence of bgidl"),
IOP( "lngidl", B3SOIDD_MOD_LNGIDL, IF_REAL, "Length dependence of ngidl"),
IOP( "lntun", B3SOIDD_MOD_LNTUN, IF_REAL, "Length dependence of ntun"),
IOP( "lndiode", B3SOIDD_MOD_LNDIODE, IF_REAL, "Length dependence of ndiode"),
IOP( "lisbjt", B3SOIDD_MOD_LISBJT, IF_REAL, "Length dependence of isbjt"),
IOP( "lisdif", B3SOIDD_MOD_LISDIF, IF_REAL, "Length dependence of isdif"),
IOP( "lisrec", B3SOIDD_MOD_LISREC, IF_REAL, "Length dependence of isrec"),
IOP( "listun", B3SOIDD_MOD_LISTUN, IF_REAL, "Length dependence of istun"),
IOP( "ledl", B3SOIDD_MOD_LEDL, IF_REAL, "Length dependence of edl"),
IOP( "lkbjt1", B3SOIDD_MOD_LKBJT1, IF_REAL, "Length dependence of kbjt1"),
IOP( "lvsdfb", B3SOIDD_MOD_LVSDFB, IF_REAL, "Length dependence of vsdfb"),
IOP( "lvsdth", B3SOIDD_MOD_LVSDTH, IF_REAL, "Length dependence of vsdth"),
/* Width Dependence */
IOP( "wnch",  B3SOIDD_MOD_WNPEAK, IF_REAL, "Width dependence of nch"),
IOP( "wnsub", B3SOIDD_MOD_WNSUB, IF_REAL, "Width dependence of nsub"),
IOP( "wngate", B3SOIDD_MOD_WNGATE, IF_REAL, "Width dependence of ngate"),
IOP( "wvth0", B3SOIDD_MOD_WVTH0, IF_REAL,"Width dependence of vto"),
IOP( "wk1",   B3SOIDD_MOD_WK1,  IF_REAL, "Width dependence of k1"),
IOP( "wk2",   B3SOIDD_MOD_WK2,  IF_REAL, "Width dependence of k2"),
IOP( "wk3",   B3SOIDD_MOD_WK3,  IF_REAL, "Width dependence of k3"),
IOP( "wk3b",  B3SOIDD_MOD_WK3B, IF_REAL, "Width dependence of k3b"),
IOP( "wvbsa",  B3SOIDD_MOD_WVBSA, IF_REAL, "Width dependence of vbsa"),
IOP( "wdelp",  B3SOIDD_MOD_WDELP, IF_REAL, "Width dependence of delp"),
IOP( "wkb1",  B3SOIDD_MOD_WKB1, IF_REAL, "Width dependence of kb1"),
IOP( "wkb3",  B3SOIDD_MOD_WKB3, IF_REAL, "Width dependence of kb3"),
IOP( "wdvbd0",  B3SOIDD_MOD_WDVBD0, IF_REAL, "Width dependence of dvbd0"),
IOP( "wdvbd1",  B3SOIDD_MOD_WDVBD1, IF_REAL, "Width dependence of dvbd1"),
IOP( "ww0",   B3SOIDD_MOD_WW0,  IF_REAL, "Width dependence of w0"),
IOP( "wnlx",  B3SOIDD_MOD_WNLX, IF_REAL, "Width dependence of nlx"),
IOP( "wdvt0", B3SOIDD_MOD_WDVT0, IF_REAL, "Width dependence of dvt0"),
IOP( "wdvt1", B3SOIDD_MOD_WDVT1, IF_REAL, "Width dependence of dvt1"),
IOP( "wdvt2", B3SOIDD_MOD_WDVT2, IF_REAL, "Width dependence of dvt2"),
IOP( "wdvt0w", B3SOIDD_MOD_WDVT0W, IF_REAL, "Width dependence of dvt0w"),
IOP( "wdvt1w", B3SOIDD_MOD_WDVT1W, IF_REAL, "Width dependence of dvt1w"),
IOP( "wdvt2w", B3SOIDD_MOD_WDVT2W, IF_REAL, "Width dependence of dvt2w"),
IOP( "wu0",  B3SOIDD_MOD_WU0, IF_REAL, "Width dependence of u0"),
IOP( "wua",  B3SOIDD_MOD_WUA, IF_REAL, "Width dependence of ua"),
IOP( "wub", B3SOIDD_MOD_WUB, IF_REAL, "Width dependence of ub"),
IOP( "wuc",  B3SOIDD_MOD_WUC, IF_REAL, "Width dependence of uc"),
IOP( "wvsat", B3SOIDD_MOD_WVSAT, IF_REAL, "Width dependence of vsat"),
IOP( "wa0", B3SOIDD_MOD_WA0, IF_REAL, "Width dependence of a0"),
IOP( "wags", B3SOIDD_MOD_WAGS, IF_REAL, "Width dependence of ags"),
IOP( "wb0",  B3SOIDD_MOD_WB0, IF_REAL, "Width dependence of b0"),
IOP( "wb1",  B3SOIDD_MOD_WB1, IF_REAL, "Width dependence of b1"),
IOP( "wketa", B3SOIDD_MOD_WKETA, IF_REAL, "Width dependence of keta"),
IOP( "wabp", B3SOIDD_MOD_WABP, IF_REAL, "Width dependence of abp"),
IOP( "wmxc", B3SOIDD_MOD_WMXC, IF_REAL, "Width dependence of mxc"),
IOP( "wadice0", B3SOIDD_MOD_WADICE0, IF_REAL, "Width dependence of adice0"),
IOP( "wa1", B3SOIDD_MOD_WA1, IF_REAL, "Width dependence of a1"),
IOP( "wa2", B3SOIDD_MOD_WA2, IF_REAL, "Width dependence of a2"),
IOP( "wrdsw", B3SOIDD_MOD_WRDSW,  IF_REAL, "Width dependence of rdsw "),
IOP( "wprwb", B3SOIDD_MOD_WPRWB,  IF_REAL, "Width dependence of prwb "),
IOP( "wprwg", B3SOIDD_MOD_WPRWG,  IF_REAL, "Width dependence of prwg "),
IOP( "wwr",  B3SOIDD_MOD_WWR, IF_REAL, "Width dependence of wr"),
IOP( "wnfactor", B3SOIDD_MOD_WNFACTOR, IF_REAL, "Width dependence of nfactor"),
IOP( "wdwg", B3SOIDD_MOD_WDWG, IF_REAL, "Width dependence of dwg"),
IOP( "wdwb", B3SOIDD_MOD_WDWB, IF_REAL, "Width dependence of dwb"),
IOP( "wvoff", B3SOIDD_MOD_WVOFF, IF_REAL, "Width dependence of voff"),
IOP( "weta0", B3SOIDD_MOD_WETA0, IF_REAL, "Width dependence of eta0"),
IOP( "wetab", B3SOIDD_MOD_WETAB, IF_REAL, "Width dependence of etab"),
IOP( "wdsub", B3SOIDD_MOD_WDSUB, IF_REAL, "Width dependence of dsub"),
IOP( "wcit",   B3SOIDD_MOD_WCIT,  IF_REAL, "Width dependence of cit"),
IOP( "wcdsc",  B3SOIDD_MOD_WCDSC, IF_REAL, "Width dependence of cdsc"),
IOP( "wcdscb", B3SOIDD_MOD_WCDSCB, IF_REAL, "Width dependence of cdscb"),
IOP( "wcdscd", B3SOIDD_MOD_WCDSCD, IF_REAL, "Width dependence of cdscd"),
IOP( "wpclm", B3SOIDD_MOD_WPCLM, IF_REAL, "Width dependence of pclm"),
IOP( "wpdiblc1", B3SOIDD_MOD_WPDIBL1, IF_REAL, "Width dependence of pdiblc1"),
IOP( "wpdiblc2", B3SOIDD_MOD_WPDIBL2, IF_REAL, "Width dependence of pdiblc2"),
IOP( "wpdiblcb", B3SOIDD_MOD_WPDIBLB, IF_REAL, "Width dependence of pdiblcb"),
IOP( "wdrout", B3SOIDD_MOD_WDROUT, IF_REAL, "Width dependence of drout"),
IOP( "wpvag", B3SOIDD_MOD_WPVAG, IF_REAL, "Width dependence of pvag"),
IOP( "wdelta", B3SOIDD_MOD_WDELTA, IF_REAL, "Width dependence of delta"),
IOP( "waii", B3SOIDD_MOD_WAII, IF_REAL, "Width dependence of aii"),
IOP( "wbii", B3SOIDD_MOD_WBII, IF_REAL, "Width dependence of bii"),
IOP( "wcii", B3SOIDD_MOD_WCII, IF_REAL, "Width dependence of cii"),
IOP( "wdii", B3SOIDD_MOD_WDII, IF_REAL, "Width dependence of dii"),
IOP( "walpha0", B3SOIDD_MOD_WALPHA0, IF_REAL, "Width dependence of alpha0"),
IOP( "walpha1", B3SOIDD_MOD_WALPHA1, IF_REAL, "Width dependence of alpha1"),
IOP( "wbeta0", B3SOIDD_MOD_WBETA0, IF_REAL, "Width dependence of beta0"),
IOP( "wagidl", B3SOIDD_MOD_WAGIDL, IF_REAL, "Width dependence of agidl"),
IOP( "wbgidl", B3SOIDD_MOD_WBGIDL, IF_REAL, "Width dependence of bgidl"),
IOP( "wngidl", B3SOIDD_MOD_WNGIDL, IF_REAL, "Width dependence of ngidl"),
IOP( "wntun", B3SOIDD_MOD_WNTUN, IF_REAL, "Width dependence of ntun"),
IOP( "wndiode", B3SOIDD_MOD_WNDIODE, IF_REAL, "Width dependence of ndiode"),
IOP( "wisbjt", B3SOIDD_MOD_WISBJT, IF_REAL, "Width dependence of isbjt"),
IOP( "wisdif", B3SOIDD_MOD_WISDIF, IF_REAL, "Width dependence of isdif"),
IOP( "wisrec", B3SOIDD_MOD_WISREC, IF_REAL, "Width dependence of isrec"),
IOP( "wistun", B3SOIDD_MOD_WISTUN, IF_REAL, "Width dependence of istun"),
IOP( "wedl", B3SOIDD_MOD_WEDL, IF_REAL, "Width dependence of edl"),
IOP( "wkbjt1", B3SOIDD_MOD_WKBJT1, IF_REAL, "Width dependence of kbjt1"),
IOP( "wvsdfb", B3SOIDD_MOD_WVSDFB, IF_REAL, "Width dependence of vsdfb"),
IOP( "wvsdth", B3SOIDD_MOD_WVSDTH, IF_REAL, "Width dependence of vsdth"),
/* Cross-term Dependence */
IOP( "pnch",  B3SOIDD_MOD_PNPEAK, IF_REAL, "Cross-term dependence of nch"),
IOP( "pnsub", B3SOIDD_MOD_PNSUB, IF_REAL, "Cross-term dependence of nsub"),
IOP( "pngate", B3SOIDD_MOD_PNGATE, IF_REAL, "Cross-term dependence of ngate"),
IOP( "pvth0", B3SOIDD_MOD_PVTH0, IF_REAL,"Cross-term dependence of vto"),
IOP( "pk1",   B3SOIDD_MOD_PK1,  IF_REAL, "Cross-term dependence of k1"),
IOP( "pk2",   B3SOIDD_MOD_PK2,  IF_REAL, "Cross-term dependence of k2"),
IOP( "pk3",   B3SOIDD_MOD_PK3,  IF_REAL, "Cross-term dependence of k3"),
IOP( "pk3b",  B3SOIDD_MOD_PK3B, IF_REAL, "Cross-term dependence of k3b"),
IOP( "pvbsa",  B3SOIDD_MOD_PVBSA, IF_REAL, "Cross-term dependence of vbsa"),
IOP( "pdelp",  B3SOIDD_MOD_PDELP, IF_REAL, "Cross-term dependence of delp"),
IOP( "pkb1",  B3SOIDD_MOD_PKB1, IF_REAL, "Cross-term dependence of kb1"),
IOP( "pkb3",  B3SOIDD_MOD_PKB3, IF_REAL, "Cross-term dependence of kb3"),
IOP( "pdvbd0",  B3SOIDD_MOD_PDVBD0, IF_REAL, "Cross-term dependence of dvbd0"),
IOP( "pdvbd1",  B3SOIDD_MOD_PDVBD1, IF_REAL, "Cross-term dependence of dvbd1"),
IOP( "pw0",   B3SOIDD_MOD_PW0,  IF_REAL, "Cross-term dependence of w0"),
IOP( "pnlx",  B3SOIDD_MOD_PNLX, IF_REAL, "Cross-term dependence of nlx"),
IOP( "pdvt0", B3SOIDD_MOD_PDVT0, IF_REAL, "Cross-term dependence of dvt0"),
IOP( "pdvt1", B3SOIDD_MOD_PDVT1, IF_REAL, "Cross-term dependence of dvt1"),
IOP( "pdvt2", B3SOIDD_MOD_PDVT2, IF_REAL, "Cross-term dependence of dvt2"),
IOP( "pdvt0w", B3SOIDD_MOD_PDVT0W, IF_REAL, "Cross-term dependence of dvt0w"),
IOP( "pdvt1w", B3SOIDD_MOD_PDVT1W, IF_REAL, "Cross-term dependence of dvt1w"),
IOP( "pdvt2w", B3SOIDD_MOD_PDVT2W, IF_REAL, "Cross-term dependence of dvt2w"),
IOP( "pu0",  B3SOIDD_MOD_PU0, IF_REAL, "Cross-term dependence of u0"),
IOP( "pua",  B3SOIDD_MOD_PUA, IF_REAL, "Cross-term dependence of ua"),
IOP( "pub", B3SOIDD_MOD_PUB, IF_REAL, "Cross-term dependence of ub"),
IOP( "puc",  B3SOIDD_MOD_PUC, IF_REAL, "Cross-term dependence of uc"),
IOP( "pvsat", B3SOIDD_MOD_PVSAT, IF_REAL, "Cross-term dependence of vsat"),
IOP( "pa0", B3SOIDD_MOD_PA0, IF_REAL, "Cross-term dependence of a0"),
IOP( "pags", B3SOIDD_MOD_PAGS, IF_REAL, "Cross-term dependence of ags"),
IOP( "pb0",  B3SOIDD_MOD_PB0, IF_REAL, "Cross-term dependence of b0"),
IOP( "pb1",  B3SOIDD_MOD_PB1, IF_REAL, "Cross-term dependence of b1"),
IOP( "pketa", B3SOIDD_MOD_PKETA, IF_REAL, "Cross-term dependence of keta"),
IOP( "pabp", B3SOIDD_MOD_PABP, IF_REAL, "Cross-term dependence of abp"),
IOP( "pmxc", B3SOIDD_MOD_PMXC, IF_REAL, "Cross-term dependence of mxc"),
IOP( "padice0", B3SOIDD_MOD_PADICE0, IF_REAL, "Cross-term dependence of adice0"),
IOP( "pa1", B3SOIDD_MOD_PA1, IF_REAL, "Cross-term dependence of a1"),
IOP( "pa2", B3SOIDD_MOD_PA2, IF_REAL, "Cross-term dependence of a2"),
IOP( "prdsw", B3SOIDD_MOD_PRDSW,  IF_REAL, "Cross-term dependence of rdsw "),
IOP( "pprwb", B3SOIDD_MOD_PPRWB,  IF_REAL, "Cross-term dependence of prwb "),
IOP( "pprwg", B3SOIDD_MOD_PPRWG,  IF_REAL, "Cross-term dependence of prwg "),
IOP( "pwr",  B3SOIDD_MOD_PWR, IF_REAL, "Cross-term dependence of wr"),
IOP( "pnfactor", B3SOIDD_MOD_PNFACTOR, IF_REAL, "Cross-term dependence of nfactor"),
IOP( "pdwg", B3SOIDD_MOD_PDWG, IF_REAL, "Cross-term dependence of dwg"),
IOP( "pdwb", B3SOIDD_MOD_PDWB, IF_REAL, "Cross-term dependence of dwb"),
IOP( "pvoff", B3SOIDD_MOD_PVOFF, IF_REAL, "Cross-term dependence of voff"),
IOP( "peta0", B3SOIDD_MOD_PETA0, IF_REAL, "Cross-term dependence of eta0"),
IOP( "petab", B3SOIDD_MOD_PETAB, IF_REAL, "Cross-term dependence of etab"),
IOP( "pdsub", B3SOIDD_MOD_PDSUB, IF_REAL, "Cross-term dependence of dsub"),
IOP( "pcit",   B3SOIDD_MOD_PCIT,  IF_REAL, "Cross-term dependence of cit"),
IOP( "pcdsc",  B3SOIDD_MOD_PCDSC, IF_REAL, "Cross-term dependence of cdsc"),
IOP( "pcdscb", B3SOIDD_MOD_PCDSCB, IF_REAL, "Cross-term dependence of cdscb"),
IOP( "pcdscd", B3SOIDD_MOD_PCDSCD, IF_REAL, "Cross-term dependence of cdscd"),
IOP( "ppclm", B3SOIDD_MOD_PPCLM, IF_REAL, "Cross-term dependence of pclm"),
IOP( "ppdiblc1", B3SOIDD_MOD_PPDIBL1, IF_REAL, "Cross-term dependence of pdiblc1"),
IOP( "ppdiblc2", B3SOIDD_MOD_PPDIBL2, IF_REAL, "Cross-term dependence of pdiblc2"),
IOP( "ppdiblcb", B3SOIDD_MOD_PPDIBLB, IF_REAL, "Cross-term dependence of pdiblcb"),
IOP( "pdrout", B3SOIDD_MOD_PDROUT, IF_REAL, "Cross-term dependence of drout"),
IOP( "ppvag", B3SOIDD_MOD_PPVAG, IF_REAL, "Cross-term dependence of pvag"),
IOP( "pdelta", B3SOIDD_MOD_PDELTA, IF_REAL, "Cross-term dependence of delta"),
IOP( "paii", B3SOIDD_MOD_PAII, IF_REAL, "Cross-term dependence of aii"),
IOP( "pbii", B3SOIDD_MOD_PBII, IF_REAL, "Cross-term dependence of bii"),
IOP( "pcii", B3SOIDD_MOD_PCII, IF_REAL, "Cross-term dependence of cii"),
IOP( "pdii", B3SOIDD_MOD_PDII, IF_REAL, "Cross-term dependence of dii"),
IOP( "palpha0", B3SOIDD_MOD_PALPHA0, IF_REAL, "Cross-term dependence of alpha0"),
IOP( "palpha1", B3SOIDD_MOD_PALPHA1, IF_REAL, "Cross-term dependence of alpha1"),
IOP( "pbeta0", B3SOIDD_MOD_PBETA0, IF_REAL, "Cross-term dependence of beta0"),
IOP( "pagidl", B3SOIDD_MOD_PAGIDL, IF_REAL, "Cross-term dependence of agidl"),
IOP( "pbgidl", B3SOIDD_MOD_PBGIDL, IF_REAL, "Cross-term dependence of bgidl"),
IOP( "pngidl", B3SOIDD_MOD_PNGIDL, IF_REAL, "Cross-term dependence of ngidl"),
IOP( "pntun", B3SOIDD_MOD_PNTUN, IF_REAL, "Cross-term dependence of ntun"),
IOP( "pndiode", B3SOIDD_MOD_PNDIODE, IF_REAL, "Cross-term dependence of ndiode"),
IOP( "pisbjt", B3SOIDD_MOD_PISBJT, IF_REAL, "Cross-term dependence of isbjt"),
IOP( "pisdif", B3SOIDD_MOD_PISDIF, IF_REAL, "Cross-term dependence of isdif"),
IOP( "pisrec", B3SOIDD_MOD_PISREC, IF_REAL, "Cross-term dependence of isrec"),
IOP( "pistun", B3SOIDD_MOD_PISTUN, IF_REAL, "Cross-term dependence of istun"),
IOP( "pedl", B3SOIDD_MOD_PEDL, IF_REAL, "Cross-term dependence of edl"),
IOP( "pkbjt1", B3SOIDD_MOD_PKBJT1, IF_REAL, "Cross-term dependence of kbjt1"),
IOP( "pvsdfb", B3SOIDD_MOD_PVSDFB, IF_REAL, "Cross-term dependence of vsdfb"),
IOP( "pvsdth", B3SOIDD_MOD_PVSDTH, IF_REAL, "Cross-term dependence of vsdth"),
/* Added for binning - END */

IP( "nmos", B3SOIDD_MOD_NMOS,  IF_FLAG, "Flag to indicate NMOS"),
IP( "pmos", B3SOIDD_MOD_PMOS,  IF_FLAG, "Flag to indicate PMOS"),
};

char *B3SOIDDnames[] = {
   "Drain",
   "Gate",
   "Source",
   "Backgate",
   "",
   "Body",
   "Temp",
   "Charge",
};

int	B3SOIDDnSize = NUMELEMS(B3SOIDDnames);
int	B3SOIDDpTSize = NUMELEMS(B3SOIDDpTable);
int	B3SOIDDmPTSize = NUMELEMS(B3SOIDDmPTable);
int	B3SOIDDiSize = sizeof(B3SOIDDinstance);
int	B3SOIDDmSize = sizeof(B3SOIDDmodel);


