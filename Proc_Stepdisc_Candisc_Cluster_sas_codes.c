proc format;
   value location_name
      1='Adogba'
      2='Ajiboda'
      3='Apete'
	  4='Egbeda'
	  5='Moleta'
	  6='Oje'
	  7='Sasa';
run;

data duck_data;
title 'Duck Biometric Data';
input Sex $ Location Shank_colour $ Bill_colour $ Bean_colour $ Crest $ Eye_colour $ Head_Length Neck_Length Body_Length Wing_Length Shank_Length Toe_Length Thigh_Length Bill_Length Breast_Length Breast_width Weight @@;
format Location location_name;
datalines;
/*data available on request*/
;
/*Stepwise Discriminant Analysis*/
title 'Duck Characterisation Data';

proc stepdisc data=duck_data;
   class Location;
run;
/*PROC STEPDISC determined that Neck_Length, Wing_Length, Weight, Toe_Length, and Head_Length were the most discriminating variables*/

/*Canonical Discriminant Analysis*/
title 'Duck Characterisation Data';

proc candisc data=duck_data ncan=3 out=outcanduck;
   ods exclude tstruc bstruc pstruc tcoef pcoef;
   class Location;
   var Neck_Length Wing_Length Weight Toe_Length Head_Length;
run;

/*discriminant analysis*/
proc discrim data=duck_data ncan=3 out=outcanduck ANOVA DISTANCE CROSSVALIDATE LIST CROSSLIST CANONICAL;
   class Location;
   var Neck_Length Wing_Length Weight Toe_Length Head_Length;
run;

/*canonical discriminant function illustrating the distribution of ducks among the different locations in ibadan*/
proc template;
   define statgraph scatter;
      begingraph / attrpriority=none;
         entrytitle 'Distribution of Ducks Among Different Geographical Locations';
         layout overlayequated / equatetype=fit
            xaxisopts=(label='Canonical Variable 1')
            yaxisopts=(label='Canonical Variable 2');
            scatterplot x=Can1 y=Can2 / group=location name='ducks'
                                        markerattrs=(size=6px);
            layout gridded / autoalign=(topright);
               discretelegend 'ducks' / border=false opaque=false;
            endlayout;
         endlayout;
      endgraph;
   end;
run;

proc sgrender data=outcanduck template=scatter;
run;

/*Cluster procedure*/
/*Macro show is used to get the cluster results*/
/*--- First, define macro show ---*/
%macro show;
   proc freq;
      tables cluster*Location / nopercent norow nocol plot=none;
   run;

   proc candisc noprint out=can;
      class cluster;
      var Neck_Length Wing_Length Weight Toe_Length Head_Length;
   run;

   proc sgplot data=can;
      scatter y=can2 x=can1 / group=cluster;
   run;
%mend;

title2 'By Ward''s Method';
ods graphics on;

proc cluster data=duck_data method=ward print=15 ccc pseudo;
   var Neck_Length Wing_Length Weight Toe_Length Head_Length;
   copy Location;
run;

proc tree noprint ncl=3 out=out;
   copy Neck_Length Wing_Length Weight Toe_Length Head_Length Location;
run;

%show;