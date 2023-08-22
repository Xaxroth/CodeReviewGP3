AscensionEvolved
The source code for my contribution to Ascension: Evolved.

1.1 Shared Scripts
1.1.1 Team12Player.cpp & Team12Player.h

Collaboration effort with Anton Vassepov. Originally this was the base controller present in the first-person template provided by Epic Games/Unreal Engine 5.1, and was extended by me and my co-worker Anton. I was responsible for the input handling for the melee weapon component. The melee weapon component attached to the player receives this input and does actions such as; charging up the weapon, releasing it and setting a cooldown. I also made values accessible for blueprints, and created methods that would update the player’s conditions and stats based on the PlayerStats component made by Anton.

1.1.2 Team12GameMode.cpp & Team12GameMode.h

Collaboration effort with Anton Vassepov. The game mode class which our custom game mode base is built upon. Features a list of all possible mutations and a timer that tracks the in-game time. Controls the different states of the game. I was responsible for the mutation list, as well as the implementable blueprint function that the designers would use to check for game overs through blueprints.

1.2 AI & Components
1.2.1 Monster.cpp & Monster.h

The main enemy for the game uses C++ for its behavior. The main control for the enemy behavior is done through the help of an Enum, which in term controls which switch case the monster will stay in during Behavior(), which is run in Tick(). Depending on the enum, it switches back and forth between different modes, such as idle, hunting, shooting or dead.

1.2.2 SpitProjectile.cpp & SpitProjectile.h

Spawned by the Monster script, the spit-projectile shot by the enemy is a physics-based actor which destroys itself upon collision with anything. If it collides with the player, it will find the PlayerStats component attached to them, and deal damage. Upon being spawned during the Shooting state of the enemy, the size of the projectile is randomly set, and a force is then applied upon BeginPlay() in the direction of the player.

1.2.3 MeleeAttack.cpp & MeleeAttack.h

The melee weapon of the player. When the player releases the attack button, it calls SetHitBoxLocation() which creates a hitbox in front of the player. This will follow the camera’s forward vector. As this method is called, another method is called “WeaponSwing()”, which enables the hitbox temporarily and sets the damage. After a set duration, the hitbox deactivates and the weapon goes on cooldown.

1.3 Mutations & Destructibles
1.3.1 MutationActor.cpp + MutationActor.h

An actor that is spawned, serves as an instruction for mutations applied to the player. It serves as a value converter that takes in a variable, such as PlayerSpeed or PlayerDamage, and either adds, subtracts, multiplies or divides it by x amount the second it is spawned. After the value is changed mathematically, it takes the old stat and replaces it with the new one, permanently.

1.3.2 MutationCluster.cpp + MutationCluster.h

The actual mutations that the player picks up in the game. It stores multiple instances of mutation actors in an array. Once the mutation cluster is spawned as a child of the player, its children, the MutationActors assigned, will automatically unpack and change the values they have been programmed to do. It has a rarity setting, as well as the possibility to give it a custom in-game name.

1.3.3 Egg.cpp & Egg.h

The class all eggs in the game inherit from. The MeleeAttack component checks if the target hit is an egg or not, if true, the actor that inherits from the egg script will take damage. Once the health of the egg is zero, it gathers the instance of the player character and increases its speed.

2 Blueprints
2.1 BPA_MonsterSpitter

Animation blueprint for the monster spitter. It is hooked up using a state-machine which is controlled by an enum that matches the state of the monster in its inherited Monster.cpp class. As the monster enters a state in the .cpp file, that enum value is read by the animation blueprint and used to enter, or exit different states such as idle, attack and death.

2.2 BPA_PlayerArms

Similar to BPA_MonsterSpitter, this one had an enum made in the blueprint which always matched the enumeration defined in the .cpp file for the Player. The difference between these two animation blueprints is that the enumeration for the player is handled through the input and not read directly through the .cpp file.
