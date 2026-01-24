/*
 * Part of Astonia Server 3.5 (c) Daniel Brockhaus. Please read license.txt.
 */

#define MAX_POLES_PER_TUNNEL_LEVEL 20 // The amount of times you can touch the poles per level of tunnels

struct tunnel_ppd {
    int clevel; // current level (resets with every entry)
    unsigned char used[204]; // already used teleport out X times
};