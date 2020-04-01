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

# boxplot temperature mensili
#    lista mesi per il testo asse x e titolo del grafico
months.label <- c("jan", "feb", "mar", "apr", "maj", "jun", "jul", "aug", "sep", "oct", "nov", "dec")
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
temperature.final.table <- data.frame()
for(i in months.label){
  tmp.target.name <- as.character(paste0(i,".MRG.typical"))
  tmp.target.dataset <- as.data.frame(mget(tmp.target.name))
  tmp.target.sub.dataset <- subset(tmp.target.dataset , select = paste0(i,".MRG.typical",".T_C"))
  tmp.table <- data.frame(MONTH=i, 
                          T_MIN=min(tmp.target.sub.dataset[,1], na.rm = TRUE), 
                          T_MAX=max(tmp.target.sub.dataset[,1], na.rm = TRUE),
                          T_MEAN=mean(tmp.target.sub.dataset[,1], na.rm = TRUE), 
                          T_SD=sd(tmp.target.sub.dataset[,1], na.rm = TRUE),
                          #T_SD_perc = round(abs(sd(tmp.target.sub.dataset[,1], na.rm = TRUE)/mean(tmp.target.sub.dataset[,1], na.rm = TRUE)*100), digits = 0),
                          nr_obs=length(tmp.target.sub.dataset[,1])
                          )
  temperature.final.table <- rbind(temperature.final.table, tmp.table)
}
write.csv(x = temperature.final.table, file = paste0("outputs/", gsub(" ","",tmp.main.title,fixed = TRUE), ".csv"))

rm(tmp.target.name, tmp.target.dataset, tmp.target.sub.dataset, tmp.table, tmp.main.title)
