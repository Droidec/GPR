/******************************************************************************
 *
 * gpr_ut_tree.c
 *
 ******************************************************************************
 *
 * Unit tests on "gpr_tree" module
 *
 *****************************************************************************/

#include "gpr_tree.h"

#include <stddef.h> // size_t
#include <stdio.h>  // fprintf, printf
#include <stdlib.h> // abort
#include <string.h> // strlen

void display_tree_node(struct gpr_tnode *node, size_t height)
{
    if (node == NULL)
        return;

    for (size_t i = 0; i < height; i++)
        printf("\t");

    strlen(node->value) == 0 ? printf("<L:\"%s\">\n", node->label) : printf("<L:\"%s\" V:\"%s\">\n", node->label, node->value);
}

void display_tree(struct gpr_tnode *origin, size_t height)
{
    if (origin == NULL)
        return;

    display_tree_node(origin, height);

    if (origin->child != NULL)
        display_tree(origin->child, height + 1);

    if (origin->sibling != NULL)
        display_tree(origin->sibling, height);
}

struct gpr_tnode *build_solar_system(void)
{
    /* Build Solar System */
    struct gpr_tnode *solar_system = gpr_tree_new_node("Solar System");

    /* Build Sun */
    struct gpr_tnode *sun = gpr_tree_new_child(solar_system, "The Sun");

    // Add a fact about the Sun
    gpr_tree_set_node_value(sun, "Outch! It's hot!");

    /* Build planets container */
    struct gpr_tnode *planets = gpr_tree_new_child(solar_system, "Planets of the Solar System");

    /* Build each planet that rotate around the Sun */
    gpr_tree_new_child(planets, "Mercury");
    gpr_tree_new_child(planets, "Venus");
    gpr_tree_new_vchild(planets, "Earth", "Cradle of Humanity");
    gpr_tree_new_child(planets, "Mars");
    struct gpr_tnode *jupiter = gpr_tree_new_child(planets, "Jupiter");
    gpr_tree_new_sibling(jupiter, "Saturn");
    gpr_tree_new_child(planets, "Uranus");
    gpr_tree_new_child(planets, "Neptune");

    /* Build a familiar asteroid */
    gpr_tree_new_vchild(gpr_tree_new_child(solar_system, "Asteroids"), "Pluto", "Planet or asteroid?");

    return solar_system;
}

struct gpr_tnode *build_milky_way(void)
{
    /* Build the Milky Way */
    struct gpr_tnode *milky_way = gpr_tree_new_node("Milky Way");

    /* Build core regions container for the Milky Way */
    struct gpr_tnode *core_regions = gpr_tree_new_child(milky_way, "Bar-shaped core regions");

    /* Build each core region */
    gpr_tree_new_child(core_regions, "Outer arm");
    gpr_tree_new_child(core_regions, "Perseus arm");

    struct gpr_tnode *orion_arm = gpr_tree_new_child(core_regions, "Orion-Cygnus arm");

    // Build Solar System
    struct gpr_tnode *solar_system = build_solar_system();
    GPR_FREE_AND_RETURN_ON_TREE_FAILURE(solar_system, milky_way);

    // Add Solar System as a child of the Orion arm
    gpr_tree_add_child(orion_arm, solar_system);

    gpr_tree_new_child(core_regions, "Carina-Sagittarius arm");
    gpr_tree_new_child(core_regions, "Scutum-Centaraus arm");
    gpr_tree_new_child(core_regions, "Norma arm");
    gpr_tree_new_child(core_regions, "3kpc arm");
    gpr_tree_new_vchild(core_regions, "Galactic Center", "There might be a Black Hole here!");

    return milky_way;
}

struct gpr_tnode *build_big_bang(void)
{
    /* Build the Big Bang */
    struct gpr_tnode *big_bang = gpr_tree_new_vnode("Big Bang", "The origin of everything");

    /* Build the Universe as part of the Big Bang */
    struct gpr_tnode *universe = gpr_tree_new_child(big_bang, "The Universe");

    /* Build a fact about the Universe */
    gpr_tree_new_vchild(universe, "Answer to the Ultimate Question of Life, the Universe, and Everything?", "%d", 42);

    /* Build the Milky Way */
    struct gpr_tnode *milky_way = build_milky_way();
    GPR_FREE_AND_RETURN_ON_TREE_FAILURE(milky_way, big_bang);

    /* Build Andromeda as a sibling of the Milky Way */
    gpr_tree_new_vsibling(milky_way, "Andromeda", "A galaxy that will collide with the Mily Way one day");

    /* Add the Milky Way as a child of the Universe */
    gpr_tree_add_child(universe, milky_way);

    /* Add a fact about the Milky Way */
    gpr_tree_set_node_value(milky_way, "%s", "It has nothing to do with milk");

    /* Check an absolute answer */
    struct gpr_tnode *match = gpr_tree_search_node_by_value(universe, "42");
    if (!match)
    {
        fprintf(stderr, "\"The Hitchhiker's Guide to the Galaxy\" has been lost\n");
        gpr_tree_free(big_bang);
        return NULL;
    }

    /* Attempt to create a paradoxal loop */

    // Search our familiar asteroid
    struct gpr_tnode *pluto = gpr_tree_search_node_by_label(big_bang, "Pluto");
    if (!pluto)
    {
        fprintf(stderr, "Houston, we have lost Pluto\n");
        gpr_tree_free(big_bang);
        return NULL;
    }

    // And what if... Pluto was the origin of everything?...
    if (gpr_tree_add_child(pluto, big_bang) == GPR_ERR_OK)
    {
        fprintf(stderr, "A paradox has occurred (Memory leak)\n");
        return NULL;
    }

    return big_bang;
}

int main()
{
    /* Build the Big Bang */
    struct gpr_tnode *big_bang = build_big_bang();
    if (big_bang == NULL)
    {
        fprintf(stderr, "The Big Bang is kaputt :( God will not be happy about this\n");
        return 1;
    }

    display_tree(big_bang, 0);

    /* Properly destroy everything that exist */
    gpr_tree_free(big_bang);

    return 0;
}
