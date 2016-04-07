from sys import stdin, stdout, stderr, argv, exit as sysexit

import os.path
import os
import subprocess

def ensure_result_dir(dname):
    if not os.path.exists(dname):
        try:
            os.mkdir(dname)
        except OSError as ose:
            stderr.write("ERROR: cannot create result directory %s: %s\n" %
                         (dname, ose))
            sysexit(1)
    if not os.path.isdir(dname):
        stderr.write("ERROR: %s is not a directory.\n" %
                     dname)
        sysexit(1)
    return dname

map_template = None

def get_map_template():
    global map_template
    if map_template is None:
        with open("map-template.html", 'r') as infile:
            map_template = infile.read()    
    if map_template is None:
        stderr("ERROR: cannot find HTML template: map-template.html\n")
        sysexit(1)
    return map_template

def run_dot(dname, filename):
    dot_output_format = "svg"
    dot_outfile_ext = "." + dot_output_format
    dotfilename = os.path.join(dname, filename + '.dot')
    psfilename = os.path.join(dname, filename + dot_outfile_ext)
    if os.path.isfile(psfilename):
        return
    cmdargs = ["dot", "-T%s" % dot_output_format, dotfilename, "-o", psfilename]
    stdout.write("running command: %s\n" % " ".join(cmdargs))
    subprocess.call(cmdargs)

def create_html_files(dname, map_count, mapround, map_descrs, region_colours):
    template = get_map_template()

    for mc in range(map_count):
        htmlfilename = "map_%s.html" % mc
        mapfilename = "map_%s.svg" % mc
        pl1fg, pl1bg = region_colours["player1"]
        pl2fg, pl2bg = region_colours["player2"]

        d = { "mapcount" : mc,
              "gameround" : mapround[mc],
              "mapfile" : mapfilename,
              "back10" : "Back 10",
              "previous" : "Previous",
              "next" : "Next",
              "forward10" : "Forward 10",
              "mapdescription" : "\n<br/>\n".join(map_descrs[mc]),
              "player1bg" : pl1bg,
              "player2bg" : pl2bg,
              "player1fg" : pl1fg,
              "player2fg" : pl2fg,
          }
        if mc >= 10:
            d["back10"] = "<a href=\"map_%s.html\">Back 10</a>" % (mc - 10)
        if mc > 0:
            d["previous"] = "<a href=\"map_%s.html\">Previous</a> " % (mc - 1)
        if mc < map_count - 1:
            d["next"] = "<a href=\"map_%s.html\">Next</a>" % (mc + 1)
        if mc < map_count - 10:
            d["forward10"] = "<a href=\"map_%s.html\">Forward 10</a> " % (mc + 10)
        
        with open(os.path.join(dname, htmlfilename), 'w') as ofile:
            ofile.write(template % d)


def write_map_as_dot(dname, filename, super_regions, region_attrs, edges_attrs):
    stdout.write("writing map into %s\n" % filename)
    with open(os.path.join(result_dirname, filename), 'w') as outfile:
        outfile.write("digraph G {\n")
        outfile.write("  margin=0.1;\n")
        outfile.write("  center=false;\n")
        sorted_sr = sorted(superregion_regions.keys(), key=lambda s: int(s))
        node_template = "%(region)s [label=\"%(region)s: %(armies)s\",style=%(style)s,fontcolor=%(fg)s,color=%(bg)s];\n"
        for sr in sorted_sr:
            rs = superregion_regions[sr]
            outfile.write('    subgraph cluster%s {\n' % sr)
            outfile.write('        label="Super region %s [%s]";\n' % 
                          (sr, superregion_value[sr]))
            outfile.write("        style=filled;\n")
            outfile.write("        color=sandybrown;\n")
            sorted_regions = sorted([r for r in rs], key=lambda r: int(r))
            for r in sorted_regions:
                outfile.write("        " + node_template % region_attrs[r])
            outfile.write("    }\n    ")


        edges_str_template = "%(source)s -> %(target)s [color=%(color)s,arrowhead=%(arrowhead)s,penwidth=%(penwidth)s];\n"
        edges = sorted(edge_attrs.keys())
        edges_str = "    ".join([ edges_str_template % 
                                  edge_attrs[st] 
                                  for st in edges])
        outfile.write(edges_str)
        outfile.write("\n}\n")


def create_ps_maps(dname):
    for root, dirs, files in os.walk(dname):
        for f in files:
            fname, ext = os.splitext(f)
            if ext == 'dot':
                run_dot(root, fname)



def draw_map(mc, result_dirname, superregion_regions, region_attrs, edge_attrs):
    filename = "map_%s.dot" % mc
    write_map_as_dot(result_dirname, 
                     filename, 
                     superregion_regions, 
                     region_attrs, 
                     edge_attrs)
    f,ext = os.path.splitext(filename)
    run_dot(result_dirname, f)


if __name__ == '__main__':

    if len(argv) != 3:
        stderr.write("Error: incorrect number or arguments.\n")
        stderr.write("Usage: wl2dot.py <result directory> <max_map_number>\n")
        sysexit(1)

    result_dirname = ensure_result_dir(argv[1])
    max_map_number = int(argv[2])

    filenameprefix = argv[1]

    regions = {}
    superregion_regions = {}
    superregion_value = {}
    region_armies = {}
    linecount = 0

    region_colours = {
        "neutral" : ('black','beige'),
        "player1" : ('black', 'red'),
        "player2" : ('white','blue')
    }
    # skip until initialmap
    line = stdin.readline().strip()
    while not line.startswith("initialmap "):
        linecount += 1
        line = stdin.readline().strip()

    initialmap = eval(line[len("initialmap "):], {}, {})
    for sr in initialmap["SuperRegions"]:
        superregion_regions[sr["id"]] = set()
        superregion_value[sr["id"]] = sr["bonus"]
    for r in initialmap["Regions"]:
        regions[r["id"]] = r["neighbors"]
        superregion_regions[r["superRegion"]].add(r["id"])

    sorted_regions = sorted([r for r in regions], key=lambda r: int(r))

    edge_attrs = {
        (int(s),int(t)) : {
            "color" : "grey",
            "arrowhead" : "none",
            "penwidth" : "1.0",
            "source" : s,
            "target" : t,
        }
        for s in sorted_regions
        for t in regions[s]
    }


    # skip until saved game
    line = stdin.readline().strip()
    while line != "Saving the game...":
        linecount += 1
        line = stdin.readline().strip()

    current_player = "neutral"
    map_count = 0
    curround = 0
    mapround = {}
    map_descriptions = {}
    map_descriptions[map_count] = []
    region_attrs = {}
    for line in stdin:
        linecount += 1
        if map_count > max_map_number:
            break
        if line.startswith('round'):
            mapround[map_count] = curround
            draw_map(map_count,
                     result_dirname,
                     superregion_regions, 
                     region_attrs, 
                     edge_attrs)                
            map_count += 1
            map_descriptions[map_count] = []
            curround = int(line[len('round '):])
        if line.startswith('map'):
            words = line[len('map '):].split()
            region_attrs = {}
            for w in words:
                region, owner, armies = w.split(";")
                fg, bg = region_colours[owner]
                region_attrs[region] = {
                    "region" : region,
                    "armies" : armies,
                    "fg" : fg,
                    "bg" : bg,
                    "style" : "filled"
                }

        elif line.startswith('player'):
            words = line.split()
            player = words[0]
            if player != current_player:
                mapround[map_count] = curround
                draw_map(map_count,
                         result_dirname,
                         superregion_regions, 
                         region_attrs, 
                         edge_attrs)                
                map_count += 1
                map_descriptions[map_count] = []                
                current_player = player
            if words[1] == 'place_armies':
                region = words[2]
                new_armies = words[3]
                rdict = [ d for d in initialmap['Regions']
                          if d['id'] == region ][0]
                map_descriptions[map_count].append(
                    "<strong>Placement</strong>: %s<br/>armies: %s, region: %s(%s)" % 
                    (player, new_armies, region, rdict['superRegion']))

            if words[1] == 'attack/transfer':
                words = line.split()
                source = words[2]
                target = words[3]
                armies = words[4]
                sdict = [ d for d in initialmap['Regions']
                          if d['id'] == source ][0]
                tdict = [ d for d in initialmap['Regions']
                          if d['id'] == target ][0]

                map_descriptions[map_count].append(
                    "<strong>Attack/transfer</strong>: %s<br/>armies: %s<br/>from %s(%s) to %s(%s)" % 
                    (player, armies, source, sdict['superRegion'], target, tdict['superRegion']))

    # the final map might not have been drawn yet:
    if (map_count <= max_map_number) and (map_descriptions[map_count] != []):
        mapround[map_count] = curround
        draw_map(map_count,
                 result_dirname,
                 superregion_regions, 
                 region_attrs, 
                 edge_attrs)
        map_count += 1
    create_html_files(result_dirname, map_count, mapround, map_descriptions, region_colours)
