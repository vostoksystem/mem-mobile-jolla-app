 
/* calcule le seuil noir/blanc d'une couleur (QColor)
  *param : QColor
  *return : float de 0 à 1
  */
var seuilCouleur=function(hex) {
    return 0.2126*hex.r+0.7152*hex.g+0.0722*hex.b;
}

/* retourne la chaîne css '#rgb' pour la couleur
  *param : valeur rgb
  *return : la chaîne css '#rgb'
  */
var cssRgb=function(r,g,b) {
    return '#'
            +(r<16?'0':'')+Math.ceil(r).toString(16)
            +(g<16?'0':'')+Math.ceil(g).toString(16)
            +(b<16?'0':'')+Math.ceil(b).toString(16);
}

/** decompose une couleur css en [r,g,b]
  *param : chaline css "#rrvvbb"
  *return : [r,g,b]
  */
var versRgb=function(css) {
    if(css.lenght<7) {
        return [0,0,0];
    }

    return [
                parseInt(css.substr(1,2),16),
                parseInt(css.substr(3,2),16),
                parseInt(css.substr(5,2),16)
            ];
}

/* retourne le code css qui correspond au mixage des valeurs de val
  *param : val, tableau de chaine css "#rrggbb"
  *return : chaine css "#rrggbb" correspondante
  */
var mixCssRgb=function(val, mix, master ) {

    if(val.length===0) {
        return "#000000";
    }

    var v=[0,0,0];
    for(var i in val) {
        var t=versRgb(val[i]);
        v[0]+=t[0];
        v[1]+=t[1];
        v[2]+=t[2];
    }
    v[0] = v[0] / val.length;
    v[1] = v[1] / val.length;
    v[2] = v[2] / val.length;

    if(typeof mix !== 'undefined') {
        var m = versRgb(master);
        var mmix = 1.0 - mix;
        v[0] = v[0] * mix + m[0] * mmix;
        v[1] = v[1] * mix + m[1] * mmix;
        v[2] = v[2] * mix + m[2] * mmix;
    }

    return cssRgb(v[0],v[1],v[2]);
}

/* converti une couleur hsv en rgb
  *param : valeur hsv
  *return : couleur au format css rgv -> '#rgb'
  */
var hsv2rgb=function (h,s,v) {
    h=(h%=360)/60;
    var i=Math.floor(h);
    var f=h-i;
    var q=[0,0,0,0,0,0];
    q[0]=q[1]=v*(1-s);
    q[2]=v*(1-s*(1-f));
    q[3]=q[4]=v;
    q[5]=v*(1-s*f);
    return(cssRgb(q[(i+4)%6]*255,q[(i+2)%6]*255,q[i%6]*255));
}

/* calcule un tableau de couleur réparti su tout le spectre
  *return : tableur de couleur
  */
var palette=function() {
    var n=10,v=[[.4,.53],[.63,.8],[.75,.9]],a=[];
    for(var i=0;i< n;i++) {
        for(var k=0;k< v.length;k++) {
            a.push(hsv2rgb(i*(360/n),v[k][0], v[k][1]));
        }
    }
    for(var m=0;m<v.length;m++) {
        a.push(hsv2rgb(30,0,v[m][1]));
    }
    return a;
}
