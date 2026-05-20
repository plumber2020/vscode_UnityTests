#include <stddef.h>
#include "commio.h"
#include "idc.h"

#include "../low_level/can_data.h"
#include "../low_level/msg_defs.h"
#include "../low_level/d_outs_defs.h"

#include "../ctrl_engine/control/variables.h"
#include "../ctrl_engine/control/an_param_model.h"
#include "../ctrl_engine/VK650_control/HLvl_data.h"
#include "../ctrl_engine/control/narabotka.h"
#include "../ctrl_engine/control/muhparam.h"

uint32_t reserve;
extern uint32_t bias_control;
/*!
*@var extern volatile TableOfDataAndStates data_main_table
*@brief Главная таблица переменных
*/
extern volatile TableOfDataAndStates data_states_table;
/*!
*@var extern volatile TableOfDiscreteInputs1stBlock d_inputs_1
*@brief Таблица состояния входных дискретных входов, 1-й блок
*/
extern volatile TableOfDiscreteInputs1stBlock d_inputs_table_1;
/*!
*@var extern volatile TableOfDiscreteInputs2ndBlock d_inputs_2
*@brief Таблица состояния входных дискретных входов, 2-й блок
*/
extern volatile TableOfDiscreteInputs2ndBlock d_inputs_table_2;
/*!
*@var extern volatile TableOfDiscreteInputs3rdBlock d_inputs_3
*@brief Таблица состояния дискретных входов, 3-й блок
*/
extern volatile TableOfDiscreteInputs3rdBlock d_inputs_table_3;

extern volatile tpOUT OUT_can;
extern uint16	Rasr_TR;   //  Разрешение тренировочного режима
extern int16 dP_PPZ, dTg_PPZ, dP_UUZ;

//const MONSTR_t monstr1[] =
//{
//	UW(16),
//	UA(16, &var_grp_1.Ntk_1),
//	SA(32, &var_grp_1.t_g_avr),  //didn`t work
//	UA(0, &EngState),
//	BF(4, 8),
//	UA(0, &d_inputs_table_1.data),
//	BF(3, 0),
//	BF(3, 3),
//	BF(3, 6),
//	BF(3, 9),
//	BF(3, 12),
//	BF(3, 15),
//	BF(3, 18),
//	BF(3, 21),
//	BF(3, 24),
//	BF(3, 27),
//	UA(0, &d_inputs_table_2.data),
//	BF(3, 0),
//	BF(3, 3),
//	BF(3, 6),
//	BF(3, 9),
//	BF(3, 12),
//	BF(3, 15),
//	BF(3, 18),
//	BF(3, 21),
//	BF(3, 24),
//	BF(3, 27),
//	/*
//	UA( 0,&message[0]),
//	BF( 1,31),	 
//	UA( 0,&message[1]),
//	BF( 1,21),
//	*/
//	END_LIST
//};


const MONSTR_t monstr1[] =
{
	UW(16),
	UA(0, &EngState),
	BF(4, 8),
	UA(16, (uint32_t)&INPUTS.Ntk),
	UA(16, (uint32_t)&var_grp_1.Ntk_prog),  
	UA(16, (uint32_t)&var_grp_1.dNtk_req), 
	UA(16, (uint32_t)&var_grp_1.Ntk_min),
	UA(16, (uint32_t)&var_grp_1.Ntk_max),
	UA(16, (uint32_t)&DEFVAL.Ntk_pr),
	UA(16, (uint32_t)&DEFVAL.Ntk_pr_max),
	UA(16, (uint32_t)&INPUTS.Ntk_sos),
	UA(16, (uint32_t)&INPUTS.Nst_dat),
	UA(16, (uint32_t)&DEFVAL.Nst_prog),
	UA(16, (uint32_t)&DEFVAL.Nst_max),
	UA(16, (uint32_t)&INPUTS.Nstr),
	UA(16, (uint32_t)&INPUTS.Nvv),
	UA(16, (uint32_t)&INPUTS.Nnv),
	SA(16, (uint32_t)&AN_PARAMS.Tg1),
	SA(16, (uint32_t)&AN_PARAMS.Tg2),
	SA(16, (uint32_t)&AN_PARAMS.Tg3),
	SA(16, (uint32_t)&AN_PARAMS.Tg4),
	SA(16, (uint32_t)&AN_PARAMS.Tg5),
	SA(16, (uint32_t)&AN_PARAMS.Tg6),
	SA(16, (uint32_t)&var_grp_1.t_g_avr),
	SA(16, (uint32_t)&var_grp_1.reserve18),
	SA(16, (uint32_t)&INPUTS.Tair),
	UA(16, (uint32_t)&INPUTS.Pair),
	UA(16, (uint32_t)&INPUTS.Pk),
	UA(16, (uint32_t)&DEFVAL.Mkr),
	SA(16, (uint32_t)&EXTERNAL.Mkr_sos),	
	UA(16, (uint32_t)&var_grp_1.Mkr_max),
	UA(16, (uint32_t)&DEFVAL.Ne),
	UA(16, (uint32_t)&var_grp_1.Nmax),
	SA(16, (uint32_t)&INPUTS.Lrud),
	SA(16, (uint32_t)&INPUTS.Lrosh),
	SA(16, (uint32_t)&INPUTS.Ldk),
	SA(16, (uint32_t)&var_grp_1.Idk),
	SA(16, (uint32_t)&var_grp_1.Idk_res),
	UA(16, (uint32_t)&var_grp_1.Gt),
	UA(16, (uint32_t)&var_grp_1.Gt_required),
	UA(16, (uint32_t)&var_grp_1.Gt_max),
	UA(16, (uint32_t)&var_grp_1.Gt_min),
	SA(16, (uint32_t)&INPUTS.Tfuel),
	SA(16, (uint32_t)&var_grp_1.t_oil_out),
	UA(16, (uint32_t)&var_grp_1.Poil_in),
	UA(16, (uint32_t)&INPUTS.Pmf),
	UA(16, (uint32_t)&var_grp_1.Xtk1),
	UA(16, (uint32_t)&var_grp_1.Xft1),
	SA(16, (uint32_t)&var_grp_1.current_ch_1),
	SA(16, (uint32_t)&var_grp_1.current_ch_2),
//	SA(16, (uint32_t)&var_grp_1.reserve1),
	SA(16, (uint32_t)&INPUTS.DeltaP),
	SA(16, (uint32_t)&AN_PARAMS.Lskr1),
	SA(16, (uint32_t)&AN_PARAMS.Lskr2),
	SA(16, (uint32_t)&var_grp_1.reserve3),
	SA(16, (uint32_t)&var_grp_1.reserve4),
	SA(16, (uint32_t)&var_grp_1.reserve5),
	SA(16, (uint32_t)&var_grp_1.reserve6),
	SA(16, (uint32_t)&var_grp_1.reserve16),
	SA(16, (uint32_t)&var_grp_1.reserve17),
	SA(16, (uint32_t)&DEFVAL.Tg_max),
	SA(16, (uint32_t)&var_grp_1.Gt_aRUD),
	SA(16, (uint32_t)&var_grp_1.Gt_nk),
	SA(16, (uint32_t)&var_grp_1.Gt_dnkdt),
	SA(16, (uint32_t)&var_grp_1.Gt_nst),
	SA(16, (uint32_t)&var_grp_1.Gt_Tg),
	SA(16, (uint32_t)&var_grp_1.Gt_Mkr_Nst),
	SA(16, (uint32_t)&var_grp_1.Ntk_1),
	SA(16, (uint32_t)&var_grp_1.Ntk_2),
	UA(16, (uint32_t)&EXTERNAL.Ntk_sos),
	SA(16, (uint32_t)&var_grp_1.Ntk_sos_r),
	SA(16, (uint32_t)&var_grp_1.Nnv_1_r),
	SA(16, (uint32_t)&var_grp_1.Nft_1),
	SA(16, (uint32_t)&var_grp_1.Nft_2),
	SA(16, (uint32_t)&var_grp_1.Nvv1_r),
	SA(16, (uint32_t)&var_grp_1.Nvv2_r),
	SA(16, (uint32_t)&var_grp_1.reserve7),
	SA(16, (uint32_t)&var_grp_1.reserve8),
	SA(16, (uint32_t)&var_grp_1.reserve10),
	SA(16, (uint32_t)&var_grp_1.reserve11),
	SA(16, (uint32_t)&var_grp_1.reserve12),
	SA(16, (uint32_t)&var_grp_1.reserve13),
	SA(16, (uint32_t)&var_grp_1.reserve14),
	SA(16, (uint32_t)&var_grp_1.reserve15),
	SA(16, (uint32_t)&var_grp_1.Arud1_raw),
	//SA(16, (uint32_t)&var_grp_1.Arud2_raw),
	SA(16, (uint32_t)&EXTERNAL.Lrud_sos),
	SA(16, (uint32_t)&var_grp_1.t_g_1_r),
	SA(16, (uint32_t)&var_grp_1.t_g_2_r),
	SA(16, (uint32_t)&var_grp_1.t_g_3_r),
	SA(16, (uint32_t)&var_grp_1.t_g_4_r),
	SA(16, (uint32_t)&var_grp_1.t_g_5_r),
	SA(16, (uint32_t)&var_grp_1.t_g_6_r),
	SA(16, (uint32_t)&var_grp_1.t_fuel_in_r),
	SA(16, (uint32_t)&var_grp_2.Tbl_1),
	SA(16, (uint32_t)&var_grp_2.Tbl_2),
	SA(16, (uint32_t)&var_grp_1.Poil_of_r),
	SA(16, (uint32_t)&var_grp_1.Lskr1_r),
	SA(16, (uint32_t)&var_grp_1.Lskr2_r),
	SA(16, (uint32_t)&INPUTS.Lrosh_MU616),
	SA(16, (uint32_t)&var_grp_1.dGt_snchr),
	SA(16, (uint32_t)&AN_PARAMS.Lrosh1),
	SA(16, (uint32_t)&EXTERNAL.Lnv),
	SA(16, (uint32_t)&var_grp_1.Ldi_1_r),
	SA(16, (uint32_t)&var_grp_1.Ldi_2_r),
	SA(16, (uint32_t)&DEFVAL.Ldi_MM),	
	SA(16, (uint32_t)&data_states_table.t_bxd),
	UA(16, (uint32_t)&var_grp_2.Usupply_3),
	UA(16, (uint32_t)&var_grp_2.Usupply_4),
	SA(16, (uint32_t)&DEFVAL.dNtk),
	SA(16, (uint32_t)&data_states_table.Ne_Pk),//SA(16, (uint32_t)&var_grp_1.res_analog),	//var_grp_1.res_analog 
	
	UA(16, (uint32_t)&var_grp_2.CAN1_status),
	UA(16, (uint32_t)&var_grp_2.CAN1_err_over),
	UA(16, (uint32_t)&var_grp_2.CAN1_on_off),
	UA(16, (uint32_t)&var_grp_2.CAN2_status),
	UA(16, (uint32_t)&var_grp_2.CAN2_err_over),
	UA(16, (uint32_t)&var_grp_2.CAN2_on_off),
	UA(16,(uint32_t)&data_states_table.d_t),
	UA(16, (uint32_t)&data_states_table.pack_num),
//	UA(16, (uint32_t)&var_grp_1.CAN1_TX_err_cnt),
//	UA(16, (uint32_t)&var_grp_1.CAN1_RX_err_cnt),
//	UA(16, (uint32_t)&var_grp_1.CAN1_err_type),

	UA(4, &KPA_EngineMode),
	UA(8, &MP.ID),
	UA(1, &MP.IsFault),
	UA(32,&MP.Data),
	UA(0, &d_inputs_table_1.data),
	BF(1, 0), 	// mech_stop_tap:4,					/*Сигнал "Механический стоп-кран закрыт"*/
	BF(1, 4), 	// start_engine:4,					/*Команда "Запуск"*/
	BF(1, 8),	// cold_rotation:4,					/*Команда "Холодная прокрутка"*/
	BF(1, 12),	// board:4,							/*Сигнал "Признак борта". Правый, если лог. 0*/
	BF(1, 16),	// stend:4,							/*Сигнал "Стенд", используется при работе двигателя на стенде*/
	BF(1, 20),	// gear_on_ground:4,				/*Сигнал "Шасси обжато" (ШО)*/
	BF(1, 24),	// stop_engine:4,					/*Команда "Останов"*/
	BF(1, 28),	// small_gas:4;						/*Команда "Малый газ"*/
	UA(0, &d_inputs_table_2.data),
	BF(1, 0),	// reserve_system:4,				/*Команда "Переход на резервную систему"*/
	BF(1, 4),	// workout_right:4,					/*Команда "Тренировка правого двигателя"*/
	BF(1, 8),	// workout_left:4,					/*Команда "Тренировка левого двигателя"*/
	BF(1, 12),	// AGFT_control:4,					/*Команда "АЗСТ контроль"*/
	BF(1, 16),	// AGFT_return:4,					/*Команда "АЗСТ возврат"*/
	BF(1, 20),	// sync_Ntk:4,						/*Команда "Синхронизация Nтк"*/
	BF(1, 24),	// Nft_down_priority:4,				/*Команда "Уменьшение Nст приоритет"*/
	BF(1, 28),	// Nft_up_priority:4;				/*Команда "Увеличение Nст приоритет"*/
	UA(0, &d_inputs_table_3.data),
	BF(1, 0),	// Nft_down_non_priority:4,			/*Команда "Уменьшение Nст не приоритет"*/
	BF(1, 4),	// Nft_up_non_priority:4,			/*Команда "Увеличение Nст не приоритет"*/
	BF(1, 8),	// Voil_min:4,						/*Сигнал "Минимальный запас масла в маслобаке"*/
	BF(1, 12),	// OEI_block:4,						/*Команда "Блокировка ОНД"*/
	BF(1, 16),	// dPgf_max:4,						/*Сигнал "Предельный перепад давления топлива на топливном фильтре"*/
	BF(1, 20),	// SAU_control:4,					/*Команда "Контроль САУ"*/
//	BF(1, 24),	// reserve_ds_1:4,					/*Резерв ДС*/
//	BF(1, 28),	// reserve_ds_2:4;					/*Резерв ДС*/
	UA(1, &OUT.IM_OSGt.value),
	UA(1, &OUT.IM_OSGt1.value),
	UA(1, &OUT.ESK.value),
	UA(1, &OUT.ESK1.value),
	UA(1, &OUT.IGN.value),
	UA(1, &OUT.IGN1.value),
	UA(1, &OUT.STG_ON.value),
	UA(1, &OUT.RCC_Pwr.value),
	UA(1, &OUT.SecondKF_Off.value),
	UA(1, &OUT.SecondKF_Off1.value),
	UA(1, &OUT.ENG_FAIL.value),
	UA(1, &OUT.OGr_On.value),
	UA(1, &OUT.OSGT_On.value),
	UA(1, &OUT.BARK.value),
	UA(1, &OUT.BARK_UP_KEY.value),
	UA(1, &OUT.NEISP.value),
	UA(1, &OUT.DvigRab.value),
	UA(1, &OUT.OEI.value),
	UA(1, &OUT.Trenirovka.value),
	UA(1, &OUT.ROTATES.value),
	UA(1, &OUT.ROTATES1.value),
	UA(1, &INPUTS.SHO_working),
	UA(1, &DEFVAL.IsStend),
	UA(0,&message[0]),
	BF(3, 0),
//	M_EW_NOTWORK = 0,								/* 0 Двигатель выключен (ДВ) */
//	M_EW_WORK,										/* 1 Двигатель работает (ДР) */
//	M_EM_MG,										/* 2 Малый газ (МГ) */
	UA(0,&message[5]),
	BF(1, 30),
//	M_RDY_HP_,										/* 30 Готовность к ХП (ГотХП) */
	UA(0,&message[0]),
	BF(19, 3),
//	M_RDY_OK_,										/* 3 Готовность к запуску (ГотЗап) */
//	M_NOT_RDY_ESK_,									/* 4 К запуску не готов - ЭСК закрыт (НеготЗап ЭСК)*/
//	M_NOT_RDY_NTK_,									/* 5 К запуску не готов - высокая Nтк (НеготЗап nтк) */
//	M_NOT_RDY_OSGT_,								/* 6 К запуску не готов - включение РС (НеготЗап РС) */
//	M_NOT_RDY_K04_,									/* 7 К запуску не готов - отказ прокрутки в ОС (НеготЗап К04) */
//	M_NOT_RDY_RUD_,									/* 8 К запуску не готов - РУД не на МГ (НеготЗап РУД) */
//	M_NOT_RDY_MSKZ_,								/* 9 К запуску не готов - МСКЗ */
//	M_NOT_RDY_MG_,									/* 10 К запуску не готов - нет МГ */
//	M_NOT_RDY_I25_,									/* 11 К запуску не готов - отказ управления системой зажигания (НеготЗап И25) */
//	M_NOT_RDY_I23_,									/* 12 К запуску не готов - отказ управления ЭСК (НеготЗап И23) */
//	M_NOT_RDY_FAULT_,								/* 13 К запуску не готов - неисправность */
//	M_EM_NOMODE,									/* 14 Нет режима */
//	M_EM_STARTING,									/* 15 Запуск идет (Зап идет) */
//	M_EM_COOLSPIN,									/* 16 Холодная прокрутка идет (ХП идет) */
//	M_EM_CONS,										/* 17 Консервация идет (Конс идет) */
//	M_EM_STOP,										/* 18 Останов (аварийный и штатный) (Ост) */
//	M_EM_FAULT_STOP,								/* 19 Аварийный останов (АОст) */
//	M_EM_MP, 										/* 20 Максимальный продолжительный (МП) */
//	M_EM_VZL,										/* 21 Взлет */
	UA(0,&MgRlnFly),
	BF(1, 1),
	UA(0,&message[0]),
	BF(10, 22),
//	M_EM_RPM30,										/* 22 ОНД 30 мин */
//	M_EM_RPM2_5,									/* 23 ОНД 2,5 мин */
//	M_EM_PMG,										/* 24 режим ПМГ */
//	M_EM_OND_MP,									/* 25 режим ОНД МП */
//	M_EM_TR,										/* 26 режим ТР */
//	CC,												/* 27 Срабатывание сигнализатора стружки (СС) */
//	COMB_TURN_OFF,									/* 28 Погасание камеры сгорания (ПогКС) */
//	O_01,											/* 29 Останов по максимальной температуре газов на запуске */
//	O_02,											/* 30 Останов по отсутствию розжига КС на запуске */
//	O_03,											/* 31 Останов по максимальному времени запуска */
	UA(32,&message[1]),
//	O_04,											/* 0 Останов по включению РС Дв на запуске */
//	O_06,											/* 1 Останов по погасанию КС при помпаже */
//	O_07,											/* 2 Останов по срабатыванию АЗСТ */
//	O_08,											/* 3 Останов по нерозжигу КС при АВРР */
//	O_09,											/* 4 Отказ СТГ на запуске */
//	O_10,											/* 5 Отказ маслосистемы */
//	O_12,											/* 6 Останов по отказу прокрутки в ОС */
//	O_13,											/* 7 Останов по нештатной работе МСХ (старший бит) */
//	M_PPZ,											/* 8 Срабатывание ППЗ (противопомпажная защита) (младший бит) */
//	M_AVRR,											/* 9 Срабатывание автоматического восстановления режима работы (АВРР) */
//	rpm_FT,											/* 10 Обороты СТ (срабатывание АЗСТ) */
//	M_WL_Ntk_max,									/* 11 Работа ограничителя Nтк макс */
//	M_WL_Ntk_pr_max,								/* 12 Работа ограничителя Nтк пр макс */
//	M_WL_Tg_max,									/* 13 Работа ограничителя Тг макс */
//	M_WL_Mkr_max,									/* 14 Работа ограничителя Мкр макс */
//	M_WL_Power_max,									/* 15 Работа ограничителя Nе макс */
//	M_WL_Gfuel_max,									/* 16 Работа ограничителя Gt_max */
//	M_WL_Gfuel_min,									/* 17 Работа ограничителя Gt_min */
//	M_WL_Ntk_min,									/* 18 Работа ограничителя Nтк мин */
//	M_WL_MAX,										/* 19 Ограничение максимальног режима */
//	M_WL_MIN,										/* 20 Ограничение минимального режима */
//	WL_FT_ROT,										/* 21 Учебная раскрутка ротора свободной турбины */
//	/* Логические сигналы системы контроля */
//	/* Двигатель */
//	H_11,											/* 22 nтк предельная */
//	H_12,											/* 23 nтк_пр предельная */
//	H_13,											/* 24 tг предельная */
//	H_14,											/* 25 nст предельная */
//	H_15,											/* 26 Мкр предельный */
//	H_17,											/* 27 Проскальзывание МСХ */
//	H_21,											/* 28 Повышенная вибрация по X1тк */
//	H_22,											/* 29 Опасная вибрация по X1тк */
//	H_23,											/* 30 Повышенная вибрация по X1ст */
//	H_24,											/* 31 Опасная вибрация по X1ст */
	UA(32,&message[2]),
//	H_31,											/* 0 Помпаж */
//	H_32,											/* 1 Погасание КС при открытом стоп-кране */
//	H_33,											/* 2 Время запуска велико */
//	H_41,											/* 3 Время выбега ротора турбокомпрессора мало */
//	H_42,											/* 4 Стружка в масле */
//	H_45,											/* 5 Время выбега ротора свободной турбины мало */
//	H_46,											/* 6 Стружка в сигнализаторе */
//	H_47,											/* 7 Отказ сигнализатора стружки */
//	/* САУ */
//	K_01,											/* 8 Отказ основной САУ по управлению двигателем */
//	K_03,											/* 9 Отказ АЗСТ */
//	K_04,											/* 10 Отказ прокрутки в ОС */
//	K_05,											/* 11 Отказ ограничения Мкр */
//	K_06,											/* 12 Отказ ППЗ */
//	K_08,											/* 13 Отказ ограничения nтк */
//	K_09,											/* 14 Отказ ограничения nст */
//	K_10,											/* 15 Отказ ограничения tг */
//	K_11,											/* 16 Отказ ЭЗУ СНП основного модуля */
//	K_12,											/* 17 Отказ ЭЗУ СНП резервного модуля */
//	K_13,											/* 18 Отказ ЭЗУ основного модуля */
//	K_14,											/* 19 Отказ ЭЗУ резервного модуля */
//	K_15,											/* 20 Отказ основного модуля */
//	K_16,											/* 21 Отказ резервного модуля */
//	K_271,											/* 22 Провал уровня напражения питания на входе в БАРК */
//	/* Топливная система */
//	K_32,											/* 23 Высокая температура топлива на входе в топливный фильтр */
//	K_34,											/* 24 Засорение топливного фильтра */
//	/* Исполнительная гидромеханическая часть САУ двигателя */
//	K_43,											/* 25 Отклонение Gт от заданного значения */
//	K_44,											/* 26 Неуправляемость ДК */
//	/* Маслосистема двигателя */
//	K_51,											/* 27 Минимальное давление масла на входе в двигатель */
//	K_53,											/* 28 Маслофильтр засорен */
//	K_54,											/* 29 Клапан перепуска на маслофильтре открыт */
//	K_55,											/* 30 Масла мало */
//	K_57,											/* 31 Велико давление масла на входе в двигатель */

	UA(0,&message[3]),
	BF(10, 0),
//	K_58,											/* 0 Высокая температура масла на выходе из двигателя */
//	K_59,											/* 1 Масло прогрето */
//	K_60,											/* 2 Двигатель прогрет */
//	K_70,											/* 3 Повышенная неравномерность температурного поля */
//	/* Сигналы по времени работы на режиме */
//	K_81,											/* 4 До превышения времени работы на режиме "ОНД 2,5 мин" осталось менее 30 с */
//	K_82,											/* 5 Время непрерывной работы на режиме "ОНД 2,5 мин" превышено */
//	K_83,											/* 6 До превышения времени работы на режиме "ОНД 30 мин" осталось менее 30 с */
//	K_84,											/* 7 Время непрерывной работы на режиме «ОНД 30 мин» превышено */
//	K_85,											/* 8 До превышения времени работы на режиме «Взл» осталось менее 30 с */
//	K_86,											/* 9 Время непрерывной работы на режиме «Взл» превышено */
//	UA(4, &data_states_table.err_Nft_ctrl_1),
	UA(0,&message[5]),
	BF(2, 28),
//	K_87,											/* 28 До превышения времени работы на режиме ОНД МП осталось менее 30 с */
//	K_88,											/* 29 Время непрерывной работы на режиме ОНД МП превышено */
	UA(0,&message[6]),
	BF(23, 0),
//	K_111,											/* 0 Отказ КС структуры настроечных параметров двигателя основного модуля управления */
//	K_112, 											/* 1 Отказ КС структуры настроечных параметров основного модуля управления */
//	K_113,											/* 2 Отказ КС программного обеспечения основного модуля управления */
//	K_114, 											/* 3 Отказ основного модуля управления */
//	K_121,											/* 4 Отказ КС структуры настроечных параметров двигателя резервного модуля управления */
//	K_122, 											/* 5 Отказ КС структуры настроечных параметров резервного модуля управления */
//	K_123,											/* 6 Отказ КС программного обеспечения резервного модуля управления */
//	K_124,					 						/* 7 Отказ резервного модуля управления */
//	K_131,											/* 8 Отказ КС структуры настроечных параметров основного модуля защиты */
//	K_132, 											/* 9 Отказ КС программного обеспечения основного модуля защиты */
//	K_133,											/* 10 Отказ основного модуля защиты */
//	K_141,											/* 11 Отказ КС структуры настроечных параметров резервного модуля защиты */
//	K_142,					 						/* 12 Отказ КС программного обеспечения резервного модуля защиты */
//	K_143,											/* 13 Отказ резервного модуля защиты */
//	K_151,											/* 14 Отказ КС структуры настроечных параметров модуля контроля */
//	K_152, 											/* 15 Отказ КС программного обеспечения модуля контроля */
//	K_153,											/* 16 Отказ модуля контроля */
//	K_161,											/* 17 Отказ КС структуры настроечных параметров первого коммутатора */
//	K_162, 											/* 18 Отказ КС программного обеспечения первого коммутатора */
//	K_163,											/* 19 Отказ первого коммутатора */
//	K_171,											/* 20 Отказ КС структуры настроечных параметров второго коммутатора */
//	K_172, 											/* 21 Отказ КС программного обеспечения второго коммутатора */
//	K_173,											/* 22 Отказ второго коммутатора */
	UA(4,&data_states_table.err_Ntk_1),		//	D_111, Отказ измерения частоты вращения ротора ТК nтк1
	UA(4,&data_states_table.err_Ntk_2),		//	D_112, Отказ измерения частоты вращения ротора ТК nтк2
	UA(4,&data_states_table.err_Ntk_3),		//	D_113, Отказ получения информации по nтк от БАРК сос
	UA(0,&message[3]),
	BF(1, 13),
//	D_117,											/* 13 Отказ nтк по перекрёстному контролю */
	UA(4,&data_states_table.err_Ntk_adj_1),
	UA(0,&message[3]),
	BF(3, 15),
//	D_123,											/* 15 Отказ получения информации по nтк_сос от БАРК сос */
//	D_127,											/* 16 Отказ nтк_сос по перекрёстному контролю */
//	D_13,											/* 17 Отказ измерения частоты вращения ротора СТ */
	UA(4,&data_states_table.err_Nft_1), 	//D_131
	UA(4,&data_states_table.err_Nft_2),		//D_132
	UA(4,&data_states_table.err_Nftr_1),	//D_133
	UA(4,&data_states_table.err_Nftr_2),	//D_134
	UA(4,&data_states_table.err_Ftm_in_1),//D_135
	UA(4,&data_states_table.err_Ftm_in_2),//D_136
	UA(0,&message[3]),
	BF(1, 22),
//	D_137,											/* 22 Отказ nст по перекрёстному контролю */
	UA(4,&data_states_table.err_Nmr_1), //D_14
	UA(4,&data_states_table.err_Tg_1),	//D_311
	UA(4,&data_states_table.err_Tg_2),	//D_321
	UA(4,&data_states_table.err_Tg_3),	//D_331
	UA(4,&data_states_table.err_Tg_4),	//D_341
	UA(4,&data_states_table.err_Tg_5),	//D_351
	UA(4,&data_states_table.err_Tg_6),	//D_361
	UA(4,&data_states_table.err_Tair_in_main),	//D_411
	UA(4,&data_states_table.err_Tair_in_2),			//D_412
	UA(0,&message[4]),
	BF(2, 0),
//	D_417,											/* 0 Отказ tвх по перекрёстному контролю */
//	D_423,											/* 1 Отказ получения информации по tвх сос от БАРК сос */
	UA(4,&data_states_table.err_Tfuel),			//D_441
	UA(4,&data_states_table.err_Pair_in_1),	//D_511
	UA(0,&message[4]),
//	BF(5, 4),
////	D_523,											/* 4 Отказ получения информации по рвх сос от БАРК сос */
////	D_551,											/* 5 Отказ измерения абсолютного давления воздуха за компрессором рк */
////	D_561,											/* 6 Отказ определения угла скручивания, 1-й канал */
////	D_562,											/* 7 Отказ определения угла скручивания, 2-й канал  */
////	D_567,											/* 8 Отказ определения угла скручивания по перекрёстному контролю */
	BF(1, 4),
//	D_523,											/* 4 Отказ получения информации по рвх сос от БАРК сос */
	UA(1,&Rasr_TR),
	UA(0,&message[4]),
	BF(3, 6),	
//	D_551,											/* 5 Отказ измерения абсолютного давления воздуха за компрессором рк */
//	D_561,											/* 6 Отказ определения угла скручивания, 1-й канал */
//	D_562,											/* 7 Отказ определения угла скручивания, 2-й канал  */
//	D_567,											/* 8 Отказ определения угла скручивания по перекрёстному контролю */
	UA(4,&data_states_table.err_Poil_of),	//D_631
	UA(4,&data_states_table.err_TLA_1),		//D_711
//	UA(4,&data_states_table.err_TLA_2),		//D_712
	UA(0,&message[4]),
	BF(1, 12),
//	D_717,											/* 12 Отказ Aруд по перекрёстному контролю */
	UA(4,&data_states_table.err_Acl_1),	//D_911
	UA(4,&data_states_table.err_Acl_2),	//D_912
	UA(0,&message[4]),
	BF(1, 15),
//	D_917,											/* 15 Отказ Aрош по перекрёстному контролю */
	UA(4,&data_states_table.err_Ldi_1),	//D_741
	UA(4,&data_states_table.err_Ldi_2),	//D_742
	UA(0,&message[4]),
	BF(4, 18),
//	D_747,											/* 18 Отказ Lди по перекрёстному контролю */
//	D_811,											/* 19 Отказ измерения виброскорости Х1 */
//	I_011,											/* 20 Отказ исполнительного механизма управления расходом топлива в двигатель, основной канал */
//	I_012,											/* 21 Отказ исполнительного механизма управления расходом топлива в двигатель, резервный канал */
	UA(2,&OUT.IM_OSGt.fault), 			// I_211
	UA(2,&OUT.IM_OSGt1.fault),			// I_212
	UA(2,&OUT.ESK.fault),						// I_231
	UA(2,&OUT.ESK1.fault),					// I_232
	UA(2,&OUT.SecondKF_Off.fault),	// I_241
	UA(2,&OUT.SecondKF_Off1.fault),	// I_242
	UA(2,&OUT.IGN.fault),						// I_251
	UA(2,&OUT.IGN1.fault),					// I_252
	UA(2,&OUT.STG_ON.fault),				// I_261
	UA(2,&OUT.RCC_Pwr.fault),				// I_341
	UA(2,&OUT.DvigRab.fault),		// I_411
	UA(2,&OUT.OGr_On.fault),				// I_421
	UA(2,&OUT.ROTATES.fault),				// I_431
	UA(2,&OUT.ROTATES1.fault),			// I_432
	UA(2,&OUT.OSGT_On.fault),				// I_441
	UA(2,&OUT.OEI.fault),						// I_451
	UA(2,&OUT.ENG_FAIL.fault),			// I_461
	UA(2,&OUT.NEISP.fault),					// I_471
	UA(2,&OUT.BARK_UP_KEY.fault),		// I_481
	UA(2,&OUT.Trenirovka.fault),		// I_491
	UA(2,&OUT.BARK.fault),					// I_501
	UA(2, &data_states_table.rapriz_power),			// I_511
	UA(2, &data_states_table.Pk1_rapriz_power),	// I_551
	UA(2, &data_states_table.Pk2_rapriz_power),	// I_552
	UA(2, &data_states_table.Pmf_rapriz_power), // I_631
	UA(2, &data_states_table.DS_power_MU1),			// I_711
	UA(2, &data_states_table.DS_power_MU2),			// I_712
	UA(2, &data_states_table.DS_power_MZ1),			// I_721
	UA(2, &data_states_table.DS_power_MZ2),			// I_722
	UA(2, &data_states_table.DS_power_MK),			// I_731
	UA(0,&message[5]),
	BF(7, 8),
//	A_02,											/* 8 Отказ передачи в БАРКсос, БХД от БАРК */
//	A_14,											/* 9 Отсутствие информации от БАРКсос в БАРК */
//	A_16,											/* 10 Отсутствие информации от БХД в БАРК */
//	A_21,											/* 11 Отказ обмена БАРК с ИДК */
//	A_11,											/* 12 Отказ обмена БАРК с БИСК */
//	D_141,											/* 13 Отказ измерения частоты вращения НВ Nнв1 */
//	D_142,											/* 14 Отказ измерения частоты вращения НВ Nнв2 */
	UA(4,&data_states_table.err_Pk_1),	// D_551
	UA(4,&data_states_table.err_Pk_2),	// D_552
	UA(0,&message[5]),
	BF(9, 17),
//	K_61,											/* 17 Предельная температура масла на выходе из двигателя */
//	sync_Ntk,										/* 18 Команда "Синхронизация по Nтк" (nтк синхр) */
//	sync_Mkp,										/* 19 Команда "Синхронизация по Mкр" (Мкр синхр) */
//	K_511,											/* 20 Минимальное давление масла на входе в двигатель */
//	left_training_left,								/* 21 Левому тренировка правого (Лев Тр Лев) */
//	right_training_right,							/* 22 Правому тренировка правого (Прав Тр Прав) */
//	right_training_left,							/* 23 Правому тренировка левого (Прав Тр Лев) */
//	H_34,											/* 24 Рукоятка РУД не в нейтрале*/
//	O_11,											/* 25 Останов по АЗТК*/
	UA(2,&valid.Pair),		
	UA(2,&valid.Ntk),
	UA(2,&valid.Nst_dat),
	UA(2,&valid.Nstr),
	UA(2,&valid.Nvv),
	UA(3,&valid.Tg),
	UA(2,&valid.Pk),
	UA(2,&valid.Lskr),
	UA(2,&valid.Lrud),
	UA(2,&valid.Ldk),
	UA(0, &d_inputs_table_1.data),
	BF(3, 1),	// mech_stop_tap:4,					/*Сигнал "Механический стоп-кран закрыт"*/
	BF(3, 5),	// start_engine:4,					/*Команда "Запуск"*/
	BF(3, 9),	// cold_rotation:4,					/*Команда "Холодная прокрутка"*/
	BF(3, 13),	// board:4,							/*Сигнал "Признак борта". Правый, если лог. 0*/
	BF(3, 17),	// stend:4,							/*Сигнал "Стенд", используется при работе двигателя на стенде*/
	BF(3, 21),	// gear_on_ground:4,				/*Сигнал "Шасси обжато" (ШО)*/
	BF(3, 25),	// stop_engine:4,					/*Команда "Останов"*/
	BF(3, 29),	// small_gas:4;						/*Команда "Малый газ"*/
	UA(0, &d_inputs_table_2.data),
	BF(3, 1),	// reserve_system:4,				/*Команда "Переход на резервную систему"*/
	BF(3, 5),	// workout_right:4,					/*Команда "Тренировка правого двигателя"*/
	BF(3, 9),	// workout_left:4,					/*Команда "Тренировка левого двигателя"*/
	BF(3, 13),	// AGFT_control:4,					/*Команда "АЗСТ контроль"*/
	BF(3, 17),	// AGFT_return:4,					/*Команда "АЗСТ возврат"*/
	BF(3, 21),	// sync_Ntk:4,						/*Команда "Синхронизация Nтк"*/
	BF(3, 25),	// Nft_down_priority:4,				/*Команда "Уменьшение Nст приоритет"*/
	BF(3, 29),	// Nft_up_priority:4;				/*Команда "Увеличение Nст приоритет"*/
	UA(0, &d_inputs_table_3.data),
	BF(3, 1),	// Nft_down_non_priority:4,			/*Команда "Уменьшение Nст не приоритет"*/
	BF(3, 5),	// Nft_up_non_priority:4,			/*Команда "Увеличение Nст не приоритет"*/
	BF(3, 9),	// Voil_min:4,						/*Сигнал "Минимальный запас масла в маслобаке"*/
	BF(3, 13),	// OEI_block:4,						/*Команда "Блокировка ОНД"*/
	BF(3, 17),	// dPgf_max:4,						/*Сигнал "Предельный перепад давления топлива на топливном фильтре"*/
	BF(3, 21),	// SAU_control:4,					/*Команда "Контроль САУ"*/
	UA(4, &var_grp_1.kontur),
	UA(1, &DEFVAL.EnableRPM),
	UA(2, &var_grp_1.MZ1_flag),
	UA(2, &var_grp_1.MZ2_flag),
	UA(2, &var_grp_1.MU_other_flag),
	UA(2, &var_grp_1.MK_flag),
	UA(2, &var_grp_1.check_connect_commutator),
//	UA(16, &data_states_table.main_guard_AZTK_AZST_flags),
//	UA(16, &data_states_table.reserve_guard_AZTK_AZST_flags),
	UA(16, &data_states_table.main_AGFT_flags[0]),
	UA(16, &data_states_table.main_AGFT_flags[1]),
	UA(16, &data_states_table.main_AGFT_flags[2]),
	UA(16, &data_states_table.main_AGFT_flags[3]),
	UA(16, &data_states_table.main_AGFT_flags[4]),
//	UA(16, &data_states_table.reserve_AGFT_flags[0]),
//	UA(16, &data_states_table.reserve_AGFT_flags[1]),
//	UA(16, &data_states_table.reserve_AGFT_flags[2]),
//	UA(16, &data_states_table.reserve_AGFT_flags[3]),
//	UA(16, &data_states_table.reserve_AGFT_flags[4]),
	UA(16, (uint32_t)&AN_PARAMS.DeltaP_rez),
	UA(16, (uint32_t)&EXTERNAL.Nst_prog_sos),
  UA(2,&valid.Ntk_sos),
  UA(2,&valid.Lrosh),	
  UA(2,&valid.Nnv),		
  UA(2,&valid.Tair),			
	UA(1, &EXTERNAL.Os_Gt_sos),
	UA(1, &EXTERNAL.ESK_sos),
	UA(1, &EXTERNAL.Otkaz_BARK_sos),
	UA(1, &EXTERNAL.NEISP_sos),
	UA(1, &EXTERNAL.MG_sos),
	UA(1, &EXTERNAL.PPZ_sos),
	UA(1, &EXTERNAL.AVRR_sos),
	UA(1, &EXTERNAL.EW_sos),
	
	UA(1, &EXTERNAL.Ready_To_TR_sos),	
	UA(1, &EXTERNAL.TR_left),
	UA(1, &EXTERNAL.Pogasanie_KS_sos),
	UA(1, &EXTERNAL.Razr_OND_sos),
	UA(1, &EXTERNAL.Ogr_MAX_sos),
	UA(1, &EXTERNAL.Ogr_MIN_sos),
	UA(1, &data_states_table.err_Poil_in_1),  //D_632
	UA(1, &data_states_table.err_Toil_out_1),  //D_451	
	UA(4, &EXTERNAL.prog_upr_sos),
	
//	UA(4, &EXTERNAL.prog_upr_sos),	//резерв!
	UA(1, &data_states_table.dPtf_max_ARINC),
	UA(1, &data_states_table.Ispr_KLS),
	UA(1, &data_states_table.Ispr_inf),
	UA(1, &data_states_table.otkaz_SDS1_flag),
	
	UA(8, &DEFVAL.Flags_Conn_BARK_sos_ch1),
	UA(8, &DEFVAL.Flags_Conn_BARK_sos_ch2),
	
	UA(1, &DEFVAL.f_Test_ARINC_1_OK),
	UA(1, &DEFVAL.f_Test_ARINC_2_OK),	
	UA(1, &DEFVAL.f_Test_CAN_1_OK),
	UA(1, &DEFVAL.f_Test_CAN_2_OK),	
	UA(1, &DEFVAL.f_Is_BHD_Connect),
	UA(2, &data_states_table.snp_from_bhd),								//Признаки для БХД
	UA(2, &data_states_table.optime_from_bhd),
	UA(2, &data_states_table.optime_from_bark),
	UA(2, &data_states_table.snp_from_bark),
	UA(2, &data_states_table.err_snp_from_bhd),
	UA(2, &data_states_table.err_snp_from_bark),
	UA(2, &data_states_table.err_optime_from_bhd),
	UA(2, &data_states_table.err_optime_from_bark),
	UA(0,&message[5]),
	BF(1, 31),	// K17_for_K01
	UA(2, &data_states_table.snp_match),
	UA(2, &data_states_table.optime_match),
	UA(1, &data_states_table.PPZ_IDC),
	UA(1, &data_states_table.AVRR_IDC),
	UA(1,&data_states_table.Mkr_off),
	UA(1,&data_states_table.CheckAZST),
	UA(1,&data_states_table.K01_Imitation),
	UA(16, (uint32_t)&EXTERNAL.Ntk),	
	UA(1,&DEFVAL.IsRazr_PPZ),
	UA(1,&DEFVAL.IsRazr_AVRR),
	UA(2,&data_states_table.resetID),
//	UA(1,&DEFVAL.IsRight),
	UA(0,&message[6]),
	BF(1, 23),//IDK_flag%32),
	BF(1, 24),//WRITE_LOG
	//BF(1,HARD_FAULT%32),
	//UA(1,&data_states_table.IDK_OK),
	//UA(1,&data_states_table.HARD_FAULT),
	//UA(1,&bias_control),												// Получение данных по ARINC429
	UA(1,&DEFVAL.IsRight),
	UA(1,&EXTERNAL.Right_sos),
	UA(0,&message[6]),
	BF(1, 26),
//H_48											/* 26 Отрицательный дельта Pмф*/
//	UA(1,&EXTERNAL.Right_sos), //резерв!	
	END_LIST
};

const MONSTR_t monstr2[] =
{
	UW(16),
	UA(0, &message[0]),
	BF(1, 1),
	UA(0, &EngState),
	BF(4, 8),
	UA(16, (uint32_t)&NRBidk.t_COMMON_h),
	UA(16, (uint32_t)&NRBidk.t_COMMON_m),
	UA(16, (uint32_t)&NRBidk.t_RPM2_5_m),
	UA(16, (uint32_t)&NRBidk.t_RPM2_5_s),
	UA(16, (uint32_t)&NRBidk.t_RPM30_m),
	UA(16, (uint32_t)&NRBidk.t_RPM30_s),
	UA(16, (uint32_t)&NRBidk.t_VZL_h),
	UA(16, (uint32_t)&NRBidk.t_VZL_m),
	UA(16, (uint32_t)&NRBidk.t_MP_h),
	UA(16, (uint32_t)&NRBidk.t_MP_m),
	UA(16, (uint32_t)&NRBidk.t_ONDMP_h),
	UA(16, (uint32_t)&NRBidk.t_ONDMP_m),
	UA(16, (uint32_t)&NRBidk.t_BARK_h),
	UA(16, (uint32_t)&NRBidk.t_BARK_m),
	UA(16, (uint32_t)&NRBidk.n_BARK_h),
	UA(16, (uint32_t)&NRBidk.n_BARK_m),
	UA(16, (uint32_t)&NRBidk.t_RUNOUT),
	UA(16, (uint32_t)&NRBidk.t_START),
	UA(16, (uint32_t)&NARAB.n_OnBARK),
	UA(16, (uint32_t)&NRBidk.n_CYCLE),
	UA(16, (uint32_t)&NARAB.n_RPM2_5),
	UA(16, (uint32_t)&NARAB.n_RPM30),
	UA(16, (uint32_t)&NARAB.n_ONDMP),
	UA(16, (uint32_t)&NARAB.n_START),
	
	UA(16, (uint32_t)&NRBidk.t_REGIM),
	
	END_LIST
};

const MONSTR_t monstr3[] =
{
	UW(16),
	UA(16, (uint32_t)&var_grp_2.ADC0),
	UA(16, (uint32_t)&var_grp_2.ADC1),
	UA(16, (uint32_t)&var_grp_2.ADC2),
	UA(16, (uint32_t)&var_grp_2.ADC3),
	UA(16, (uint32_t)&var_grp_2.ADC4),
	UA(16, (uint32_t)&var_grp_2.ADC5),
	UA(16, (uint32_t)&var_grp_2.ADC6),
	UA(16, (uint32_t)&var_grp_2.ADC7),
	UA(16, (uint32_t)&var_grp_2.ADC_angle_1),
	UA(16, (uint32_t)&var_grp_2.ADC_angle_2),
	UA(16, (uint32_t)&var_grp_2.ADC_angle_3),
	UA(16, (uint32_t)&var_grp_2.ADC_angle_4),
	UA(16, (uint32_t)&var_grp_2.Usupply_1),
	UA(16, (uint32_t)&var_grp_2.Usupply_2),
	SA(16, (uint32_t)&var_grp_2.Tbl_1),
	SA(16, (uint32_t)&var_grp_2.Tbl_2),
	UA(16, (uint32_t)&var_grp_2.Usupply_3),
	UA(16, (uint32_t)&var_grp_2.Usupply_4),
	UA(16, (uint32_t)&var_grp_2.soft_crc),
	UA(16, (uint32_t)&var_grp_2.pss_crc_engine),
	UA(16, (uint32_t)&var_grp_2.pss_crc_module),
	UA(16, (uint32_t)&var_grp_2.soft_crc_MU_other),
	UA(16, (uint32_t)&var_grp_2.pss_crc_engine_MU_other),
	UA(16, (uint32_t)&var_grp_2.pss_crc_module_MU_other),
	UA(16, (uint32_t)&var_grp_2.soft_crc_MZ1),
	UA(16, (uint32_t)&var_grp_2.pss_crc_module_MZ1),
	UA(16, (uint32_t)&var_grp_2.soft_crc_MZ2),
	UA(16, (uint32_t)&var_grp_2.pss_crc_module_MZ2),
	UA(16, (uint32_t)&var_grp_2.soft_crc_MK),
	UA(16, (uint32_t)&var_grp_2.pss_crc_module_MK),
	UA(16, (uint32_t)&var_grp_2.soft_crc_Commutator1),
	UA(16, (uint32_t)&var_grp_2.soft_crc_Commutator2),
	UA(16, (uint32_t)&var_grp_2.adc_ctrl_Gt1_curr),
	SA(16, (uint32_t)&var_grp_2.current_ch_1),
	UA(16, (uint32_t)&var_grp_2.adc_ctrl_Gt1_volt),
	SA(16, (uint32_t)&var_grp_2.voltage_ch_1),

//	UA(16, (uint32_t)&var_grp_2.adc_ctrl_Gt2_curr),
	SA(16, (uint32_t)&var_grp_2.current_ch_2),		/* Поступает по CAN */
//	UA(16, (uint32_t)&var_grp_2.adc_ctrl_Gt2_volt),
//	SA(16, (uint32_t)&var_grp_2.voltage_ch_2),
	
	SA(16, (uint32_t)&var_grp_2.adc_Usupply_1),
	SA(16, (uint32_t)&var_grp_2.adc_Usupply_2),
	UA(16, (uint32_t)&var_grp_2.adc_ctrl_rpms_1),
	UA(16, (uint32_t)&var_grp_2.adc_ctrl_rpms_2),

//	UA(32, (uint32_t)&var_grp_2.adc_ctrl_Gt1_volt),
//	UA(32, (uint32_t)&var_grp_2.adc_ctrl_Gt1_curr),
//	UA(32, (uint32_t)&var_grp_2.temp_3),
//	UA(32, (uint32_t)&var_grp_2.temp_4),
//	UA(32, (uint32_t)&var_grp_2.temp_5),
//	UA(32, (uint32_t)&var_grp_2.temp_6),
//	UA(32, (uint32_t)&var_grp_2.temp_7),
//	UA(32, (uint32_t)&var_grp_2.temp_8),
//	UA(32, (uint32_t)&var_grp_2.temp_9),
//	UA(32, (uint32_t)&var_grp_2.temp_10),

	UA(16, (uint32_t)&var_grp_2.CAN1_status),
	UA(16, (uint32_t)&var_grp_2.CAN1_err_over),
	UA(16, (uint32_t)&var_grp_2.CAN1_on_off),
	UA(16, (uint32_t)&var_grp_2.CAN2_status),
	UA(16, (uint32_t)&var_grp_2.CAN2_err_over),
	UA(16, (uint32_t)&var_grp_2.CAN2_on_off),

	UA(16, (uint32_t)&var_grp_2.temp_11),
	
	UA(8, (uint32_t)&var_grp_2.ARINC429_err_1),
	UA(8, (uint32_t)&var_grp_2.ARINC429_err_2),

//	SA(16, (uint32_t)&var_grp_2.temp_12),
		
	END_LIST
};

