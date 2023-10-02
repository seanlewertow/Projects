from pakuri import *

class Pakudex:
    #constructor
    def __init__(self, capacity = 20):
        self.pakuris = []
        self.capacity = capacity
        self.size = 0

    def get_size(self):
        return self.size

    def get_capacity(self):
        return self.capacity
    #returns a list of species in normal order
    def get_species_array(self):
        species_array = []
        try:
            if self.pakuris[0] is not None:
                for i in range(len(self.pakuris)):
                    species_array.append(self.pakuris[i].get_species())
                return species_array
        except:
            return None

    # returns an int list of 1 species in list
    def get_stats(self, species):
        try:
            stat_list = []
            for i in range(len(self.pakuris)):
                if species == self.pakuris[i].get_species():
                    stat_list.append(self.pakuris[i].attack)
                    stat_list.append(self.pakuris[i].defense)
                    stat_list.append(self.pakuris[i].speed)
                    return stat_list
        except:
            return None

    # Python sorting
    def sort_pakuri(self):
        species_array = []
        for i in range(len(self.pakuris)):
            species_array.append(self.pakuris[i].get_species())
        return species_array


    # adds species to the Pakudex
    def add_pakuri(self, species):
        new_species = Pakuri(species)
        self.pakuris.append(new_species)
        self.size += 1
        return True

    #doubles attack, quad defense, trip speed
    def evolve_species(self, species):
        try:
            for i in range(len(self.pakuris)):
                if self.pakuris[i].get_species() == species:
                    self.pakuris[i].evolve()
                    return True
        except:
            return False

