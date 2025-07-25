#include <Enemy/EnemyManager.hpp>
#include <Enemy/Conductor.hpp>
#include <Enemy/Enemy.hpp>
#include <System/TimeRec.hpp>
#include <MSound/MAnmSound.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/Strategy.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/SDLModel.hpp>
#include <JSystem/JDrama/JDRViewObjPtrList.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DTransform.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

TSpineEnemyParams::TSpineEnemyParams(const char* path)
    : TParams(path)
    , PARAM_INIT(mSLHeadHeight, 120.0f)
    , PARAM_INIT(mSLBodyRadius, 30.0f)
    , PARAM_INIT(mSLWallRadius, 50.0f)
    , PARAM_INIT(mSLClipRadius, 300.0f)
    , PARAM_INIT(mSLFarClip, 10000.0f)
    , PARAM_INIT(mSLHitPointMax, 1)
    , PARAM_INIT(mSLInstanceNum, 100)
    , PARAM_INIT(mSLActiveEnemyNum, 10)
{
	TParams::load(mPrmPath);
}

void TSharedMActorSet::init(MActorAnmData* param_1, J3DModelData* param_2,
                            const char* param_3, int param_4)
{
	unk4 = param_4;
	unk0 = new MActor*[unk4];

	f32 coeff = 1.0f / unk4;
	for (int i = 0; i < unk4; ++i) {
		J3DModel* model = new J3DModel(param_2, 0, 1);
		unk0[i]         = new MActor(param_1);
		unk0[i]->setModel(model, 0);
		unk0[i]->setBck(param_3);
		J3DFrameCtrl* ctrl = unk0[i]->getFrameCtrl(0);
		ctrl->setFrame(coeff * ctrl->getEndFrame() * i);
	}

	unk8 = unk0[0]->getCurAnmIdx(0);
}

void TSharedMActorSet::calcAnm() { }

void TSharedMActorSet::setScale(const JGeometry::TVec3<f32>&) { }

TEnemyManager::TEnemyManager(const char* name)
    : TLiveManager(name)
    , unk38(nullptr)
    , unk3C(1.0f)
    , unk40(nullptr)
    , unk44(0)
    , unk48(0)
    , unk4C(0xffffffff)
    , unk50(0)
{
	gpConductor->registerEnemyManager(this);
}

TEnemyManager::~TEnemyManager() { }

void TEnemyManager::createSharedMActorSet(const char** param_1)
{
	if (unk44 <= 0)
		return;

	u32 num = 0;
	for (int i = 0; param_1[i] != nullptr; ++i)
		++num;

	// TODO: ewwwwwwwwwwww
	u32 prev = (volatile int&)unk44;
	unk44    = num;
	unk40    = new TSharedMActorSet[unk44];

	for (int i = 0; i < unk44; ++i) {
		unk40[i].init(getMActorAnmData(),
		              getModelDataKeeper()->getNthData(0)->getModelData(),
		              param_1[i], prev);
	}
}

TSharedMActorSet* TEnemyManager::getSharedMActorSet(int idx)
{
	if (!unk40)
		return nullptr;

	for (int i = 0; i < unk44; ++i)
		if (idx == unk40[i].getIdx())
			return &unk40[i];

	return nullptr;
}

void TEnemyManager::load(JSUMemoryInputStream& stream)
{
	TLiveManager::load(stream);
	createModelData();
	stream.read(&unk44, 4);
}

TSpineEnemy* TEnemyManager::createEnemyInstance() { return nullptr; }

void TEnemyManager::createEnemy() { }

void TEnemyManager::createEnemies(int param_1)
{
	if (param_1 + unk14 > unk10)
		param_1 = unk10 - unk14;

	if (unk38 != nullptr && param_1 + unk14 > unk38->mSLInstanceNum.get())
		param_1 = unk38->mSLInstanceNum.get() - unk14;

	if (param_1 < 0)
		return;

	for (int i = 0; i < param_1; ++i) {
		TSpineEnemy* enemy = createEnemyInstance();

		if (!enemy)
			continue;

		JDrama::TNameRefGen::search<TIdxGroupObj>("敵グループ")->add(enemy);

		enemy->init(this);
	}
}

void TEnemyManager::clipEnemies(JDrama::TGraphics* graphics)
{
	f32 fVar1;
	f32 fVar2;
	if (unk38 == nullptr) {
		fVar1 = 300.0f;
		fVar2 = gpConductor->unk84.mEnemyFarClip.get();
	} else {
		fVar2 = unk38->mSLFarClip.get();
		fVar1 = unk38->mSLClipRadius.get();
	}

	TLiveManager::clipActorsAux(graphics, fVar2, fVar1);
}

void TEnemyManager::setSharedFlags()
{
	if (!unk40)
		return;

	for (int i = 0; i < objNum(); ++i) {
		TSpineEnemy* enemy = getObj(i);
		enemy->offLiveFlag(0x4000);
		if (!enemy->checkLiveFlag(0x6)) {
			int idx = enemy->getUnk74()->getCurAnmIdx(0);
			for (int i = 0; i < unk44; ++i) {
				if (idx < 0 || idx == unk40[i].getIdx()) {
					enemy->onLiveFlag(0x4000);
					break;
				}
			}
		}
	}
}

void TEnemyManager::updateAnmSoundShared()
{
	if (!unk40)
		return;

	if (!getObj(0)->getUnk80())
		return;

	for (int i = 0; i < objNum(); ++i) {
		TSpineEnemy* enemy = getObj(i);
		if (!enemy->checkLiveFlag(0x6)) {
			int idx = enemy->getUnk74()->getCurAnmIdx(0);
			for (int i = 0; i < unk44; ++i) {
				if (idx < 0 || idx == unk40[i].getIdx()) {
					// unused (mistake)
					J3DFrameCtrl* ctrl2 = unk40[i]
					                          .getMActor(enemy->getUnk7C())
					                          ->getFrameCtrl(0);
					if (enemy->unk84) {
						J3DFrameCtrl* ctrl = unk40[i]
						                         .getMActor(enemy->getUnk7C())
						                         ->getFrameCtrl(0);

						enemy->getUnk80()->animeLoop(
						    (Vec*)&enemy->getPosition(),
						    ctrl->getCurrentFrame(), ctrl->getRate(), 0, 4);
					}
					break;
				}
			}
		}
	}
}

void TEnemyManager::copyFromShared()
{
	Mtx afStack_88;
	MTXCopy(j3dSys.getViewMtx(), afStack_88);

	s32 r29 = getActiveObjNum();

	for (int i = 0; i < r29; ++i) {
		TSpineEnemy* enemy = getObj(i);
		if (!enemy->checkLiveFlag(0x4000) || enemy->checkLiveFlag(6))
			continue;

		int iVar6 = enemy->getUnk74()->getCurAnmIdx(0);
		for (int j = 0; j < unk44; ++j) {
			if (iVar6 >= 0 && iVar6 != unk40[j].unk8)
				continue;

			J3DModel* model = unk40[j].getMActor(enemy->getUnk7C())->getUnk4();
			MtxPtr src      = enemy->getModel()->getBaseTRMtx();
			MTXScaleApply(src, src, enemy->mScaling.x, enemy->mScaling.y,
			              enemy->mScaling.z);
			Mtx afStack_58;
			MTXConcat(afStack_88, src, afStack_58);
			j3dSys.setViewMtx(afStack_58);

			model->viewCalc();

			enemy->getModel()->swapAllMtx();
			enemy->getModel()->calcNrmMtx();
			enemy->getModel()->prepareShapePackets();

			J3DPSMtxArrayCopy(*model->getDrawMtxPtr(),
			                  *enemy->getModel()->getDrawMtxPtr(),
			                  model->getModelData()->getDrawMtxNum());

			DCStoreRange(enemy->getModel()->getDrawMtxPtr(),
			             model->getModelData()->getDrawMtxNum() * sizeof(Mtx));

			break;
		}
	}

	j3dSys.setViewMtx(afStack_88);
}

void TEnemyManager::performShared(u32 param_1, JDrama::TGraphics* param_2)
{
	TTimeRec::startTimer(0xffffffff);

	int num2     = getActiveObjNum();
	int aliveNum = 0;
	for (int i = 0; i < num2; ++i)
		if (!getObj(i)->checkLiveFlag(0x1))
			++aliveNum;

	if (aliveNum <= 0) {
		if ((unk30 & 1))
			TTimeRec::endTimer();
		return;
	}

	if (param_1 & 2) {
		clipEnemies(param_2);
		for (int i = 0; i < unk44; ++i)
			for (int j = 0; j < unk40[i].unk4; ++j)
				unk40[i].unk0[j]->calcAnm();
		setSharedFlags();
		updateAnmSoundShared();
	}

	if (param_1 & 4)
		copyFromShared();

	if (unk30 & 1) {
		TTimeRec::endTimer();
		TTimeRec::startTimer(0xff0000ff);
	}

	int num = getActiveObjNum();
	if (param_1 & 1) {
		for (int i = num; i < unk14; ++i)
			getObj(i)->onHitFlag(0x1);
	}

	for (int i = 0; i < num; ++i) {
		TSpineEnemy* enemy = getObj(i);
		if (enemy->checkLiveFlag(0x1))
			continue;

		if (param_1 & 1)
			enemy->moveObject();

		if (param_1 & 2) {
			enemy->updateSquareToMario();
			enemy->calcRootMatrix();
			if (!enemy->checkLiveFlag(0x4000)) {
				enemy->getUnk74()->frameUpdate();
				if (!enemy->checkLiveFlag(0x6)) {
					enemy->getUnk74()->calc();
					enemy->updateAnmSound();
				}
			} else {
				enemy->getUnk74()->matAnmFrameUpdate();
			}

			if (param_1 & 4)
				enemy->requestShadow();

			if (!enemy->checkLiveFlag(0x6)) {
				if ((param_1 & 4) && !enemy->checkLiveFlag(0x4000))
					enemy->getUnk74()->viewCalc();
				if (param_1 & 0x200) {
					enemy->getUnk74()->setLightData(enemy->getUnkC4(),
					                                enemy->mPosition);
					enemy->getUnk74()->entry();
				}
			}
		}
	}

	if (unk30 & 1)
		TTimeRec::endTimer();
}

void TEnemyManager::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	changeDrawBuffer(param_1);
	if (unk40) {
		performShared(param_1, param_2);
		restoreDrawBuffer(param_1);
		return;
	}

	if (unk30 & 1)
		TTimeRec::startTimer(0xffffffff);

	if (param_1 & 2) {
		clipEnemies(param_2);
		setFlagOutOfCube();
	}

	if (unk30 & 1) {
		TTimeRec::endTimer();
		TTimeRec::startTimer(0xff0000ff);
	}

	int num = getActiveObjNum();
	if (param_1 & 1) {
		for (int i = num; i < unk14; ++i)
			getObj(i)->onLiveFlag(0x1);
	} else {
		for (int i = num; i < unk14; ++i)
			; // TODO: debug print or something?
	}

	for (int i = 0; i < num; ++i)
		getObj(i)->testPerform(param_1, param_2);

	restoreDrawBuffer(param_1);
	if (unk30 & 1)
		TTimeRec::endTimer();
}

TSpineEnemy* TEnemyManager::getNearestEnemy(const JGeometry::TVec3<f32>& p)
{
	f32 dist          = 0.0f;
	TSpineEnemy* best = nullptr;

	for (int i = 0; i < getActiveObjNum(); ++i) {
		if (getObj(i)->checkLiveFlag(0x1))
			continue;

		TSpineEnemy* candidate = getObj(i);
		f32 d = VECDistance((Vec*)&p, (Vec*)&candidate->mPosition);
		if (!best || dist > d) {
			best = candidate;
			dist = d;
		}
	}

	return best;
}

TSpineEnemy* TEnemyManager::getDeadEnemy()
{
	s32 num = getActiveObjNum();
	for (int i = 0; i < num; ++i)
		if (getObj(i)->checkLiveFlag(0x1))
			return getObj(i);

	return nullptr;
}

TSpineEnemy* TEnemyManager::getFarOutEnemy()
{
	f32 dist          = -1.0f;
	TSpineEnemy* best = nullptr;

	for (int i = 0; i < getActiveObjNum(); ++i) {
		TSpineEnemy* enemy = getObj(i);
		if (enemy->checkLiveFlag(0x1)) {
			best = enemy;
			break;
		}

		if (!enemy->checkLiveFlag(0x4) || !enemy->checkLiveFlag(0x800))
			continue;

		if (!best || dist > enemy->getUnk134()) {
			best = enemy;
			dist = enemy->getUnk134();
		}
	}

	return best;
}

void TEnemyManager::killChildren()
{
	for (int i = 0; i < objNum(); ++i)
		getObj(i)->onLiveFlag(0x41);
}

void TEnemyManager::killChildrenWithin(const JGeometry::TVec3<f32>& p, f32 r)
{
	if (!objNum())
		return;

	if (!getObj(0)->checkActorType(0x10000000)
	    && !getObj(0)->checkActorType(0x8000000))
		return;

	for (int i = 0; i < objNum(); ++i) {
		TSpineEnemy* enemy = getObj(i);
		if (VECSquareDistance((Vec*)&p, (Vec*)&enemy->getPosition()) <= r * r)
			enemy->onLiveFlag(0x41);
	}
}

void TEnemyManager::killOtherEnemies() { }

int TEnemyManager::countLivingEnemy() const
{
	s32 num = getActiveObjNum();

	int result = 0;
	for (int i = 0; i < num; ++i)
		if (!getObj(i)->checkLiveFlag(0x1))
			++result;

	return result;
}

void TEnemyManager::createCopyAnmMtx(int) { }

bool TEnemyManager::copyAnmMtx(TSpineEnemy* enemy)
{
	if (unk4C != enemy->getUnk74()->getCurAnmIdx(0))
		return false;

	f32 f = enemy->getCurAnmFrameNo(0);
	enemy->calcRootMatrix();
	enemy->updateAnmSound();
	enemy->getUnk74()->frameUpdate();
	J3DModel* model = enemy->getUnk74()->getUnk4();
	MtxPtr mtx      = model->getBaseTRMtx();

	mtx[0][0] *= enemy->mScaling.x;
	mtx[0][1] *= enemy->mScaling.y;
	mtx[0][2] *= enemy->mScaling.z;
	mtx[1][0] *= enemy->mScaling.x;
	mtx[1][1] *= enemy->mScaling.y;
	mtx[1][2] *= enemy->mScaling.z;
	mtx[2][0] *= enemy->mScaling.x;
	mtx[2][1] *= enemy->mScaling.y;
	mtx[2][2] *= enemy->mScaling.z;

	for (int i = 0; i < unk50; ++i) {
		Mtx afStack_5C;
		MTXConcat(mtx, unk48[i], afStack_5C);
		MTXCopy(afStack_5C, enemy->getUnk74()->getUnk4()->getAnmMtx(i));
	}

	if (enemy->getUnk74()->getUnk4()->getModelData()->getWEvlpMtxNum())
		enemy->getModel()->calcWeightEnvelopeMtx();

	return true;
}
