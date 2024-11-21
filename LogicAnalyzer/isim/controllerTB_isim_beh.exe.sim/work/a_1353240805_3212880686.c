/**********************************************************************/
/*   ____  ____                                                       */
/*  /   /\/   /                                                       */
/* /___/  \  /                                                        */
/* \   \   \/                                                       */
/*  \   \        Copyright (c) 2003-2009 Xilinx, Inc.                */
/*  /   /          All Right Reserved.                                 */
/* /---/   /\                                                         */
/* \   \  /  \                                                      */
/*  \___\/\___\                                                    */
/***********************************************************************/

/* This file is designed for use with ISim build 0x7708f090 */

#define XSI_HIDE_SYMBOL_SPEC true
#include "xsi.h"
#include <memory.h>
#ifdef __GNUC__
#include <stdlib.h>
#else
#include <malloc.h>
#define alloca _alloca
#endif
static const char *ng0 = "D:/FpgaProjects/LogicAnalyzer/LogicAnalyzerController.vhd";
extern char *IEEE_P_2592010699;

unsigned char ieee_p_2592010699_sub_1605435078_503743352(char *, unsigned char , unsigned char );
unsigned char ieee_p_2592010699_sub_1690584930_503743352(char *, unsigned char );
unsigned char ieee_p_2592010699_sub_1744673427_503743352(char *, char *, unsigned int , unsigned int );
unsigned char ieee_p_2592010699_sub_2545490612_503743352(char *, unsigned char , unsigned char );
char *ieee_p_2592010699_sub_43738421_503743352(char *, char *, char *, char *, char *, char *);


static void work_a_1353240805_3212880686_p_0(char *t0)
{
    char t1[16];
    char t9[16];
    char *t2;
    char *t3;
    char *t4;
    char *t5;
    unsigned int t6;
    unsigned int t7;
    unsigned int t8;
    char *t10;
    char *t11;
    int t12;
    unsigned int t13;
    char *t14;
    unsigned int t15;
    unsigned char t16;
    char *t17;
    char *t18;
    char *t19;
    char *t20;
    char *t21;
    char *t22;

LAB0:    xsi_set_current_line(147, ng0);

LAB3:    t2 = (t0 + 4872U);
    t3 = *((char **)t2);
    t2 = (t0 + 20328U);
    t4 = (t0 + 6152U);
    t5 = *((char **)t4);
    t6 = (15 - 15);
    t7 = (t6 * 1U);
    t8 = (0 + t7);
    t4 = (t5 + t8);
    t10 = (t9 + 0U);
    t11 = (t10 + 0U);
    *((int *)t11) = 15;
    t11 = (t10 + 4U);
    *((int *)t11) = 8;
    t11 = (t10 + 8U);
    *((int *)t11) = -1;
    t12 = (8 - 15);
    t13 = (t12 * -1);
    t13 = (t13 + 1);
    t11 = (t10 + 12U);
    *((unsigned int *)t11) = t13;
    t11 = ieee_p_2592010699_sub_43738421_503743352(IEEE_P_2592010699, t1, t3, t2, t4, t9);
    t14 = (t1 + 12U);
    t13 = *((unsigned int *)t14);
    t15 = (1U * t13);
    t16 = (8U != t15);
    if (t16 == 1)
        goto LAB5;

LAB6:    t17 = (t0 + 12424);
    t18 = (t17 + 56U);
    t19 = *((char **)t18);
    t20 = (t19 + 56U);
    t21 = *((char **)t20);
    memcpy(t21, t11, 8U);
    xsi_driver_first_trans_fast(t17);

LAB2:    t22 = (t0 + 12136);
    *((int *)t22) = 1;

LAB1:    return;
LAB4:    goto LAB2;

LAB5:    xsi_size_not_matching(8U, t15, 0);
    goto LAB6;

}

static void work_a_1353240805_3212880686_p_1(char *t0)
{
    char t1[16];
    char t9[16];
    char *t2;
    char *t3;
    char *t4;
    char *t5;
    unsigned int t6;
    unsigned int t7;
    unsigned int t8;
    char *t10;
    char *t11;
    int t12;
    unsigned int t13;
    char *t14;
    unsigned int t15;
    unsigned char t16;
    char *t17;
    char *t18;
    char *t19;
    char *t20;
    char *t21;
    char *t22;

LAB0:    xsi_set_current_line(148, ng0);

LAB3:    t2 = (t0 + 5032U);
    t3 = *((char **)t2);
    t2 = (t0 + 20344U);
    t4 = (t0 + 6152U);
    t5 = *((char **)t4);
    t6 = (15 - 7);
    t7 = (t6 * 1U);
    t8 = (0 + t7);
    t4 = (t5 + t8);
    t10 = (t9 + 0U);
    t11 = (t10 + 0U);
    *((int *)t11) = 7;
    t11 = (t10 + 4U);
    *((int *)t11) = 0;
    t11 = (t10 + 8U);
    *((int *)t11) = -1;
    t12 = (0 - 7);
    t13 = (t12 * -1);
    t13 = (t13 + 1);
    t11 = (t10 + 12U);
    *((unsigned int *)t11) = t13;
    t11 = ieee_p_2592010699_sub_43738421_503743352(IEEE_P_2592010699, t1, t3, t2, t4, t9);
    t14 = (t1 + 12U);
    t13 = *((unsigned int *)t14);
    t15 = (1U * t13);
    t16 = (8U != t15);
    if (t16 == 1)
        goto LAB5;

LAB6:    t17 = (t0 + 12488);
    t18 = (t17 + 56U);
    t19 = *((char **)t18);
    t20 = (t19 + 56U);
    t21 = *((char **)t20);
    memcpy(t21, t11, 8U);
    xsi_driver_first_trans_fast(t17);

LAB2:    t22 = (t0 + 12152);
    *((int *)t22) = 1;

LAB1:    return;
LAB4:    goto LAB2;

LAB5:    xsi_size_not_matching(8U, t15, 0);
    goto LAB6;

}

static void work_a_1353240805_3212880686_p_2(char *t0)
{
    char *t1;
    char *t2;
    int t3;
    unsigned int t4;
    unsigned int t5;
    unsigned int t6;
    unsigned char t7;
    char *t8;
    char *t9;
    int t10;
    unsigned int t11;
    unsigned int t12;
    unsigned int t13;
    unsigned char t14;
    unsigned char t15;
    char *t16;
    char *t17;
    int t18;
    unsigned int t19;
    unsigned int t20;
    unsigned int t21;
    unsigned char t22;
    unsigned char t23;
    char *t24;
    char *t25;
    int t26;
    unsigned int t27;
    unsigned int t28;
    unsigned int t29;
    unsigned char t30;
    unsigned char t31;
    char *t32;
    char *t33;
    int t34;
    unsigned int t35;
    unsigned int t36;
    unsigned int t37;
    unsigned char t38;
    unsigned char t39;
    char *t40;
    char *t41;
    int t42;
    unsigned int t43;
    unsigned int t44;
    unsigned int t45;
    unsigned char t46;
    unsigned char t47;
    char *t48;
    char *t49;
    int t50;
    unsigned int t51;
    unsigned int t52;
    unsigned int t53;
    unsigned char t54;
    unsigned char t55;
    char *t56;
    char *t57;
    int t58;
    unsigned int t59;
    unsigned int t60;
    unsigned int t61;
    unsigned char t62;
    unsigned char t63;
    char *t64;
    char *t65;
    int t66;
    unsigned int t67;
    unsigned int t68;
    unsigned int t69;
    unsigned char t70;
    unsigned char t71;
    char *t72;
    char *t73;
    int t74;
    unsigned int t75;
    unsigned int t76;
    unsigned int t77;
    unsigned char t78;
    unsigned char t79;
    char *t80;
    char *t81;
    int t82;
    unsigned int t83;
    unsigned int t84;
    unsigned int t85;
    unsigned char t86;
    unsigned char t87;
    char *t88;
    char *t89;
    int t90;
    unsigned int t91;
    unsigned int t92;
    unsigned int t93;
    unsigned char t94;
    unsigned char t95;
    char *t96;
    char *t97;
    int t98;
    unsigned int t99;
    unsigned int t100;
    unsigned int t101;
    unsigned char t102;
    unsigned char t103;
    char *t104;
    char *t105;
    int t106;
    unsigned int t107;
    unsigned int t108;
    unsigned int t109;
    unsigned char t110;
    unsigned char t111;
    char *t112;
    char *t113;
    int t114;
    unsigned int t115;
    unsigned int t116;
    unsigned int t117;
    unsigned char t118;
    unsigned char t119;
    char *t120;
    char *t121;
    int t122;
    unsigned int t123;
    unsigned int t124;
    unsigned int t125;
    unsigned char t126;
    unsigned char t127;
    char *t128;
    char *t129;
    char *t130;
    char *t131;
    char *t132;
    char *t133;

LAB0:    xsi_set_current_line(150, ng0);

LAB3:    t1 = (t0 + 5192U);
    t2 = *((char **)t1);
    t3 = (0 - 7);
    t4 = (t3 * -1);
    t5 = (1U * t4);
    t6 = (0 + t5);
    t1 = (t2 + t6);
    t7 = *((unsigned char *)t1);
    t8 = (t0 + 5192U);
    t9 = *((char **)t8);
    t10 = (1 - 7);
    t11 = (t10 * -1);
    t12 = (1U * t11);
    t13 = (0 + t12);
    t8 = (t9 + t13);
    t14 = *((unsigned char *)t8);
    t15 = ieee_p_2592010699_sub_1605435078_503743352(IEEE_P_2592010699, t7, t14);
    t16 = (t0 + 5192U);
    t17 = *((char **)t16);
    t18 = (2 - 7);
    t19 = (t18 * -1);
    t20 = (1U * t19);
    t21 = (0 + t20);
    t16 = (t17 + t21);
    t22 = *((unsigned char *)t16);
    t23 = ieee_p_2592010699_sub_1605435078_503743352(IEEE_P_2592010699, t15, t22);
    t24 = (t0 + 5192U);
    t25 = *((char **)t24);
    t26 = (3 - 7);
    t27 = (t26 * -1);
    t28 = (1U * t27);
    t29 = (0 + t28);
    t24 = (t25 + t29);
    t30 = *((unsigned char *)t24);
    t31 = ieee_p_2592010699_sub_1605435078_503743352(IEEE_P_2592010699, t23, t30);
    t32 = (t0 + 5192U);
    t33 = *((char **)t32);
    t34 = (4 - 7);
    t35 = (t34 * -1);
    t36 = (1U * t35);
    t37 = (0 + t36);
    t32 = (t33 + t37);
    t38 = *((unsigned char *)t32);
    t39 = ieee_p_2592010699_sub_1605435078_503743352(IEEE_P_2592010699, t31, t38);
    t40 = (t0 + 5192U);
    t41 = *((char **)t40);
    t42 = (5 - 7);
    t43 = (t42 * -1);
    t44 = (1U * t43);
    t45 = (0 + t44);
    t40 = (t41 + t45);
    t46 = *((unsigned char *)t40);
    t47 = ieee_p_2592010699_sub_1605435078_503743352(IEEE_P_2592010699, t39, t46);
    t48 = (t0 + 5192U);
    t49 = *((char **)t48);
    t50 = (6 - 7);
    t51 = (t50 * -1);
    t52 = (1U * t51);
    t53 = (0 + t52);
    t48 = (t49 + t53);
    t54 = *((unsigned char *)t48);
    t55 = ieee_p_2592010699_sub_1605435078_503743352(IEEE_P_2592010699, t47, t54);
    t56 = (t0 + 5192U);
    t57 = *((char **)t56);
    t58 = (7 - 7);
    t59 = (t58 * -1);
    t60 = (1U * t59);
    t61 = (0 + t60);
    t56 = (t57 + t61);
    t62 = *((unsigned char *)t56);
    t63 = ieee_p_2592010699_sub_1605435078_503743352(IEEE_P_2592010699, t55, t62);
    t64 = (t0 + 5352U);
    t65 = *((char **)t64);
    t66 = (0 - 7);
    t67 = (t66 * -1);
    t68 = (1U * t67);
    t69 = (0 + t68);
    t64 = (t65 + t69);
    t70 = *((unsigned char *)t64);
    t71 = ieee_p_2592010699_sub_1605435078_503743352(IEEE_P_2592010699, t63, t70);
    t72 = (t0 + 5352U);
    t73 = *((char **)t72);
    t74 = (1 - 7);
    t75 = (t74 * -1);
    t76 = (1U * t75);
    t77 = (0 + t76);
    t72 = (t73 + t77);
    t78 = *((unsigned char *)t72);
    t79 = ieee_p_2592010699_sub_1605435078_503743352(IEEE_P_2592010699, t71, t78);
    t80 = (t0 + 5352U);
    t81 = *((char **)t80);
    t82 = (2 - 7);
    t83 = (t82 * -1);
    t84 = (1U * t83);
    t85 = (0 + t84);
    t80 = (t81 + t85);
    t86 = *((unsigned char *)t80);
    t87 = ieee_p_2592010699_sub_1605435078_503743352(IEEE_P_2592010699, t79, t86);
    t88 = (t0 + 5352U);
    t89 = *((char **)t88);
    t90 = (3 - 7);
    t91 = (t90 * -1);
    t92 = (1U * t91);
    t93 = (0 + t92);
    t88 = (t89 + t93);
    t94 = *((unsigned char *)t88);
    t95 = ieee_p_2592010699_sub_1605435078_503743352(IEEE_P_2592010699, t87, t94);
    t96 = (t0 + 5352U);
    t97 = *((char **)t96);
    t98 = (4 - 7);
    t99 = (t98 * -1);
    t100 = (1U * t99);
    t101 = (0 + t100);
    t96 = (t97 + t101);
    t102 = *((unsigned char *)t96);
    t103 = ieee_p_2592010699_sub_1605435078_503743352(IEEE_P_2592010699, t95, t102);
    t104 = (t0 + 5352U);
    t105 = *((char **)t104);
    t106 = (5 - 7);
    t107 = (t106 * -1);
    t108 = (1U * t107);
    t109 = (0 + t108);
    t104 = (t105 + t109);
    t110 = *((unsigned char *)t104);
    t111 = ieee_p_2592010699_sub_1605435078_503743352(IEEE_P_2592010699, t103, t110);
    t112 = (t0 + 5352U);
    t113 = *((char **)t112);
    t114 = (6 - 7);
    t115 = (t114 * -1);
    t116 = (1U * t115);
    t117 = (0 + t116);
    t112 = (t113 + t117);
    t118 = *((unsigned char *)t112);
    t119 = ieee_p_2592010699_sub_1605435078_503743352(IEEE_P_2592010699, t111, t118);
    t120 = (t0 + 5352U);
    t121 = *((char **)t120);
    t122 = (7 - 7);
    t123 = (t122 * -1);
    t124 = (1U * t123);
    t125 = (0 + t124);
    t120 = (t121 + t125);
    t126 = *((unsigned char *)t120);
    t127 = ieee_p_2592010699_sub_1605435078_503743352(IEEE_P_2592010699, t119, t126);
    t128 = (t0 + 12552);
    t129 = (t128 + 56U);
    t130 = *((char **)t129);
    t131 = (t130 + 56U);
    t132 = *((char **)t131);
    *((unsigned char *)t132) = t127;
    xsi_driver_first_trans_fast(t128);

LAB2:    t133 = (t0 + 12168);
    *((int *)t133) = 1;

LAB1:    return;
LAB4:    goto LAB2;

}

static void work_a_1353240805_3212880686_p_3(char *t0)
{
    char *t1;
    char *t2;
    unsigned char t3;
    char *t4;
    unsigned char t5;
    unsigned char t6;
    char *t7;
    unsigned char t8;
    unsigned char t9;
    char *t10;
    char *t11;
    char *t12;
    char *t13;
    char *t14;

LAB0:    xsi_set_current_line(155, ng0);

LAB3:    t1 = (t0 + 6312U);
    t2 = *((char **)t1);
    t3 = *((unsigned char *)t2);
    t1 = (t0 + 1512U);
    t4 = *((char **)t1);
    t5 = *((unsigned char *)t4);
    t6 = ieee_p_2592010699_sub_1605435078_503743352(IEEE_P_2592010699, t3, t5);
    t1 = (t0 + 3752U);
    t7 = *((char **)t1);
    t8 = *((unsigned char *)t7);
    t9 = ieee_p_2592010699_sub_1605435078_503743352(IEEE_P_2592010699, t6, t8);
    t1 = (t0 + 12616);
    t10 = (t1 + 56U);
    t11 = *((char **)t10);
    t12 = (t11 + 56U);
    t13 = *((char **)t12);
    *((unsigned char *)t13) = t9;
    xsi_driver_first_trans_fast(t1);

LAB2:    t14 = (t0 + 12184);
    *((int *)t14) = 1;

LAB1:    return;
LAB4:    goto LAB2;

}

static void work_a_1353240805_3212880686_p_4(char *t0)
{
    char *t1;
    char *t2;
    unsigned char t3;
    char *t4;
    char *t5;
    char *t6;
    char *t7;
    char *t8;

LAB0:    xsi_set_current_line(156, ng0);

LAB3:    t1 = (t0 + 6472U);
    t2 = *((char **)t1);
    t3 = *((unsigned char *)t2);
    t1 = (t0 + 12680);
    t4 = (t1 + 56U);
    t5 = *((char **)t4);
    t6 = (t5 + 56U);
    t7 = *((char **)t6);
    *((unsigned char *)t7) = t3;
    xsi_driver_first_trans_fast(t1);

LAB2:    t8 = (t0 + 12200);
    *((int *)t8) = 1;

LAB1:    return;
LAB4:    goto LAB2;

}

static void work_a_1353240805_3212880686_p_5(char *t0)
{
    char *t1;
    char *t2;
    unsigned char t3;
    char *t4;
    char *t5;
    char *t6;
    char *t7;
    char *t8;

LAB0:    xsi_set_current_line(157, ng0);

LAB3:    t1 = (t0 + 6472U);
    t2 = *((char **)t1);
    t3 = *((unsigned char *)t2);
    t1 = (t0 + 12744);
    t4 = (t1 + 56U);
    t5 = *((char **)t4);
    t6 = (t5 + 56U);
    t7 = *((char **)t6);
    *((unsigned char *)t7) = t3;
    xsi_driver_first_trans_fast(t1);

LAB2:    t8 = (t0 + 12216);
    *((int *)t8) = 1;

LAB1:    return;
LAB4:    goto LAB2;

}

static void work_a_1353240805_3212880686_p_6(char *t0)
{
    char *t1;
    char *t2;
    unsigned char t3;
    char *t4;
    char *t5;
    char *t6;
    char *t7;
    char *t8;

LAB0:    xsi_set_current_line(160, ng0);

LAB3:    t1 = (t0 + 7432U);
    t2 = *((char **)t1);
    t3 = *((unsigned char *)t2);
    t1 = (t0 + 12808);
    t4 = (t1 + 56U);
    t5 = *((char **)t4);
    t6 = (t5 + 56U);
    t7 = *((char **)t6);
    *((unsigned char *)t7) = t3;
    xsi_driver_first_trans_fast_port(t1);

LAB2:    t8 = (t0 + 12232);
    *((int *)t8) = 1;

LAB1:    return;
LAB4:    goto LAB2;

}

static void work_a_1353240805_3212880686_p_7(char *t0)
{
    char *t1;
    char *t2;
    unsigned char t3;
    char *t4;
    unsigned char t5;
    unsigned char t6;
    char *t7;
    char *t8;
    char *t9;
    char *t10;
    char *t11;

LAB0:    xsi_set_current_line(162, ng0);

LAB3:    t1 = (t0 + 6632U);
    t2 = *((char **)t1);
    t3 = *((unsigned char *)t2);
    t1 = (t0 + 1832U);
    t4 = *((char **)t1);
    t5 = *((unsigned char *)t4);
    t6 = ieee_p_2592010699_sub_2545490612_503743352(IEEE_P_2592010699, t3, t5);
    t1 = (t0 + 12872);
    t7 = (t1 + 56U);
    t8 = *((char **)t7);
    t9 = (t8 + 56U);
    t10 = *((char **)t9);
    *((unsigned char *)t10) = t6;
    xsi_driver_first_trans_fast(t1);

LAB2:    t11 = (t0 + 12248);
    *((int *)t11) = 1;

LAB1:    return;
LAB4:    goto LAB2;

}

static void work_a_1353240805_3212880686_p_8(char *t0)
{
    char *t1;
    char *t2;
    unsigned char t3;
    unsigned char t4;
    char *t5;
    char *t6;
    char *t7;
    char *t8;
    char *t9;

LAB0:    xsi_set_current_line(163, ng0);

LAB3:    t1 = (t0 + 6952U);
    t2 = *((char **)t1);
    t3 = *((unsigned char *)t2);
    t4 = ieee_p_2592010699_sub_1690584930_503743352(IEEE_P_2592010699, t3);
    t1 = (t0 + 12936);
    t5 = (t1 + 56U);
    t6 = *((char **)t5);
    t7 = (t6 + 56U);
    t8 = *((char **)t7);
    *((unsigned char *)t8) = t4;
    xsi_driver_first_trans_fast(t1);

LAB2:    t9 = (t0 + 12264);
    *((int *)t9) = 1;

LAB1:    return;
LAB4:    goto LAB2;

}

static void work_a_1353240805_3212880686_p_9(char *t0)
{
    char *t1;
    char *t2;
    char *t3;
    char *t4;
    char *t5;
    char *t6;
    char *t7;

LAB0:    xsi_set_current_line(165, ng0);

LAB3:    t1 = (t0 + 2952U);
    t2 = *((char **)t1);
    t1 = (t0 + 13000);
    t3 = (t1 + 56U);
    t4 = *((char **)t3);
    t5 = (t4 + 56U);
    t6 = *((char **)t5);
    memcpy(t6, t2, 8U);
    xsi_driver_first_trans_fast(t1);

LAB2:    t7 = (t0 + 12280);
    *((int *)t7) = 1;

LAB1:    return;
LAB4:    goto LAB2;

}

static void work_a_1353240805_3212880686_p_10(char *t0)
{
    char *t1;
    char *t2;
    unsigned char t3;
    char *t4;
    char *t5;
    char *t6;
    char *t7;
    char *t8;

LAB0:    xsi_set_current_line(169, ng0);

LAB3:    t1 = (t0 + 5832U);
    t2 = *((char **)t1);
    t3 = *((unsigned char *)t2);
    t1 = (t0 + 13064);
    t4 = (t1 + 56U);
    t5 = *((char **)t4);
    t6 = (t5 + 56U);
    t7 = *((char **)t6);
    *((unsigned char *)t7) = t3;
    xsi_driver_first_trans_fast_port(t1);

LAB2:    t8 = (t0 + 12296);
    *((int *)t8) = 1;

LAB1:    return;
LAB4:    goto LAB2;

}

static void work_a_1353240805_3212880686_p_11(char *t0)
{
    char *t1;
    char *t2;
    char *t3;
    int t4;
    char *t5;
    char *t6;
    int t7;
    char *t8;
    int t10;
    char *t11;
    char *t12;
    unsigned char t13;
    char *t14;
    char *t15;
    char *t16;
    char *t17;

LAB0:    xsi_set_current_line(241, ng0);
    t1 = (t0 + 2312U);
    t2 = *((char **)t1);
    t1 = (t0 + 20589);
    t4 = xsi_mem_cmp(t1, t2, 2U);
    if (t4 == 1)
        goto LAB3;

LAB7:    t5 = (t0 + 20591);
    t7 = xsi_mem_cmp(t5, t2, 2U);
    if (t7 == 1)
        goto LAB4;

LAB8:    t8 = (t0 + 20593);
    t10 = xsi_mem_cmp(t8, t2, 2U);
    if (t10 == 1)
        goto LAB5;

LAB9:
LAB6:    xsi_set_current_line(245, ng0);
    t1 = (t0 + 1992U);
    t2 = *((char **)t1);
    t13 = *((unsigned char *)t2);
    t1 = (t0 + 13320);
    t3 = (t1 + 56U);
    t5 = *((char **)t3);
    t6 = (t5 + 56U);
    t8 = *((char **)t6);
    *((unsigned char *)t8) = t13;
    xsi_driver_first_trans_fast(t1);

LAB2:    t1 = (t0 + 12312);
    *((int *)t1) = 1;

LAB1:    return;
LAB3:    xsi_set_current_line(242, ng0);
    t11 = (t0 + 1992U);
    t12 = *((char **)t11);
    t13 = *((unsigned char *)t12);
    t11 = (t0 + 13128);
    t14 = (t11 + 56U);
    t15 = *((char **)t14);
    t16 = (t15 + 56U);
    t17 = *((char **)t16);
    *((unsigned char *)t17) = t13;
    xsi_driver_first_trans_fast(t11);
    goto LAB2;

LAB4:    xsi_set_current_line(243, ng0);
    t1 = (t0 + 1992U);
    t2 = *((char **)t1);
    t13 = *((unsigned char *)t2);
    t1 = (t0 + 13192);
    t3 = (t1 + 56U);
    t5 = *((char **)t3);
    t6 = (t5 + 56U);
    t8 = *((char **)t6);
    *((unsigned char *)t8) = t13;
    xsi_driver_first_trans_fast(t1);
    goto LAB2;

LAB5:    xsi_set_current_line(244, ng0);
    t1 = (t0 + 1992U);
    t2 = *((char **)t1);
    t13 = *((unsigned char *)t2);
    t1 = (t0 + 13256);
    t3 = (t1 + 56U);
    t5 = *((char **)t3);
    t6 = (t5 + 56U);
    t8 = *((char **)t6);
    *((unsigned char *)t8) = t13;
    xsi_driver_first_trans_fast(t1);
    goto LAB2;

LAB10:;
}

static void work_a_1353240805_3212880686_p_12(char *t0)
{
    char *t1;
    unsigned char t2;
    char *t3;
    char *t4;
    unsigned int t5;
    unsigned int t6;
    unsigned int t7;
    char *t8;
    char *t9;
    char *t10;
    char *t11;
    char *t12;

LAB0:    xsi_set_current_line(253, ng0);
    t1 = (t0 + 2592U);
    t2 = ieee_p_2592010699_sub_1744673427_503743352(IEEE_P_2592010699, t1, 0U, 0U);
    if (t2 != 0)
        goto LAB2;

LAB4:
LAB3:    t1 = (t0 + 12328);
    *((int *)t1) = 1;

LAB1:    return;
LAB2:    xsi_set_current_line(254, ng0);
    t3 = (t0 + 5992U);
    t4 = *((char **)t3);
    t5 = (7 - 2);
    t6 = (t5 * 1U);
    t7 = (0 + t6);
    t3 = (t4 + t7);
    t8 = (t0 + 13384);
    t9 = (t8 + 56U);
    t10 = *((char **)t9);
    t11 = (t10 + 56U);
    t12 = *((char **)t11);
    memcpy(t12, t3, 3U);
    xsi_driver_first_trans_fast(t8);
    goto LAB3;

}

static void work_a_1353240805_3212880686_p_13(char *t0)
{
    char *t1;
    unsigned char t2;
    char *t3;
    char *t4;
    unsigned char t5;
    unsigned char t6;
    char *t7;
    char *t8;
    char *t9;
    char *t10;
    char *t11;

LAB0:    xsi_set_current_line(261, ng0);
    t1 = (t0 + 6752U);
    t2 = ieee_p_2592010699_sub_1744673427_503743352(IEEE_P_2592010699, t1, 0U, 0U);
    if (t2 != 0)
        goto LAB2;

LAB4:
LAB3:    t1 = (t0 + 12344);
    *((int *)t1) = 1;

LAB1:    return;
LAB2:    xsi_set_current_line(262, ng0);
    t3 = (t0 + 1032U);
    t4 = *((char **)t3);
    t5 = *((unsigned char *)t4);
    t6 = (t5 == (unsigned char)3);
    if (t6 != 0)
        goto LAB5;

LAB7:
LAB6:    goto LAB3;

LAB5:    xsi_set_current_line(263, ng0);
    t3 = (t0 + 2792U);
    t7 = *((char **)t3);
    t3 = (t0 + 13448);
    t8 = (t3 + 56U);
    t9 = *((char **)t8);
    t10 = (t9 + 56U);
    t11 = *((char **)t10);
    memcpy(t11, t7, 8U);
    xsi_driver_first_trans_fast(t3);
    goto LAB6;

}


extern void work_a_1353240805_3212880686_init()
{
	static char *pe[] = {(void *)work_a_1353240805_3212880686_p_0,(void *)work_a_1353240805_3212880686_p_1,(void *)work_a_1353240805_3212880686_p_2,(void *)work_a_1353240805_3212880686_p_3,(void *)work_a_1353240805_3212880686_p_4,(void *)work_a_1353240805_3212880686_p_5,(void *)work_a_1353240805_3212880686_p_6,(void *)work_a_1353240805_3212880686_p_7,(void *)work_a_1353240805_3212880686_p_8,(void *)work_a_1353240805_3212880686_p_9,(void *)work_a_1353240805_3212880686_p_10,(void *)work_a_1353240805_3212880686_p_11,(void *)work_a_1353240805_3212880686_p_12,(void *)work_a_1353240805_3212880686_p_13};
	xsi_register_didat("work_a_1353240805_3212880686", "isim/controllerTB_isim_beh.exe.sim/work/a_1353240805_3212880686.didat");
	xsi_register_executes(pe);
}
