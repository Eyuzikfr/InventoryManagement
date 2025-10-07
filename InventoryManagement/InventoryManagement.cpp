#include <iostream>
#include <vector>
#include <memory>

// util function to hold screen till player inputs
static void HoldScreen(std::string message) {
	std::cout << "\n\n" << message << std::endl;
	std::cin.get();
	std::cin.ignore();
}

enum PlayerChoice {
	Pick = 1, Drop, Equip, Unequip, Display, Exit
};

class Item {
private:
	std::string m_Name;
	int m_Value;

public:
	Item(std::string name, int value) : m_Name(name), m_Value(value) {}

	~Item() {}

	std::string GetName() const { return m_Name; }
	int GetValue() const { return m_Value; }
};

class Inventory {
private:
	std::vector<std::shared_ptr<Item>> items;
public:
	void AddItem(std::shared_ptr<Item> item) {
		items.push_back(item);
		std::cout << "Item (" << item->GetName() << ") has been added to the inventory." << std::endl;
	}

	void RemoveItem(int index) {
		if (index >= 0 && index < items.size()) {
			std::cout << "Item (" << items[index]->GetName() << ") has been removed from the inventory." << std::endl;
			items.erase(items.begin() + index);
		}
	}

	void ListItems() const {
		int i = 1;
		std::cout << "\n-----Inventory-----" << std::endl;
		if (items.size() <= 0) {
			std::cout << "[Empty]" << std::endl;
		}
		else {
			for (auto& item : items) {
				std::cout << i << ". " << item->GetName() << std::endl;
				i++;
			}
		}
	}

	std::shared_ptr<Item> GetItem(int index) const {
		if (index < items.size()) return items[index];
		return nullptr;
	}

	int GetSize() { return items.size(); }
};

class Hotbar {
private:
	std::vector<std::weak_ptr<Item>> slots;
public:
	Hotbar(int numSlots) : slots(numSlots) {}

	void EquipItem(int slotIndex, std::weak_ptr<Item> item) {
		if (slotIndex < slots.size() && slotIndex >= 0) {
			slots[slotIndex] = item;
			std::cout << "Equipping item " << item.lock()->GetName() << " to slot " << slotIndex + 1 << "." << std::endl;
		}
		else
		{
			std::cout << "Invalid slot index." << std::endl;
		}
	}

	void UnequipItem(int slotIndex) {
		if (slotIndex < slots.size() && slotIndex >= 0) {
			std::cout << "Unequipping item " << slots[slotIndex].lock()->GetName() << " from slot " << slotIndex + 1 << "." << std::endl;
			slots[slotIndex].reset();
		}
		else
		{
			std::cout << "Invalid slot index." << std::endl;
		}
	}

	void DisplayHotbar() const {
		int i = 0;
		std::cout << "-----Hotbar-----" << std::endl;
		for (; i < slots.size(); i++) {
			std::cout << i + 1 << ". ";
			if (auto slotLock = slots[i].lock()) {
				std::cout << slotLock->GetName() << std::endl;
			}
			else {
				std::cout << "[Empty]" << std::endl;
			}
		}
	}
	
	int GetSize() { return slots.size(); }
};

static void ListAvailableItems(std::vector<std::shared_ptr<Item>> &availableItems) {
	int i = 1;
	std::cout << "\n-----Available Items-----" << std::endl;
	for (auto& item : availableItems) {
		std::cout << i << ". " << item->GetName() << std::endl;
		i++;
	}
}

void showTitle() {
	for (int i = 0; i < 30; i++)
		std::cout << "=";
	std::cout << "\n";

	for (int i = 0; i < 5; i++)
		std::cout << " ";
	std::cout << "INVENTORY MANAGEMENT" << std::endl;

	for (int i = 0; i < 30; i++)
		std::cout << "=";
	std::cout << "\n\n";
}

int main() {
	Inventory inventory;	// player inventory
	Hotbar hotbar(3);		// player hotbar
	std::vector<std::shared_ptr<Item>> availableItems;	// items player can pick up
	int playerChoice;		// holds player choices
	bool isRunning = true;	// bool var to loop till player chooses to exit


	// add three items that are available to be picked up
	availableItems.push_back(std::make_shared<Item>("Sword", 50));
	availableItems.push_back(std::make_shared<Item>("Heal Potion", 60));
	availableItems.push_back(std::make_shared<Item>("Wooden Shield", 30));

	// main game loop
	while (isRunning) {
		// title and menu
		{
		showTitle();
		hotbar.DisplayHotbar();
			std::cout << "\nChoose an action: " << std::endl
				<< "1. Pick Item" << std::endl
				<< "2. Drop Item" << std::endl
				<< "3. Equip Item" << std::endl
				<< "4. Unequip Item" << std::endl
				<< "5. Show Inventory" << std::endl
				<< "6. Exit" << std::endl
				<< "\n-> ";
			std::cin >> playerChoice;
		}

		switch (playerChoice) {
		case Pick:
		{
			int pickedItemIndex;
			ListAvailableItems(availableItems);
			std::cout << availableItems.size() + 1 << ". << Cancel >>" << std::endl;
			std::cout << "\nPick an item: " << std::endl;
			std::cin >> pickedItemIndex;

			if (pickedItemIndex == availableItems.size() + 1) {
				break;
			}

			if (pickedItemIndex <= availableItems.size() && pickedItemIndex > 0) {
				inventory.AddItem(std::move(availableItems[pickedItemIndex - 1]));
				availableItems.erase(availableItems.begin() + pickedItemIndex - 1);
			}
			else {
				std::cout << "Item unavailable." << std::endl;
			}
			break;
		}

		case Drop:
		{
			if (inventory.GetSize() <= 0) {
				std::cout << "Inventory is empty. Nothing to drop." << std::endl;
				break;
			}
			int dropItemIndex;
			inventory.ListItems();
			std::cout << inventory.GetSize() + 1 << ". << Cancel >>" << std::endl;
			std::cout << "\nDrop Item: " << std::endl;
			std::cin >> dropItemIndex;

			if (dropItemIndex == inventory.GetSize() + 1) {
				break;
			}

			std::weak_ptr<Item> droppedItem = inventory.GetItem(dropItemIndex - 1);

			if (auto droppedItemLock = droppedItem.lock()) {
				availableItems.push_back(droppedItemLock);
				inventory.RemoveItem(dropItemIndex - 1);
			}
			break;
		}

		case Equip:
		{
			if (inventory.GetSize() <= 0) {
				std::cout << "Inventory is empty. Nothing to equip." << std::endl;
				break;
			}
			int equipItemIndex;
			int slotIndex;
			inventory.ListItems();
			std::cout << inventory.GetSize() + 1 << ". << Cancel >>" << std::endl;
			std::cout << "\nEquip Item: " << std::endl;
			std::cin >> equipItemIndex;
			std::cout << "\nTo Slot: " << std::endl;
			std::cin >> slotIndex;

			if (equipItemIndex == inventory.GetSize() + 1) {
				break;
			}

			if (equipItemIndex > 0 && equipItemIndex <= inventory.GetSize()) {
				hotbar.EquipItem(slotIndex - 1, inventory.GetItem(equipItemIndex - 1));
			}
			else
			{
				std::cout << "Invalid inventory index." << std::endl;
			}
			break;
		}

		case Unequip:
		{
			system("cls");
			int slotIndex;
			hotbar.DisplayHotbar();
			std::cout << hotbar.GetSize() + 1 << ". << Cancel >>" << std::endl;
			std::cout << "\nRemove Item at Slot: " << std::endl;
			std::cin >> slotIndex;

			if (slotIndex == hotbar.GetSize() + 1) {
				break;
			}

			hotbar.UnequipItem(slotIndex - 1);

			break;
		}

		case Display:
		{
			inventory.ListItems();
			break;
		}
			
		case Exit:
		{
			std::cout << "Thank you for using Inventory Management System!" << std::endl;
			isRunning = false;
			break;
		}
		
		default:
		{
			std::cout << "Invalid choice. Please try again!" << std::endl;
			break;
		}
		}
		HoldScreen("Press Enter to continue...");
		system("cls");
	}

	std::cin.get();
}