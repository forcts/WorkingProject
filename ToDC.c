#include "main.h"

void CommuToDCTask(void)
{
    CanTxMessage TxMessage; // ·¢ËÍcanÐÅÏ¢
    static u8 Send = 0;
    u8 Mail, TxTimeOut = 5;
    TxMessage.IDE = CAN_STANDARD_ID;
    TxMessage.RTR = CAN_RTRQ_DATA;
    TxMessage.DLC = 8;
    switch (++Send % 10)
    {
    case 0:
        TxMessage.StdId = 0x11;
        TxMessage.Data[0] = (Global.BMS_INFO.Bat_V & 0xff00) >> 8;
        TxMessage.Data[1] = Global.BMS_INFO.Bat_V & 0x00ff;
        TxMessage.Data[2] = (Global.BMS_INFO.Bat_I & 0xff00) >> 8;
        TxMessage.Data[3] = Global.BMS_INFO.Bat_I & 0x00ff;
        TxMessage.Data[4] = (Global.BMS_INFO.Surplus_Capacity & 0xff00) >> 8;
        TxMessage.Data[5] = Global.BMS_INFO.Surplus_Capacity & 0x00ff;
        TxMessage.Data[6] = (Global.BMS_INFO.Nominal_Capacity & 0xff00) >> 8;
        TxMessage.Data[7] = Global.BMS_INFO.Nominal_Capacity & 0x00ff;
        break;
    case 1:
        TxMessage.StdId = 0x12;
        TxMessage.Data[0] = (Global.BMS_INFO.Cycle_Cnt & 0xff00) >> 8;
        TxMessage.Data[1] = Global.BMS_INFO.Cycle_Cnt & 0x00ff;
        TxMessage.Data[2] = (Global.BMS_INFO.Production_Date & 0xff00) >> 8;
        TxMessage.Data[3] = Global.BMS_INFO.Production_Date & 0x00ff;
        TxMessage.Data[4] = (Global.BMS_INFO.Equalize_Sta_L & 0xff00) >> 8;
        TxMessage.Data[5] = Global.BMS_INFO.Equalize_Sta_L & 0x00ff;
        TxMessage.Data[6] = (Global.BMS_INFO.Equalize_Sta_H & 0xff00) >> 8;
        TxMessage.Data[7] = Global.BMS_INFO.Equalize_Sta_H & 0x00ff;
        break;
    case 2:
        TxMessage.DLC = 7;
        TxMessage.StdId = 0x13;
        TxMessage.Data[0] = (Global.BMS_INFO.Fault_Sta.all & 0xff00) >> 8;
        TxMessage.Data[1] = Global.BMS_INFO.Fault_Sta.all & 0x00ff;
        TxMessage.Data[2] = Global.BMS_INFO.Soft_Ver;
        TxMessage.Data[3] = Global.BMS_INFO.RSOC;
        TxMessage.Data[4] = Global.BMS_INFO.Fet_Sta;
        TxMessage.Data[5] = (Global.BMS_INFO.Alarm_Sta.all & 0xff00) >> 8;
        TxMessage.Data[6] = Global.BMS_INFO.Alarm_Sta.all & 0x00ff;
        break;
    case 3:
        TxMessage.StdId = 0x14;
        TxMessage.Data[0] = (Global.BMS_INFO.Ambient_Temp & 0xff00) >> 8;
        TxMessage.Data[1] = Global.BMS_INFO.Ambient_Temp & 0x00ff;
        TxMessage.Data[2] = (Global.BMS_INFO.Fet_Temp & 0xff00) >> 8;
        TxMessage.Data[3] = Global.BMS_INFO.Fet_Temp & 0x00ff;
        TxMessage.Data[4] = Global.BMS_INFO.NTC_Num;
        TxMessage.Data[5] = Global.BMS_INFO.Cell_Num;
        TxMessage.Data[6] = (Global.BMS_INFO.NTC_Value[0] & 0xff00) >> 8;
        TxMessage.Data[7] = Global.BMS_INFO.NTC_Value[0] & 0x00ff;
        break;
    case 4:
        TxMessage.StdId = 0x15;
        TxMessage.Data[0] = (Global.BMS_INFO.NTC_Value[1] & 0xff00) >> 8;
        TxMessage.Data[1] = Global.BMS_INFO.NTC_Value[1] & 0x00ff;
        TxMessage.Data[2] = (Global.BMS_INFO.NTC_Value[2] & 0xff00) >> 8;
        TxMessage.Data[3] = Global.BMS_INFO.NTC_Value[2] & 0x00ff;
        TxMessage.Data[4] = (Global.BMS_INFO.NTC_Value[3] & 0xff00) >> 8;
        TxMessage.Data[5] = Global.BMS_INFO.NTC_Value[3] & 0x00ff;
        TxMessage.Data[6] = (Global.BMS_INFO.NTC_Value[4] & 0xff00) >> 8;
        TxMessage.Data[7] = Global.BMS_INFO.NTC_Value[4] & 0x00ff;
        break;
    case 5:
        TxMessage.StdId = 0x16;
        TxMessage.Data[0] = (Global.BMS_INFO.Cell_Vol[0] & 0xff00) >> 8;
        TxMessage.Data[1] = Global.BMS_INFO.Cell_Vol[0] & 0x00ff;
        TxMessage.Data[2] = (Global.BMS_INFO.Cell_Vol[1] & 0xff00) >> 8;
        TxMessage.Data[3] = Global.BMS_INFO.Cell_Vol[1] & 0x00ff;
        TxMessage.Data[4] = (Global.BMS_INFO.Cell_Vol[2] & 0xff00) >> 8;
        TxMessage.Data[5] = Global.BMS_INFO.Cell_Vol[2] & 0x00ff;
        TxMessage.Data[6] = (Global.BMS_INFO.Cell_Vol[3] & 0xff00) >> 8;
        TxMessage.Data[7] = Global.BMS_INFO.Cell_Vol[3] & 0x00ff;
        break;
    case 6:
        TxMessage.StdId = 0x17;
        TxMessage.Data[0] = (Global.BMS_INFO.Cell_Vol[4] & 0xff00) >> 8;
        TxMessage.Data[1] = Global.BMS_INFO.Cell_Vol[4] & 0x00ff;
        TxMessage.Data[2] = (Global.BMS_INFO.Cell_Vol[5] & 0xff00) >> 8;
        TxMessage.Data[3] = Global.BMS_INFO.Cell_Vol[5] & 0x00ff;
        TxMessage.Data[4] = (Global.BMS_INFO.Cell_Vol[6] & 0xff00) >> 8;
        TxMessage.Data[5] = Global.BMS_INFO.Cell_Vol[6] & 0x00ff;
        TxMessage.Data[6] = (Global.BMS_INFO.Cell_Vol[7] & 0xff00) >> 8;
        TxMessage.Data[7] = Global.BMS_INFO.Cell_Vol[7] & 0x00ff;
        break;
    case 7:
        TxMessage.StdId = 0x18;
        TxMessage.Data[0] = (Global.BMS_INFO.Cell_Vol[8] & 0xff00) >> 8;
        TxMessage.Data[1] = Global.BMS_INFO.Cell_Vol[8] & 0x00ff;
        TxMessage.Data[2] = (Global.BMS_INFO.Cell_Vol[9] & 0xff00) >> 8;
        TxMessage.Data[3] = Global.BMS_INFO.Cell_Vol[9] & 0x00ff;
        TxMessage.Data[4] = (Global.BMS_INFO.Cell_Vol[10] & 0xff00) >> 8;
        TxMessage.Data[5] = Global.BMS_INFO.Cell_Vol[10] & 0x00ff;
        TxMessage.Data[6] = (Global.BMS_INFO.Cell_Vol[11] & 0xff00) >> 8;
        TxMessage.Data[7] = Global.BMS_INFO.Cell_Vol[11] & 0x00ff;
        break;
    case 8:
        TxMessage.StdId = 0x19;
        TxMessage.Data[0] = (Global.BMS_INFO.Cell_Vol[12] & 0xff00) >> 8;
        TxMessage.Data[1] = Global.BMS_INFO.Cell_Vol[12] & 0x00ff;
        TxMessage.Data[2] = (Global.BMS_INFO.Cell_Vol[13] & 0xff00) >> 8;
        TxMessage.Data[3] = Global.BMS_INFO.Cell_Vol[13] & 0x00ff;
        TxMessage.Data[4] = (Global.BMS_INFO.Cell_Vol[14] & 0xff00) >> 8;
        TxMessage.Data[5] = Global.BMS_INFO.Cell_Vol[14] & 0x00ff;
        TxMessage.Data[6] = (Global.BMS_INFO.Cell_Vol[15] & 0xff00) >> 8;
        TxMessage.Data[7] = Global.BMS_INFO.Cell_Vol[15] & 0x00ff;
        break;
    case 9:
        TxMessage.StdId = 0x1A;
        TxMessage.Data[0] = (Global.BMS_INFO.Cell_Vol[16] & 0xff00) >> 8;
        TxMessage.Data[1] = Global.BMS_INFO.Cell_Vol[16] & 0x00ff;
        TxMessage.Data[2] = (Global.BMS_INFO.Cell_Vol[17] & 0xff00) >> 8;
        TxMessage.Data[3] = Global.BMS_INFO.Cell_Vol[17] & 0x00ff;
        TxMessage.Data[4] = (Global.BMS_INFO.Cell_Vol[18] & 0xff00) >> 8;
        TxMessage.Data[5] = Global.BMS_INFO.Cell_Vol[18] & 0x00ff;
        TxMessage.Data[6] = (Global.BMS_INFO.Cell_Vol[19] & 0xff00) >> 8;
        TxMessage.Data[7] = Global.BMS_INFO.Cell_Vol[19] & 0x00ff;
        break;
    }
    Mail = CAN_Transmit_Message_initializes(CAN, &TxMessage);
    while (CAN_TXSTS_OK != CAN_Transmit_Status_Get(CAN, Mail) && TxTimeOut > 0)
        TxTimeOut--;
}
