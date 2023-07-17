import QtQuick 2.2
import Sailfish.Silica 1.0
import "../composents"
import "../js/fichier.js" as FichierJS

Page {
    id:page
    width: root.width

    property bool active: status===PageStatus.Active

    property string app_id: "harbour-vostok_meminfo"
    property string _icone: "image://theme/icon-m-sailfish"
    property string _titre
    property string _version
    property string _date

    property int _total:0
    property int _application:0
    property int _config:0
    property int _donnee:0
    property int _cache:0

    property bool _entravail:BackupService.entravail
    property bool _encache:false
    property bool _backupPossible:true // pas de restaure et seulement cette app pas en backup


    //---------------------------
    function supprimerBackup(b) {
        if(remorsepop.visible) {
            return;
        }

        (function(){
            var backup = b;
            remorsepop.execute("Deleting", function() {
                BackupService.effaceBackup(app_id, backup);
            });
        })();
    }

    //---------------------------
    function lancerBackup() {
        if(BackupService.backupRestaurePossible(app_id)===false) {
            return;
        }

        BackupService.backupPour(app_id);
    }

    //---------------------------
    function lancerRestauration(back_id) {
        if(BackupService.backupRestaurePossible(app_id)===false) {
            return;
        }

       BackupService.restaureBackup(app_id,back_id);
    }

    //---------------------------
    function effacerCache() {
        _encache=true;
        BackupService.clearCache(app_id);
    }

    ListModel {
        id: listeBackup

        //-----------------------------------
        function rafraichir() {
            var l = BackupService.listePour(app_id);
            var locale= Qt.locale();

            var tmp = [];
            for(var i = 0; i< l.length ; i++) {
                var v = FichierJS.nomSansExt(l[i]);
                var d = new Date();
                d.setTime(v*1000);

                tmp.push({
                             back_id:l[i],
                             date:v,
                             nom:
                             d.toLocaleDateString(locale,"ddd dd MMM yy") + " " +
                             d.toLocaleTimeString(locale,"hh:mm")
                         });
            }

            tmp = tmp.sort(function(a,b) {return b.date - a.date;});

            clear();

            for(var j=0 ; j< tmp.length ; j++) {
                append(tmp[j]);
            }
        }
    }

    ListModel {
        id:partageList
    }

    RemorsePopup { id: remorsepop }


    SilicaFlickable {
        width: parent.width
        height: parent.height
        contentHeight: column.height
        anchors.fill: parent

        VerticalScrollDecorator { }

        PullDownMenu {
            MenuItem {
                text: vi18n.get("menu.config")
                onClicked: pageStack.push(Qt.resolvedUrl("config.qml"))
            }
        }

        Column {
            id: column
            x:Theme.horizontalPageMargin
            width: page.width - (Theme.horizontalPageMargin*2)
            spacing: Theme.paddingMedium
            anchors.margins: Theme.horizontalPageMargin



            Spacer{}
            Item {
                height: Math.max(t_info.height, t_icon.height)
                width: parent.width

                Item {
                    id: t_info
                    height:t_titre.height + t_version.height + t_date.height
                    anchors {
                        left: parent.left
                        verticalCenter: parent.verticalCenter
                    }

                    Label {
                        id: t_titre
                        text: _titre
                        font.pixelSize: Theme.fontSizeLarge
                        font.bold: true
                    }

                    Label {
                        id: t_version
                        text:_version
                        font.pixelSize: Theme.fontSizeSmall
                        anchors {
                            top : t_titre.bottom
                            topMargin: Theme.paddingMedium
                        }
                    }

                    Label {
                        id: t_date
                        text:_date
                        font.pixelSize: Theme.fontSizeSmall
                        anchors {
                            top : t_version.bottom
                            topMargin: Theme.paddingMedium
                        }
                    }
                }

                Image {
                    id: t_icon
                    width: Theme.iconSizeLarge
                    height: width
                    anchors {
                        verticalCenter: parent.verticalCenter
                        right: parent.right
                        rightMargin: Theme.horizontalPageMargin
                    }
                    source: _icone
                }

            }
            Spacer{
                width: parent.width
            }

            //-----------------------------
            LabelRubrique {
                text: vi18n.get("app.stockage")
                position: true
            }

            Column {
                spacing: Theme.paddingMedium
                width: parent.width

                Item {
                    width: parent.width
                    height: Theme.fontSizeMedium
                    Label {
                        color: Theme.highlightColor
                        text: vi18n.get("app.app")
                    }
                    Label {
                        color: Theme.highlightColor
                        anchors.right: parent.right
                        anchors.rightMargin: Theme.horizontalPageMargin
                        text: _application > 1024 ? FichierJS.getTaille(_application) : 0
                    }
                }
                Item {
                    width: parent.width
                    height: Theme.fontSizeMedium
                    Label {
                        color: Theme.highlightColor
                        text: vi18n.get("app.data")
                    }
                    Label {
                        color: Theme.highlightColor
                        anchors.right: parent.right
                        anchors.rightMargin: Theme.horizontalPageMargin
                        text: _donnee + _config > 1024 ? FichierJS.getTaille(_donnee + _config) : 0
                    }
                }
                Item {
                    width: parent.width
                    height: Theme.fontSizeMedium
                    Label {
                        color: Theme.highlightColor
                        text: vi18n.get("app.cache")
                    }
                    Label {
                        color: Theme.highlightColor
                        anchors.right: parent.right
                        anchors.rightMargin: Theme.horizontalPageMargin
                        text: _cache > 1024 ? FichierJS.getTaille(_cache) : 0
                    }
                }
                Rectangle {
                    color: Theme.highlightColor
                    width: parent.width
                    height: 1
                }
                Item {
                    width: parent.width
                    height: Theme.fontSizeMedium
                    Label {
                        color: Theme.highlightColor
                        text: vi18n.get("app.total")
                    }
                    Label {
                        color: Theme.highlightColor
                        anchors.right: parent.right
                        anchors.rightMargin: Theme.horizontalPageMargin
                        text: _total > 1024 ? FichierJS.getTaille(_total) : ""
                    }
                }

            }
            Spacer{}

            LabelRubrique {
                visible: partageList.count >0
                text: vi18n.get("app.partage")
                position: true
            }

            Repeater {
                visible: partageList.count >0
                width: parent.width

                model: partageList

                delegate: Label {
                    text: titre
                    color: Theme.highlightColor
                }
            }

            //-----------------------------
            LabelRubrique {
                text: vi18n.get("app.sauve")
                position: true
            }

            Repeater {
                width: parent.width
                visible: listeBackup.count > 0

                model: listeBackup

                delegate: BackgroundItem {
                    width: parent.width
                    contentHeight: Theme.itemSizeSmall
                    enabled: _entravail === false

                    Label {
                        text: nom
                        color: highlighted ? Theme.highlightColor : Theme.primaryColor
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    IconButton {
                        icon.source: "image://theme/icon-m-delete"
                        width: Theme.iconSizeSmall
                        height: width
                        anchors {
                            right: parent.right
                            rightMargin: Theme.horizontalPageMargin
                            verticalCenter: parent.verticalCenter
                        }
                        onClicked: supprimerBackup(back_id);
                    }
                    onClicked: {
                        var dialog = pageStack.push("../composents/DialogRestaure.qml");
                        dialog.accepted.connect(function() {
                            lancerRestauration(back_id);
                        })
                    }
                }
            }

            Label {
                visible: listeBackup.count === 0
                text:  vi18n.get("app.pasdebackup")
                anchors.horizontalCenter: parent.horizontalCenter
                font.pixelSize: Theme.fontSizeLarge
                font.bold: true
                color: Theme.highlightColor
            }

            Button {
                text: vi18n.get("app.sauver")
                anchors.horizontalCenter: parent.horizontalCenter
                enabled: _backupPossible === true && ((_config + _donnee) > 0) && SdcardLocator.present
                onClicked: lancerBackup()
            }
            Spacer{}

            //-----------------------------
            LabelRubrique {
                text: vi18n.get("app.cache")
                position: true
            }

            Button {
                text: vi18n.get("app.effcache")
                anchors.horizontalCenter: parent.horizontalCenter
                enabled: (_cache > 0) && (_encache === false)
                onClicked: effacerCache()
            }
            Spacer{}
        }
    }

    Component.onCompleted: {
        _version = CalculApplication.getVersion(app_id);
        _backupPossible = BackupService.backupRestaurePossible(app_id);

        var tmpd = CalculApplication.getDateInstall(app_id);
        if(tmpd>0) {
            var d = new Date();
            d.setTime(tmpd*1000);
            _date=d.toLocaleDateString(Qt.locale(),"ddd dd MMM yy");
        }

        var part = CalculApplication.getPartage(app_id);
        for(var i =0 ; i< part.length ; i++) {
            partageList.append({titre: part[i]});
        }

        listeBackup.rafraichir();

        var desk = DesktopService.foundForAsQVariant(app_id);
        if(desk === undefined) {
            return;
        }

        _titre = desk.name;
        _icone = desk.icon;

        var taille = CalculApplication.getTaille(app_id);
        if(taille === undefined) {
            return;
        }

        _application = taille.application;
        _config = taille.config;
        _donnee = taille.donnee;
        _cache = taille.cache;
        _total = _application + _donnee + _cache;

    }

    Connections {
        target: CalculApplication

        onCalculDisponible : {

            if( taille === undefined) {
                return;
            }

            if( app_id !== id_desktop ) {
                return;
            }

            _application = taille.application;
            _config = taille.config;
            _donnee = taille.donnee;
            _cache = taille.cache;
            _total = _application + _donnee + _cache + _config;
        }
    }

    Connections {
        target: BackupService

        onBackupFait: {
            if(page.app_id !==app_id ) {
                return;
            }

            listeBackup.rafraichir();
        }

        onBackupSupprimer: {
            if(page.app_id !==app_id ) {
                return;
            }
            listeBackup.rafraichir();
        }

        onRestaureFait: {
            if(page.app_id !==app_id ) {
                return;
            }
        }

        onCacheEfface: {
            if(page.app_id !==app_id ) {
                return;
            }

            _cache = 0;
            _encache=false;
        }

        onEnTravailChanged: {
            _backupPossible=BackupService.backupRestaurePossible(app_id);
        }
    }
}
