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

#include "xsi.h"

struct XSI_INFO xsi_info;

char *STD_STANDARD;
char *IEEE_P_3499444699;
char *IEEE_P_3564397177;
char *STD_TEXTIO;
char *IEEE_P_3620187407;
char *IEEE_P_2592010699;


int main(int argc, char **argv)
{
    xsi_init_design(argc, argv);
    xsi_register_info(&xsi_info);

    xsi_register_min_prec_unit(-12);
    ieee_p_2592010699_init();
    ieee_p_3499444699_init();
    ieee_p_3620187407_init();
    work_a_1682360212_3212880686_init();
    work_a_2260383155_3212880686_init();
    work_a_3668199810_3212880686_init();
    work_a_3252842415_3212880686_init();
    work_a_3941298598_3212880686_init();
    work_a_0719001517_3212880686_init();
    std_textio_init();
    ieee_p_3564397177_init();
    xilinxcorelib_a_0894114970_2959432447_init();
    xilinxcorelib_a_1331218753_1709443946_init();
    xilinxcorelib_a_2466990523_0543512595_init();
    xilinxcorelib_a_0850875649_3212880686_init();
    work_a_2135660084_2026789854_init();
    work_a_0738797295_3212880686_init();


    xsi_register_tops("work_a_0738797295_3212880686");

    STD_STANDARD = xsi_get_engine_memory("std_standard");
    IEEE_P_3499444699 = xsi_get_engine_memory("ieee_p_3499444699");
    IEEE_P_3564397177 = xsi_get_engine_memory("ieee_p_3564397177");
    STD_TEXTIO = xsi_get_engine_memory("std_textio");
    IEEE_P_3620187407 = xsi_get_engine_memory("ieee_p_3620187407");
    IEEE_P_2592010699 = xsi_get_engine_memory("ieee_p_2592010699");
    xsi_register_ieee_std_logic_1164(IEEE_P_2592010699);

    return xsi_run_simulation(argc, argv);

}
