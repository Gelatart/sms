#ifndef ENEMY_SMALL_ENEMY_HPP
#define ENEMY_SMALL_ENEMY_HPP

#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>
#include <Strategic/Nerve.hpp>

class TCoin;
class TSmallEnemy;
class TJuiceBlock;

class TSmallEnemyParams : public TSpineEnemyParams {
public:
	TSmallEnemyParams(const char*);

	/* 0xA8 */ TParamRT<f32> mSLJumpForce;
	/* 0xBC */ TParamRT<f32> mSLSearchLength;
	/* 0xD0 */ TParamRT<f32> mSLSearchHeight;
	/* 0xE4 */ TParamRT<f32> mSLSearchAware;
	/* 0xF8 */ TParamRT<f32> mSLSearchAngle;
	/* 0x10C */ TParamRT<f32> mSLGiveUpLength;
	/* 0x120 */ TParamRT<f32> mSLGiveUpHeight;
	/* 0x134 */ TParamRT<s32> mSLAttackWait;
	/* 0x148 */ TParamRT<s32> mSLFreezeWait;
	/* 0x15C */ TParamRT<s32> mSLDamageRadius;
	/* 0x170 */ TParamRT<s32> mSLDamageHeight;
	/* 0x184 */ TParamRT<s32> mSLAttackRadius;
	/* 0x198 */ TParamRT<s32> mSLAttackHeight;
	/* 0x1AC */ TParamRT<f32> mSLTurnSpeedLow;
	/* 0x1C0 */ TParamRT<f32> mSLTurnSpeedHigh;
	/* 0x1D4 */ TParamRT<f32> mSLBodyScaleLow;
	/* 0x1E8 */ TParamRT<f32> mSLBodyScaleHigh;
	/* 0x1FC */ TParamRT<f32> mSLGenItemRate;
	/* 0x210 */ TParamRT<f32> mSLGenEggRate;
	/* 0x224 */ TParamRT<u8> mSLPolluteRange;
	/* 0x238 */ TParamRT<s32> mSLWaitTime;
	/* 0x24C */ TParamRT<s32> mSLPolluteRMin;
	/* 0x260 */ TParamRT<s32> mSLPolluteRMax;
	/* 0x274 */ TParamRT<s32> mSLPolluteCycle;
	/* 0x288 */ TParamRT<u8> mSLStampRange;
	/* 0x29C */ TParamRT<s32> mSLPolluteInterval;
	/* 0x2B0 */ TParamRT<u8> mSLGenerateOnlyDead;
	/* 0x2C4 */ f32 unk2C4;
	/* 0x2C8 */ f32 unk2C8;
	/* 0x2CC */ f32 unk2CC;
	/* 0x2D0 */ f32 unk2D0;
};

class TSmallEnemyManager : public TEnemyManager {
public:
	TSmallEnemyManager(const char*);
	void createModelData();
	void load(JSUMemoryInputStream&);
	void loadAfter();
	void createEnemies(int);
	TSmallEnemy* getHolder(int);

	virtual void initSetEnemies();

	// fabricated
	TSmallEnemyParams* getSaveParam() const
	{
		return (TSmallEnemyParams*)unk38;
	}
	u16 getUnk58() const { return unk58; }

	static int mBlockWaitTime;
	static f32 mBlockMoveSpeed;
	static f32 mBlockWaitMoveY;
	static u32 mChangeBlockTime;
	static f32 mBlockXZScale;
	static f32 mBlockYScale;

	static u8 mTestJuiceType;

public:
	/* 0x54 */ char unk54[0x4];
	/* 0x58 */ u16 unk58;
	/* 0x5C */ u32 unk5C;
};

class TSmallEnemy : public TSpineEnemy {
public:
	TSmallEnemy(const char*);

	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void perform(u32, JDrama::TGraphics*);
	virtual BOOL receiveMessage(THitActor*, u32);
	virtual void init(TLiveManager*);
	virtual void moveObject();
	virtual void kill();
	virtual void updateAnmSound();
	virtual void reset();
	virtual void genRandomItem();
	virtual void genEventCoin();
	virtual void generateItem();
	virtual bool isEatenByYosshi();
	virtual void setBehavior();
	virtual void jumpBehavior();
	virtual void behaveToWater(THitActor*);
	virtual bool changeByJuice();
	virtual bool changeMove();
	virtual int getChangeBlockTime();
	virtual void scalingChangeActor();
	virtual void changeOut();
	virtual void behaveToTaken(THitActor*);
	virtual void behaveToRelease();
	virtual void setGenerateAnm();
	virtual void setWalkAnm();
	virtual void setDeadAnm() { }
	virtual void setFreezeAnm();
	virtual void setMeltAnm();
	virtual void setWaitAnm() { }
	virtual void setRunAnm();
	virtual void attackToMario();
	virtual void forceKill();
	virtual void setMActorAndKeeper();
	virtual void initAttacker(THitActor*);
	virtual bool isHitValid(u32);
	virtual bool isCollidMove(THitActor*);
	virtual BOOL isInhibitedForceMove();
	virtual void endHitWaterJump();
	virtual void sendAttackMsgToMario();
	virtual void decHpByWater(THitActor*);
	virtual void setBckAnm(int);
	virtual void setDeadEffect();
	virtual void setAfterDeadEffect();
	virtual void doKeepDistance();
	virtual void generateEffectColumWater();
	virtual bool isFindMario(float);

	bool isMarioInWater() const;
	bool isFindMarioFromParam(float) const;
	void expandCollision();
	bool isEaten();
	bool isHitWallInBound();
	void behaveToHitOthers(THitActor*);

	// fabricated
	TSmallEnemyParams* getSaveParam() const
	{
		return (TSmallEnemyParams*)TSpineEnemy::getSaveParam();
	}
	f32 getUnk158() const { return unk158; }
	u8 getUnk184() const { return unk184; }

	static bool mIsPolluter;
	static bool mIsAmpPolluter;

public:
	/* 0x150 */ u32 unk150;
	/* 0x154 */ f32 unk154;
	/* 0x158 */ f32 unk158;
	/* 0x15C */ u32 unk15C;
	/* 0x160 */ int unk160;
	/* 0x164 */ u8 unk164;
	/* 0x165 */ u8 unk165;
	/* 0x166 */ char unk166[2];
	/* 0x168 */ char unk168[0x174 - 0x168];
	/* 0x174 */ u32 unk174;
	/* 0x178 */ TJuiceBlock* unk178;
	/* 0x17C */ int unk17C;
	/* 0x180 */ TCoin* unk180;
	/* 0x184 */ u8 unk184;
	/* 0x185 */ u8 unk185;
	/* 0x188 */ f32 unk188;
	/* 0x18C */ int unk18C;
	/* 0x190 */ f32 unk190;
};

class TLiveActor;

DECLARE_NERVE(TNerveSmallEnemyDie, TLiveActor);
DECLARE_NERVE(TNerveSmallEnemyFreeze, TLiveActor);
DECLARE_NERVE(TNerveSmallEnemyJump, TLiveActor);
DECLARE_NERVE(TNerveSmallEnemyHitWaterJump, TLiveActor);
DECLARE_NERVE(TNerveSmallEnemyWait, TLiveActor);
DECLARE_NERVE(TNerveSmallEnemyChange, TLiveActor);

#endif
