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
#define _RTW_REGDB_RTK_COMMON_C_

static const int RTW_ChannelPlanMap_size = sizeof(RTW_ChannelPlanMap) / sizeof(RTW_ChannelPlanMap[0]);

static u8 rtk_regdb_get_default_regd_2g(u8 id)
{
	if (id < RTW_ChannelPlanMap_size)
		return RTW_ChannelPlanMap[id].regd_2g;
	return RTW_REGD_NA;
}

#if CONFIG_IEEE80211_BAND_5GHZ
static u8 rtk_regdb_get_default_regd_5g(u8 id)
{
	if (id < RTW_ChannelPlanMap_size)
		return RTW_ChannelPlanMap[id].regd_5g;
	return RTW_REGD_NA;
}
#endif

static bool rtk_regdb_is_domain_code_valid(u8 id)
{
	if (id < RTW_ChannelPlanMap_size) {
		const struct chplan_ent_t *chplan_map = &RTW_ChannelPlanMap[id];

		if (chplan_map->chd_2g != RTW_CHD_2G_INVALID
			#if CONFIG_IEEE80211_BAND_5GHZ
			&& chplan_map->chd_5g != RTW_CHD_5G_INVALID
			#endif
		)
			return true;
	}

	return false;
}

static bool rtk_regdb_domain_get_ch(u8 id, u32 ch, u8 *flags)
{
	u8 index, attrib;

	if (flags)
		*flags = 0;

#if CONFIG_IEEE80211_BAND_5GHZ
	if (ch > 14) {
		u8 chd_5g = RTW_ChannelPlanMap[id].chd_5g;

		attrib = CH_LIST_ATTRIB(rtw_channel_def_5g[chd_5g]);

		for (index = 0; index < CH_LIST_LEN(rtw_channel_def_5g[chd_5g]); index++) {
			if (CH_LIST_CH(rtw_channel_def_5g[chd_5g], index) == ch) {
				if (flags) {
					if ((rtw_is_5g_band1(ch) && (attrib & CLA_5G_B1_PASSIVE)) /* band1 passive */
						|| (rtw_is_5g_band2(ch) && (attrib & CLA_5G_B2_PASSIVE)) /* band2 passive */
						|| (rtw_is_5g_band3(ch) && (attrib & CLA_5G_B3_PASSIVE)) /* band3 passive */
						|| (rtw_is_5g_band4(ch) && (attrib & CLA_5G_B4_PASSIVE)) /* band4 passive */
					)
						*flags |= RTW_CHF_NO_IR;

					if ((rtw_is_5g_band2(ch) && (attrib & CLA_5G_B2_DFS))
						|| (rtw_is_5g_band3(ch) && (attrib & CLA_5G_B3_DFS))
						|| (rtw_is_5g_band4(ch) && (attrib & CLA_5G_B4_DFS)))
						*flags |= RTW_CHF_DFS;
				}
				return true;
			}
		}
	} else
#endif /* CONFIG_IEEE80211_BAND_5GHZ */
	 {
		u8 chd_2g = RTW_ChannelPlanMap[id].chd_2g;

		attrib = CH_LIST_ATTRIB(rtw_channel_def_2g[chd_2g]);

		for (index = 0; index < CH_LIST_LEN(rtw_channel_def_2g[chd_2g]); index++) {
			if (CH_LIST_CH(rtw_channel_def_2g[chd_2g], index) == ch) {
				if (flags) {
					if (ch >= 12 && ch <= 14 && (attrib & CLA_2G_12_14_PASSIVE))
						*flags |= RTW_CHF_NO_IR;
				}
				return true;
			}
		}
	}

	return false;
}

#if CONFIG_IEEE80211_BAND_6GHZ
static const int rtw_chplan_6g_map_size = sizeof(rtw_chplan_6g_map) / sizeof(rtw_chplan_6g_map[0]);

static u8 rtk_regdb_get_default_regd_6g(u8 id)
{
	if (id < rtw_chplan_6g_map_size)
		return rtw_chplan_6g_map[id].regd;
	return RTW_REGD_NA;
}

static bool rtk_regdb_is_domain_code_6g_valid(u8 id)
{
	if (id < rtw_chplan_6g_map_size) {
		const struct chplan_6g_ent_t *chplan_map = &rtw_chplan_6g_map[id];

		if (chplan_map->chd != RTW_CHD_6G_INVALID)
			return true;
	}

	return false;
}

static bool rtk_regdb_domain_6g_get_ch(u8 id, u32 ch, u8 *flags)
{
	u8 index, attrib;
	u8 chd_6g;

	if (flags)
		*flags = 0;

	chd_6g = rtw_chplan_6g_map[id].chd;

	attrib = CH_LIST_ATTRIB(rtw_channel_def_6g[chd_6g]);

	for (index = 0; index < CH_LIST_LEN(rtw_channel_def_6g[chd_6g]); index++) {
		if (CH_LIST_CH(rtw_channel_def_6g[chd_6g], index) == ch) {
			if (flags) {
				if ((rtw_is_6g_band1(ch) && (attrib & CLA_6G_B1_PASSIVE)) /* band1 passive */
					|| (rtw_is_6g_band2(ch) && (attrib & CLA_6G_B2_PASSIVE)) /* band2 passive */
					|| (rtw_is_6g_band3(ch) && (attrib & CLA_6G_B3_PASSIVE)) /* band3 passive */
					|| (rtw_is_6g_band4(ch) && (attrib & CLA_6G_B4_PASSIVE)) /* band4 passive */
				)
					*flags |= RTW_CHF_NO_IR;
			}
			return true;
		}
	}

	return false;
}
#endif /* CONFIG_IEEE80211_BAND_6GHZ */

static const int country_chplan_map_sz = sizeof(country_chplan_map) / sizeof(country_chplan_map[0]);

static bool rtk_regdb_get_chplan_from_alpha2(const char *alpha2, struct country_chplan *ent)
{
	const struct country_chplan *map = country_chplan_map;
	int i;

	for (i = 0; i < country_chplan_map_sz; i++) {
		if (strncmp(alpha2, map[i].alpha2, 2) == 0) {
			if (ent)
				_rtw_memcpy(ent, &map[i], sizeof(*ent));
			return true;
		}
	}
	return false;
}

#ifdef CONFIG_RTW_DEBUG
static void rtk_regdb_dump_chplan_test(void *sel)
{
	int i, j;

	/* check 2G CHD redundent */
	for (i = RTW_CHD_2G_00; i < RTW_CHD_2G_MAX; i++) {
		for (j = RTW_CHD_2G_00; j < i; j++) {
			if (CH_LIST_LEN(rtw_channel_def_2g[i]) == CH_LIST_LEN(rtw_channel_def_2g[j])
				&& _rtw_memcmp(&CH_LIST_CH(rtw_channel_def_2g[i], 0), &CH_LIST_CH(rtw_channel_def_2g[j], 0), CH_LIST_LEN(rtw_channel_def_2g[i]) + 1) == _TRUE)
				RTW_PRINT_SEL(sel, "2G chd:%u and %u is the same\n", i, j);
		}
	}

	/* check 2G CHD invalid channel */
	for (i = RTW_CHD_2G_00; i < RTW_CHD_2G_MAX; i++) {
		for (j = 0; j < CH_LIST_LEN(rtw_channel_def_2g[i]); j++) {
			if (rtw_bch2freq(BAND_ON_24G, CH_LIST_CH(rtw_channel_def_2g[i], j)) == 0)
				RTW_PRINT_SEL(sel, "2G invalid ch:%u at (%d,%d)\n", CH_LIST_CH(rtw_channel_def_2g[i], j), i, j);
		}
	}

#if CONFIG_IEEE80211_BAND_5GHZ
	/* check 5G CHD redundent */
	for (i = RTW_CHD_5G_00; i < RTW_CHD_5G_MAX; i++) {
		for (j = RTW_CHD_5G_00; j < i; j++) {
			if (CH_LIST_LEN(rtw_channel_def_5g[i]) == CH_LIST_LEN(rtw_channel_def_5g[j])
				&& _rtw_memcmp(&CH_LIST_CH(rtw_channel_def_5g[i], 0), &CH_LIST_CH(rtw_channel_def_5g[j], 0), CH_LIST_LEN(rtw_channel_def_5g[i]) + 1) == _TRUE)
				RTW_PRINT_SEL(sel, "5G chd:%u and %u is the same\n", i, j);
		}
	}

	/* check 5G CHD invalid channel */
	for (i = RTW_CHD_5G_00; i < RTW_CHD_5G_MAX; i++) {
		for (j = 0; j < CH_LIST_LEN(rtw_channel_def_5g[i]); j++) {
			if (rtw_bch2freq(BAND_ON_5G, CH_LIST_CH(rtw_channel_def_5g[i], j)) == 0)
				RTW_PRINT_SEL(sel, "5G invalid ch:%u at (%d,%d)\n", CH_LIST_CH(rtw_channel_def_5g[i], j), i, j);
		}
	}
#endif

#if CONFIG_IEEE80211_BAND_6GHZ
	/* check 6G CHD redundent */
	for (i = RTW_CHD_6G_00; i < RTW_CHD_6G_MAX; i++) {
		for (j = RTW_CHD_6G_00; j < i; j++) {
			if (CH_LIST_LEN(rtw_channel_def_6g[i]) == CH_LIST_LEN(rtw_channel_def_6g[j])
				&& _rtw_memcmp(&CH_LIST_CH(rtw_channel_def_6g[i], 0), &CH_LIST_CH(rtw_channel_def_6g[j], 0), CH_LIST_LEN(rtw_channel_def_6g[i]) + 1) == _TRUE)
				RTW_PRINT_SEL(sel, "6G chd:%u and %u is the same\n", i, j);
		}
	}

	/* check 6G CHD invalid channel */
	for (i = RTW_CHD_6G_00; i < RTW_CHD_6G_MAX; i++) {
		for (j = 0; j < CH_LIST_LEN(rtw_channel_def_6g[i]); j++) {
			if (rtw_bch2freq(BAND_ON_6G, CH_LIST_CH(rtw_channel_def_6g[i], j)) == 0)
				RTW_PRINT_SEL(sel, "6G invalid ch:%u at (%d,%d)\n", CH_LIST_CH(rtw_channel_def_6g[i], j), i, j);
		}
	}
#endif

	/* check chplan 2G_5G redundent */
	for (i = 0; i < RTW_ChannelPlanMap_size; i++) {
		if (!rtw_is_channel_plan_valid(i))
			continue;
		for (j = 0; j < i; j++) {
			if (!rtw_is_channel_plan_valid(j))
				continue;
			if (_rtw_memcmp(&RTW_ChannelPlanMap[i], &RTW_ChannelPlanMap[j], sizeof(RTW_ChannelPlanMap[i])) == _TRUE)
				RTW_PRINT_SEL(sel, "channel plan 0x%02x and 0x%02x is the same\n", i, j);
		}
	}

#if CONFIG_IEEE80211_BAND_6GHZ
	/* check chplan 6G redundent */
	for (i = 0; i < rtw_chplan_6g_map_size; i++) {
		if (!rtw_is_channel_plan_6g_valid(i))
			continue;
		for (j = 0; j < i; j++) {
			if (!rtw_is_channel_plan_6g_valid(j))
				continue;
			if (_rtw_memcmp(&rtw_chplan_6g_map[i], &rtw_chplan_6g_map[j], sizeof(rtw_chplan_6g_map[i])) == _TRUE)
				RTW_PRINT_SEL(sel, "channel plan 6g 0x%02x and 0x%02x is the same\n", i, j);
		}
	}
#endif


	/* check country invalid chplan/chplan_6g */
{
	struct country_chplan ent;
	u8 code[2];

	for (code[0] = 'A'; code[0] <= 'Z'; code[0]++) {
		for (code[1] = 'A'; code[1] <= 'Z'; code[1]++) {
			if (!rtw_get_chplan_from_country(code, &ent))
				continue;
			if (!rtw_is_channel_plan_valid(ent.domain_code))
				RTW_PRINT_SEL(sel, "country \"%c%c\" has invalid domain_code:0x%02X\n", code[0], code[1], ent.domain_code);
			#if CONFIG_IEEE80211_BAND_6GHZ
			if (!rtw_is_channel_plan_6g_valid(ent.domain_code_6g))
				RTW_PRINT_SEL(sel, "country \"%c%c\" has invalid domain_code_6g:0x%02X\n", code[0], code[1], ent.domain_code_6g);
			#endif
		}
	}
}
}
#endif /* CONFIG_RTW_DEBUG */

static void rtk_regdb_get_ver_str(char *buf, size_t buf_len)
{
	snprintf(buf, buf_len, "%s%s"
		"%s%s-%s%s"
		, RTW_MODULE_NAME, strlen(RTW_MODULE_NAME) ? "-" : ""
		, RTW_DOMAIN_MAP_VER, RTW_DOMAIN_MAP_M_VER, RTW_COUNTRY_MAP_VER, RTW_COUNTRY_MAP_M_VER);
}

struct rtw_regdb_ops regdb_ops = {
	.get_default_regd_2g = rtk_regdb_get_default_regd_2g,
#if CONFIG_IEEE80211_BAND_5GHZ
	.get_default_regd_5g = rtk_regdb_get_default_regd_5g,
#endif
	.is_domain_code_valid = rtk_regdb_is_domain_code_valid,
	.domain_get_ch = rtk_regdb_domain_get_ch,

#if CONFIG_IEEE80211_BAND_6GHZ
	.get_default_regd_6g = rtk_regdb_get_default_regd_6g,
	.is_domain_code_6g_valid = rtk_regdb_is_domain_code_6g_valid,
	.domain_6g_get_ch = rtk_regdb_domain_6g_get_ch,
#endif

	.get_chplan_from_alpha2 = rtk_regdb_get_chplan_from_alpha2,

#ifdef CONFIG_RTW_DEBUG
	.dump_chplan_test = rtk_regdb_dump_chplan_test,
#endif
	.get_ver_str = rtk_regdb_get_ver_str,
};

