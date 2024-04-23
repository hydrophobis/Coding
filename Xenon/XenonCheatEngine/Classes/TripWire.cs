using SharpDX;
using XenonCheatEngine.Classes;
using System;
using XenonCheatEngine.Internal;
using System.Timers;

// Deprecated, not needed

namespace XenonCheatEngine.Utils
{
    public class TripWire
    {
        public bool active;
        public bool IsBeingTripped { get; set; } = false;

        public int x1, y1, x2, y2, x3, y3, x4, y4, z;

        public Team triggerForTeam;

        public int chance;

        public bool disableOnTriggered { get; set; } = true;

        public bool resetOnLeave { get; set; } = false;

        public bool checkFromMemory { get; set; } = false;

        public delegate void TripWireTrigger(TripWire TripWire);

        public event TripWireTrigger OnTriggered;

        private static System.Timers.Timer ticker;

        public TripWire(dynamic points, int triggerChance = 100, Team triggerOnlyForTeam = Team.Unknown, int checkSpeed = 500)
        {
    
    }
}
}
