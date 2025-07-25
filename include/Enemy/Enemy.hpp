#ifndef ENEMY_ENEMY_HPP
#define ENEMY_ENEMY_HPP

#include <Strategic/LiveActor.hpp>
#include <Strategic/SolidStack.hpp>
#include <Enemy/PathNode.hpp>

class TGraphTracer;
class TSpineEnemyParams;

class TSpineEnemy : public TLiveActor {
public:
	TSpineEnemy(const char*);
	~TSpineEnemy();

	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32, JDrama::TGraphics*);
	virtual BOOL receiveMessage(THitActor*, u32);
	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual void reset();
	virtual void resetToPosition(const JGeometry::TVec3<f32>&);
	virtual void resetSRTV(const JGeometry::TVec3<f32>&,
	                       const JGeometry::TVec3<f32>&,
	                       const JGeometry::TVec3<f32>&,
	                       const JGeometry::TVec3<f32>&);
	virtual TSpineEnemyParams* getSaveParam() const;
	virtual void getPhaseShift() const;
	virtual BOOL isReachedToGoal() const { }

	void calcEnemyRootMatrix();
	f32 calcMinimumTurnRadius(f32, f32) const;
	void calcTurnSpeedToReach(f32, f32) const;
	void updateSquareToMario();
	BOOL isInSight(const JGeometry::TVec3<f32>&, f32, f32, f32) const;
	void setGoalPathFromGraph();
	void goToInitialVisibleNode(f32, f32);
	void goToInitialGraphNodeCheckY(f32);
	void goToShortestNextGraphNode();
	int jumpToNextGraphNode();
	void goToRandomNextGraphNode();
	void goToRandomEscapeGraphNode();
	void goToExclusiveNextGraphNode();
	void goToDirectedNextGraphNode(const JGeometry::TVec3<f32>&);
	void goToDirLimitedNextGraphNode(f32);
	void updateStayCount(f32);
	void turnToCurPathNode(f32);
	void walkToCurPathNode(f32, f32, f32);
	void zigzagToCurPathNode(f32, f32, f32, f32);
	void doShortCut();
	void searchNearestBrother() const;
	f32 getCurAnmFrameNo(int) const;
	BOOL checkCurAnmEnd(int) const;

	// fabricated
	f32 getUnk134() const { return unk134; }
	f32 getMarchSpeed() const { return mMarchSpeed; }
	f32 getTurnSpeed() const { return mTurnSpeed; }

public:
	/* 0xF4 */ THitActor* unkF4; // TODO: type is a wild guess
	/* 0xF8 */ JGeometry::TVec3<f32> unkF8;
	/* 0x104 */ THitActor* unk104; // TODO: type is a wild guess
	/* 0x108 */ JGeometry::TVec3<f32> unk108;
	/* 0x114 */ TSolidStack<TPathNode> unk114;
	/* 0x124 */ TGraphTracer* unk124;
	/* 0x128 */ u16 unk128;
	/* 0x12C */ f32 unk12C;
	/* 0x130 */ s8 unk130;
	/* 0x134 */ f32 unk134;
	/* 0x138 */ const TBGCheckData* unk138; // TODO: type is a wild guess
	/* 0x13C */ u8 mHitPoints;
	/* 0x140 */ f32 mMarchSpeed;
	/* 0x144 */ f32 mTurnSpeed;
	/* 0x148 */ f32 mBodyScale;
	/* 0x14C */ f32 unk14C;
};

#endif
