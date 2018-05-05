// PVZHelper.cpp: 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "PVZ_1_0.h"
#include "PVZ_1_2_EN_1073.h"
#include "PVZ_1_2_CN_REPAIR.h"
#include "PVZ_1_2_CN_ENDLESS.h"
#include "stdio.h"
#include "commctrl.h"
#include "DllFromMem.h"

#define exstd(ret) __declspec(dllexport) ##ret __stdcall
#define exstdvoid exstd(void)
//游戏
PVZ* pvz = NULL;
//怪
bool zombies[33];
//常规
extern "C"
{
    //初始化游戏
    exstd(int) InitGame(/*HWND hWnd*/)
    {
        Process process;
        switch (process.OpenByName((char*)"popcapgame1.exe"))
        {
        case NOT_FOUND:
            switch (process.OpenByName((char*)"PlantsVsZombies.exe"))
            {
            case NOT_FOUND:
                //MessageBox(hWnd, "找不到游戏", "错误", MB_OK);
                //break;
                return 1;
            case OPEN_FAILED:
                //MessageBox(hWnd, "打开游戏失败", "错误", MB_OK);
                //break;
                return 2;
            case SUCCESS:
                break;
            }
            break;
        case OPEN_FAILED:
            //MessageBox(hWnd, "打开游戏失败", "错误", MB_OK);
            //break;
            return 2;
        case SUCCESS:
            break;
        }
        if (process.IsValid())
        {
            delete pvz;
            if (process.ReadMemory(4, 0x42748e) == 0xff563de8)
            {
                pvz = new PVZ_1_0(process);
            }
            else if (process.ReadMemory(4, 0x429a83) == 0xffff7489)
            {
                pvz = new PVZ_1_2_EN_1073(process);
            }
            else if (process.ReadMemory(4, 0x428902) == 0x03c109e8)
            {
                pvz = new PVZ_1_2_CN_REPAIR(process);
            }
            else if (process.ReadMemory(4, 0x434d57) == 0x5b5d5e5f)
            {
                pvz = new PVZ_1_2_CN_ENDLESS(process);
            }
            else
            {
                pvz = NULL;
                //MessageBox(hWnd, "不支持的版本", "错误", MB_OK);
                //return;
                return 3;
            }
            pvz->Patch();
            pvz->InitExtraCode();
        }
        return 0;
    }
    //设置阳光
    exstdvoid SetSun(int sun)
    {
        pvz->SetSun(sun);
    }
    //设置金钱，自动乘十
    exstdvoid SetMoney(int money)
    {
        pvz->SetMoney(money);
    }
    //自动收集
    exstdvoid AutoPickup(bool b)
    {
        pvz->AutoPickup(b);
    }
    //无视阳光
    exstdvoid IgnoreSun(bool b)
    {
        pvz->IgnoreSun(b);
    }
    //启动除草机
    exstdvoid StartCar()
    {
        pvz->StartCar();
    }
    //恢复除草机
    exstdvoid ResetCar()
    {
        pvz->ResetCar();
    }
    //禁止删档
    exstdvoid NoDelete(bool b)
    {
        pvz->NoDelete(b);
    }
    //禁止存档
    exstdvoid NoSave(bool b)
    {
        pvz->NoSave(b);
    }
    //自由放置
    exstdvoid Anywhere(bool b)
    {
        pvz->Anywhere(b);
    }
    //允许后台
    exstdvoid NoPause(bool b)
    {
        pvz->NoPause(b);
    }
    //传送带无延迟
    exstdvoid FastBelt(bool b)
    {
        pvz->FastBelt(b);
    }
    //显示隐藏小游戏
    exstdvoid ShowHide(bool b)
    {
        pvz->ShowHide(b);
    }
    //全屏浓雾
    exstdvoid FullFog(bool b)
    {
        pvz->FullFog(b);
    }
    //浓雾透视
    exstdvoid SeeFog(bool b)
    {
        pvz->SeeFog(b);
    }
    //花瓶透视
    exstdvoid SeeVase(bool b)
    {
        pvz->SeeVase(b);
    }
    //种植植物
    exstdvoid Planting(int row, int col, int type, bool copy)
    {
        if (type == 48)
            type = 52;
        pvz->Planting(row, col, type, copy);
    }
    //放置僵尸
    exstdvoid PutZombie(int row, int col, int type)
    {
        if (type == 33)
            type = 36;
        pvz->PutZombie(row, col, type);
    }
    //放梯子
    exstdvoid PutLadder(int row, int col)
    {
        pvz->PutLadder(row, col);
    }
    //放墓碑
    exstdvoid PutGrave(int row, int col)
    {
        pvz->PutGrave(row, col);
    }
    //自动搭梯
    exstdvoid AutoLadder()
    {
        pvz->AutoLadder();
    }
    //设置块
    exstdvoid SetBlock(int row, int col, int type)
    {
        pvz->SetBlock(row, col, type);
    }
    //行判定
    exstdvoid SetRow(int row, int type)
    {
        pvz->SetRow(row, type);
    }
}
//穿越
extern "C"
{
    //生存解密跳关
    exstdvoid JmpLevel(int level)
    {
        pvz->JmpLevel(level);
    }
    //混合
    exstdvoid MixMode(int mode, int lv)
    {
        if (lv == 0)
        {
            lv = 1;
        }
        else if (mode > 42)
        {
            mode++;
            if (mode == 50)
            {
                mode = 60;
            }
            else if (mode == 51)
            {
                mode = 70;
            }
        }
        pvz->MixMode(mode, lv);
    }
    //全解锁
    exstdvoid Unlock(bool b)
    {
        pvz->Unlock(b);
    }
    //直接过关
    exstdvoid Win()
    {
        pvz->Win();
    }
    //锁定阵型
    exstdvoid Lock(bool b, int mode)
    {
        pvz->Lock(b, mode);
    }
    //小僵尸
    exstdvoid Little(bool b)
    {
        pvz->Little(b);
    }
    //下雨
    exstdvoid Rain(bool b)
    {
        pvz->Rain(b);
    }
    //暴风雨
    exstdvoid Storm(bool b)
    {
        pvz->Storm(b);
    }
    //引力
    exstdvoid Force(bool b)
    {
        pvz->Force(b);
    }
    //像柱子一样
    exstdvoid LikeCol(bool b)
    {
        pvz->LikeCol(b);
    }
    //僵尸快跑
    exstdvoid RunFast(bool b)
    {
        pvz->RunFast(b);
    }
    //启动传送门
    exstdvoid StartDoor(bool b)
    {
        pvz->StartDoor(b);
    }
    //固定传送门
    exstdvoid StableDoor(bool b)
    {
        pvz->StableDoor(b);
    }
    //生成传送门
    exstdvoid SetDoor(int b1r, int b1c, int b2r, int b2c, int w1r, int w1c, int w2r, int w2c)
    {
        pvz->SetDoor(b1r, b1c, b2r, b2c, w1r, w1c, w2r, w2c);
    }
}
//植物
extern "C"
{
	//玉米炮
	exstdvoid CornNoWait(bool b)
	{
		pvz->CornNoWait(b);
	}
	//磁力菇
	exstdvoid MagnetNoWait(bool b)
	{
		pvz->MagnetNoWait(b);
	}
	//土豆雷
	exstdvoid PotatoNoWait(bool b)
	{
		pvz->PotatoNoWait(b);
	}
	//大嘴花
	exstdvoid MouthNoWait(bool b)
	{
		pvz->MouthNoWait(b);
	}
	//强壮
	exstdvoid PStrong()
	{
		pvz->PStrong();
	}
	//正常
	exstdvoid PNormal()
	{
		pvz->PNormal();
	}
	//虚弱
	exstdvoid PWeak()
	{
		pvz->PWeak();
	}
    //毁灭菇无痕
    exstdvoid NoMark(bool b)
    {
        pvz->NoMark(b);
    }
    //蘑菇免唤醒
    exstdvoid NoSleep(bool b)
    {
        pvz->NoSleep(b);
    }
    //强三叶草
    exstdvoid StrongWind(bool b)
    {
        pvz->StrongWind(b);
    }
    //强豌豆
    exstdvoid StrongBean(bool b)
    {
        pvz->StrongBean(b);
    }
    //保护伞
    exstdvoid Umbrella(bool b)
    {
        pvz->Umbrella(b);
    }
    //无冷却
    exstdvoid NoCD(bool b)
    {
        pvz->NoCD(b);
    }
    //铲子锁定
    exstdvoid LockScoop(bool b)
    {
        pvz->LockScoop(b);
    }
    //宝石随心移
    exstdvoid AnyMove(bool b)
    {
        pvz->AnyMove(b);
    }
    //紫卡无限制
    exstdvoid NoPurple(bool b)
    {
        pvz->NoPurple(b);
    }
    //锁定黄油
    exstdvoid LockButter(bool b)
    {
        pvz->LockButter(b);
    }
    //锁定玉米
    exstdvoid LockCorn(bool b)
    {
        pvz->LockCorn(b);
    }
    //爆炸全屏
    exstdvoid FSBoom(bool b)
    {
        pvz->FSBoom(b);
    }
    //窝瓜全屏
    exstdvoid FSSquash(bool b)
    {
        pvz->FSSquash(b);
    }
    //磁力菇全屏
    exstdvoid FSMagnet(bool b)
    {
        pvz->FSMagnet(b);
    }
    //溅射全屏
    exstdvoid FSSputter(bool b)
    {
        pvz->FSSputter(b);
    }
    //大嘴花全屏
    exstdvoid FSMouth(bool b)
    {
        pvz->FSMouth(b);
    }
    //地刺全屏
    exstdvoid FSPrick(bool b)
    {
        pvz->FSPrick(b);
    }
    //喷射全屏
    exstdvoid FSSpout(bool b)
    {
        pvz->FSSpout(b);
    }
    //猫式全屏
    exstdvoid FSCat(bool b)
    {
        pvz->FSCat(b);
    }
    //触碰伤害
    exstdvoid CatTouch(bool b)
    {
        pvz->CatTouch(b);
    }
}
//僵尸
extern "C"
{
    //立即爆炸
    exstdvoid BoomIm()
    {
        pvz->BoomIm();
    }
    //正常
    exstdvoid BoomNormal()
    {
        pvz->BoomNormal();
    }
    //不爆炸
    exstdvoid BoomNever()
    {
        pvz->BoomNever();
    }
    //隐形
    exstdvoid VisiableNo()
    {
        pvz->VisiableNo();
    }
    //正常
    exstdvoid VisiableNormal()
    {
        pvz->VisiableNormal();
    }
    //可见
    exstdvoid VisiableYes()
    {
        pvz->VisiableYes();
    }
    //无敌
    exstdvoid ZStrong()
    {
        pvz->ZStrong();
    }
    //正常
    exstdvoid ZNormal()
    {
        pvz->ZNormal();
    }
    //虚弱
    exstdvoid ZWeak()
    {
        pvz->ZWeak();
    }
    //不减速
    exstdvoid NoSlow(bool b)
    {
        pvz->NoSlow(b);
    }
    //黄油
    exstdvoid NoButter(bool b)
    {
        pvz->NoButter(b);
    }
    //屏蔽3人组
    exstdvoid No3zGroup(bool b)
    {
        pvz->No3zGroup(b);
    }
    //不扔小鬼
    exstdvoid NoIMP(bool b)
    {
        pvz->NoIMP(b);
    }
    //冰车无痕
    exstdvoid NoICE(bool b)
    {
        pvz->NoICE(b);
    }
    //巨人可被吃
    exstdvoid ZEatable(bool b)
    {
        pvz->ZEatable(b);
    }
    //暂停刷僵尸
    exstdvoid StopZTimer(bool b)
    {
        pvz->StopZTimer(b);
    }
    //僵尸不前进
    exstdvoid StopZombies(bool b)
    {
        pvz->StopZombies(b);
    }
    //定BOSS
    exstdvoid StopBoss(bool b)
    {
        pvz->StopBoss(b);
    }
    //气球自爆
    exstdvoid BalloonBoom(bool b)
    {
        pvz->BalloonBoom(b);
    }
    //加速刷僵尸
    exstdvoid ZTimerSpeed(int value)
    {
        pvz->ZTimerSpeed(value);
    }
    //吸怪
    exstdvoid GatherZombie(bool b, float pos)
    {
        pvz->GatherZombie(b, pos);
    }
    //秒杀
    exstdvoid KillAllZombies()
    {
        pvz->KillAllZombies();
    }
    //魅惑
    exstdvoid ConfuseZombies()
    {
        pvz->ConfuseZombies();
    }
}
//调节
extern "C"
{
    //卡槽数量
    exstdvoid SetCardNum(int num)
    {
        pvz->SetCardNum(num);
    }
    //修改卡片
    exstdvoid SetCard(int cid, int type, bool copy)
    {
        if (type == 48)
            type = 52;
        pvz->SetCard(cid, type, copy);
    }
    //自然出怪
    exstdvoid NaturalSeed(int* zombies, int znum)
    {
        pvz->NaturalSeed(zombies, znum);
    }
    //极限出怪
    exstdvoid LimitSeed(int* zombies, int znum, bool thief, bool red)
    {
        pvz->LimitSeed(zombies, znum, thief, red);
    }
    //恢复初始
    exstdvoid ResetSeed()
    {
        pvz->ResetSeed();
    }
    //初始化查看剩余
    exstdvoid SeeLeft()
    {
        pvz->SeeLeftZombies(zombies);
    }
    //查看剩余
    exstd(bool) SeeLeftZombie(int index)
    {
        if (index < 33 && index >= 0)
        {
            return zombies[index];
        }
        else
        {
            return false;
        }
    }
}
//花园
extern "C"
{
    //花肥
    exstdvoid FlowerNPK(bool b)
    {
        pvz->FlowerNPK(b);
    }
    //杀虫剂
    exstdvoid Medicine(bool b)
    {
        pvz->Medicine(b);
    }
    //巧克力
    exstdvoid Chocolate(bool b)
    {
        pvz->Chocolate(b);
    }
    //树肥
    exstdvoid TreeNPK(bool b)
    {
        pvz->TreeNPK(b);
    }
    //树高
    exstdvoid TreeHeight(int h)
    {
        pvz->TreeHeight(h);
    }
    //花数
    exstdvoid FlowerNum(int n)
    {
        pvz->FlowerNum(n);
    }
    //改花
    exstdvoid ChangeFlower(int fid, int type)
    {
        pvz->ChangeFlower(fid, type);
    }
    //改花大小
    exstdvoid ChangeFSize(int fid, int size)
    {
        pvz->ChangeFSize(fid, size);
    }
    //改花方向
    exstdvoid ChangeFDir(int fid, int dir)
    {
        pvz->ChangeFDir(fid, dir);
    }
    //改花位置
    exstdvoid ChangeFPlace(int fid, int place)
    {
        pvz->ChangeFPlace(fid, place);
    }
    //获取花数量
    exstd(int) GetFlowerNum()
    {
        return pvz->GetFlowerNum();
    }
    //花种类
    exstd(int) GetFlower(int fid)
    {
        return pvz->GetFlower(fid);
    }
    //花大小
    exstd(int) GetFSize(int fid)
    {
        return pvz->GetFSize(fid);
    }
    //花方向
    exstd(int) GetFDir(int fid)
    {
        return pvz->GetFDir(fid);
    }
    //花位置
    exstd(int) GetFPlace(int fid)
    {
        return pvz->GetFPlace(fid);
    }
}