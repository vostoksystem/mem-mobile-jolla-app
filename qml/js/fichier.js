var getDuree = function (d) {
    var o=d;
    var h = (Math.floor(d/3600.0)).toFixed();
    d %= 3600;
    var m = (Math.floor(d/60.0)).toFixed();
    var s = d%60;

    var f= (h>0 ? h + ":":"") +
            (m <10? "0" : "")+ m + ":"+
            (s<10? "0" : "")+ s;
    return h>0 || m>0 || s>0 ? f : "--";
};

/*
 */
var urlVirtuelle=function(v) {
    var l = serv_config.listeSource;
    for(var i in l) {
        if(v.startsWith(l[i])) {
            return v.replace(l[i],"");
        }
    }

    return null;
}

/* retourne le rep parent d'un chemin
  *param : url ; peut être relative
  *return : le chemin parent ou vide si ne trouve pas
  */
var repParent=function(v) {
    var r = new RegExp("");
    var a = /(.*)\/[^\/]+$/.exec(v);
    if(a.length<=1) {
        return "/";
    }

    return a[1];
}

/**
  *param url
  *return le nom ou chaîne vide
  */
var nomDepuisUrl=function(url) {
   var r = new RegExp("([^/]+)$");
    var a = r.exec(url);
    if(!a || a.length<2) {
        return "";
    }
    return a[1];
}

/**
  *param url
  *return le nom ou chaîne vide
  */
var nomSansExt=function(nom) {
   var r = new RegExp("^(.[^\\.]*)");
    var a = r.exec(nom);
    if(!a || a.length<2) {
        return "";
    }
    return a[1];
}

/**
  *retourne la taille en format "humain"
  *param v : tailel en octet
  *return chaine corespondante : xMb, Gb...
  */
var getTaille=function(v, precision) {
    precision = typeof precision === "undefined" ? 1 : precision;

    if(v > 1073741824) {
        return (v / 1073741824).toFixed(precision) + "Gb";
    }
    if(v > 1048576) {
        return (v / 1048576).toFixed(precision) + "Mb";
    }
    if(v > 1024) {
        return (v / 1024).toFixed(precision) + "Kb";
    }

    return v + "octets";
}
