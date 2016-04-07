from random import randint as rndint, random
import sys

MIN_BONUS=2
MAX_BONUS=6
MIN_NR_REGIONS=2
MAX_NR_REGIONS=5
MIN_NR_SUPER_REGIONS=10
MAX_NR_SUPER_REGIONS=20
MIN_NR_NEIGHBOURS=1
MAX_NR_NEIGHBOURS=2

super_regions = range(rndint(MIN_NR_SUPER_REGIONS, MAX_NR_SUPER_REGIONS))
bonus = { sr : b 
          for sr in super_regions 
          for b in [rndint(MIN_BONUS, MAX_BONUS)] }

total_nr_regions = 0
regions = []
for sr in super_regions:
    nr_regions = rndint(MIN_NR_REGIONS, MAX_NR_REGIONS)
    total_nr_regions += nr_regions
    new_regions = [{"id" : i+len(regions), 
                    "superRegion" : sr,
                    "neighbors" : [r 
                                   for r in range(len(regions),i+len(regions)) + range(i+len(regions)+1,nr_regions+len(regions))
                                   if random()<0.85] } 
                   for i in range(nr_regions)]
    regions.extend(new_regions)

for r in regions:
    if r["id"] < total_nr_regions-1:
        nbs = set(r["neighbors"])
        for i in range(rndint(MIN_NR_NEIGHBOURS, MAX_NR_NEIGHBOURS)):
            nb = rndint(r["id"]+1, total_nr_regions-1)
            nbs.add(nb)
        r["neighbors"] = sorted(nbs)


map_super_regions = [ {"id": sr, "bonus": bonus[sr] } for sr in super_regions ]


newmap = { "Regions": regions, "SuperRegions" : map_super_regions }
newmap_str = repr(newmap).replace("'", '"')

sys.stdout.write("Super-regions: %d, bonuses: %s\n" %(len(super_regions), str(bonus)))
sys.stdout.write("Regions: %d\n" % total_nr_regions)
if len(sys.argv)>1:
    with open(sys.argv[1], 'w') as outfile:
        outfile.write(newmap_str)
        sys.stdout.write("Map written to %s\n" % sys.argv[1])
else:
    sys.stderr.write("ERROR: No file name provided to save the map.\n")
    sys.stderr.write("usage: python createmap.py <map file name>\n")
