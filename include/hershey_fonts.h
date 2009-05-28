/* Hershey fonts */

/* This distribution of the Hershey Fonts may be used by anyone for
 *  any purpose, commercial or otherwise, providing that:
 *  1. The following acknowledgements must be distributed with
 *  the font data:
 *  - The Hershey Fonts were originally created by Dr.
 *  A. V. Hershey while working at the U. S.
 *  National Bureau of Standards.
 *  - The format of the Font data in this distribution
 *  was originally created by
 *  James Hurt
 *  Cognition, Inc.
 *  900 Technology Park Drive
 *  Billerica, MA 01821
 *  (mit-eddie!ci-dandelion!hurt)
 *  2. The font data in this distribution may be converted into
 *  any other format *EXCEPT* the format distributed by
 *  the U.S. NTIS (which organization holds the rights
 *  to the distribution and use of the font data in that
 *  particular format). Not that anybody would really
 *  *want* to use their format... each point is described
 *  in eight bytes as "xxx yyy:", where xxx and yyy are
 *  the coordinate values as ASCII numbers.
 */

/*
 *  Hershey fonts are vectored fonts.
 * Note one can find many formats for these vectored fonts
 * here is the formats used :
 *  >shapes are a set of polygons.
 *  >A given shape includes one or more polygons.
 *  >corner coordinates are coded by a XY pair.
 *  >The value of each coordinate is <ascii code> - 'R'
 *  >The coordinate (-50,0) or " R" is the Pen Up command (end of the current polygon)
 */

/* Hershey Simplex Roman (sans) definition */
const static char* hershey_simplex[] =
{
    "JZ",
    "MWRFRT RRYQZR[SZRY",
    "JZNFNM RVFVM",
    "H]SBLb RYBRb RLOZO RKUYU",
    "H\\PBP_ RTBT_ RYIWGTFPFMGKIKKLMMNOOUQWRXSYUYXWZT[P[MZKX",
    "F^[FI[ RNFPHPJOLMMKMIKIIJGLFNFPGSHVHYG[F RWTUUTWTYV[X[ZZ[X[VYTWT",
    "E_\\O\\N[MZMYNXPVUTXRZP[L[JZIYHWHUISJRQNRMSKSIRGPFNGMIMKNNPQUXWZY[[[\\Z\\Y",
    "MWRHQGRFSGSIRKQL",
    "KYVBTDRGPKOPOTPYR]T`Vb",
    "KYNBPDRGTKUPUTTYR]P`Nb",
    "JZRFRR RMIWO RWIMO",
    "E_RIR[ RIR[R",
    "MWSZR[QZRYSZS\\R^Q_",
    "E_IR[R",
    "MWRYQZR[SZRY",
    "G][BIb",
    "H\\QFNGLJKOKRLWNZQ[S[VZXWYRYOXJVGSFQF",
    "H\\NJPISFS[",
    "H\\LKLJMHNGPFTFVGWHXJXLWNUQK[Y[",
    "H\\MFXFRNUNWOXPYSYUXXVZS[P[MZLYKW",
    "H\\UFKTZT RUFU[",
    "H\\WFMFLOMNPMSMVNXPYSYUXXVZS[P[MZLYKW",
    "H\\XIWGTFRFOGMJLOLTMXOZR[S[VZXXYUYTXQVOSNRNOOMQLT",
    "H\\YFO[ RKFYF",
    "H\\PFMGLILKMMONSOVPXRYTYWXYWZT[P[MZLYKWKTLRNPQOUNWMXKXIWGTFPF",
    "H\\XMWPURRSQSNRLPKMKLLINGQFRFUGWIXMXRWWUZR[P[MZLX",
    "MWRMQNROSNRM RRYQZR[SZRY",
    "MWRMQNROSNRM RSZR[QZRYSZS\\R^Q_",
    "F^ZIJRZ[",
    "E_IO[O RIU[U",
    "F^JIZRJ[",
    "I[LKLJMHNGPFTFVGWHXJXLWNVORQRT RRYQZR[SZRY",
    "E`WNVLTKQKOLNMMPMSNUPVSVUUVS RQKOMNPNSOUPV RWKVSVUXVZV\\T]Q]O\\L[JYHWGTFQFNGLHJJILHOHRIUJWLYNZQ[T[WZYYZX RXKWSWUXV",
    "I[RFJ[ RRFZ[ RMTWT",
    "G\\KFK[ RKFTFWGXHYJYLXNWOTP RKPTPWQXRYTYWXYWZT[K[",
    "H]ZKYIWGUFQFOGMILKKNKSLVMXOZQ[U[WZYXZV",
    "G\\KFK[ RKFRFUGWIXKYNYSXVWXUZR[K[",
    "H[LFL[ RLFYF RLPTP RL[Y[",
    "HZLFL[ RLFYF RLPTP",
    "H]ZKYIWGUFQFOGMILKKNKSLVMXOZQ[U[WZYXZVZS RUSZS",
    "G]KFK[ RYFY[ RKPYP",
    "NVRFR[",
    "JZVFVVUYTZR[P[NZMYLVLT",
    "G\\KFK[ RYFKT RPOY[",
    "HYLFL[ RL[X[",
    "F^JFJ[ RJFR[ RZFR[ RZFZ[",
    "G]KFK[ RKFY[ RYFY[",
    "G]PFNGLIKKJNJSKVLXNZP[T[VZXXYVZSZNYKXIVGTFPF",
    "G\\KFK[ RKFTFWGXHYJYMXOWPTQKQ",
    "G]PFNGLIKKJNJSKVLXNZP[T[VZXXYVZSZNYKXIVGTFPF RSWY]",
    "G\\KFK[ RKFTFWGXHYJYLXNWOTPKP RRPY[",
    "H\\YIWGTFPFMGKIKKLMMNOOUQWRXSYUYXWZT[P[MZKX",
    "JZRFR[ RKFYF",
    "G]KFKULXNZQ[S[VZXXYUYF",
    "I[JFR[ RZFR[",
    "F^HFM[ RRFM[ RRFW[ R\\FW[",
    "H\\KFY[ RYFK[",
    "I[JFRPR[ RZFRP",
    "H\\YFK[ RKFYF RK[Y[",
    "KYOBOb RPBPb ROBVB RObVb",
    "KYKFY^",
    "KYTBTb RUBUb RNBUB RNbUb",
    "JZPLRITL RMORJWO RRJR[",
    "JZJ]Z]",
    "MWSFRGQIQKRLSKRJ",
    "I\\XMX[ RXPVNTMQMONMPLSLUMXOZQ[T[VZXX",
    "H[LFL[ RLPNNPMSMUNWPXSXUWXUZS[P[NZLX",
    "I[XPVNTMQMONMPLSLUMXOZQ[T[VZXX",
    "I\\XFX[ RXPVNTMQMONMPLSLUMXOZQ[T[VZXX",
    "I[LSXSXQWOVNTMQMONMPLSLUMXOZQ[T[VZXX",
    "MYWFUFSGRJR[ ROMVM",
    "I\\XMX]W`VaTbQbOa RXPVNTMQMONMPLSLUMXOZQ[T[VZXX",
    "I\\MFM[ RMQPNRMUMWNXQX[",
    "NVQFRGSFREQF RRMR[",
    "MWRFSGTFSERF RSMS^RaPbNb",
    "IZMFM[ RWMMW RQSX[",
    "NVRFR[",
    "CaGMG[ RGQJNLMOMQNRQR[ RRQUNWMZM\\N]Q][",
    "I\\MMM[ RMQPNRMUMWNXQX[",
    "I\\QMONMPLSLUMXOZQ[T[VZXXYUYSXPVNTMQM",
    "H[LMLb RLPNNPMSMUNWPXSXUWXUZS[P[NZLX",
    "I\\XMXb RXPVNTMQMONMPLSLUMXOZQ[T[VZXX",
    "KXOMO[ ROSPPRNTMWM",
    "J[XPWNTMQMNNMPNRPSUTWUXWXXWZT[Q[NZMX",
    "MYRFRWSZU[W[ ROMVM",
    "I\\MMMWNZP[S[UZXW RXMX[",
    "JZLMR[ RXMR[",
    "G]JMN[ RRMN[ RRMV[ RZMV[",
    "J[MMX[ RXMM[",
    "JZLMR[ RXMR[P_NaLbKb",
    "J[XMM[ RMMXM RM[X[",
    "KYTBRCQDPFPHQJRKSMSOQQ RRCQEQGRISJTLTNSPORSTTVTXSZR[Q]Q_Ra RQSSUSWRYQZP\\P^Q`RaTb",
    "NVRBRb",
    "KYPBRCSDTFTHSJRKQMQOSQ RRCSESGRIQJPLPNQPURQTPVPXQZR[S]S_Ra RSSQUQWRYSZT\\T^S`RaPb",
    "F^IUISJPLONOPPTSVTXTZS[Q RISJQLPNPPQTTVUXUZT[Q[O",
    "KYQFOGNINKOMQNSNUMVKVIUGSFQF"
};

/* Hershey Duplex Roman (sans bold) definition */
const static char* hershey_duplex[] =
{
    "JZ",
    "MXRFRTST RRFSFST RRXQYQZR[S[TZTYSXRX RRYRZSZSYRY",
    "I[NFMGMM RNGMM RNFOGMM RWFVGVM RWGVM RWFXGVM",
    "H]SFLb RYFRb RLQZQ RKWYW",
    "I\\RBR_S_ RRBSBS_ RWIYIWGTFQFNGLILKMMNNVRWSXUXWWYTZQZOYNX RWIVHTGQGNHMIMKNMVQXSYUYWXYWZT[Q[NZLXNX RXXUZ",
    "F^[FI[ RNFPHPJOLMMKMIKIIJGLFNFPGSHVHYG[F RWTUUTWTYV[X[ZZ[X[VYTWT",
    "F_\\MZMXNWPUVTXSYQZMZKYJWJUKSLRQOSMTKTISGQFPFNGMIMKNNPQUWXZZ[\\[ R\\M\\NZNXO RYNXPVVUXSZQ[M[KZJYIWIUJSLQQNRMSKSIRG RSHQGPGNH ROGNINKONQQVWXYZZ\\Z\\[",
    "MXTHSIRIQHQGRFSFTGTJSLQM RRGRHSHSGRG RSITJ RTHSL",
    "KYUBSDQGOKNPNTOYQ]S`UbVb RUBVBTDRGPKOPOTPYR]T`Vb",
    "KYNBPDRGTKUPUTTYR]P`NbOb RNBOBQDSGUKVPVTUYS]Q`Ob",
    "JZRFQGSQRR RRFRR RRFSGQQRR RMINIVOWO RMIWO RMIMJWNWO RWIVINOMO RWIMO RWIWJMNMO",
    "F_RIRZSZ RRISISZ RJQ[Q[R RJQJR[R",
    "MXTZS[R[QZQYRXSXTYT\\S^Q_ RRYRZSZSYRY RS[T\\ RTZS^",
    "F_JQ[Q[R RJQJR[R",
    "MXRXQYQZR[S[TZTYSXRX RRYRZSZSYRY",
    "G^[BIbJb R[B\\BJb",
    "H\\QFNGLJKOKRLWNZQ[S[VZXWYRYOXJVGSFQF ROGMJLOLRMWOZ RNYQZSZVY RUZWWXRXOWJUG RVHSGQGNH",
    "H\\NJPISFS[ RNJNKPJRHR[S[",
    "H\\LKLJMHNGPFTFVGWHXJXLWNUQL[ RLKMKMJNHPGTGVHWJWLVNTQK[ RLZYZY[ RK[Y[",
    "H\\MFXFQO RMFMGWG RWFPO RQNSNVOXQYTYUXXVZS[P[MZLYKWLW RPOSOVPXS RTOWQXTXUWXTZ RXVVYSZPZMYLW ROZLX",
    "H\\UIU[V[ RVFV[ RVFKVZV RUILV RLUZUZV",
    "H\\MFLO RNGMN RMFWFWG RNGWG RMNPMSMVNXPYSYUXXVZS[P[MZLYKWLW RLOMOONSNVOXR RTNWPXSXUWXTZ RXVVYSZPZMYLW ROZLX",
    "H\\VGWIXIWGTFRFOGMJLOLTMXOZR[S[VZXXYUYTXQVOSNRNOOMQ RWHTGRGOH RPGNJMOMTNXQZ RMVOYRZSZVYXV RTZWXXUXTWQTO RXSVPSOROOPMS RQONQMT",
    "H\\KFYFO[ RKFKGXG RXFN[O[",
    "H\\PFMGLILKMMNNPOTPVQWRXTXWWYTZPZMYLWLTMRNQPPTOVNWMXKXIWGTFPF RNGMIMKNMPNTOVPXRYTYWXYWZT[P[MZLYKWKTLRNPPOTNVMWKWIVG RWHTGPGMH RLXOZ RUZXX",
    "H\\WPURRSQSNRLPKMKLLINGQFRFUGWIXMXRWWUZR[P[MZLXMXNZ RWMVPSR RWNUQRRQRNQLN RPRMPLMLLMIPG RLKNHQGRGUHWK RSGVIWMWRVWTZ RUYRZPZMY",
    "MXRMQNQORPSPTOTNSMRM RRNROSOSNRN RRXQYQZR[S[TZTYSXRX RRYRZSZSYRY",
    "MXRMQNQORPSPTOTNSMRM RRNROSOSNRN RTZS[R[QZQYRXSXTYT\\S^Q_ RRYRZSZSYRY RS[T\\ RTZS^",
    "F^ZIJRZ[",
    "F_JM[M[N RJMJN[N RJU[U[V RJUJV[V",
    "F^JIZRJ[",
    "I\\LKLJMHNGQFTFWGXHYJYLXNWOUPRQ RLKMKMJNHQGTGWHXJXLWNUORP RMIPG RUGXI RXMTP RRPRTSTSP RRXQYQZR[S[TZTYSXRX RRYRZSZSYRY",
    "E`WNVLTKQKOLNMMPMSNUPVSVUUVS RQKOMNPNSOUPV RWKVSVUXVZV\\T]Q]O\\L[JYHWGTFQFNGLHJJILHOHRIUJWLYNZQ[T[WZYYZX RXKWSWUXV",
    "H\\RFJ[ RRIK[J[ RRIY[Z[ RRFZ[ RMUWU RLVXV",
    "H\\LFL[ RMGMZ RLFTFWGXHYJYMXOWPTQ RMGTGWHXJXMWOTP RMPTPWQXRYTYWXYWZT[L[ RMQTQWRXTXWWYTZMZ",
    "H]ZKYIWGUFQFOGMILKKNKSLVMXOZQ[U[WZYXZV RZKYKXIWHUGQGOHMKLNLSMVOYQZUZWYXXYVZV",
    "H]LFL[ RMGMZ RLFSFVGXIYKZNZSYVXXVZS[L[ RMGSGVHWIXKYNYSXVWXVYSZMZ",
    "I\\MFM[ RNGNZ RMFYF RNGYGYF RNPTPTQ RNQTQ RNZYZY[ RM[Y[",
    "I[MFM[ RNGN[M[ RMFYF RNGYGYF RNPTPTQ RNQTQ",
    "H]ZKYIWGUFQFOGMILKKNKSLVMXOZQ[U[WZYXZVZRUR RZKYKXIWHUGQGOHNIMKLNLSMVNXOYQZUZWYXXYVYSUSUR",
    "G]KFK[ RKFLFL[K[ RYFXFX[Y[ RYFY[ RLPXP RLQXQ",
    "NWRFR[S[ RRFSFS[",
    "J[VFVVUYSZQZOYNVMV RVFWFWVVYUZS[Q[OZNYMV",
    "H]LFL[M[ RLFMFM[ RZFYFMR RZFMS RPOY[Z[ RQOZ[",
    "IZMFM[ RMFNFNZ RNZYZY[ RM[Y[",
    "F^JFJ[ RKKK[J[ RKKR[ RJFRX RZFRX RYKR[ RYKY[Z[ RZFZ[",
    "G]KFK[ RLIL[K[ RLIY[ RKFXX RXFXX RXFYFY[",
    "G]PFNGLIKKJNJSKVLXNZP[T[VZXXYVZSZNYKXIVGTFPF RQGNHLKKNKSLVNYQZSZVYXVYSYNXKVHSGQG",
    "H\\LFL[ RMGM[L[ RLFUFWGXHYJYMXOWPUQMQ RMGUGWHXJXMWOUPMP",
    "G]PFNGLIKKJNJSKVLXNZP[T[VZXXYVZSZNYKXIVGTFPF RQGNHLKKNKSLVNYQZSZVYXVYSYNXKVHSGQG RSXX]Y] RSXTXY]",
    "H\\LFL[ RMGM[L[ RLFTFWGXHYJYMXOWPTQMQ RMGTGWHXJXMWOTPMP RRQX[Y[ RSQY[",
    "H\\YIWGTFPFMGKIKKLMMNOOTQVRWSXUXXWYTZPZNYMXKX RYIWIVHTGPGMHLILKMMONTPVQXSYUYXWZT[P[MZKX",
    "J[RGR[ RSGS[R[ RLFYFYG RLFLGYG",
    "G]KFKULXNZQ[S[VZXXYUYF RKFLFLUMXNYQZSZVYWXXUXFYF",
    "H\\JFR[ RJFKFRX RZFYFRX RZFR[",
    "E_GFM[ RGFHFMX RRFMX RRIM[ RRIW[ RRFWX R]F\\FWX R]FW[",
    "H\\KFX[Y[ RKFLFY[ RYFXFK[ RYFL[K[",
    "I\\KFRPR[S[ RKFLFSP RZFYFRP RZFSPS[",
    "H\\XFK[ RYFL[ RKFYF RKFKGXG RLZYZY[ RK[Y[",
    "KYOBOb RPBPb ROBVB RObVb",
    "KYKFY^",
    "KYTBTb RUBUb RNBUB RNbUb",
    "JZPLRITL RMORJWO RRJR[",
    "JZJ]Z]",
    "MXTFRGQIQLRMSMTLTKSJRJQK RRKRLSLSKRK RRGQK RQIRJ",
    "H\\WMW[X[ RWMXMX[ RWPUNSMPMNNLPKSKULXNZP[S[UZWX RWPSNPNNOMPLSLUMXNYPZSZWX",
    "H\\LFL[M[ RLFMFM[ RMPONQMTMVNXPYSYUXXVZT[Q[OZMX RMPQNTNVOWPXSXUWXVYTZQZMX",
    "I[XPVNTMQMONMPLSLUMXOZQ[T[VZXX RXPWQVOTNQNOONPMSMUNXOYQZTZVYWWXX",
    "H\\WFW[X[ RWFXFX[ RWPUNSMPMNNLPKSKULXNZP[S[UZWX RWPSNPNNOMPLSLUMXNYPZSZWX",
    "I[MTXTXQWOVNTMQMONMPLSLUMXOZQ[T[VZXX RMSWSWQVOTNQNOONPMSMUNXOYQZTZVYWWXX",
    "LZWFUFSGRJR[S[ RWFWGUGSH RTGSJS[ ROMVMVN ROMONVN",
    "H\\XMWMW\\V_U`SaQaO`N_L_ RXMX\\W_UaSbPbNaL_ RWPUNSMPMNNLPKSKULXNZP[S[UZWX RWPSNPNNOMPLSLUMXNYPZSZWX",
    "H\\LFL[M[ RLFMFM[ RMQPNRMUMWNXQX[ RMQPORNTNVOWQW[X[",
    "NWRFQGQHRISITHTGSFRF RRGRHSHSGRG RRMR[S[ RRMSMS[",
    "NWRFQGQHRISITHTGSFRF RRGRHSHSGRG RRMRbSb RRMSMSb",
    "H[LFL[M[ RLFMFM[ RXMWMMW RXMMX RPTV[X[ RQSX[",
    "NWRFR[S[ RRFSFS[",
    "CbGMG[H[ RGMHMH[ RHQKNMMPMRNSQS[ RHQKOMNONQORQR[S[ RSQVNXM[M]N^Q^[ RSQVOXNZN\\O]Q][^[",
    "H\\LML[M[ RLMMMM[ RMQPNRMUMWNXQX[ RMQPORNTNVOWQW[X[",
    "I\\QMONMPLSLUMXOZQ[T[VZXXYUYSXPVNTMQM RQNOONPMSMUNXOYQZTZVYWXXUXSWPVOTNQN",
    "H\\LMLbMb RLMMMMb RMPONQMTMVNXPYSYUXXVZT[Q[OZMX RMPQNTNVOWPXSXUWXVYTZQZMX",
    "H\\WMWbXb RWMXMXb RWPUNSMPMNNLPKSKULXNZP[S[UZWX RWPSNPNNOMPLSLUMXNYPZSZWX",
    "KYOMO[P[ ROMPMP[ RPSQPSNUMXM RPSQQSOUNXNXM",
    "J[XPWNTMQMNNMPNRPSUUWV RVUWWWXVZ RWYTZQZNY ROZNXMX RXPWPVN RWOTNQNNO RONNPOR RNQPRUTWUXWXXWZT[Q[NZMX",
    "MXRFR[S[ RRFSFS[ ROMVMVN ROMONVN",
    "H\\LMLWMZO[R[TZWW RLMMMMWNYPZRZTYWW RWMW[X[ RWMXMX[",
    "JZLMR[ RLMMMRY RXMWMRY RXMR[",
    "F^IMN[ RIMJMNX RRMNX RRPN[ RRPV[ RRMVX R[MZMVX R[MV[",
    "I[LMW[X[ RLMMMX[ RXMWML[ RXMM[L[",
    "JZLMR[ RLMMMRY RXMWMRYNb RXMR[ObNb",
    "I[VNL[ RXMNZ RLMXM RLMLNVN RNZXZX[ RL[X[",
    "KYTBRCQDPFPHQJRKSMSOQQ RRCQEQGRISJTLTNSPORSTTVTXSZR[Q]Q_Ra RQSSUSWRYQZP\\P^Q`RaTb",
    "NVRBRb",
    "KYPBRCSDTFTHSJRKQMQOSQ RRCSESGRIQJPLPNQPURQTPVPXQZR[S]S_Ra RSSQUQWRYSZT\\T^S`RaPb",
    "F^IUISJPLONOPPTSVTXTZS[Q RISJQLPNPPQTTVUXUZT[Q[O",
    "KYQFOGNINKOMQNSNUMVKVIUGSFQF RQFNIOMSNVKUGQF RSFOGNKQNUMVISF"
};
