Travail de Mathieu HERESBACH, Justin BAUER, Arnaud LELOIR.

Le fonctionnement retenu pour le moment pour la messagerie est le suivant :

Les clients envoient des requêtes au serveur sous la forme : 
<!-- <nom_train>:<no_procedure>:<data>: -->
Chaque procédure correspond à une action élémentaire : 
- 1 sans data pour ajouter un train (le serveur renvoie 1 s'il a réussi, 0 sinon, par exemple si un train est déjà présent sur le canton 0)
- 2 sans data pour retirer un train (le serveur renvoie 1 quoi qu'il arrive, il faut prévoir qu'il renvoie 0 si le train faisant la demande ne se trouve pas sur le dernier canton)
- 3 avec data = numéro du canton de destination souhaité pour demander la permission de mouvement jusqu'à un canton, le serveur renvoie 1 si l'autorisation est accordée et 0 sinon
- 4 avec data = numéro du canton de destination souhaité pour effectuer le mouvement jusqu'à un canton, le serveur renvoie 1 s'il a réussi à effectuer le mouvement

Les fichiers contiennent toutes les fonctions nécessaires à la gestion du registre de trains. Certaines améliorations sont cependant à prévoir :
- Revoir la structure de stockage des trains. Actuellement, on utilise un array de la taille du circuit, dont chaque case contient soit un objet 'train vide', soit les données du train présent sur le canton. Plutôt prévoir une structure contenant uniquement les trains présents sur le circuit, dont le struct contient leurs coordonnées.
- Globalement les fonctions sont mal rangées : besoin de nettoyage, voire de rajouter quelques fonctions pour une meilleure modularité du code.

Ces défauts seront a priori corrigés pour l'application mini unity pro.