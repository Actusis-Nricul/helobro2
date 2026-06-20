using MelonLoader;
using Harmony;
using Il2CppAssets.Scripts.Unity;
using Il2CppAssets.Scripts.Models.Towers;
using Il2CppAssets.Scripts.Models.Towers.Weapons;
using UnityEngine;
using System.Collections.Generic;

[assembly: MelonInfo(typeof(DamageToggle.Main), "DamageToggle", "2.0", "you")]
[assembly: MelonGame("Ninja Kiwi", "BloonsTD6")]

namespace DamageToggle
{
    public class Main : MelonMod
    {
        public static bool enabled = false;
        public static float multiplier = 100f;

        // Store original damages
        private static Dictionary<WeaponModel, float> originalDamage = new Dictionary<WeaponModel, float>();

        public override void OnUpdate()
        {
            if (Input.GetKeyDown(KeyCode.Alpha0))
            {
                enabled = !enabled;

                if (enabled)
                {
                    ApplyMultiplier();
                    MelonLogger.Msg("💥 100x DAMAGE ON");
                }
                else
                {
                    RevertDamage();
                    MelonLogger.Msg("🧯 Damage back to normal");
                }
            }
        }

        private static void ApplyMultiplier()
        {
            var model = Game.instance.model;

            foreach (var tower in model.towers)
            {
                foreach (var attack in tower.GetAttackModels())
                {
                    foreach (var weapon in attack.weapons)
                    {
                        if (!originalDamage.ContainsKey(weapon))
                        {
                            originalDamage[weapon] = weapon.projectile.damage;
                        }

                        weapon.projectile.damage = originalDamage[weapon] * multiplier;
                    }
                }
            }
        }

        private static void RevertDamage()
        {
            foreach (var entry in originalDamage)
            {
                entry.Key.projectile.damage = entry.Value;
            }
        }
    }
}