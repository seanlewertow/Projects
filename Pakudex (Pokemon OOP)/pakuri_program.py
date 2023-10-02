from pakudex import *


def menu():  # print menu
    print(
        f"Pakudex Main Menu\n-----------------\n1. List Pakuri\n2. Show Pakuri\n3. Add Pakuri\n4. Evolve Pakuri\n5. "
        f"Sort Pakuri\n6. Exit\n")


def menu_option():   # excpetion for menu options
    while True:
        try:
            select_opt = int(input("What would you like to do? "))
            if select_opt < 1 or select_opt > 6:
                print("Unrecognized menu selection!\n")
                menu()
            else:
                return select_opt
        except:
            print("Unrecognized menu selection!\n")
            menu()


if __name__ == '__main__':  # start program
    sort = False
    print("Welcome to Pakudex: Tracker Extraordinaire!")
    capacity_valid = True
    capacity = int

    while capacity_valid:    # notes the capacity system can hold for species
        try:
            capacity = int(input("Enter max capacity of the Pakudex: "))
            if capacity > 0:
                print(f"The Pakudex can hold {capacity} species of Pakuri.\n")
                capacity_valid = False
            else:
                print("Please enter a valid size.")
        except:  # exception if user inputs letter instead of numbers
            print("Please enter a valid size.")
    pakudex_list = Pakudex(capacity)

while True:
    menu()  # print menu
    menu_opt = menu_option()
    if menu_opt == 1:  # prints out a list of added species in numerical order
        if pakudex_list.get_species_array() is None:
            print(f"No Pakuri in Pakudex yet!\n")
        elif len(pakudex_list.get_species_array()) >= 3 and sort: # prints out a list of added species in sorted order
            print("Pakuri In Pakudex:")
            for i in range(len(pakudex_list.get_species_array())):  # prints added species if there aren't more than 3
                # species (i.e 1 and 2)
                print(f"{i + 1}. {pakudex_list.sort_pakuri()[i]}")
            print()

        elif len(pakudex_list.get_species_array()) < 3 and sort:  # prints out a list of added species in sorted order
            print("Pakuri In Pakudex:")
            print("sorted list:", pakudex_list.sort_pakuri(), "unsorted:", pakudex_list.get_species_array())
            for i in range(len(pakudex_list.sort_pakuri())):  # prints added species if there aren't more than 3
                # species (i.e 1 and 2)
                print(f"{i + 1}. {pakudex_list.sort_pakuri()[i]}")
            print()

        elif len(pakudex_list.get_species_array()) >= 3:
            print("Pakuri In Pakudex:")
            for i in range(len(pakudex_list.get_species_array())):  # prints added species if there aren't more than 3
                # species (i.e 1 and 2)
                print(f"{i + 1}. {pakudex_list.get_species_array()[i]}")
            print()

        else:
            print("Pakuri In Pakudex:")
            for i in range(len(pakudex_list.get_species_array())):  # prints added species if there aren't more than 3
                # species (i.e 1 and 2)
                print(f"{i + 1}. {pakudex_list.get_species_array()[i]}")
            print()

    elif menu_opt == 2:  # displays species name and data
        species = str(input("Enter the name of the species to display: "))
        if pakudex_list.get_species_array() is None:
            print("Error: No such Pakuri!\n")
        elif species not in pakudex_list.get_species_array():
            print("Error: No such Pakuri!\n")
        else:
            for i in range(len(pakudex_list.get_species_array())):    # iterates to find species, then displays species' stats
                if species == pakudex_list.get_species_array()[i]:
                    print("\nSpecies: ", species)
                    print("Attack: ", pakudex_list.get_stats(species)[0])
                    print("Defense: ", pakudex_list.get_stats(species)[1])
                    print("Speed: ", pakudex_list.get_stats(species)[2])


    elif menu_opt == 3:  # adds new species if capacity is not met
        if pakudex_list.get_size() < pakudex_list.get_capacity():
            species = str(input("Enter the name of the species to add: "))

            if pakudex_list.get_stats(species) and species in pakudex_list.get_species_array():  # if there is a species and species in list
                print("Error: Pakudex already contains this species!\n")

            else:
                pakudex_list.add_pakuri(species)
                print(f"Pakuri species {species} successfully added!")
                print()
        else:
            print("Error: Pakudex is full!")
            print()


    elif menu_opt == 4:  # evolves species, multiplies their stats
            species = str(input(f"Enter the name of the species to evolve: "))
            if not pakudex_list.evolve_species(species) is None:
                if species in pakudex_list.get_species_array():
                    print(f"{species} has evolved!\n")
            else:
                print("Error: No such Pakuri!\n")

    elif menu_opt == 5:  # sorts lists
        sort = True
        if sort:
            pakudex_list.sort_pakuri()
        print("Pakuri have been sorted!\n")

    elif menu_opt == 6:  # quits game
        print("Thanks for using Pakudex! Bye!")
        break
