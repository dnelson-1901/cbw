/*
 * Routines to use knowledge of Zee to expand knowledge of Perms.
 *
 * Bob Baldwin, January 1985.
 */


#include	<stdio.h>
#include	"window.h"
#include	"specs.h"


extern	int	kzee[];
extern	int	kzeeinv[];


/* User command to propage info from Ai to Aj using Zee**(j-i).
 * Returns NULL if sucessful.
 */
char *pgate(str)
char	*str;
{
	FILE	*fd;
	int		i;
	int		k;
	int		from, to;
	int		from1, from2, to1, to2;
	int		*zeek, *zeeinvk;	/* Zee ** k */
	int		*fromperm, *tmp1perm, *tmp2perm;
	int		kexp[BLOCKSIZE+1], kexpinv[BLOCKSIZE+1];
	int		ktmp1perm[BLOCKSIZE+1];
	int		ktmp2perm[BLOCKSIZE+1];

	tmp1perm = ktmp1perm;
	tmp2perm = ktmp2perm;
	zeek = kexp;
	zeeinvk = kexpinv;

	if ((i = sscanf(str,"%*[^:]: %d-%d %*[^:]: %d-%d", &from1, &from2, &to1, &to2)) != 4) {
		return("Could not parse all the arguments.");
		}

	for (to = to1; to <= to2; to++)
	{
		for (from = from1; from <= from2; from++)
		{
			if (dbsgetblk(&dbstore) != to)
				dbssetblk(&dbstore, to);

			k = to - from;
			if (k >= 0) {
				expperm(kzee, zeek, k);
				expperm(kzeeinv, zeeinvk, k);
				}
			else {
				expperm(kzee, zeeinvk, -k);
				expperm(kzeeinv, zeek, -k);
				}

			multperm(refperm(from), zeek, tmp1perm);
			multperm(zeeinvk, tmp1perm, tmp2perm);

			if (!dbsmerge(&dbstore, tmp2perm))  {
				wl_rcursor(&user);
				return("Merge conflicts with current plaintext.");
				}

			wl_rcursor(&user);
			}
		}
	return(NULL);
}
