using System;
using System.Collections.Generic;
using SharpDX;
using XenonCheatEngine.Punishments;
using XenonCheatEngine.Utils;
using XenonCheatEngine.Internal;
using XenonCheatEngine.Data;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;
using System.Timers;
using XenonCheatEngine.Win32;
using XenonCheatEngine.Classes.Utils;

namespace XenonCheatEngine.Classes
{
    public abstract class Map : IDisposable
    {
        bool disposed = false;
        public virtual Int32 MapID { get; set; }

        public List<Punishment> Punishments = new List<Punishment>();

        public List<TripWire> TripWires = new List<TripWire>();

        protected Map()
        {
            // Set up cheats

            Punishments.Add(new MaxRecoil());

            Program.GameData.MatchInfo.OnMatchNewRound += NewRound;
        }

        public void resetTripWires()
        {
            // Deprecated, not needed
        }

        virtual public void NewRound(object sender, EventArgs e)
        {
            Program.GameConsole.SendCommand("status");
            resetTripWires();
            PlayerConfig.ResetConfig();
        }

        public void Dispose()
        {
            foreach (Punishment Punishment in Punishments)
            {
                Punishment.Dispose();
            }
            Program.GameData.MatchInfo.OnMatchNewRound -= NewRound;
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        protected virtual void Dispose(bool disposing)
        {
            if (disposed)
                return;

            if (disposing)
            {
                Console.WriteLine("disposed");
            }

            disposed = true;
        }

        public enum stage1TripWirePunishments { }
        public enum stage2TripWirePunishments { ViolenceSpeedMomentum, ReverseSpeedhack, DrunkenMaster }

        public void tripWirePunishments(TripWire TripWire)
        {
            // Dperecated, not needed
        }

    }
}
