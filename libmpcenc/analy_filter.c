/*
 * Musepack audio compression
 * Copyright (c) 2005-2009, The Musepack Development Team
 * Copyright (C) 1999-2004 Buschmann/Klemm/Piecha/Wolf
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; If not, see <http://www.gnu.org/licenses/>
 */

#include <string.h>

#include <mpc/mpcmath.h>
#include <mpc/datatypes.h>

#define FASTER

/* C O N S T A N T S */

#undef _
#define _(value)  (float)(value##.##L / 0x200000)

static float  Ci_opt [512] = {
    _(   0), _(  213), _( 2037), _(  6574), _(75038), _( 6574), _(2037), _(213),
    _(  -1), _(  218), _( 2000), _(  5959), _(74992), _( 7134), _(2063), _(208),
    _(  -1), _(  222), _( 1952), _(  5288), _(74856), _( 7640), _(2080), _(202),
    _(  -1), _(  225), _( 1893), _(  4561), _(74630), _( 8092), _(2087), _(196),
    _(  -1), _(  227), _( 1822), _(  3776), _(74313), _( 8492), _(2085), _(190),
    _(  -1), _(  228), _( 1739), _(  2935), _(73908), _( 8840), _(2075), _(183),
    _(  -1), _(  228), _( 1644), _(  2037), _(73415), _( 9139), _(2057), _(176),
    _(  -2), _(  227), _( 1535), _(  1082), _(72835), _( 9389), _(2032), _(169),
    _(  -2), _(  224), _( 1414), _(    70), _(72169), _( 9592), _(2001), _(161),
    _(  -2), _(  221), _( 1280), _(  -998), _(71420), _( 9750), _(1962), _(154),
    _(  -2), _(  215), _( 1131), _( -2122), _(70590), _( 9863), _(1919), _(147),
    _(  -3), _(  208), _(  970), _( -3300), _(69679), _( 9935), _(1870), _(139),
    _(  -3), _(  200), _(  794), _( -4533), _(68692), _( 9966), _(1817), _(132),
    _(  -4), _(  189), _(  605), _( -5818), _(67629), _( 9959), _(1759), _(125),
    _(  -4), _(  177), _(  402), _( -7154), _(66494), _( 9916), _(1698), _(117),
    _(  -5), _(  163), _(  185), _( -8540), _(65290), _( 9838), _(1634), _(111),
    _(  -5), _(  146), _(  -45), _( -9975), _(64019), _( 9727), _(1567), _(104),
    _(  -6), _(  127), _( -288), _(-11455), _(62684), _( 9585), _(1498), _( 97),
    _(  -7), _(  106), _( -545), _(-12980), _(61289), _( 9416), _(1428), _( 91),
    _(  -7), _(   83), _( -814), _(-14548), _(59838), _( 9219), _(1356), _( 85),
    _(  -8), _(   57), _(-1095), _(-16155), _(58333), _( 8998), _(1283), _( 79),
    _(  -9), _(   29), _(-1388), _(-17799), _(56778), _( 8755), _(1210), _( 73),
    _( -10), _(   -2), _(-1692), _(-19478), _(55178), _( 8491), _(1137), _( 68),
    _( -11), _(  -36), _(-2006), _(-21189), _(53534), _( 8209), _(1064), _( 63),
    _( -13), _(  -72), _(-2330), _(-22929), _(51853), _( 7910), _( 991), _( 58),
    _( -14), _( -111), _(-2663), _(-24694), _(50137), _( 7597), _( 919), _( 53),
    _( -16), _( -153), _(-3004), _(-26482), _(48390), _( 7271), _( 848), _( 49),
    _( -17), _( -197), _(-3351), _(-28289), _(46617), _( 6935), _( 779), _( 45),
    _( -19), _( -244), _(-3705), _(-30112), _(44821), _( 6589), _( 711), _( 41),
    _( -21), _( -294), _(-4063), _(-31947), _(43006), _( 6237), _( 645), _( 38),
    _( -24), _( -347), _(-4425), _(-33791), _(41176), _( 5879), _( 581), _( 35),
    _( -26), _( -401), _(-4788), _(-35640), _(39336), _( 5517), _( 519), _( 31),
    _( -29), _( -459), _(-5153), _(-37489), _(37489), _( 5153), _( 459), _( 29),
    _( -31), _( -519), _(-5517), _(-39336), _(35640), _( 4788), _( 401), _( 26),
    _( -35), _( -581), _(-5879), _(-41176), _(33791), _( 4425), _( 347), _( 24),
    _( -38), _( -645), _(-6237), _(-43006), _(31947), _( 4063), _( 294), _( 21),
    _( -41), _( -711), _(-6589), _(-44821), _(30112), _( 3705), _( 244), _( 19),
    _( -45), _( -779), _(-6935), _(-46617), _(28289), _( 3351), _( 197), _( 17),
    _( -49), _( -848), _(-7271), _(-48390), _(26482), _( 3004), _( 153), _( 16),
    _( -53), _( -919), _(-7597), _(-50137), _(24694), _( 2663), _( 111), _( 14),
    _( -58), _( -991), _(-7910), _(-51853), _(22929), _( 2330), _(  72), _( 13),
    _( -63), _(-1064), _(-8209), _(-53534), _(21189), _( 2006), _(  36), _( 11),
    _( -68), _(-1137), _(-8491), _(-55178), _(19478), _( 1692), _(   2), _( 10),
    _( -73), _(-1210), _(-8755), _(-56778), _(17799), _( 1388), _( -29), _(  9),
    _( -79), _(-1283), _(-8998), _(-58333), _(16155), _( 1095), _( -57), _(  8),
    _( -85), _(-1356), _(-9219), _(-59838), _(14548), _(  814), _( -83), _(  7),
    _( -91), _(-1428), _(-9416), _(-61289), _(12980), _(  545), _(-106), _(  7),
    _( -97), _(-1498), _(-9585), _(-62684), _(11455), _(  288), _(-127), _(  6),
    _(-104), _(-1567), _(-9727), _(-64019), _( 9975), _(   45), _(-146), _(  5),
    _(-111), _(-1634), _(-9838), _(-65290), _( 8540), _( -185), _(-163), _(  5),
    _(-117), _(-1698), _(-9916), _(-66494), _( 7154), _( -402), _(-177), _(  4),
    _(-125), _(-1759), _(-9959), _(-67629), _( 5818), _( -605), _(-189), _(  4),
    _(-132), _(-1817), _(-9966), _(-68692), _( 4533), _( -794), _(-200), _(  3),
    _(-139), _(-1870), _(-9935), _(-69679), _( 3300), _( -970), _(-208), _(  3),
    _(-147), _(-1919), _(-9863), _(-70590), _( 2122), _(-1131), _(-215), _(  2),
    _(-154), _(-1962), _(-9750), _(-71420), _(  998), _(-1280), _(-221), _(  2),
    _(-161), _(-2001), _(-9592), _(-72169), _(  -70), _(-1414), _(-224), _(  2),
    _(-169), _(-2032), _(-9389), _(-72835), _(-1082), _(-1535), _(-227), _(  2),
    _(-176), _(-2057), _(-9139), _(-73415), _(-2037), _(-1644), _(-228), _(  1),
    _(-183), _(-2075), _(-8840), _(-73908), _(-2935), _(-1739), _(-228), _(  1),
    _(-190), _(-2085), _(-8492), _(-74313), _(-3776), _(-1822), _(-227), _(  1),
    _(-196), _(-2087), _(-8092), _(-74630), _(-4561), _(-1893), _(-225), _(  1),
    _(-202), _(-2080), _(-7640), _(-74856), _(-5288), _(-1952), _(-222), _(  1),
    _(-208), _(-2063), _(-7134), _(-74992), _(-5959), _(-2000), _(-218), _(  1),
};
#undef _


static float M [1024];

void
Klemm ( void )
{
    int    i;
    int    k;
    float  S [512];

    for ( i=0; i<32; i++ ) {
        for ( k=0; k<32; k++ ) {
            M [i*32 + k] = (float) cos ( ((2*i+1)*k & 127) * M_PI/64 );
        }
    }

#ifdef FASTER
    for ( i = 0; i < 384; i++ )
        S[i] = Ci_opt[i];
    for ( i = 384; i < 392; i++ )
        S[i] = 0;
    for ( i = 392; i < 512; i++ )
        S[i] = -Ci_opt[i];
    for ( i = 0; i < 512; i++ )
       Ci_opt[i] = S[i];
    for ( i = 0; i < 128; i++ )
       Ci_opt[i] = S[(i&7) + 120 - (i&120)];
    for ( i = 128; i < 384; i++ )
       Ci_opt[i] = S[i];
    for ( i = 384; i < 512; i++ )
       Ci_opt[i] = S[ 384 + (i&7) + 120 - (i&120)];
#endif
}

 /* D E F I N E S */
#define X_MEM    1152

/* V A R I A B L E S */
float  X_L [ X_MEM + 480 ];
float  X_R [ X_MEM + 480 ];


/* F U N C T I O N S */
// vectoring & partial calculation

static void
Vectoring ( const float* x, float* y )
{
#ifdef FASTER
    int           i = 0;
    const float*  c1;
    const float*  c2;
    const float*  x1;
    const float*  x2;

# define EXPR(c,x)  (c[0]*x[0] + c[1]*x[64] + c[2]*x[128] + c[3]*x[192] + c[4]*x[256] + c[5]*x[320] + c[6]*x[384] + c[7]*x[448])

    i++;
    *y++ = EXPR ((Ci_opt+128),(x+31));

    c1 = Ci_opt - 8;
    c2 = Ci_opt + 128;
    x1 = x + 16;
    x2 = x + 31;
    do {
        x1--, x2--, i++;
        c1 += 8, c2 += 8;
        *y++ = EXPR (c1,x1) + EXPR (c2,x2);
    } while ( i < 16 );

    i++;
    *y++ = EXPR ((Ci_opt+120),(x+0)) + EXPR ((Ci_opt+256),(x+32));

    c1 = Ci_opt + 384 - 8;
    c2 = Ci_opt + 256;
    x1 = x + 47;
    x2 = x + 32;

    do {
        x1++, x2++, i++;
        c1 += 8, c2 += 8;
        *y++ = EXPR (c1,x1) + EXPR (c2,x2);
    } while ( i < 32 );
#else
    int           i;
    const float*  c = Ci_opt;

    for ( i = 0; i < 16; i++, c += 32, x += 4, y += 4 ) {
        y[0] = c[ 0] * x[  0] + c[ 1] * x[ 64] + c[ 2] * x[128] + c[ 3] * x[192] + c[ 4] * x[256] + c[ 5] * x[320] + c[ 6] * x[384] + c[ 7] * x[448];
        y[1] = c[ 8] * x[  1] + c[ 9] * x[ 65] + c[10] * x[129] + c[11] * x[193] + c[12] * x[257] + c[13] * x[321] + c[14] * x[385] + c[15] * x[449];
        y[2] = c[16] * x[  2] + c[17] * x[ 66] + c[18] * x[130] + c[19] * x[194] + c[20] * x[258] + c[21] * x[322] + c[22] * x[386] + c[23] * x[450];
        y[3] = c[24] * x[  3] + c[25] * x[ 67] + c[26] * x[131] + c[27] * x[195] + c[28] * x[259] + c[29] * x[323] + c[30] * x[387] + c[31] * x[451];
    }
#endif
}

// matrixing with Mi[32][32] = Mi[1024]

static void
Matrixing ( const int MaxBand, const float* mi, const float* y, float* samples )
{
    int  i;
#ifdef FASTER
    for ( i = 0; i <= MaxBand; i++, mi += 32, samples += 72 ) {                          // 144 = sizeof(SubbandFloatTyp)/sizeof(float)
        samples[0] =          y[ 0] + mi[ 1] * y[ 1] + mi[ 2] * y[ 2] + mi[ 3] * y[ 3]
                   + mi[ 4] * y[ 4] + mi[ 5] * y[ 5] + mi[ 6] * y[ 6] + mi[ 7] * y[ 7]
                   + mi[ 8] * y[ 8] + mi[ 9] * y[ 9] + mi[10] * y[10] + mi[11] * y[11]
                   + mi[12] * y[12] + mi[13] * y[13] + mi[14] * y[14] + mi[15] * y[15]
                   + mi[16] * y[16] + mi[17] * y[17] + mi[18] * y[18] + mi[19] * y[19]
                   + mi[20] * y[20] + mi[21] * y[21] + mi[22] * y[22] + mi[23] * y[23]
                   + mi[24] * y[24] + mi[25] * y[25] + mi[26] * y[26] + mi[27] * y[27]
                   + mi[28] * y[28] + mi[29] * y[29] + mi[30] * y[30] + mi[31] * y[31];
    }
#else
    for ( i = 0; i <= MaxBand; i++, mi += 32, samples += 72 ) {                          // 144 = sizeof(SubbandFloatTyp)/sizeof(float)
        samples[0] =           y[16]        + mi[ 1] * (y[15]+y[17])
                   + mi[ 2] * (y[14]+y[18]) + mi[ 3] * (y[13]+y[19])
                   + mi[ 4] * (y[12]+y[20]) + mi[ 5] * (y[11]+y[21])
                   + mi[ 6] * (y[10]+y[22]) + mi[ 7] * (y[ 9]+y[23])
                   + mi[ 8] * (y[ 8]+y[24]) + mi[ 9] * (y[ 7]+y[25])
                   + mi[10] * (y[ 6]+y[26]) + mi[11] * (y[ 5]+y[27])
                   + mi[12] * (y[ 4]+y[28]) + mi[13] * (y[ 3]+y[29])
                   + mi[14] * (y[ 2]+y[30]) + mi[15] * (y[ 1]+y[31])
                   + mi[16] * (y[ 0]+y[32])
                   + mi[31] * (y[47]-y[49]) + mi[30] * (y[46]-y[50])
                   + mi[29] * (y[45]-y[51]) + mi[28] * (y[44]-y[52])
                   + mi[27] * (y[43]-y[53]) + mi[26] * (y[42]-y[54])
                   + mi[25] * (y[41]-y[55]) + mi[24] * (y[40]-y[56])
                   + mi[23] * (y[39]-y[57]) + mi[22] * (y[38]-y[58])
                   + mi[21] * (y[37]-y[59]) + mi[20] * (y[36]-y[60])
                   + mi[19] * (y[35]-y[61]) + mi[18] * (y[34]-y[62])
                   + mi[17] * (y[33]-y[63]);
    }
#endif
}

// Analysis-Filterbank
void
Analyse_Filter ( const PCMDataTyp* in, SubbandFloatTyp* out, const int MaxBand )
{
#ifdef FASTER
    float         Y_L [32];
    float         Y_R [32];
#else
    float         Y_L [64];
    float         Y_R [64];
#endif
    float*        x;
    const float*  pcm;
    int           n;
    int           i;

    /************************* calculate L-signal ***************************/
    memcpy ( X_L + X_MEM, X_L, 480*sizeof(*X_L) );
    x      = X_L + X_MEM;
    pcm    = in->L + 479;                               // 479 = CENTER + 31
    for ( n = 0; n < 36; n++, pcm += 64 ) {
        x  -= 32;                                       // updating vector x
#ifdef FASTER
        for ( i = 0; i < 16; i++ )
            x[i] = *pcm--;
        for ( i = 31; i >= 16; i-- )
            x[i] = *pcm--;
#else
        for ( i = 0; i < 32; i++ )
            x[i] = *pcm--;
#endif
        Vectoring ( x, Y_L );                           // vectoring & partial calculation
        Matrixing ( MaxBand, M, Y_L, &out[0].L[n] );    // matrixing
    }

    /************************* calculate R-signal ***************************/
    memcpy ( X_R + X_MEM, X_R, 480*sizeof(*X_R) );
    x      = X_R + X_MEM;
    pcm    = in->R + 479;                               // 479 = CENTER + 31
    for ( n = 0; n < 36; n++, pcm += 64 ) {
        x  -= 32;                                       // updating vector x
#ifdef FASTER
        for ( i = 0; i < 16; i++ )
            x[i] = *pcm--;
        for ( i = 31; i >= 16; i-- )
            x[i] = *pcm--;
#else
        for ( i = 0; i < 32; i++ )
            x[i] = *pcm--;
#endif
        Vectoring ( x, Y_R );                           // vectoring & partial calculation
        Matrixing ( MaxBand, M, Y_R, &out[0].R[n] );    // matrixing
    }
}

void
Analyse_Init ( float Left, float Right, SubbandFloatTyp* out, const int MaxBand )
{
#ifdef FASTER
    float         Y_L [32];
    float         Y_R [32];
#else
    float         Y_L [64];
    float         Y_R [64];
#endif
    float*        x;
    int           n;
    int           i;

    /************************* calculate L-signal ***************************/
    memcpy ( X_L + X_MEM, X_L, 480*sizeof(*X_L) );
    x      = X_L + X_MEM;

    for ( n = 0; n < 36; n++ ) {
        x  -= 32;                                       // updating vector x
#ifdef FASTER
        for ( i = 0; i < 16; i++ )
            x[i] = Left;
        for ( i = 31; i >= 16; i-- )
            x[i] = Left;
#else
        for ( i = 0; i < 32; i++ )
            x[i] = Left;
#endif
        Vectoring ( x, Y_L );                           // vectoring & partial calculation
        Matrixing ( MaxBand, M, Y_L, &out[0].L[n] );    // matrixing
    }

    /************************* calculate R-signal ***************************/
    memcpy ( X_R + X_MEM, X_R, 480*sizeof(*X_R) );
    x      = X_R + X_MEM;
    for ( n = 0; n < 36; n++ ) {
        x  -= 32;                                       // updating vector x
#ifdef FASTER
        for ( i = 0; i < 16; i++ )
            x[i] = Right;
        for ( i = 31; i >= 16; i-- )
            x[i] = Right;
#else
        for ( i = 0; i < 32; i++ )
            x[i] = Right;
#endif
        Vectoring ( x, Y_R );                           // vectoring & partial calculation
        Matrixing ( MaxBand, M, Y_R, &out[0].R[n] );    // matrixing
    }
}

/* end of analy_filter.c */
