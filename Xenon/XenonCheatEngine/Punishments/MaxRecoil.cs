using XenonCheatEngine.Classes;
using XenonCheatEngine.Utils;
using XenonCheatEngine.Utils.Maths;
using XenonCheatEngine.Utils.SendInput;
using SharpDX;
using System;
using System.Collections.Generic;
using System.Threading;
using System.Threading.Tasks;
using System.Timers;
using System.Windows.Forms;

namespace XenonCheatEngine.Punishments
{
    /*
     PUNISHMENT:    TriggerBot
     DESCRIPTION:   Example cheat
    */
    class MaxRecoil : Punishment
    {
        public override int ActivateOnRound { get; set; } = 0;
        public override bool DisposeOnReset { get; set; } = false;

        public static bool logDelay = false;

        public Weapons LastActiveWeapon;
        public int BulletCount { get; set; } = -1;

        private bool AimedAtEnemy { get; set; } = false;

        public MaxRecoil() : base(0, true, 25)
        {
            BulletCount = Player.AmmoCount;
        }

        override public void Tick(Object source, ElapsedEventArgs e)
        {
            try
            {
                if (Program.GameProcess.IsValidAndActiveWindow == false || Player.IsAlive() == false || base.CanActivate() == false || Program.GameData.MatchInfo.isFreezeTime)
                {
                    return;
                }

                if(IsAimingAtEnemy())
                {
                    startTime = DateTime.Now;
                    AimedAtEnemy = true;
                }

                Double elapsedMillisecs = ((TimeSpan)(DateTime.Now - startTime)).TotalSeconds;
                if (elapsedMillisecs > 6)
                {
                    AimedAtEnemy = false;
                }

                int AmmoInWeapon = Player.AmmoCount;

                if(LastActiveWeapon != (Weapons)Player.ActiveWeapon)
                {
                    LastActiveWeapon = (Weapons)Player.ActiveWeapon;
                    BulletCount = AmmoInWeapon;
                }

                if (BulletCount != AmmoInWeapon && AimedAtEnemy)
                {
                    BulletCount = AmmoInWeapon;
                    ActivatePunishment();
                }
            }
            catch (Exception ex)
            {
                Log.AddEntry(new LogEntry()
                {
                    LogTypes = new List<LogTypes> { LogTypes.Analytics },
                    AnalyticsCategory = "Error",
                    AnalyticsAction = "MaxRecoilException",
                    AnalyticsLabel = ex.Message
                });
            }
        }
        
        public bool IsAimingAtEnemy()
        {
            Vector3 AimDirection = Player.AimDirection;
            Vector3 GetPlayerLocation = Player.EyePosition;

            // get aim ray in world
            if (AimDirection.Length() < 0.001)
            {
                return false;
            }

            var aimRayWorld = new Line3D(GetPlayerLocation, GetPlayerLocation + AimDirection * 8192);

            // Go through entities
            foreach (var entity in GameData.Entities)
            {

                if (!entity.IsAlive() || entity.AddressBase == Player.AddressBase)
                {
                    continue;
                }

                if (entity.Team != Player.Team)
                {
                    continue;
                }

                if (entity.Spotted == false)
                {
                    continue;
                }

                // Check if hitbox intersect with aimRay
                var hitBoxId = Helper.IntersectsHitBox(aimRayWorld, entity);
                if (hitBoxId >= 0)
                {
                    return true;
                }

                Thread.Sleep(10);
            }

            return false;
        }

        public void ActivatePunishment()
        {
            if (base.CanActivate() == false) return;

            for (int i = 0; i < 5; i++)
            {
                // Fire
                Task.Run(() =>
                { // Implment entity.EyePosition aiming
                    SendInput.MouseLeftDown();
                });
            }
        }

        override public void Reset()
        {
            BulletCount = -1;
            AimedAtEnemy = false;
        }

    }
}
