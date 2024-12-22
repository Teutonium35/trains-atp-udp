Travail de Mathieu HERESBACH, Justin BAUER, Arnaud LELOIR.

Les fichiers contiennent toutes les fonctions nécessaires à la gestion du registre de trains. Certaines améliorations sont cependant à prévoir :
- Revoir la structure de stockage des trains. Actuellement, on utilise un array de la taille du circuit, dont chaque case contient soit un objet 'train vide', soit les données du train présent sur le canton. Plutôt prévoir une structure contenant uniquement les trains présents sur le circuit, dont le struct contient leurs coordonnées.
- Globalement les fonctions sont mal rangées : besoin de nettoyage, voire de rajouter quelques fonctions pour une meilleure modularité du code.

Ces défauts seront a priori corrigés pour l'application mini unity pro.