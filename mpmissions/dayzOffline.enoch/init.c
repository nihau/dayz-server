//CI CD TEST

void main()
{
        //INIT ECONOMY--------------------------------------
        Hive ce = CreateHive();
        if ( ce )
                ce.InitOffline();

        //DATE RESET AFTER ECONOMY INIT-------------------------
        int year, month, day, hour, minute;
        int reset_month = 7, reset_day = 17;
        GetGame().GetWorld().GetDate(year, month, day, hour, minute);

        if ((month == reset_month) && (day < reset_day))
        {
                GetGame().GetWorld().SetDate(year, reset_month, reset_day, hour, minute);
        }
        else
        {
                if ((month == reset_month + 1) && (day > reset_day))
                {
                        GetGame().GetWorld().SetDate(year, reset_month, reset_day, hour, minute);
                }
                else
                {
                        if ((month < reset_month) || (month > reset_month + 1))
                        {
                                GetGame().GetWorld().SetDate(year, reset_month, reset_day, hour, minute);
                        }
                }
        }
		
		// Call every 5 minutes (300000 ms)
		//GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(RefuelAllCars, 300000, true);
		//надо фильтровать машины в которых уже не сидит водятло 💀 (иначе начинается дикий десинк)
}

void RefuelAllCars()
{
    if (!GetGame().IsServer())
        return;

    array<Object> objects = new array<Object>;
    array<CargoBase> proxy = new array<CargoBase>;

    // Get all objects on the map
    GetGame().GetObjectsAtPosition("0 0 0", 1000000, objects, proxy);

    foreach (Object obj : objects)
    {
        Car car = Car.Cast(obj);
        if (!car)
            continue;

        // Skip ruined cars
        if (car.GetHealthLevel() == GameConstants.STATE_RUINED)
            continue;

        float maxFuel = car.GetFluidCapacity(CarFluid.FUEL);
        if (maxFuel <= 0)
            continue;

        car.Fill(CarFluid.FUEL, maxFuel);
		car.Fill( CarFluid.COOLANT, 1000 );
    }
}

class CustomMission: MissionServer
{
        override PlayerBase CreateCharacter(PlayerIdentity identity, vector pos, ParamsReadContext ctx, string characterName)
        {
                Entity playerEnt;
                playerEnt = GetGame().CreatePlayer( identity, characterName, pos, 0, "NONE" );
                Class.CastTo( m_player, playerEnt );

                GetGame().SelectPlayer( identity, m_player );

                return m_player;
        }

        override void StartingEquipSetup(PlayerBase player, bool clothesChosen)
		{
			player.RemoveAllItems();
			
			player.GetStatEnergy().Set(player.GetStatEnergy().GetMax()); // full energy
			player.GetStatWater().Set(player.GetStatWater().GetMax());   // full water
						
			{
				EntityAI backpackEnt = player.GetInventory().CreateAttachment("TortillaBag");
			
				if (backpackEnt)
				{
					player.SetQuickBarEntityShortcut(backpackEnt.GetInventory().CreateAttachment("Compass"), 7);
										
					EntityAI gps = backpackEnt.GetInventory().CreateAttachment("GPSReceiver");
					player.SetQuickBarEntityShortcut(gps, 8);
					
					if (gps)
					{
						gps.GetInventory().CreateAttachment("Battery9V");
					}
					
					EntityAI canister = backpackEnt.GetInventory().CreateInInventory("CanisterGasoline");

					if (canister)
					{
						canister.SetLiquidType(LIQUID_WATER);
						canister.SetQuantity(canister.GetQuantityMax());
					}
					
					backpackEnt.GetInventory().CreateInInventory("TacticalBaconCan_Opened");
					backpackEnt.GetInventory().CreateInInventory("TacticalBaconCan_Opened");
					backpackEnt.GetInventory().CreateInInventory("TacticalBaconCan_Opened");
					backpackEnt.GetInventory().CreateInInventory("TacticalBaconCan_Opened");
				}
			
				player.GetInventory().CreateAttachment("TacticalGoggles");
				player.GetInventory().CreateAttachment("TacticalGloves_Beige");
				player.GetInventory().CreateAttachment("BomberJacket_Blue");
				
				EntityAI belt = player.GetInventory().CreateAttachment("MilitaryBelt");
				
				if (belt)
				{
					EntityAI sheath = belt.GetInventory().CreateAttachment("NylonKnifeSheath");
					
					if (sheath)
					{
						EntityAI knife = sheath.GetInventory().CreateAttachment("CombatKnife");
						player.SetQuickBarEntityShortcut(knife, 0);
					}
					
					EntityAI holster = belt.GetInventory().CreateAttachment("PlateCarrierHolster");
					
					if (holster)
					{
						//
					}
					
					EntityAI canteen = belt.GetInventory().CreateAttachment("Canteen");
					
					if (canteen)
					{
						canteen.SetLiquidType(LIQUID_WATER);
						canteen.SetQuantity(canteen.GetQuantityMax());
					}
				}
				
				EntityAI helmet = player.GetInventory().CreateAttachment("Mich2001Helmet");		

				if (helmet)
				{
					EntityAI nv = helmet.GetInventory().CreateAttachment("NVGoggles");
					
					if (nv)
					{
						nv.GetInventory().CreateAttachment("Battery9V");
					}
					
					EntityAI flashlight = helmet.GetInventory().CreateAttachment("UniversalLight");
					
					if (flashlight)
					{
						flashlight.GetInventory().CreateAttachment("Battery9V");
					}
				}
				
				EntityAI vest = player.GetInventory().CreateAttachment("SmershVest");
				
				if (vest)
				{
					vest.GetInventory().CreateAttachment("SmershBag");
					
					player.SetQuickBarEntityShortcut(vest.GetInventory().CreateAttachment("M67Grenade"), 4);
					vest.GetInventory().CreateAttachment("M67Grenade");
					vest.GetInventory().CreateAttachment("Ammo_40mm_Explosive");
					vest.GetInventory().CreateAttachment("Ammo_40mm_Explosive");
				}
				
				EntityAI pants = player.GetInventory().CreateAttachment("GorkaPants_Summer");
				
				if (pants)
				{
					player.SetQuickBarEntityShortcut(pants.GetInventory().CreateInInventory("BandageDressing"), 5);
					pants.GetInventory().CreateInInventory("BandageDressing");
					
					player.SetQuickBarEntityShortcut(pants.GetInventory().CreateInInventory("ChernarusMap"), 9);
					
					player.SetQuickBarEntityShortcut(pants.GetInventory().CreateInInventory("Epinephrine"), 6);
					pants.GetInventory().CreateInInventory("Epinephrine");
					
					pants.GetInventory().CreateInInventory("Morphine");
					pants.GetInventory().CreateInInventory("Morphine");
				}
				
				EntityAI boots = player.GetInventory().CreateAttachment("MilitaryBoots_Redpunk");
				
				if (boots)
				{
					//
				}
				
				EntityAI weapon1 = player.GetHumanInventory().CreateAttachment("Aug");
				
				if (weapon1)
				{				
					weapon1.GetInventory().CreateAttachment("M4_Suppressor");
					
					EntityAI optics = weapon1.GetInventory().CreateAttachment("ACOGOptic_6x");
					
					if (optics)
					{
						//optics.GetInventory().CreateAttachment("Battery9V");
					}
					
					EntityAI magEntity = weapon1.GetInventory().CreateAttachment("Mag_STANAG_60Rnd");
					
					Magazine mag = Magazine.Cast(magEntity);

					if (mag) 
					{
						mag.ServerSetAmmoMax();
					}
					
					for (int i = 0; i < 2; ++i) 
					{
						magEntity = vest.GetInventory().CreateInInventory("Mag_STANAG_60Rnd");
						mag = Magazine.Cast(magEntity);
						mag.ServerSetAmmoMax();
					}
					
					Weapon_Base.Cast(weapon1).FillChamber("Ammo_556x45");
					
					player.SetQuickBarEntityShortcut(weapon1, 2);
				}
				
				EntityAI weapon2 = player.GetInventory().CreateAttachment("M79");
				
				if (weapon2)
				{
					Weapon_Base.Cast(weapon2).FillChamber("Ammo_40mm_Explosive");
					
					player.SetQuickBarEntityShortcut(weapon2, 3);
				}
			}
		}
		
		/*
		// Dictionary to track vehicles used by players
		ref array<EntityAI> usedVehicles = {};

		void OnPlayerEnterVehicle(PlayerBase player, CarScript vehicle)
		{
			if (vehicle && !usedVehicles.Contains(vehicle))
			{
				usedVehicles.Insert(vehicle);
			}
		}

		// Call this periodically to despawn vehicles
		void DespawnUsedVehicles()
		{
			for (int i = usedVehicles.Count() - 1; i >= 0; i--)
			{
				CarScript car = CarScript.Cast(usedVehicles[i]);
				if (!car)
				{
					usedVehicles.Remove(i);
					continue;
				}

				// Only despawn if vehicle is not ruined
				if (car.GetHealthLevel() < GameConstants.STATE_RUINED)
				{
					GetGame().ObjectDelete(car);
					usedVehicles.Remove(i);
				}
			}
		}*/
};

Mission CreateCustomMission(string path)
{
        return new CustomMission();
}
