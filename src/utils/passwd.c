#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

char trans[16][64] =
{
    "aI2ShnqWHKp8X(w9v6zUZ)7jRrE3Oc0ufBQPJeMbmLxAydsDCtVioYGTFlkNg145",
    "LM3OJsQj9AH4abS6dnxPkR)TE785Cr(WY0NFeDoBwmfKz2VvtUZlcquXhgIpGiy1",
    "WI8M1cOiZxhafuYvC6(z3HUtTPkB0NdyoQ9JlL2bGg5ARSp)nswXjmD47FEreqVK",
    "wy(XnajMKZDUH8fIRux4LvAEob9ChOWt76)iQrgcVsdlFz3q0S5BGpPTk2NJmYe1",
    "tGMxANmD6Ry79IiucU54Cg1EqaQPYWX(r0ZJO3zj2bnBTkfSp8dsh)KFewVolLvH",
    "3ArGl5pD8jI9q6NWMLSeOBTtihC(b2H0kcf1Z)KanwQo7vYzXPFysVJR4muxdgEU",
    "LKhBcfY91SXZGnt7j2idl0x6AbJykvE8(TOQeCMD5FRIHamp4UogWqsNPVr)uz3w",
    "Fi0HUu8gqDQo3r7JxGXjYZeVEkhCspKtS)NaOc42ylvM5dL6AmWBznRwP1T(fIb9",
    "8r73zHmxI9(S6FoPWJYcCXeD5)blZutvp2QhAqRjKdEfaiOgV0ns4U1ywNMGBkTL",
    "O0tiWC5oKHqZflc6AYB(MXT8mhr)skLzn9bPxgaJwEVuRFQUG1vSNp3je7y4D2Id",
    "bC(JOBpS78vzW0HEVPGZrIc)yoiwmqYLhaTnutxKA6dRg9F3D4fleNkM5QU1sj2X",
    "M2dctqO)EhU5ymnBpW76F(AIZgk9u3VzX4oD018YairGRfHLlQvsKJTNSCxjbweP",
    "S9wM2oNyKCeGFsPLrxuEzXcmVTkWZbRtlHf)haq3YJ8U4(AiB50O6QjDvnp7dI1g",
    "KEGpi5m6COzXAWLk4cIyuQose9d0N(lVbFD3BUHft2q7J1YZS8Rvhnj)aPwMgTxr",
    "d9QeMgIUzoDNhw8kW2y)lrAE(OR16PiHfLJuvscYb3jpKC5xZSntBaTX0GF47mqV",
    "x7hHv3CcwN0mAyUPTqrWE9D2eu(Qgk6nfSYLIj)8a1JiGsVdKOtlXF5zpRM4ZoBb"
};

void main(void)
{
    char name[256], *nl;
    int i, j;
    FILE *f;

    printf("Enter name: ");

    fgets(name, 256, stdin);
    nl = strchr(name, '\n');
    if (nl != 0) *nl = 0;

    if (strlen(name) < 5)
    {
	printf("Name must be at least 5 characters\n");
	return;
    }

    f = fopen("code.dat", "w");

    fprintf(f, "%s\n", name);

    for (i = 0, j = 0; i < strlen(name); i++)
    {
	fprintf(f, "%c", trans[j][name[i] & 63]);
	j = (j + 1) & 15;

	fprintf(f, "%c", trans[j][(name[i] >> 4) & 63]);
	j = (j + 1) & 15;
    }

    fprintf(f, "\n");

    fclose(f);

    /*
    char *valid="1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz()";
    char perm[65];
    int i, j, k;

    srandom(time(0));
	  
    for (k = 0; k < 16; k++)
    {
	for (i = 0; i < 65; i++)
	  perm[i] = 0;

	for (i = 0; i < 64; i++)
	{
	    do j = random() % 64; while (perm[j] != 0);

	    perm[j] = valid[i];
	}

	printf("%s\n", perm);
    }
    */
}
