#ifndef PLASMAREPULSER_H
#define PLASMAREPULSER_H

#include "Weapon.h"

class CProjectile;
//class CRepulseGfx;
#if !defined HEADLESS
class CShieldPartProjectile;
#endif // !defined HEADLESS

class CPlasmaRepulser :
	public CWeapon
{
	CR_DECLARE(CPlasmaRepulser);
public:
	CPlasmaRepulser(CUnit* owner);
	~CPlasmaRepulser(void);
	void Update(void);
	void NewProjectile(CWeaponProjectile* p);
	float NewBeam(CWeapon* emitter, float3 start, float3 dir, float length, float3& newDir);
	/// returns true if its a repulsing shield
	bool BeamIntercepted(CWeapon* emitter, float damageMultiplier = 1.0f);
	void Init(void);

	std::list<CWeaponProjectile*> incoming;
	std::list<CWeaponProjectile*> hasGfx;
#if !defined HEADLESS
	std::list<CShieldPartProjectile*> visibleShieldParts;
#endif // !defined HEADLESS

	void DependentDied(CObject* o);
	void SlowUpdate(void);

	float curPower;

	float radius;
	float sqRadius;

	int hitFrames;
	int rechargeDelay;

	bool isEnabled;
	bool wasDrawn;
	bool startShowingShield;

private:	
	virtual void FireImpl() {};
};

#endif // PLASMAREPULSER_H
