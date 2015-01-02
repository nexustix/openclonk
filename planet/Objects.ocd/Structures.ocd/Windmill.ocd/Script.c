/**
	Windmill
	Crushes seeds into flour using power. It can either use its own power
	generated by the mill or drain power from the network if the wind does
	not supply enough.
	
	@authors Ringwaul, Clonkonaut, Maikel
*/

#include Library_Structure
#include Library_Ownable
#include Library_Producer

/*-- Initialization --*/

local last_power;
local wheel;

func TurnAnimation() { return "Spin"; }
func MinRevolutionTime() { return 18000; } // in frames

protected func Construction()
{
	SetProperty("MeshTransformation", Trans_Rotate(-30, 0, 1, 0));
	SetAction("Default");	
	return _inherited(...);
}

protected func Initialize()
{
	// Create a helper object for the wheel.
	wheel = CreateObject(WindGenerator_Wheel, 0, 0, NO_OWNER);
	wheel->SetParent(this, 150);
	// Start the animation for the wheel.
	PlayAnimation(TurnAnimation(), 5, wheel->Anim_R(GetAnimationLength(TurnAnimation()), 0), Anim_Const(1000));
	// Initialize a regular check of the wheel's position and speed, also handles power updates.
	last_power = 0;
	AddTimer("Wind2Turn", 4);
	Wind2Turn();
	// Another timer for the collection zone.
	AddTimer("CollectionZone", 1);
	return _inherited(...);
}

// Timer, check for objects to collect in the designated collection zone.
public func CollectionZone()
{
	if (GetCon() < 100) 
		return;
	for (var object in FindObjects(Find_InRect(-18 + 21 * GetDir(), 35, 15, 15), Find_OCF(OCF_Collectible), Find_NoContainer(), Find_Layer(GetObjectLayer())))
		Collect(object);
	return;
}

protected func Collection()
{
	Sound("Clonk");
	return;
}

/*-- Power Production --*/

// Returns the wind weighted over several points.
private func GetWeightedWind()
{
	return (
		(10 * GetWind(-150, -30)) + 
		(25 * GetWind( -75, -30)) + 
		(30 * GetWind(   0, -30)) + 
		(25 * GetWind( +75, -30)) + 
		(10 * GetWind(+150, -30))
		) / 100;
}

// Turns wind into power and adjusts the power production accordingly.
public func Wind2Turn()
{	
	// Only produce power if fully constructed.
	if (GetCon() < 100) 
		return;
	// Determine the current power production.	
	var power = 0;
	if (!wheel->Stuck() && !wheel->HasStopped())
	{
		power = Abs(wheel->GetRDir(MinRevolutionTime() / 90));
		if (power < 5) 
			power = 0;
		else 
			power = Max((power + 5) / 25, 1) * 50;
	}
	// Update the power consumption if the produced power has changed.
	if (last_power != power)
	{
		last_power = power;
		// If in production update the power consumption.
		if (GetEffect("InProduction", this))
			RegisterPowerRequest(PowerNeed());
	}
	// Adjust the wheel speed.
	var current_wind = GetWeightedWind();
	wheel->SetRDir(current_wind * 90, MinRevolutionTime());
	// Make some sounds.
	if (Abs(current_wind) >= 10 && Random(15 - Abs(current_wind / 10)) < 5)
		Sound(["WoodCreak?","HingeCreak?"][Random(2)], false, nil, nil, nil, 75);
	return;
}

// Power need for the production library.
private func PowerNeed() 
{
	return Max(0, 75 - last_power); 
}


/*-- Production --*/

public func IsInteractable() { return true; }

private func IgnoreKnowledge() { return true; }

private func IsProduct(id product_id)
{
	return product_id->~IsWindmillProduct();
}

private func ProductionTime(id toProduce) { return 290; }

public func NeedRawMaterial(id rawmat_id)
{
	if (rawmat_id == Seeds)
		return true;
	return false;
}

public func OnProductionStart(id product)
{
	AddEffect("InProduction", this, 100, 10, this);
	AddEffect("Crushing", this, 100, 10, this);
	return;
}

public func OnProductionContinued(id product)
{
	AddEffect("Crushing", this, 100, 10, this);
	return;
}

public func OnProductionHold(id product)
{
	RemoveEffect("Crushing", this);
	return;
}

public func OnProductionFinish(id product)
{
	RemoveEffect("InProduction", this);
	RemoveEffect("Crushing", this);
	return;
}	

public func FxInProductionTimer(object target, proplist effect, int time)
{
	return 1;
}

public func FxCrushingTimer(object target, proplist effect, int time)
{
	var dir = GetCalcDir();
	var particles =
	{
		Prototype = Particles_WoodChip(),
		R = 255,
		G = 200,
		B = 100
	};
	CreateParticle("Dust", PV_Random(11 * dir, 13 * dir), 40, PV_Random(-5, 5), PV_Random(-13, -6), PV_Random(36 * 3, 36 * 10), particles, 3);
	
	return 1;
}

public func OnProductEjection(object product)
{
	product->SetPosition(GetX() - 25 * GetCalcDir(), GetY() + 40);
	product->SetSpeed(0, -17);
	product->SetR(30 - Random(59));
	Sound("Pop");
	return;
}

protected func RejectCollect(id item, object collect)
{
	if (collect->~IsMillIngredient()) 
		return false;
	return true;
}


/*-- Properties --*/

local ActMap = {
	Default = {
		Prototype = Action,
		Name = "Default",
		Procedure = DFA_NONE,
		Directions = 2,
		FlipDir = 1,
		Length = 1,
		Delay = 0,
		FacetBase = 1,
		NextAction = "Default",
	},
};

protected func Definition(def) 
{
	SetProperty("PictureTransformation", Trans_Mul(Trans_Translate(2000, 0, 7000), Trans_Rotate(-20, 1, 0, 0), Trans_Rotate(30, 0, 1, 0)), def);
}

local ContainBlast = true;
local BlastIncinerate = 100;
local HitPoints = 70;
local Name = "$Name$";
local Description = "$Description$";
