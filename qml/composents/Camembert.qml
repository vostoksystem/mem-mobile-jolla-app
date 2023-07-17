import QtQuick 2.0
import Sailfish.Silica 1.0

Canvas {
    id: canvas
    width: parent.width
    height: parent.width/1.5
 //   height: parent.width
    x:parent.x
    antialiasing: true

    property var model
    property real ratio:0.0

    property real centerWidth: width /2
    property real centerHeight: height/2
    property real radius0:(height*0.20).toFixed(0)
    property real radiusmax:(height*(0.20+0.30*ratio)).toFixed(0)

    Behavior on ratio {
        NumberAnimation { duration: 1500; easing.type: Easing.Linear }
    }

    onPaint: {
   //     console.log("fait un repaint")
        if( model.count === 0) {
            return;
        }

        var ctx = getContext("2d");

        ctx.save();
        ctx.lineWidth = 1;

        ctx.clearRect(0, 0, canvas.width, canvas.height);
        ctx.translate(canvas.centerWidth, canvas.centerHeight);
        ctx.globalCompositeOperation = 'qt-doft-light';

        var v=0;
        for(var i=0;i< model.count;i++) {
            tracerSecteur(ctx, v, canvas.model.get(i).pourcentage, canvas.model.get(i).couleur );
            v+=canvas.model.get(i).pourcentage;
        }
//console.log( canvas.width + " " + canvas.height)
        ctx.restore();
    }

    function tracerSecteur(ctx, debut, v, couleur) {

        var angleA= (debut *  Math.PI ) *2;
        var angleB= ((debut+v) * Math.PI ) *2;
        var radiusFin = canvas.rayon(v,canvas.radius0, canvas.radiusmax);

        ctx.strokeStyle = couleur;
        ctx.fillStyle = couleur;

        ctx.beginPath();
        ctx.moveTo(canvas.radius0 * Math.cos(angleA),canvas.radius0 * Math.sin(angleA));
        ctx.arc(0,0,canvas.radius0,angleA,angleB);
        ctx.arc(0,0,radiusFin,angleB,angleA, true);
        ctx.closePath();

        ctx.stroke();
        ctx.fill();
    }

    /**
      * calcule un rayon pour la valeur
      * v de 0 à 1.0
      */
    function rayon(v, min, max) {
        return ((max-min)*Math.log(v+1)*Math.LOG2E+min).toFixed(0);
    }

    onModelChanged: {
  //      console.log("changement modele")
        requestPaint();
    }

    onRatioChanged: {
        requestPaint();
    }

    Component.onCompleted: {
       ratio=1.0;
    }
/*
    Connections {
        target: app
        onApplicationActiveChanged: {
            if(app.applicationActive) {
                requestPaint();
            }
        }
    }
*/

}
