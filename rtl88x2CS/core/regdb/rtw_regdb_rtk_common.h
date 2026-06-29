/******************************************************************************
 *
 * Copyright(c) 2007 - 2024 Realtek Corporation.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 *****************************************************************************/
#ifndef __RTW_REGDB_RTK_COMMON_H__
#define __RTW_REGDB_RTK_COMMON_H__

#define rtw_is_5g_band1(ch) ((ch) >= 36 && (ch) <= 48)
#define rtw_is_5g_band2(ch) ((ch) >= 52 && (ch) <= 64)
#define rtw_is_5g_band3(ch) ((ch) >= 100 && (ch) <= 144)
#define rtw_is_5g_band4(ch) ((ch) >= 149 && (ch) <= 177)

#define rtw_is_6g_band1(ch) ((ch) >= 1 && (ch) <= 93)
#define rtw_is_6g_band2(ch) ((ch) >= 97 && (ch) <= 117)
#define rtw_is_6g_band3(ch) ((ch) >= 121 && (ch) <= 189)
#define rtw_is_6g_band4(ch) ((ch) >= 193 && (ch) <= 237)

struct ch_list_t {
	u8 *len_ch_attr;
};

#define CLA_2G_12_14_PASSIVE	BIT0

#define CLA_5G_B1_PASSIVE		BIT0
#define CLA_5G_B2_PASSIVE		BIT1
#define CLA_5G_B3_PASSIVE		BIT2
#define CLA_5G_B4_PASSIVE		BIT3
#define CLA_5G_B2_DFS			BIT4
#define CLA_5G_B3_DFS			BIT5
#define CLA_5G_B4_DFS			BIT6

#define CLA_6G_B1_PASSIVE		BIT0
#define CLA_6G_B2_PASSIVE		BIT1
#define CLA_6G_B3_PASSIVE		BIT2
#define CLA_6G_B4_PASSIVE		BIT3

#define CH_LIST_ENT(_len, arg...) \
	{.len_ch_attr = (u8[_len + 2]) {_len, ##arg}, }

#define CH_LIST_LEN(_ch_list) (_ch_list.len_ch_attr[0])
#define CH_LIST_CH(_ch_list, _i) (_ch_list.len_ch_attr[_i + 1])
#define CH_LIST_ATTRIB(_ch_list) (_ch_list.len_ch_attr[CH_LIST_LEN(_ch_list) + 1])

struct chplan_ent_t {
	u8 regd_2g; /* value of enum rtw_regd */
	u8 chd_2g;
#if CONFIG_IEEE80211_BAND_5GHZ
	u8 regd_5g; /* value of enum rtw_regd */
	u8 chd_5g;
#endif
};

#if CONFIG_IEEE80211_BAND_5GHZ
#define CHPLAN_ENT(_regd_2g, _chd_2g, _regd_5g, _chd_5g) {.regd_2g = RTW_REGD_##_regd_2g, .chd_2g = RTW_CHD_2G_##_chd_2g, .regd_5g = RTW_REGD_##_regd_5g, .chd_5g = RTW_CHD_5G_##_chd_5g}
#else
#define CHPLAN_ENT(_regd_2g, _chd_2g, _regd_5g, _chd_5g) {.regd_2g = RTW_REGD_##_regd_2g, .chd_2g = RTW_CHD_2G_##_chd_2g}
#endif

#define CHPLAN_ENT_NOT_DEFINED CHPLAN_ENT(NA, INVALID, NA, INVALID)

struct chplan_6g_ent_t {
	u8 regd; /* value of enum rtw_regd */
	u8 chd;
};

#define CHPLAN_6G_ENT(_regd, _chd) {.regd = RTW_REGD_##_regd, .chd = RTW_CHD_6G_##_chd}

#define CHPLAN_6G_ENT_NOT_DEFINED CHPLAN_6G_ENT(NA, INVALID)

#endif /* __RTW_REGDB_RTK_COMMON_H__ */

