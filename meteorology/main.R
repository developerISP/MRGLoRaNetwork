# Analisi dati meteo, anno tipo a Col Margherita
# Federico dallo
# 1 aprile 2020
# Obiettivo principale: aggregazione mensile delle variabili meteorologiche
# Obiettivo secondario: trend annuali di variabili -> month/yr, season/yr
# --------------------------------------------------------------------------

rm(list = ls())

### AGGREGAZIONE MENSILE DELLE VARIABILI METEO
### AGGREGAZIONE MENSILE DELLE VARIABILI METEO
### AGGREGAZIONE MENSILE DELLE VARIABILI METEO

# l'header del file è stato modificato per l'importazione
MRG.typical.year <- read.csv(file = "data/MRG_2017_2018.csv",
                             header = TRUE, sep = ",", dec = ".")
MRG.typical.year$Data_solare <- as.POSIXct(MRG.typical.year$Data_solare, format="%d/%m/%Y %H:%M")
MRG.typical.year$Pressione_atm_haPa <- as.numeric(MRG.typical.year$Pressione_atm_haPa)

# subset mensile
jan.MRG.typical <- subset(MRG.typical.year, format.Date(MRG.typical.year$Data_solare, "%m")=="01")
feb.MRG.typical <- subset(MRG.typical.year, format.Date(MRG.typical.year$Data_solare, "%m")=="02")
mar.MRG.typical <- subset(MRG.typical.year, format.Date(MRG.typical.year$Data_solare, "%m")=="03")
apr.MRG.typical <- subset(MRG.typical.year, format.Date(MRG.typical.year$Data_solare, "%m")=="04")
maj.MRG.typical <- subset(MRG.typical.year, format.Date(MRG.typical.year$Data_solare, "%m")=="05")
jun.MRG.typical <- subset(MRG.typical.year, format.Date(MRG.typical.year$Data_solare, "%m")=="06")
jul.MRG.typical <- subset(MRG.typical.year, format.Date(MRG.typical.year$Data_solare, "%m")=="07")
aug.MRG.typical <- subset(MRG.typical.year, format.Date(MRG.typical.year$Data_solare, "%m")=="08")
sep.MRG.typical <- subset(MRG.typical.year, format.Date(MRG.typical.year$Data_solare, "%m")=="09")
oct.MRG.typical <- subset(MRG.typical.year, format.Date(MRG.typical.year$Data_solare, "%m")=="10") # mancano dati per il mese di ottobre
nov.MRG.typical <- subset(MRG.typical.year, format.Date(MRG.typical.year$Data_solare, "%m")=="11")
dec.MRG.typical <- subset(MRG.typical.year, format.Date(MRG.typical.year$Data_solare, "%m")=="12")
#    lista mesi per il testo asse x di alcuni grafici
months.label <- c("jan", "feb", "mar", "apr", "maj", "jun", "jul", "aug", "sep", "oct", "nov", "dec")

# TEMPERATURA
# TEMPERATURA
# TEMPERATURA
# boxplot temperature mensili
#    titolo temporaneo per gli outputs
tmp.main.title <- "MRG temperature typical year"
#    salviamo il plot in png esterno
png(filename = paste0("outputs/", gsub(" ","",tmp.main.title,fixed = TRUE), ".png"), width=3840, height=2160, res=300)#4K
boxplot(jan.MRG.typical$T_C, feb.MRG.typical$T_C, mar.MRG.typical$T_C,
        apr.MRG.typical$T_C, maj.MRG.typical$T_C, jun.MRG.typical$T_C,
        jul.MRG.typical$T_C, aug.MRG.typical$T_C, sep.MRG.typical$T_C,
        oct.MRG.typical$T_C, nov.MRG.typical$T_C, dec.MRG.typical$T_C,
        names = months.label, ylab = "T (°C)", main = tmp.main.title)
text(x = 10, y = mean(MRG.typical.year$T_C, na.rm = TRUE), labels = "missing data", srt=90)
grid(nx = NA, ny = NULL)
dev.off() # questo comando chiude il grafico (è necessario!)

# facciamo una tabella che contenga i dati
tmp_var <- "T_C"
temperature.final.table <- data.frame()
for(i in months.label){
  tmp.target.name <- as.character(paste0(i,".MRG.typical"))
  tmp.target.dataset <- as.data.frame(mget(tmp.target.name))
  tmp.target.sub.dataset <- subset(tmp.target.dataset , select = paste0(i,".MRG.typical.",tmp_var))
  tmp.table <- data.frame(MONTH=i, 
                          MIN=min(tmp.target.sub.dataset[,1], na.rm = TRUE), 
                          MAX=max(tmp.target.sub.dataset[,1], na.rm = TRUE),
                          MEAN=mean(tmp.target.sub.dataset[,1], na.rm = TRUE), 
                          SD=sd(tmp.target.sub.dataset[,1], na.rm = TRUE),
                          #SD_perc = round(abs(sd(tmp.target.sub.dataset[,1], na.rm = TRUE)/mean(tmp.target.sub.dataset[,1], na.rm = TRUE)*100), digits = 0),
                          nr_obs=length(tmp.target.sub.dataset[,1])
                          )
  temperature.final.table <- rbind(temperature.final.table, tmp.table)
}
write.csv(x = temperature.final.table, file = paste0("outputs/", gsub(" ","",tmp.main.title,fixed = TRUE), ".csv"))
rm(tmp.target.name, tmp.target.dataset, tmp.target.sub.dataset, tmp.table, tmp.main.title)

# UMIDITA' RELATIVA
# UMIDITA' RELATIVA
# UMIDITA' RELATIVA
# boxplot mensili umidità relativa
#    titolo temporaneo per gli outputs
tmp.main.title <- "MRG relative humidity typical year"
#    salviamo il plot in png esterno
png(filename = paste0("outputs/", gsub(" ","",tmp.main.title,fixed = TRUE), ".png"), width=3840, height=2160, res=300)#4K
boxplot(jan.MRG.typical$RH, feb.MRG.typical$RH, mar.MRG.typical$RH,
        apr.MRG.typical$RH, maj.MRG.typical$RH, jun.MRG.typical$RH,
        jul.MRG.typical$RH, aug.MRG.typical$RH, sep.MRG.typical$RH,
        oct.MRG.typical$RH, nov.MRG.typical$RH, dec.MRG.typical$RH,
        names = months.label, ylab = "RH (%)", main = tmp.main.title)
text(x = 10, y = mean(MRG.typical.year$RH, na.rm = TRUE), labels = "missing data", srt=90)
grid(nx = NA, ny = NULL)
dev.off() # questo comando chiude il grafico (è necessario!)

# facciamo una tabella che contenga i dati
tmp_var <- "RH"
relativeHumidity.final.table <- data.frame()
for(i in months.label){
  tmp.target.name <- as.character(paste0(i,".MRG.typical"))
  tmp.target.dataset <- as.data.frame(mget(tmp.target.name))
  tmp.target.sub.dataset <- subset(tmp.target.dataset , select = paste0(i,".MRG.typical.",tmp_var))
  tmp.table <- data.frame(MONTH=i, 
                          MIN=min(tmp.target.sub.dataset[,1], na.rm = TRUE), 
                          MAX=max(tmp.target.sub.dataset[,1], na.rm = TRUE),
                          MEAN=mean(tmp.target.sub.dataset[,1], na.rm = TRUE), 
                          SD=sd(tmp.target.sub.dataset[,1], na.rm = TRUE),
                          #SD_perc = round(abs(sd(tmp.target.sub.dataset[,1], na.rm = TRUE)/mean(tmp.target.sub.dataset[,1], na.rm = TRUE)*100), digits = 0),
                          nr_obs=length(tmp.target.sub.dataset[,1])
                          )
  relativeHumidity.final.table <- rbind(relativeHumidity.final.table, tmp.table)
}
write.csv(x = relativeHumidity.final.table, file = paste0("outputs/", gsub(" ","",tmp.main.title,fixed = TRUE), ".csv"))
rm(tmp.target.name, tmp.target.dataset, tmp.target.sub.dataset, tmp.table, tmp_var, tmp.main.title)

# PRESSIONE ATMOSFERICA 
# PRESSIONE ATMOSFERICA 
# PRESSIONE ATMOSFERICA 
# boxplot mensili pressione atmosferica
#    titolo temporaneo per gli outputs
tmp.main.title <- "MRG atmospheric pressure typical year"
#    salviamo il plot in png esterno
png(filename = paste0("outputs/", gsub(" ","",tmp.main.title,fixed = TRUE), ".png"), width=3840, height=2160, res=300)#4K
boxplot(jan.MRG.typical$Pressione_atm_haPa, feb.MRG.typical$Pressione_atm_haPa, mar.MRG.typical$Pressione_atm_haPa,
        apr.MRG.typical$Pressione_atm_haPa, maj.MRG.typical$Pressione_atm_haPa, jun.MRG.typical$Pressione_atm_haPa,
        jul.MRG.typical$Pressione_atm_haPa, aug.MRG.typical$Pressione_atm_haPa, sep.MRG.typical$Pressione_atm_haPa,
        oct.MRG.typical$Pressione_atm_haPa, nov.MRG.typical$Pressione_atm_haPa, dec.MRG.typical$Pressione_atm_haPa,
        names = months.label, ylab = "P (hPa)", main = tmp.main.title)
text(x = 10, y = mean(MRG.typical.year$Pressione_atm_haPa, na.rm = TRUE), labels = "missing data", srt=90)
grid(nx = NA, ny = NULL)
dev.off() # questo comando chiude il grafico (è necessario!)

# facciamo una tabella che contenga i dati
tmp_var <- "Pressione_atm_haPa"
Pressione_atm_haPa.final.table <- data.frame()
for(i in months.label){
  tmp.target.name <- as.character(paste0(i,".MRG.typical"))
  tmp.target.dataset <- as.data.frame(mget(tmp.target.name))
  tmp.target.sub.dataset <- subset(tmp.target.dataset , select = paste0(i,".MRG.typical.",tmp_var))
  tmp.table <- data.frame(MONTH=i, 
                          MIN=min(tmp.target.sub.dataset[,1], na.rm = TRUE), 
                          MAX=max(tmp.target.sub.dataset[,1], na.rm = TRUE),
                          MEAN=mean(tmp.target.sub.dataset[,1], na.rm = TRUE), 
                          SD=sd(tmp.target.sub.dataset[,1], na.rm = TRUE),
                          #SD_perc = round(abs(sd(tmp.target.sub.dataset[,1], na.rm = TRUE)/mean(tmp.target.sub.dataset[,1], na.rm = TRUE)*100), digits = 0),
                          nr_obs=length(tmp.target.sub.dataset[,1])
                          )
  Pressione_atm_haPa.final.table <- rbind(Pressione_atm_haPa.final.table, tmp.table)
}
write.csv(x = Pressione_atm_haPa.final.table, file = paste0("outputs/", gsub(" ","",tmp.main.title,fixed = TRUE), ".csv"))
rm(tmp.target.name, tmp.target.dataset, tmp.target.sub.dataset, tmp.table, tmp_var, tmp.main.title)

# RADIAZIONE SOLARE 
# RADIAZIONE SOLARE 
# RADIAZIONE SOLARE 
# boxplot mensili radiazione solare
#    titolo temporaneo per gli outputs
tmp.main.title <- "MRG solar radiation typical year"
#    salviamo il plot in png esterno
png(filename = paste0("outputs/", gsub(" ","",tmp.main.title,fixed = TRUE), ".png"), width=3840, height=2160, res=300)#4K
boxplot(jan.MRG.typical$Rad_sol_KJ_mq, feb.MRG.typical$Rad_sol_KJ_mq, mar.MRG.typical$Rad_sol_KJ_mq,
        apr.MRG.typical$Rad_sol_KJ_mq, maj.MRG.typical$Rad_sol_KJ_mq, jun.MRG.typical$Rad_sol_KJ_mq,
        jul.MRG.typical$Rad_sol_KJ_mq, aug.MRG.typical$Rad_sol_KJ_mq, sep.MRG.typical$Rad_sol_KJ_mq,
        oct.MRG.typical$Rad_sol_KJ_mq, nov.MRG.typical$Rad_sol_KJ_mq, dec.MRG.typical$Rad_sol_KJ_mq,
        names = months.label, ylab = "Solar Radiation (kJ/mq)", main = tmp.main.title)
text(x = 10, y = mean(MRG.typical.year$Rad_sol_KJ_mq, na.rm = TRUE), labels = "missing data", srt=90)
grid(nx = NA, ny = NULL)
dev.off() # questo comando chiude il grafico (è necessario!)

# facciamo una tabella che contenga i dati
tmp_var <- "Pressione_atm_haPa"
RadiazioneSolare_kJmq.final.table <- data.frame()
for(i in months.label){
  tmp.target.name <- as.character(paste0(i,".MRG.typical"))
  tmp.target.dataset <- as.data.frame(mget(tmp.target.name))
  tmp.target.sub.dataset <- subset(tmp.target.dataset , select = paste0(i,".MRG.typical.",tmp_var))
  tmp.table <- data.frame(MONTH=i, 
                          MIN=min(tmp.target.sub.dataset[,1], na.rm = TRUE), 
                          MAX=max(tmp.target.sub.dataset[,1], na.rm = TRUE),
                          MEAN=mean(tmp.target.sub.dataset[,1], na.rm = TRUE), 
                          SD=sd(tmp.target.sub.dataset[,1], na.rm = TRUE),
                          #SD_perc = round(abs(sd(tmp.target.sub.dataset[,1], na.rm = TRUE)/mean(tmp.target.sub.dataset[,1], na.rm = TRUE)*100), digits = 0),
                          nr_obs=length(tmp.target.sub.dataset[,1])
                          )
  RadiazioneSolare_kJmq.final.table <- rbind(RadiazioneSolare_kJmq.final.table, tmp.table)
}
write.csv(x = RadiazioneSolare_kJmq.final.table, file = paste0("outputs/", gsub(" ","",tmp.main.title,fixed = TRUE), ".csv"))
rm(tmp.target.name, tmp.target.dataset, tmp.target.sub.dataset, tmp.table, tmp_var, tmp.main.title)

# VELOCITA' DEL VENTO 
# VELOCITA' DEL VENTO 
# VELOCITA' DEL VENTO 
# boxplot mensili velocità del vento
#    titolo temporaneo per gli outputs
tmp.main.title <- "MRG wind speed typical year"
#    salviamo il plot in png esterno
png(filename = paste0("outputs/", gsub(" ","",tmp.main.title,fixed = TRUE), ".png"), width=3840, height=2160, res=300)#4K
boxplot(jan.MRG.typical$V_Vent_m_s, feb.MRG.typical$V_Vent_m_s, mar.MRG.typical$V_Vent_m_s,
        apr.MRG.typical$V_Vent_m_s, maj.MRG.typical$V_Vent_m_s, jun.MRG.typical$V_Vent_m_s,
        jul.MRG.typical$V_Vent_m_s, aug.MRG.typical$V_Vent_m_s, sep.MRG.typical$V_Vent_m_s,
        oct.MRG.typical$V_Vent_m_s, nov.MRG.typical$V_Vent_m_s, dec.MRG.typical$V_Vent_m_s,
        names = months.label, ylab = "Wind speed (m/s)", main = tmp.main.title)
text(x = 10, y = mean(MRG.typical.year$V_Vent_m_s, na.rm = TRUE), labels = "missing data", srt=90)
grid(nx = NA, ny = NULL)
dev.off() # questo comando chiude il grafico (è necessario!)

# facciamo una tabella che contenga i dati
tmp_var <- "V_Vent_m_s"
WindSpeed_ms.final.table <- data.frame()
for(i in months.label){
  tmp.target.name <- as.character(paste0(i,".MRG.typical"))
  tmp.target.dataset <- as.data.frame(mget(tmp.target.name))
  tmp.target.sub.dataset <- subset(tmp.target.dataset , select = paste0(i,".MRG.typical.",tmp_var))
  tmp.table <- data.frame(MONTH=i, 
                          MIN=min(tmp.target.sub.dataset[,1], na.rm = TRUE), 
                          MAX=max(tmp.target.sub.dataset[,1], na.rm = TRUE),
                          MEAN=mean(tmp.target.sub.dataset[,1], na.rm = TRUE), 
                          SD=sd(tmp.target.sub.dataset[,1], na.rm = TRUE),
                          #SD_perc = round(abs(sd(tmp.target.sub.dataset[,1], na.rm = TRUE)/mean(tmp.target.sub.dataset[,1], na.rm = TRUE)*100), digits = 0),
                          nr_obs=length(tmp.target.sub.dataset[,1])
                          )
  WindSpeed_ms.final.table <- rbind(WindSpeed_ms.final.table, tmp.table)
}
write.csv(x = WindSpeed_ms.final.table, file = paste0("outputs/", gsub(" ","",tmp.main.title,fixed = TRUE), ".csv"))
rm(tmp.target.name, tmp.target.dataset, tmp.target.sub.dataset, tmp.table, tmp_var, tmp.main.title)


# DIREZIONE DEL VENTO 
# DIREZIONE DEL VENTO # attenzione alle classi di vento  
# DIREZIONE DEL VENTO 
library(ggplot2)
library(RColorBrewer)

# WindRose.R funzione presa da https://stackoverflow.com/questions/17266780/wind-rose-with-ggplot-r
plot.windrose <- function(data,
                          spd,
                          dir,
                          spdres = 2,
                          dirres = 30,
                          spdmin = 2,
                          spdmax = 20,
                          spdseq = NULL,
                          palette = "YlGnBu",
                          countmax = NA,
                          debug = 0,
                          title = ""){
  # Look to see what data was passed in to the function
  if (is.numeric(spd) & is.numeric(dir)){
    # assume that we've been given vectors of the speed and direction vectors
    data <- data.frame(spd = spd,
                       dir = dir)
    spd = "spd"
    dir = "dir"
  } else if (exists("data")){
    # Assume that we've been given a data frame, and the name of the speed 
    # and direction columns. This is the format we want for later use.    
  }  
  # Tidy up input data ----
  n.in <- NROW(data)
  dnu <- (is.na(data[[spd]]) | is.na(data[[dir]]))
  data[[spd]][dnu] <- NA
  data[[dir]][dnu] <- NA
  # figure out the wind speed bins ----
  if (missing(spdseq)){
    spdseq <- seq(spdmin,spdmax,spdres)
  } else {
    if (debug >0){
      cat("Using custom speed bins \n")
    }
  }
  # get some information about the number of bins, etc.
  n.spd.seq <- length(spdseq)
  n.colors.in.range <- n.spd.seq - 1
  # create the color map
  spd.colors <- colorRampPalette(brewer.pal(min(max(3,
                                                    n.colors.in.range),
                                                min(9,
                                                    n.colors.in.range)),                                               
                                            palette))(n.colors.in.range)
  if (max(data[[spd]],na.rm = TRUE) > spdmax){    
    spd.breaks <- c(spdseq,
                    max(data[[spd]],na.rm = TRUE))
    spd.labels <- c(paste(c(spdseq[1:n.spd.seq-1]),
                          '-',
                          c(spdseq[2:n.spd.seq])),
                    paste(spdmax,
                          "-",
                          max(data[[spd]],na.rm = TRUE)))
    spd.colors <- c(spd.colors, "grey50")
  } else{
    spd.breaks <- spdseq
    spd.labels <- paste(c(spdseq[1:n.spd.seq-1]),
                        '-',
                        c(spdseq[2:n.spd.seq]))    
  }
  data$spd.binned <- cut(x = data[[spd]],
                         breaks = spd.breaks,
                         labels = spd.labels,
                         ordered_result = TRUE)
  # clean up the data
  data. <- na.omit(data)
  # figure out the wind direction bins
  dir.breaks <- c(-dirres/2,
                  seq(dirres/2, 360-dirres/2, by = dirres),
                  360+dirres/2)  
  dir.labels <- c(paste(360-dirres/2,"-",dirres/2),
                  paste(seq(dirres/2, 360-3*dirres/2, by = dirres),
                        "-",
                        seq(3*dirres/2, 360-dirres/2, by = dirres)),
                  paste(360-dirres/2,"-",dirres/2))
  # assign each wind direction to a bin
  dir.binned <- cut(data[[dir]],
                    breaks = dir.breaks,
                    ordered_result = TRUE)
  levels(dir.binned) <- dir.labels
  data$dir.binned <- dir.binned
  # Run debug if required ----
  if (debug>0){    
    cat(dir.breaks,"\n")
    cat(dir.labels,"\n")
    cat(levels(dir.binned),"\n")       
  }  
  # deal with change in ordering introduced somewhere around version 2.2
  if(packageVersion("ggplot2") > "2.2"){    
    cat("Hadley broke my code\n")
    data$spd.binned = with(data, factor(spd.binned, levels = rev(levels(spd.binned))))
    spd.colors = rev(spd.colors)
  }
  # create the plot ----
  p.windrose <- ggplot(data = data,
                       aes(x = dir.binned,
                           fill = spd.binned)) +
    geom_bar() + 
    scale_x_discrete(drop = FALSE,
                     labels = waiver()) +
    coord_polar(start = -((dirres/2)/360) * 2*pi) +
    scale_fill_manual(name = paste0(title,"\nWind Speed (m/s)"), 
                      values = spd.colors,
                      drop = FALSE) +
    theme(axis.title.x = element_blank())
  # adjust axes if required
  if (!is.na(countmax)){
    p.windrose <- p.windrose +
      ylim(c(0,countmax))
  }
  # print the plot
  print(p.windrose)  
  # return the handle to the wind rose
  return(p.windrose)
}
# Multiple plot function (http://www.cookbook-r.com/Graphs/Multiple_graphs_on_one_page_(ggplot2)/)
#
# ggplot objects can be passed in ..., or to plotlist (as a list of ggplot objects)
# - cols:   Number of columns in layout
# - layout: A matrix specifying the layout. If present, 'cols' is ignored.
#
# If the layout is something like matrix(c(1,2,3,3), nrow=2, byrow=TRUE),
# then plot 1 will go in the upper left, 2 will go in the upper right, and
# 3 will go all the way across the bottom.
#
multiplot <- function(..., plotlist=NULL, file, cols=1, layout=NULL) {
  library(grid)
  # Make a list from the ... arguments and plotlist
  plots <- c(list(...), plotlist)
  numPlots = length(plots)
  # If layout is NULL, then use 'cols' to determine layout
  if (is.null(layout)) {
    # Make the panel
    # ncol: Number of columns of plots
    # nrow: Number of rows needed, calculated from # of cols
    layout <- matrix(seq(1, cols * ceiling(numPlots/cols)),
                     ncol = cols, nrow = ceiling(numPlots/cols))
  }
  if (numPlots==1) {
    print(plots[[1]])
  } else {
    # Set up the page
    grid.newpage()
    pushViewport(viewport(layout = grid.layout(nrow(layout), ncol(layout))))
    # Make each plot, in the correct location
    for (i in 1:numPlots) {
      # Get the i,j matrix positions of the regions that contain this subplot
      matchidx <- as.data.frame(which(layout == i, arr.ind = TRUE))
      print(plots[[i]], vp = viewport(layout.pos.row = matchidx$row,
                                      layout.pos.col = matchidx$col))
    }
  }
}

#    per fare la media della direzione del vento contenuta nel vettore "vecWindD" bisogna usare la seguente formula:
#    media_vecWindD <- (360+(atan2(sum(sin(vecWindD*pi/180)),sum(cos(vecWindD*pi/180)))*180/pi))%%360
#
# al posto dei boxplot facciamo polar plots mensili direzione del vento
#    titolo temporaneo per gli outputs
tmp.main.title <- "MRG wind direction typical year"
dummy_df_jan <- jan.MRG.typical[,4:5]
dummy_df_feb <- feb.MRG.typical[,4:5]
dummy_df_mar <- mar.MRG.typical[,4:5]
dummy_df_apr <- apr.MRG.typical[,4:5]
dummy_df_maj <- maj.MRG.typical[,4:5]
dummy_df_jun <- jun.MRG.typical[,4:5]
dummy_df_jul <- jul.MRG.typical[,4:5]
dummy_df_aug <- aug.MRG.typical[,4:5]
dummy_df_sep <- sep.MRG.typical[,4:5]
dummy_df_oct <- oct.MRG.typical[,4:5]
dummy_df_nov <- nov.MRG.typical[,4:5]
dummy_df_dec <- dec.MRG.typical[,4:5]
plot_jan <- plot.windrose(title = "January", spd = dummy_df_jan$V_Vent_m_s, dir = dummy_df_jan$Classi_vento_azimut, countmax = 320, spdmin = 0, spdmax = 25, spdseq = c(0,3,6,9,12,15,18,21,25))
plot_feb <- plot.windrose(title = "February", spd = dummy_df_feb$V_Vent_m_s, dir = dummy_df_feb$Classi_vento_azimut, countmax = 320, spdmin = 0, spdmax = 25, spdseq = c(0,3,6,9,12,15,18,21,25))
plot_mar <- plot.windrose(title = "March", spd = dummy_df_mar$V_Vent_m_s, dir = dummy_df_mar$Classi_vento_azimut, countmax = 320, spdmin = 0, spdmax = 25, spdseq = c(0,3,6,9,12,15,18,21,25))
plot_apr <- plot.windrose(title = "April", spd = dummy_df_apr$V_Vent_m_s, dir = dummy_df_apr$Classi_vento_azimut, countmax = 320, spdmin = 0, spdmax = 25, spdseq = c(0,3,6,9,12,15,18,21,25))
plot_maj <- plot.windrose(title = "May", spd = dummy_df_maj$V_Vent_m_s, dir = dummy_df_maj$Classi_vento_azimut, countmax = 320, spdmin = 0, spdmax = 25, spdseq = c(0,3,6,9,12,15,18,21,25))
plot_jun <- plot.windrose(title = "June", spd = dummy_df_jun$V_Vent_m_s, dir = dummy_df_jun$Classi_vento_azimut, countmax = 320, spdmin = 0, spdmax = 25, spdseq = c(0,3,6,9,12,15,18,21,25))
plot_jul <- plot.windrose(title = "July", spd = dummy_df_jul$V_Vent_m_s, dir = dummy_df_jul$Classi_vento_azimut, countmax = 320, spdmin = 0, spdmax = 25, spdseq = c(0,3,6,9,12,15,18,21,25))
plot_aug <- plot.windrose(title = "August", spd = dummy_df_aug$V_Vent_m_s, dir = dummy_df_aug$Classi_vento_azimut, countmax = 320, spdmin = 0, spdmax = 25, spdseq = c(0,3,6,9,12,15,18,21,25))
plot_sep <- plot.windrose(title = "September", spd = dummy_df_sep$V_Vent_m_s, dir = dummy_df_sep$Classi_vento_azimut, countmax = 320, spdmin = 0, spdmax = 25, spdseq = c(0,3,6,9,12,15,18,21,25))
plot_oct <- plot.windrose(title = "October", spd = dummy_df_oct$V_Vent_m_s, dir = dummy_df_oct$Classi_vento_azimut, countmax = 320, spdmin = 0, spdmax = 25, spdseq = c(0,3,6,9,12,15,18,21,25))
plot_nov <- plot.windrose(title = "November", spd = dummy_df_nov$V_Vent_m_s, dir = dummy_df_nov$Classi_vento_azimut, countmax = 320, spdmin = 0, spdmax = 25, spdseq = c(0,3,6,9,12,15,18,21,25))
plot_dec <- plot.windrose(title = "December", spd = dummy_df_dec$V_Vent_m_s, dir = dummy_df_dec$Classi_vento_azimut, countmax = 320, spdmin = 0, spdmax = 25, spdseq = c(0,3,6,9,12,15,18,21,25))
#    salviamo il plot in png esterno
#inverno
png(filename = paste0("outputs/", gsub(" ","",tmp.main.title,fixed = TRUE), "_inverno.png"), width=6400, height=1280, res=300)
multiplot(plot_jan, plot_feb, plot_mar, cols = 3) 
dev.off() # questo comando chiude il grafico (è necessario!)
#primavera
png(filename = paste0("outputs/", gsub(" ","",tmp.main.title,fixed = TRUE), "_primavera.png"), width=6400, height=1280, res=300)
multiplot(plot_apr, plot_maj, plot_jun, cols = 3)
dev.off()
#estate
png(filename = paste0("outputs/", gsub(" ","",tmp.main.title,fixed = TRUE), "_estate.png"), width=6400, height=1280, res=300)
multiplot(plot_jul, plot_aug, plot_sep, cols = 3)
dev.off()
#autunno
png(filename = paste0("outputs/", gsub(" ","",tmp.main.title,fixed = TRUE), "_autunno.png"), width=6400, height=1280, res=300)
multiplot(plot_nov, plot_dec, cols = 3)
dev.off()

# facciamo una tabella che contenga i dati
tmp_var <- "Classi_vento_azimut"
WindDirection_deg.final.table <- data.frame()
for(i in months.label){
  tmp.target.name <- as.character(paste0(i,".MRG.typical"))
  tmp.target.dataset <- as.data.frame(mget(tmp.target.name))
  tmp.target.sub.dataset <- subset(tmp.target.dataset , select = paste0(i,".MRG.typical.",tmp_var))
  tmp.table <- data.frame(MONTH=i, 
                          #MIN=min(tmp.target.sub.dataset[,1], na.rm = TRUE), 
                          #MAX=max(tmp.target.sub.dataset[,1], na.rm = TRUE),
                          #MEAN=mean(tmp.target.sub.dataset[,1], na.rm = TRUE), 
                          MEAN=(360+(atan2(sum(sin(tmp.target.sub.dataset[,1]*pi/180)),sum(cos(tmp.target.sub.dataset[,1]*pi/180)))*180/pi))%%360, 
                          #SD=sd(tmp.target.sub.dataset[,1], na.rm = TRUE),
                          #SD_perc = round(abs(sd(tmp.target.sub.dataset[,1], na.rm = TRUE)/mean(tmp.target.sub.dataset[,1], na.rm = TRUE)*100), digits = 0),
                          nr_obs=length(tmp.target.sub.dataset[,1])
                          )
  WindDirection_deg.final.table <- rbind(WindDirection_deg.final.table, tmp.table)
}
write.csv(x = WindDirection_deg.final.table, file = paste0("outputs/", gsub(" ","",tmp.main.title,fixed = TRUE), ".csv"))
rm(tmp.target.name, tmp.target.dataset, tmp.target.sub.dataset, tmp.table, tmp_var, tmp.main.title)

#  # uso funzioni di base per non caricare librerie (https://stackoverflow.com/questions/41627124/r-base-graphics-circular-histogram):
#  add.filled.arc <- function(center.x, center.y, radius, angle.start, angle.end, col='black') {
#    theta <- seq(angle.start, angle.end, .0001)
#    segments(0, 0, radius*cos(theta), radius*sin(theta), col)
#    segments(0, 0, cos(angle.start), sin(angle.start), col='gray')
#    segments(0, 0, cos(angle.end), sin(angle.end), col='gray')
#  }
#  plot.coord.polar <- function(df) {
#    df <- df[complete.cases(df),]
#    df <- df[order(df[,1]),]
#    df[,1] <- df[,1]*(pi/180) # convert dir to radian
#    df[,2] <- df[,2] / max(df[,2]) # normalize magnitude within [0-1]
#    plot(-1:1, -1:1, type= 'n', xlab='', ylab='', xaxt='n', yaxt='n')
#    sapply(1:(nrow(df)-1), function(i) add.filled.arc(0, 0, df[i,2], df[i,1], df[i+1,1], rainbow(nrow(df))[i]))
#    theta <- seq(0, 2*pi, 0.0001)
#    lines(cos(theta), sin(theta), col='gray')
#  }
# test dataset gennaio
# i <- "jan"
# tmp_var <- "Classi_vento_azimut"
# tmp.target.name <- as.character(paste0(i,".MRG.typical"))
# tmp.target.dataset <- as.data.frame(mget(tmp.target.name))
# tmp.target.sub.dataset <- subset(tmp.target.dataset , select = paste0(i,".MRG.typical.",tmp_var))
# print(hist(tmp.target.sub.dataset[,1], breaks = seq(0,360,30)))
# dummy_df <- data.frame(CLASSI=hist(tmp.target.sub.dataset[,1], breaks = seq(0,360,30))$breaks[1:12],
#                        CONTEGGI=hist(tmp.target.sub.dataset[,1], breaks = seq(0,360,30))$counts)
# avg_dummy_df <- (360+(atan2(sum(sin(tmp.target.sub.dataset[,1]*pi/180)),sum(cos(tmp.target.sub.dataset[,1]*pi/180)))*180/pi))%%360
# avg_dummy_df
# # plot.coord.polar(dummy_df)
# ggplot(dummy_df, aes(x=CLASSI, y=CONTEGGI)) + geom_bar(stat='identity') + coord_polar(start=) 

