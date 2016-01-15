#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <Windows.h>

using namespace std;


enum typeMana {
		none = -1,
		Blanc = 1,
		Rouge = 2,
		Noir = 3
};

struct CoutMana {
	
	typeMana type;
	int coutMana;
};

struct Carte {
	CoutMana coutConverti;
	string name;
	bool exhaust;
};

struct Terrain{
	Carte carte;
	typeMana type;
	int production;
};

struct Creature {
	Carte carte;
	int attack;
	int endurance;
};

struct Blast {
	Carte carte;
	int dmg;
};

struct Deck {
	Carte** bibliotheque;
	int nextCard;
	int nbTerrain;
};

struct Main {
	Carte** hand;
	int tailleMax;
	int indexCarte;
};

struct Battlefield {
	Carte** bfP1;
	int indexP1;
	Carte** bfP2;
	int indexP2;
};

struct Joueur {
	Deck bibliotheque;
	Main hand;
	int pv;
	CoutMana manaPool;
	int nbLandPlayedThisTurn;
	int potentialMana;
};

void initialStep(Battlefield* battle, int tailleDeck, bool turnP1);
void firstDrawStep(Joueur* p1, Joueur* p2, int tailleDeck);
bool drawStep(Joueur* planeswalker);
void createDeck(Joueur* planeswalker, int tailleDeck);
void mainStep(Joueur* actif, Joueur* passif, Battlefield* battle, int tailleDeck, bool turnP1);
void playCardFromHand(Joueur* actif, Joueur* passif, Battlefield* battle, int tailleDeck, bool turnP1, bool reaction);
void battleStep(Joueur* attaquant, Joueur* defenseur, Battlefield* battle, int tailleDeck, bool turnP1);
void targetStep(Joueur* actif, Joueur* passif, Battlefield* battle, int tailleDeck, bool turnP1, bool reaction);
void endStep(Joueur* actif, Battlefield* battle, int tailleDeck, bool turnP1);
void updateManaPlayers(Joueur* actif, Joueur* passif, Battlefield* battle, int tailleDeck, bool turnP1);
void tapMana(Battlefield* battle, int tailleDeck, int nbManaToTap, int player);
void displayBattlefield(Battlefield* battle, int tailleDeck);
int* displayHand(Joueur* planeswalker, int tailleDeck, bool turnP1);

void initializeMagicGame(Joueur* planeswalker)
{

	bool goodSelection;
	int choix;

	goodSelection = false;

	while (!goodSelection)
	{
		cout << "- 1 : Blanc" << endl;
		cout << "- 2 : Rouge" << endl;
		cout << "- 3 : Noir" << endl;
		cin >> choix;

		if (choix <= 3 && choix > 0)
		{
			goodSelection = true;
		}
		else
		{
			cout << "Erreur, vous n'avez pas autant de doigts" << endl << endl;
		}
	}

	switch (choix)
	{
		case 1:
			planeswalker->manaPool.type = typeMana::Blanc;
			break;
		case 2:
			planeswalker->manaPool.type = typeMana::Rouge;
			break;
		case 3:
			planeswalker->manaPool.type = typeMana::Noir;
			break;
		default:
			break;
	}
}

void firstDrawStep(Joueur* p1, Joueur* p2, int tailleDeck)
{
	bool goodSelection = false;
	string s = "";
	for (int i = 0; i < 2; i++)
	{
		goodSelection = false;
		s = "";

		if(i == 0)
			createDeck(p1, tailleDeck);
		if(i == 1)
			createDeck(p2, tailleDeck);

		for (int j = 0; j < 7; j++)
		{
			if(i == 0)
				drawStep(p1);
			if (i == 1)
				drawStep(p2);
		}


		while (!goodSelection)
		{
			if (i == 0)
			{
				displayHand(p1, tailleDeck, true);
				cout << endl;
				cout << "Keep this hand? ( y / n )" << endl;
				cin >> s;

				if (s == "n")
				{
					goodSelection = true;
					i--;
				}
				else
				{
					if (s == "y")
						goodSelection = true;
					else
						cout << "Error selection" << endl;
				}
			}

			if (i == 1)
			{
				displayHand(p2, tailleDeck, false);
				cout << endl;
				cout << "Keep this hand? ( y / n )" << endl;
				cin >> s;

				if (s == "n")
				{
					goodSelection = true;
					i--;
				}
				else
				{
					if (s == "y")
						goodSelection = true;
					else
						cout << "Error selection" << endl;
				}
			}
		}
	}
}

void initialStep(Battlefield* battle, int tailleDeck,bool turnP1)
{
	for (int i = 0; i < tailleDeck; i++)
	{
		if (turnP1)
		{
			if (battle->bfP1[i] != nullptr)
				battle->bfP1[i]->exhaust = false;
		}
		else
		{
			if (battle->bfP2[i] != nullptr)
				battle->bfP2[i]->exhaust = false;
		}
	}
}

bool drawStep(Joueur* planeswalker)
{
	planeswalker->nbLandPlayedThisTurn = 0;

	if (planeswalker->bibliotheque.nextCard < 60)
	{
		planeswalker->hand.hand[planeswalker->hand.indexCarte] = planeswalker->bibliotheque.bibliotheque[planeswalker->bibliotheque.nextCard];
		planeswalker->hand.indexCarte++;
		planeswalker->bibliotheque.nextCard++;
		return false;
	}
	else
	{
		return true;
	}
}

void createDeck(Joueur* planeswalker , int tailleDeck)
{
	int cartRand;
	int coutConvertis;
	planeswalker->bibliotheque.nextCard = 0;
	planeswalker->hand.indexCarte = 0;
	planeswalker->bibliotheque.nbTerrain = 0;

	for (int i = 0; i < tailleDeck; i++)
	{
		cartRand = rand() % 100 + 1;

		switch (planeswalker->manaPool.type)
		{
		case typeMana::Blanc:
				if (cartRand < 35)
				{
					if (planeswalker->bibliotheque.nbTerrain < 20)
					{
						planeswalker->bibliotheque.bibliotheque[i] = (Carte*) new Terrain{ Carte { CoutMana{typeMana::Blanc,0}, "Plain", false } ,typeMana::Blanc, 1 };
						planeswalker->bibliotheque.nbTerrain++;
					}
					else
					{
						coutConvertis = rand() % 10 + 2;
						planeswalker->bibliotheque.bibliotheque[i] = (Carte*) new Creature{ Carte{ CoutMana{ typeMana::Blanc, coutConvertis }, "Angel", false}, coutConvertis / 3, (int) (coutConvertis * 1.5) };
					}
				}
				else
				{
					if (cartRand < 80)
					{
						coutConvertis = rand() % 7;
						planeswalker->bibliotheque.bibliotheque[i] = (Carte*) new Creature{ Carte{ CoutMana{ typeMana::Blanc, coutConvertis }, "Angel", false }, coutConvertis / 3, (int)(coutConvertis * 1.5) + 1 };
					}
					else
					{
						coutConvertis = rand() % 3 + 1;
						planeswalker->bibliotheque.bibliotheque[i] = (Carte*) new Blast{ Carte{ CoutMana{typeMana::Blanc, coutConvertis}, "Blame", false }, coutConvertis };
					}
				}
				break;
		case typeMana::Noir:
				if (cartRand < 30)
				{
					if (planeswalker->bibliotheque.nbTerrain < 20)
					{
						planeswalker->bibliotheque.bibliotheque[i] = (Carte*) new Terrain{ Carte{ CoutMana{ typeMana::Noir,0 }, "Swamp", false} ,typeMana::Noir, 1 };
						planeswalker->bibliotheque.nbTerrain++;
					}
					else
					{
						coutConvertis = rand() % 8 + 1;
						planeswalker->bibliotheque.bibliotheque[i] = (Carte*) new Creature{ Carte{ CoutMana{ typeMana::Noir, coutConvertis }, "Zombie", false}, coutConvertis, coutConvertis};
					}
				}
				else
				{
					if (cartRand < 70)
					{
						coutConvertis = rand() % 6;
						planeswalker->bibliotheque.bibliotheque[i] = (Carte*) new Creature{ Carte{ CoutMana{ typeMana::Noir, coutConvertis }, "Zombie", false }, coutConvertis, coutConvertis + 1 };
					}
					else
					{
						coutConvertis = rand() % 8 + 1;
						planeswalker->bibliotheque.bibliotheque[i] = (Carte*) new Blast{ Carte{ CoutMana{ typeMana::Noir, coutConvertis }, "Assassinate", false}, (int) (coutConvertis * 1.5) };
					}
				}
				break;
			case typeMana::Rouge:
				if (cartRand < 35)
				{
					if (planeswalker->bibliotheque.nbTerrain < 20)
					{
						planeswalker->bibliotheque.bibliotheque[i] = (Carte*) new Terrain{ Carte{ CoutMana{ typeMana::Rouge,0 }, "Mountain", false} ,typeMana::Rouge, 1 };
						planeswalker->bibliotheque.nbTerrain++;
					}
					else
					{
						coutConvertis = rand() % 10 + 2;
						planeswalker->bibliotheque.bibliotheque[i] = (Carte*) new Blast{ Carte{ CoutMana{ typeMana::Rouge, coutConvertis }, "Fireball",false}, coutConvertis * 3 };
					}
				}
				else
				{
					if (cartRand < 55)
					{
						coutConvertis = rand() % 4;
						planeswalker->bibliotheque.bibliotheque[i] = (Carte*) new Creature{ Carte{ CoutMana{ typeMana::Rouge, coutConvertis }, "Gobelin",false} , (int) (coutConvertis * 1.5) , coutConvertis / 3 + 1};
					}
					else
					{
						coutConvertis = rand() % 7 + 1;
						planeswalker->bibliotheque.bibliotheque[i] = (Carte*) new Blast{ Carte{ CoutMana{ typeMana::Rouge, coutConvertis }, "Fireball",false }, coutConvertis * 3 };
					}
				}
				break;
			default:
				break;
		}
	}
}

void mainStep(Joueur* actif, Joueur* passif, Battlefield* battle, int tailleDeck, bool turnP1)
{
	string selecText = "";

	playCardFromHand(actif, passif, battle, tailleDeck, turnP1, false);

	displayBattlefield(battle, tailleDeck);
	updateManaPlayers(actif, passif, battle, tailleDeck, turnP1);
	displayHand(passif, tailleDeck, turnP1);

	cout << "Do you want to react? (y/n)" << endl;

	do
	{
		cin >> selecText;
	} while (selecText != "y" && selecText != "n");

	if (selecText == "y")
	{
		playCardFromHand(passif, actif, battle, tailleDeck, turnP1, true);
	}

}

void battleStep(Joueur* attaquant, Joueur* defenseur, Battlefield* battle, int tailleDeck, bool turnP1)
{
	bool endStep = false;

	while (!endStep)
	{
		int* toSelec = displayHand(attaquant, tailleDeck, turnP1);
	}
}

void endStep(Joueur* actif, Battlefield* battle, int tailleDeck, bool turnP1)
{
	bool endStep = false;

	while (!endStep)
	{
		int* toSelec = displayHand(actif, tailleDeck, turnP1);
	}
}

void playCardFromHand(Joueur* actif, Joueur* passif, Battlefield* battle, int tailleDeck, bool turnP1, bool reaction)
{
	bool endStep = false;
	int selection = 0;

	while (!endStep)
	{

		displayBattlefield(battle, tailleDeck);
		updateManaPlayers(actif, passif, battle, tailleDeck, turnP1);
		int* toSelec = displayHand(actif, tailleDeck, turnP1);

		cout << endl;
		cout << "- 1 : Play a card" << endl;
		cout << "- 2 : Tap Land" << endl;
		cout << "- -1 : End Step" << endl;
		cin >> selection;

		switch (selection)
		{
		case 1:
			cout << "Which card?" << endl;
			do
			{
				cin >> selection;

				selection--;

				if (toSelec[selection] != -1)
				{
					for (int i = 0; i < tailleDeck; i++)
					{
						if ((turnP1 && !reaction) || (!turnP1 && reaction))
						{
							if (battle->bfP1[i] == nullptr)
							{
								if (actif->hand.hand[toSelec[selection]]->name == "Plain" || actif->hand.hand[toSelec[selection]]->name == "Swamp" || actif->hand.hand[toSelec[selection]]->name == "Mountain")
								{
									if (actif->nbLandPlayedThisTurn == 0 && !reaction)
									{
										battle->bfP1[i] = actif->hand.hand[toSelec[selection]];
										actif->hand.hand[toSelec[selection]] = nullptr;
										actif->nbLandPlayedThisTurn++;
									}
								}
								else
								{
									if (actif->hand.hand[toSelec[selection]]->coutConverti.coutMana >= actif->manaPool.coutMana)
									{
										actif->manaPool.coutMana -= actif->hand.hand[toSelec[selection]]->coutConverti.coutMana;
										battle->bfP1[i] = actif->hand.hand[toSelec[selection]];
										actif->hand.hand[toSelec[selection]] = nullptr;
									}
								}
								break;
							}
						}
						else
						{
							if (battle->bfP2[i] == nullptr)
							{
								if (actif->hand.hand[toSelec[selection]]->name == "Plain" || actif->hand.hand[toSelec[selection]]->name == "Swamp" || actif->hand.hand[toSelec[selection]]->name == "Mountain")
								{
									if (actif->nbLandPlayedThisTurn == 0 && !reaction)
									{
										battle->bfP2[i] = actif->hand.hand[toSelec[selection]];
										actif->hand.hand[toSelec[selection]] = nullptr;
										actif->nbLandPlayedThisTurn++;
									}
								}
								else
								{
									if (actif->hand.hand[toSelec[selection]]->coutConverti.coutMana >= actif->manaPool.coutMana)
									{
										actif->manaPool.coutMana -= actif->hand.hand[toSelec[selection]]->coutConverti.coutMana;
										battle->bfP2[i] = actif->hand.hand[toSelec[selection]];
										actif->hand.hand[toSelec[selection]] = nullptr;
									}
								}
								break;
							}
						}
					}
				}

			} while (toSelec[selection] == -1);
			break;
		case 2:
			cout << "How many?" << endl;
			do
			{
				cin >> selection;
				if (selection > actif->potentialMana && selection < 0)
				{
					if (reaction)
					{
						if (turnP1)
							tapMana(battle, tailleDeck, selection, 1);
						else
							tapMana(battle, tailleDeck, selection, 2);
					}
					else
					{
						if (turnP1)
							tapMana(battle, tailleDeck, selection, 0);
						else
							tapMana(battle, tailleDeck, selection, 1);
					}
				}

			} while (selection > actif->potentialMana && selection < 0);
			break;
		case -1:
			endStep = true;
			break;
		default:
			cout << "Error Bad Selection" << endl;
			break;
		}
	}
}

void targetStep(Joueur* actif, Joueur* passif, Battlefield* battle, int tailleDeck, bool turnP1, bool reaction)
{

}

void updateManaPlayers(Joueur* actif, Joueur* passif, Battlefield* battle, int tailleDeck, bool turnP1)
{
	if (turnP1)
	{
		for (int i = 0; i < tailleDeck; i++)
		{
			if (battle->bfP1[i] != nullptr && (battle->bfP1[i]->name == "Plain" || battle->bfP1[i]->name == "Swamp" || battle->bfP1[i]->name == "Mountain"))
			{
				if (!battle->bfP1[i]->exhaust)
					actif->potentialMana++;
			}

			if (battle->bfP2[i] != nullptr && (battle->bfP1[i]->name == "Plain" || battle->bfP1[i]->name == "Swamp" || battle->bfP1[i]->name == "Mountain"))
			{
				if (!battle->bfP2[i]->exhaust)
					passif->potentialMana++;
			}
		}
	}
	else
	{
		for (int i = 0; i < tailleDeck; i++)
		{
			if (battle->bfP1[i] != nullptr && (battle->bfP1[i]->name == "Plain" || battle->bfP1[i]->name == "Swamp" || battle->bfP1[i]->name == "Mountain"))
			{
				if (!battle->bfP1[i]->exhaust)
					passif->potentialMana++;
			}

			if (battle->bfP2[i] != nullptr && (battle->bfP1[i]->name == "Plain" || battle->bfP1[i]->name == "Swamp" || battle->bfP1[i]->name == "Mountain"))
			{
				if (!battle->bfP2[i]->exhaust)
					actif->potentialMana++;
			}
		}
	}
}

void tapMana(Battlefield* battle, int tailleDeck, int nbManaToTap, int player)
{
	int j = nbManaToTap;
	if (player == 0)
	{		
		for (int i = 0; i < tailleDeck && j > 0; i++)
		{
			if (battle->bfP1[i] != nullptr && (battle->bfP1[i]->name == "Plain" || battle->bfP1[i]->name == "Swamp" || battle->bfP1[i]->name == "Mountain"))
			{
				battle->bfP1[i]->exhaust = true;
				j--;
			}
		}
	}
	else
	{
		for (int i = 0; i < tailleDeck && j > 0; i++)
		{
			if (battle->bfP2[i] != nullptr && (battle->bfP2[i]->name == "Plain" || battle->bfP2[i]->name == "Swamp" || battle->bfP2[i]->name == "Mountain"))
			{
				battle->bfP2[i]->exhaust = true;
				j--;
			}
		}
	}
}

void displayBattlefield(Battlefield* battle, int tailleDeck)
{
	int nbLandDTP1 = 0, nbLandTP1 = 0, nbLandDTP2 = 0, nbLandTP2 = 0;

	for (int i = 0; i < tailleDeck; i++)
	{
		if (battle->bfP1[i] != nullptr && (battle->bfP1[i]->name == "Plain" || battle->bfP1[i]->name == "Swamp" || battle->bfP1[i]->name == "Mountain"))
		{
			if (battle->bfP1[i]->exhaust)
				nbLandTP1++;
			else
				nbLandDTP1++;
		}

		if (battle->bfP2[i] != nullptr && (battle->bfP1[i]->name == "Plain" || battle->bfP1[i]->name == "Swamp" || battle->bfP1[i]->name == "Mountain"))
		{
			if (battle->bfP2[i]->exhaust)
				nbLandTP2++;
			else
				nbLandDTP2++;
		}
	}

	system("cls");
	cout << "--------------------------------------------------------------------------------" << endl;
	cout << "                               BATTLEFIELD                                      " << endl;
	cout << "--------------------------------------------------------------------------------" << endl;
	cout << "Planeswalker 1: Nb Land Untapped : " << nbLandDTP1 << " - Nb Land Tapped : " << nbLandTP1 << endl;
	cout << "Planeswalker 1 Creature: " << endl;
	for (int i = 0; i < tailleDeck; i++)
	{
		if (battle->bfP1[i] != nullptr && (battle->bfP1[i]->name == "Angel" || battle->bfP1[i]->name == "Zombie" || battle->bfP1[i]->name == "Gobelin"))
		{
			cout << "Creature: " << battle->bfP1[i]->name;
			if (battle->bfP1[i]->exhaust)
			{
				cout << " tapped";
			}
			else
			{
				cout << " untapped";
			}
			cout << ", Attack: " << ((Creature*)battle->bfP1[i])->attack << ", Defense: " << ((Creature*)battle->bfP1[i])->endurance << endl;
		}
	}
	cout << "Planeswalker 2: Nb Land Untapped : " << nbLandDTP2 << " - Nb Land Tapped : " << nbLandTP2 << endl;
	cout << "Planeswalker 1 Creature: " << endl;
	for (int i = 0; i < tailleDeck; i++)
	{
		if (battle->bfP2[i] != nullptr && (battle->bfP2[i]->name == "Angel" || battle->bfP2[i]->name == "Zombie" || battle->bfP2[i]->name == "Gobelin"))
		{
			cout << "Creature: " << battle->bfP2[i]->name;
			if (battle->bfP2[i]->exhaust)
			{
				cout << " tapped" << endl;
			}
			else
			{
				cout << " untapped" << endl;
			}
			cout << ", Attack: " << ((Creature*)battle->bfP1[i])->attack << ", Defense: " << ((Creature*)battle->bfP1[i])->endurance << endl;
		}
	}
	cout << endl;

}

int* displayHand(Joueur* planeswalker, int tailleDeck, bool turnP1)
{
	cout << endl;
	cout << endl;
	cout << "--------------------------------------------------------------------------------" << endl;
	if (turnP1)
		cout << "                            HAND OF  PLANESWALKER 1" << endl;
	else
		cout << "                            HAND OF  PLANESWALKER 2" << endl;
	cout << "--------------------------------------------------------------------------------" << endl;

	int* realNbCard = new int[tailleDeck];

	for (int i = 0; i < tailleDeck; i++)
	{
		realNbCard[i] = -1;
	}

	int nbCard = 1;

	cout << " MANA POOL : " << planeswalker->manaPool.coutMana << endl;

	for (int i = 0; i < tailleDeck; i++)
	{
		if (planeswalker->hand.hand[i] != nullptr)
		{
			cout << "- " << nbCard << " : " << planeswalker->hand.hand[i]->name << ", cout : " << planeswalker->hand.hand[i]->coutConverti.coutMana;

			if (planeswalker->hand.hand[i]->name == "Angel" || planeswalker->hand.hand[i]->name == "Zombie" || planeswalker->hand.hand[i]->name == "Gobelin")
				cout << ", Attack: " << ((Creature*)planeswalker->hand.hand[i])->attack << ", Defense: " << ((Creature*)planeswalker->hand.hand[i])->endurance;

			if (planeswalker->hand.hand[i]->name == "Blame" || planeswalker->hand.hand[i]->name == "Assassinate" || planeswalker->hand.hand[i]->name == "Fireball")
				cout << ", Attack: " << ((Blast*)planeswalker->hand.hand[i])->dmg;

			cout << endl;
			realNbCard[nbCard - 1] = i;
			nbCard++;
		}
	}
	
	return realNbCard;
}

void MTG()
{
	int tailleDeck = 60;
	bool turnP1 = true;
	bool firstTurn = true;
	int choixJoueur1, choixJoueur2;
	bool win = false;
	Deck deckP1 = { new Carte*[tailleDeck] {}, 0, 0 };
	Deck deckP2 = { new Carte*[tailleDeck] {}, 0, 0 };
	Main mainP1 = { new Carte*[tailleDeck] {}, 7, 0 };
	Main mainP2 = { new Carte*[tailleDeck] {}, 7, 0 };
	Joueur p1 = { deckP1, mainP1, 20 , CoutMana{ typeMana::none, 0 }, 0, 0};
	Joueur p2 = { deckP2, mainP2, 20 , CoutMana{ typeMana::none, 0 }, 0, 0 };
	Battlefield battle = { new Carte* [tailleDeck] {}, 0, new Carte*[tailleDeck] {}, 0 };

	cout << "Joueur 1, quelle couleur voulez vous jouer?" << endl;
	initializeMagicGame(&p1);

	system("cls");

	cout << "Joueur 2, quelle couleur voulez vous jouer?" << endl;
	initializeMagicGame(&p2);

	firstDrawStep(&p1, &p2, tailleDeck);

	while (!win)
	{
		initialStep(&battle, tailleDeck, turnP1);
		
		if (firstTurn)
		{
			firstTurn = false;
		}
		else
		{
			if (turnP1)
				win = drawStep(&p1);
			else
				win = drawStep(&p2);
		}

		displayBattlefield(&battle, tailleDeck);

		if(turnP1)
			mainStep(&p1, &p2, &battle, tailleDeck, turnP1);
		else
			mainStep(&p2, &p1, &battle, tailleDeck, turnP1);

		displayBattlefield(&battle, tailleDeck);
		
		if (turnP1)
			battleStep(&p1, &p2, &battle, tailleDeck, turnP1);
		else
			battleStep(&p2, &p1, &battle, tailleDeck, turnP1);

		displayBattlefield(&battle, tailleDeck);

		if (turnP1)
			mainStep(&p1, &p2, &battle, tailleDeck, turnP1);
		else
			mainStep(&p2, &p1, &battle, tailleDeck, turnP1);

		displayBattlefield(&battle, tailleDeck);

		if (turnP1)
			endStep(&p1, &battle, tailleDeck, turnP1);
		else
			endStep(&p2, &battle, tailleDeck, turnP1);

		turnP1 = !turnP1;

	}
}

void cptVsCpt()
{
	int doigtsHumain = 5;
	int choixSommeHumain = 0;
	int choixDoigtsHumain = 0;
	int doigtsCpt = 5;
	int choixSommeCpt = 0;
	int choixDoigtsCpt = 0;

	bool win = false;
	bool goodSelection = false;

	while (!win)
	{

		choixDoigtsCpt = rand() % doigtsCpt + 1;
		choixSommeCpt = rand() % doigtsHumain + choixDoigtsCpt + 1;

		choixDoigtsHumain = rand() % doigtsHumain + 1;
		choixSommeHumain = rand() % doigtsCpt + choixDoigtsHumain + 1;

		system("cls");

		cout << "Doigts CPT1 : " << doigtsHumain << endl;
		cout << "Doigts CPT2 : " << doigtsCpt << endl;
		cout << endl << endl;
		cout << "Estimation CPT1 : " << choixSommeHumain << endl;
		cout << "Nombre de doigts CPT1 montres : " << choixDoigtsHumain << endl;
		cout << "Estimation CPT2  : " << choixSommeCpt << endl;
		cout << "Nombre de doigts CPT2 montres : " << choixDoigtsCpt << endl;

		cout << endl;

		if (choixSommeHumain == (choixDoigtsCpt + choixDoigtsHumain) && choixSommeCpt == (choixDoigtsCpt + choixDoigtsHumain))
		{
			cout << "Egalite sur la manche" << endl;
			doigtsCpt--;
			doigtsHumain--;
		}
		else
		{
			if (choixSommeHumain == (choixDoigtsCpt + choixDoigtsHumain))
			{
				cout << "CPT1 remporte la manche" << endl;
				doigtsHumain--;
			}

			if (choixSommeCpt == (choixDoigtsCpt + choixDoigtsHumain))
			{
				cout << "CPT2 remporte la manche" << endl;
				doigtsCpt--;
			}
		}

		system("pause");
		system("cls");

		if (doigtsHumain == 1 && doigtsCpt == 1)
		{
			cout << endl << "EGALITE" << endl;
			win = true;
		}
		else
		{
			if (doigtsHumain == 1)
			{
				cout << endl << "CPT1 gagne" << endl;
				win = true;
			}

			if (doigtsCpt == 1)
			{
				cout << endl << "CPT2 gagne" << endl;
				win = true;
			}
		}

		if (win)
		{
			bool selec = false;
			while (!selec)
			{
				string s;
				cout << endl << endl;
				cout << "Voulez vous reessayer? o/n" << endl;
				cin >> s;

				if (s == "o")
				{
					selec = true;
					win = false;
					doigtsHumain = 5;
					doigtsCpt = 5;
				}
				else
				{
					if (s == "n")
					{
						selec = true;
					}
					else
					{
						cout << "Erreur de selection" << endl;
					}
				}
			}
		}
	}
}

void hVsCpt()
{
	int doigtsHumain = 5;
	int choixSommeHumain = 0;
	int choixDoigtsHumain = 0;
	int doigtsCpt = 5;
	int choixSommeCpt = 0;
	int choixDoigtsCpt = 0;

	bool win = false;
	bool goodSelection = false;

	while (!win)
	{

		choixDoigtsCpt = rand() % doigtsCpt + 1;
		choixSommeCpt = rand() % doigtsHumain + choixDoigtsCpt + 1;

		cout << "Vos doigts : " << doigtsHumain << endl;
		cout << "Doigts Adversaire : " << doigtsCpt << endl;

		cout << endl;

		goodSelection = false;

		while (!goodSelection)
		{
			cout << "Combien de doigts voulez vous montrer?" << endl;
			cin >> choixDoigtsHumain;

			if (choixDoigtsHumain <= doigtsHumain && choixDoigtsHumain > 0)
			{
				goodSelection = true;
			}
			else
			{
				cout << "Erreur, vous n'avez pas autant de doigts" << endl;
			}
		}

		system("cls");

		cout << "Vos doigts : " << doigtsHumain << endl;
		cout << "Doigts Adversaire : " << doigtsCpt << endl;

		cout << endl;

		goodSelection = false;

		while (!goodSelection)
		{
			cout << "Combien de doigts pensez vous qu'il va y avoir?" << endl;
			cin >> choixSommeHumain;

			if (choixSommeHumain <= (doigtsCpt + doigtsHumain) && choixSommeHumain > 0)
			{
				goodSelection = true;
			}
			else
			{
				cout << "Erreur, il ne peut pas y avoir autant de doigt" << endl;
			}
		}

		system("cls");
		
		cout << "Votre estimation : " << choixSommeHumain << endl;
		cout << "Nombre de doigts montres : " << choixDoigtsHumain << endl;
		cout << "Estimation Adverse : " << choixSommeCpt << endl;
		cout << "Nombre de doigts adverse montres : " << choixDoigtsCpt << endl;

		cout << endl;

		if (choixSommeHumain == (choixDoigtsCpt + choixDoigtsHumain) && choixSommeCpt == (choixDoigtsCpt + choixDoigtsHumain))
		{
			cout << "Egalite sur la manche" << endl;
			doigtsCpt--;
			doigtsHumain--;
		}
		else
		{
			if (choixSommeHumain == (choixDoigtsCpt + choixDoigtsHumain))
			{
				cout << "Vous remportez la manche" << endl;
				doigtsHumain--;
			}

			if (choixSommeCpt == (choixDoigtsCpt + choixDoigtsHumain))
			{
				cout << "L'adversaire remporte la manche" << endl;
				doigtsCpt--;
			}
		}

		system("pause");
		system("cls");

		if (doigtsHumain == 1 && doigtsCpt == 1)
		{
			cout << endl << "EGALITE" << endl;
			win = true;
		}
		else
		{
			if (doigtsHumain == 1)
			{
				cout << endl << "Vous avez gagne" << endl;
				win = true;
			}

			if (doigtsCpt == 1)
			{
				cout << endl << "Vous avez perdu" << endl;
				win = true;
			}
		}

		if (win)
		{
			bool selec = false;
			while (!selec)
			{
				string s;
				cout << endl << endl;
				cout << "Voulez vous reessayer? o/n" << endl;
				cin >> s;

				if (s == "o")
				{
					selec = true;
					win = false;
					doigtsHumain = 5;
					doigtsCpt = 5;
				}
				else
				{
					if (s == "n")
					{
						selec = true;
					}
					else
					{
						cout << "Erreur de selection" << endl;
					}
				}
			}
		}
	}
}

int main()
{

	bool run = true;
	int selection;
	srand(time(NULL));

	while (run)
	{
		cout << endl;
		cout << "-------------------------" << endl;
		cout << "Jeu de Mourre" << endl;
		cout << "-------------------------" << endl;
		cout << endl;
		cout << "- 1 : Mode Ordi vs Ordi" << endl;
		cout << "- 2 : Mode Humain vs Ordi" << endl;
		cout << endl;
		cout << "-------------------------" << endl;
		cout << "Magic The Gathering" << endl;
		cout << "-------------------------" << endl; 
		cout << endl;
		cout << "- 3 : Mode Humain vs Humain" << endl;
		cout << endl;
		cout << "- 9 : Quitter" << endl;

		cin >> selection;

		system("cls");

		switch (selection)
		{
			case 1:
				cptVsCpt();
				selection = 0;
				break;
			case 2:
				hVsCpt();
				selection = 0;
				break;
			case 3:
				MTG();
				selection = 0;
				break;
			case 9:
				run = false;
				break;
			default:
				"Erreur : Mauvaise Selection";
				break;
		}
	}

	return 0;
}