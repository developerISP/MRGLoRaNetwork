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


# working in progress # # DIREZIONE DEL VENTO 
# working in progress # # DIREZIONE DEL VENTO # attenzione alle classi di vento  
# working in progress # # DIREZIONE DEL VENTO 
# working in progress # 
# working in progress # # WindRose.R funzione presa da https://stackoverflow.com/questions/17266780/wind-rose-with-ggplot-r
# working in progress # require(ggplot2)
# working in progress # require(RColorBrewer)
# working in progress # 
# working in progress # plot.windrose <- function(data,
# working in progress #                           spd,
# working in progress #                           dir,
# working in progress #                           spdres = 2,
# working in progress #                           dirres = 30,
# working in progress #                           spdmin = 2,
# working in progress #                           spdmax = 20,
# working in progress #                           spdseq = NULL,
# working in progress #                           palette = "YlGnBu",
# working in progress #                           countmax = NA,
# working in progress #                           debug = 0){
# working in progress #   
# working in progress #   
# working in progress #   # Look to see what data was passed in to the function
# working in progress #   if (is.numeric(spd) & is.numeric(dir)){
# working in progress #     # assume that we've been given vectors of the speed and direction vectors
# working in progress #     data <- data.frame(spd = spd,
# working in progress #                        dir = dir)
# working in progress #     spd = "spd"
# working in progress #     dir = "dir"
# working in progress #   } else if (exists("data")){
# working in progress #     # Assume that we've been given a data frame, and the name of the speed 
# working in progress #     # and direction columns. This is the format we want for later use.    
# working in progress #   }  
# working in progress #   
# working in progress #   # Tidy up input data ----
# working in progress #   n.in <- NROW(data)
# working in progress #   dnu <- (is.na(data[[spd]]) | is.na(data[[dir]]))
# working in progress #   data[[spd]][dnu] <- NA
# working in progress #   data[[dir]][dnu] <- NA
# working in progress #   
# working in progress #   # figure out the wind speed bins ----
# working in progress #   if (missing(spdseq)){
# working in progress #     spdseq <- seq(spdmin,spdmax,spdres)
# working in progress #   } else {
# working in progress #     if (debug >0){
# working in progress #       cat("Using custom speed bins \n")
# working in progress #     }
# working in progress #   }
# working in progress #   # get some information about the number of bins, etc.
# working in progress #   n.spd.seq <- length(spdseq)
# working in progress #   n.colors.in.range <- n.spd.seq - 1
# working in progress #   
# working in progress #   # create the color map
# working in progress #   spd.colors <- colorRampPalette(brewer.pal(min(max(3,
# working in progress #                                                     n.colors.in.range),
# working in progress #                                                 min(9,
# working in progress #                                                     n.colors.in.range)),                                               
# working in progress #                                             palette))(n.colors.in.range)
# working in progress #   
# working in progress #   if (max(data[[spd]],na.rm = TRUE) > spdmax){    
# working in progress #     spd.breaks <- c(spdseq,
# working in progress #                     max(data[[spd]],na.rm = TRUE))
# working in progress #     spd.labels <- c(paste(c(spdseq[1:n.spd.seq-1]),
# working in progress #                           '-',
# working in progress #                           c(spdseq[2:n.spd.seq])),
# working in progress #                     paste(spdmax,
# working in progress #                           "-",
# working in progress #                           max(data[[spd]],na.rm = TRUE)))
# working in progress #     spd.colors <- c(spd.colors, "grey50")
# working in progress #   } else{
# working in progress #     spd.breaks <- spdseq
# working in progress #     spd.labels <- paste(c(spdseq[1:n.spd.seq-1]),
# working in progress #                         '-',
# working in progress #                         c(spdseq[2:n.spd.seq]))    
# working in progress #   }
# working in progress #   data$spd.binned <- cut(x = data[[spd]],
# working in progress #                          breaks = spd.breaks,
# working in progress #                          labels = spd.labels,
# working in progress #                          ordered_result = TRUE)
# working in progress #   # clean up the data
# working in progress #   data. <- na.omit(data)
# working in progress #   
# working in progress #   # figure out the wind direction bins
# working in progress #   dir.breaks <- c(-dirres/2,
# working in progress #                   seq(dirres/2, 360-dirres/2, by = dirres),
# working in progress #                   360+dirres/2)  
# working in progress #   dir.labels <- c(paste(360-dirres/2,"-",dirres/2),
# working in progress #                   paste(seq(dirres/2, 360-3*dirres/2, by = dirres),
# working in progress #                         "-",
# working in progress #                         seq(3*dirres/2, 360-dirres/2, by = dirres)),
# working in progress #                   paste(360-dirres/2,"-",dirres/2))
# working in progress #   # assign each wind direction to a bin
# working in progress #   dir.binned <- cut(data[[dir]],
# working in progress #                     breaks = dir.breaks,
# working in progress #                     ordered_result = TRUE)
# working in progress #   levels(dir.binned) <- dir.labels
# working in progress #   data$dir.binned <- dir.binned
# working in progress #   
# working in progress #   # Run debug if required ----
# working in progress #   if (debug>0){    
# working in progress #     cat(dir.breaks,"\n")
# working in progress #     cat(dir.labels,"\n")
# working in progress #     cat(levels(dir.binned),"\n")       
# working in progress #   }  
# working in progress #   
# working in progress #   # deal with change in ordering introduced somewhere around version 2.2
# working in progress #   if(packageVersion("ggplot2") > "2.2"){    
# working in progress #     cat("Hadley broke my code\n")
# working in progress #     data$spd.binned = with(data, factor(spd.binned, levels = rev(levels(spd.binned))))
# working in progress #     spd.colors = rev(spd.colors)
# working in progress #   }
# working in progress #   
# working in progress #   # create the plot ----
# working in progress #   p.windrose <- ggplot(data = data,
# working in progress #                        aes(x = dir.binned,
# working in progress #                            fill = spd.binned)) +
# working in progress #     geom_bar() + 
# working in progress #     scale_x_discrete(drop = FALSE,
# working in progress #                      labels = waiver()) +
# working in progress #     coord_polar(start = -((dirres/2)/360) * 2*pi) +
# working in progress #     scale_fill_manual(name = "Wind Speed (m/s)", 
# working in progress #                       values = spd.colors,
# working in progress #                       drop = FALSE) +
# working in progress #     theme(axis.title.x = element_blank())
# working in progress #   
# working in progress #   # adjust axes if required
# working in progress #   if (!is.na(countmax)){
# working in progress #     p.windrose <- p.windrose +
# working in progress #       ylim(c(0,countmax))
# working in progress #   }
# working in progress #   
# working in progress #   # print the plot
# working in progress #   print(p.windrose)  
# working in progress #   
# working in progress #   # return the handle to the wind rose
# working in progress #   return(p.windrose)
# working in progress # }
# working in progress # 
# working in progress # #    per fare la media della direzione del vento contenuta nel vettore "vecWindD" bisogna usare la seguente formula:
# working in progress # #    media_vecWindD <- (360+(atan2(sum(sin(vecWindD*pi/180)),sum(cos(vecWindD*pi/180)))*180/pi))%%360
# working in progress # #
# working in progress # # al posto dei boxplot facciamo polar plots mensili direzione del vento
# working in progress # #    titolo temporaneo per gli outputs
# working in progress # tmp.main.title <- "MRG wind direction typical year"
# working in progress # #    salviamo il plot in png esterno
# working in progress # # png(filename = paste0("outputs/", gsub(" ","",tmp.main.title,fixed = TRUE), ".png"), width=3840, height=2160, res=300)#4K
# working in progress # # dev.off() # questo comando chiude il grafico (è necessario!)
# working in progress # 
# working in progress # # facciamo una tabella che contenga i dati
# working in progress # tmp_var <- "Classi_vento_azimut"
# working in progress # WindDirection_deg.final.table <- data.frame()
# working in progress # for(i in months.label){
# working in progress #   tmp.target.name <- as.character(paste0(i,".MRG.typical"))
# working in progress #   tmp.target.dataset <- as.data.frame(mget(tmp.target.name))
# working in progress #   tmp.target.sub.dataset <- subset(tmp.target.dataset , select = paste0(i,".MRG.typical.",tmp_var))
# working in progress #   tmp.table <- data.frame(MONTH=i, 
# working in progress #                           #MIN=min(tmp.target.sub.dataset[,1], na.rm = TRUE), 
# working in progress #                           #MAX=max(tmp.target.sub.dataset[,1], na.rm = TRUE),
# working in progress #                           #MEAN=mean(tmp.target.sub.dataset[,1], na.rm = TRUE), 
# working in progress #                           MEAN=(360+(atan2(sum(sin(tmp.target.sub.dataset[,1]*pi/180)),sum(cos(tmp.target.sub.dataset[,1]*pi/180)))*180/pi))%%360, 
# working in progress #                           #SD=sd(tmp.target.sub.dataset[,1], na.rm = TRUE),
# working in progress #                           #SD_perc = round(abs(sd(tmp.target.sub.dataset[,1], na.rm = TRUE)/mean(tmp.target.sub.dataset[,1], na.rm = TRUE)*100), digits = 0),
# working in progress #                           nr_obs=length(tmp.target.sub.dataset[,1])
# working in progress #                           )
# working in progress #   WindDirection_deg.final.table <- rbind(WindDirection_deg.final.table, tmp.table)
# working in progress # }
# working in progress # write.csv(x = WindDirection_deg.final.table, file = paste0("outputs/", gsub(" ","",tmp.main.title,fixed = TRUE), ".csv"))
# working in progress # rm(tmp.target.name, tmp.target.dataset, tmp.target.sub.dataset, tmp.table, tmp_var, tmp.main.title)
# working in progress # 
# working in progress # #  # uso funzioni di base per non caricare librerie (https://stackoverflow.com/questions/41627124/r-base-graphics-circular-histogram):
# working in progress # #  add.filled.arc <- function(center.x, center.y, radius, angle.start, angle.end, col='black') {
# working in progress # #    theta <- seq(angle.start, angle.end, .0001)
# working in progress # #    segments(0, 0, radius*cos(theta), radius*sin(theta), col)
# working in progress # #    segments(0, 0, cos(angle.start), sin(angle.start), col='gray')
# working in progress # #    segments(0, 0, cos(angle.end), sin(angle.end), col='gray')
# working in progress # #  }
# working in progress # #  plot.coord.polar <- function(df) {
# working in progress # #    df <- df[complete.cases(df),]
# working in progress # #    df <- df[order(df[,1]),]
# working in progress # #    df[,1] <- df[,1]*(pi/180) # convert dir to radian
# working in progress # #    df[,2] <- df[,2] / max(df[,2]) # normalize magnitude within [0-1]
# working in progress # #    plot(-1:1, -1:1, type= 'n', xlab='', ylab='', xaxt='n', yaxt='n')
# working in progress # #    sapply(1:(nrow(df)-1), function(i) add.filled.arc(0, 0, df[i,2], df[i,1], df[i+1,1], rainbow(nrow(df))[i]))
# working in progress # #    theta <- seq(0, 2*pi, 0.0001)
# working in progress # #    lines(cos(theta), sin(theta), col='gray')
# working in progress # #  }
# working in progress # # test dataset gennaio
# working in progress # # i <- "jan"
# working in progress # # tmp_var <- "Classi_vento_azimut"
# working in progress # # tmp.target.name <- as.character(paste0(i,".MRG.typical"))
# working in progress # # tmp.target.dataset <- as.data.frame(mget(tmp.target.name))
# working in progress # # tmp.target.sub.dataset <- subset(tmp.target.dataset , select = paste0(i,".MRG.typical.",tmp_var))
# working in progress # # print(hist(tmp.target.sub.dataset[,1], breaks = seq(0,360,30)))
# working in progress # # dummy_df <- data.frame(CLASSI=hist(tmp.target.sub.dataset[,1], breaks = seq(0,360,30))$breaks[1:12],
# working in progress # #                        CONTEGGI=hist(tmp.target.sub.dataset[,1], breaks = seq(0,360,30))$counts)
# working in progress # # avg_dummy_df <- (360+(atan2(sum(sin(tmp.target.sub.dataset[,1]*pi/180)),sum(cos(tmp.target.sub.dataset[,1]*pi/180)))*180/pi))%%360
# working in progress # # avg_dummy_df
# working in progress # # # plot.coord.polar(dummy_df)
# working in progress # # ggplot(dummy_df, aes(x=CLASSI, y=CONTEGGI)) + geom_bar(stat='identity') + coord_polar(start=) 
# working in progress # 
# working in progress # #par(mfrow=c(1,2))
# working in progress # dummy_df_jan <- jan.MRG.typical[,4:5]
# working in progress # dummy_df_feb <- feb.MRG.typical[,4:5]
# working in progress # dummy_df_mar <- mar.MRG.typical[,4:5]
# working in progress # plot.windrose(spd = dummy_df_jan$V_Vent_m_s, dir = dummy_df_jan$Classi_vento_azimut, spdseq = c(0,3,6,12,20,50))
# working in progress # plot.windrose(spd = dummy_df_feb$V_Vent_m_s, dir = dummy_df_feb$Classi_vento_azimut, spdseq = c(0,3,6,12,20,50))
# working in progress # plot.windrose(spd = dummy_df_mar$V_Vent_m_s, dir = dummy_df_mar$Classi_vento_azimut, spdseq = c(0,3,6,12,20,50))
# working in progress # 